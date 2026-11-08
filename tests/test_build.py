"""Builder routing and failure-handling tests without a native toolchain."""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location("build", Path(__file__).resolve().parents[1] / "scripts/build.py")
build = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(build)


class BuildTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.commands = []

    def run_build(self, system, arguments, failure=None, missing_git=False):
        def run(command, **kwargs):
            self.commands.append(command)
            if command[0] == "git":
                if missing_git:
                    raise FileNotFoundError("git")
                return subprocess.CompletedProcess(command, 1, "", "not a checkout")
            if "-o" in command:
                if failure == "launch":
                    raise FileNotFoundError("compiler")
                target = Path(command[command.index("-o") + 1])
                if failure != "missing-output":
                    target.write_text("partial" if failure == "compiler" else "binary")
                return subprocess.CompletedProcess(command, 1 if failure == "compiler" else 0)
            return subprocess.CompletedProcess(command, 1 if failure == "probe" else 0, "", "")

        with patch.object(build, "ROOT", self.root), \
             patch.object(build.platform, "system", return_value=system), \
             patch.object(build.subprocess, "run", side_effect=run), \
             patch.object(build.sys, "argv", ["build.py", *arguments]), \
             contextlib.redirect_stdout(io.StringIO()):
            return build.main()

    def test_c_routes_to_native_platform_without_touching_other_binary(self):
        for system, directory, other in (("Linux", "linux", "darwin"), ("Darwin", "darwin", "linux")):
            with self.subTest(system=system):
                original = self.root / other / "c" / "ackermann"
                original.parent.mkdir(parents=True, exist_ok=True)
                original.write_text("untouched")
                self.assertEqual(self.run_build(system, ["--implementation", "c", "--benchmark", "ackermann"]), 0)
                self.assertEqual((self.root / directory / "c" / "ackermann").read_text(), "binary")
                self.assertEqual(original.read_text(), "untouched")
                self.assertFalse((self.root / "c" / "ackermann").exists())

    def test_failure_keeps_previous_binary_and_cleans_partial_output(self):
        original = self.root / "linux" / "c" / "ackermann"
        original.parent.mkdir(parents=True)
        original.write_text("previous")
        self.assertEqual(self.run_build("Linux", ["--implementation", "c", "--benchmark", "ackermann"], failure="compiler"), 1)
        self.assertEqual(original.read_text(), "previous")
        self.assertEqual(list(original.parent.glob("*.tmp")), [])
        report = json.loads((self.root / "build/linux/build-results.json").read_text())
        self.assertEqual(report["results"][0]["returncode"], 1)

    def test_c_special_sources_and_recurrence_define(self):
        self.assertEqual(self.run_build("Linux", ["--implementation", "c", "--benchmark", "fibers",
                                                "--benchmark", "recurrence-jump"]), 0)
        commands = [command for command in self.commands if "-o" in command]
        self.assertIn(str(self.root / "c/fibers.c"), commands[0])
        self.assertIn(str(self.root / "c/fibers-cli.c"), commands[0])
        self.assertIn("-DRECURRENCE_JUMP", commands[1])

    def test_source_archive_build_does_not_require_git(self):
        self.assertEqual(self.run_build("Linux", ["--implementation", "c", "--benchmark", "ackermann"],
                                        missing_git=True), 0)
        report = json.loads((self.root / "build/linux/build-results.json").read_text())
        self.assertIsNone(report["benchmark_revision"])

    @unittest.skipUnless(platform.system() in ("Linux", "Darwin"), "Requires a supported native host")
    def test_optimized_python_still_rejects_incorrect_executable(self):
        scripts = self.root / "scripts"
        scripts.mkdir()
        source = Path(__file__).resolve().parents[1] / "scripts/smoke-test.py"
        shutil.copy2(source, scripts / "smoke-test.py")
        executable = self.root / platform.system().lower() / "c/ackermann"
        executable.parent.mkdir(parents=True)
        executable.write_text("#!/bin/sh\necho wrong\n")
        executable.chmod(0o755)
        result = subprocess.run([sys.executable, "-O", str(scripts / "smoke-test.py"),
                                 "--implementation", "c", "--benchmark", "ackermann"],
                                capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL ackermann", result.stderr)
        self.assertNotIn("PASS", result.stdout)

    def test_lang_preserves_names_and_requires_derived_terms(self):
        compiler = self.root / "compiler"
        cli = compiler / "dist/src/modules/lang/rt/cli.js"
        cli.parent.mkdir(parents=True)
        cli.touch()
        arguments = ["--implementation", "lang", "--compiler", str(compiler),
                     "--benchmark", "fibers", "--benchmark", "sorted-k", "--benchmark", "sorted-first"]
        self.assertEqual(self.run_build("Linux", arguments), 0)
        for name in ("fibers-dynamic", "sorted_k", "sorted-first", "sorted_first"):
            self.assertTrue((self.root / "linux/lang" / name).is_file())
        for command in self.commands:
            if "-o" in command:
                self.assertIn("--require-derived-the-term", command)
                self.assertIn("--machine-evidence", command)
                self.assertIn(str(self.root / "lang"), command)

    def test_missing_lang_compiler_reports_failure(self):
        self.assertEqual(self.run_build("Linux", ["--implementation", "lang", "--benchmark", "ackermann",
                                                "--compiler", str(self.root / "missing")]), 1)
        self.assertFalse((self.root / "linux/lang/ackermann").exists())
        self.assertIn("Compiler not built", (self.root / "build/linux/lang-ackermann.log").read_text())

    def test_missing_compiler_command_or_output_keeps_previous_binary(self):
        original = self.root / "linux/c/ackermann"
        original.parent.mkdir(parents=True)
        for failure in ("launch", "missing-output"):
            with self.subTest(failure=failure):
                original.write_text("previous")
                self.assertEqual(self.run_build("Linux", ["--implementation", "c", "--benchmark", "ackermann"],
                                                failure=failure), 1)
                self.assertEqual(original.read_text(), "previous")

    def test_failed_machine_evidence_probe_keeps_previous_lang_binary(self):
        compiler = self.root / "compiler"
        cli = compiler / "dist/src/modules/lang/rt/cli.js"
        cli.parent.mkdir(parents=True)
        cli.touch()
        original = self.root / "linux/lang/ackermann"
        original.parent.mkdir(parents=True)
        original.write_text("previous")
        self.assertEqual(self.run_build("Linux", ["--implementation", "lang", "--compiler", str(compiler),
                                                "--benchmark", "ackermann"], failure="probe"), 1)
        self.assertEqual(original.read_text(), "previous")
        self.assertFalse(any("-o" in command for command in self.commands))

    def test_unsupported_platform_and_c_only_selection_rejected(self):
        for system, arguments in (("Windows", ["--implementation", "c"]),
                                  ("Linux", ["--implementation", "lang", "--benchmark", "recurrence-jump"])):
            with self.subTest(system=system), contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit) as raised:
                    self.run_build(system, arguments)
                self.assertEqual(raised.exception.code, 2)


if __name__ == "__main__":
    unittest.main()
