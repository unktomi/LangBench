#!/usr/bin/env python3
"""Build native benchmarks into <darwin|linux>/<c|lang>, never source folders."""
import argparse
import json
import os
from pathlib import Path
import platform
import shlex
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent.parent
BENCHMARKS = (
    "ackermann", "fibers", "gibonacci", "hanoi-fixed", "iterate",
    "pair-energy", "recurrence", "sorted-first", "sorted-k", "string-length",
)
LANG_SOURCES = {
    name: ("fibers-dynamic-cli.lang" if name == "fibers" else
           "hanoi-fixed.lang" if name == "hanoi-fixed" else f"{name}-cli.lang")
    for name in BENCHMARKS
}
# Preserve the names of the upstream macOS executables on both platforms.
LANG_NAMES = {"fibers": "fibers-dynamic", "sorted-k": "sorted_k"}


def host_platform():
    host = platform.system().lower()
    if host not in ("darwin", "linux"):
        raise ValueError(f"Unsupported host: {platform.system()}; use macOS or Linux")
    return host


def revision(path):
    try:
        result = subprocess.run(["git", "-C", str(path), "rev-parse", "HEAD"],
                                capture_output=True, text=True)
    except OSError:
        return None
    return result.stdout.strip() if result.returncode == 0 else None


def parser():
    result = argparse.ArgumentParser(description=__doc__)
    result.add_argument("--implementation", choices=("all", "c", "lang"), default="all")
    result.add_argument("--benchmark", action="append", choices=(*BENCHMARKS, "recurrence-jump"),
                        help="Build selected workloads only (repeatable)")
    result.add_argument("--compiler", type=Path,
                        default=Path(os.environ.get("LANG_TOOLCHAIN", ROOT.parent / "Lang")),
                        help="Built Lang checkout (default: LANG_TOOLCHAIN or ../Lang)")
    return result


def main():
    args = parser().parse_args()
    try:
        host = host_platform()
    except ValueError as error:
        parser().error(str(error))
    implementations = ("c", "lang") if args.implementation == "all" else (args.implementation,)
    selected = args.benchmark or list(BENCHMARKS) + ["recurrence-jump"]
    if args.implementation == "lang" and args.benchmark and "recurrence-jump" in selected:
        parser().error("recurrence-jump is a C-only workload")
    compiler = args.compiler.expanduser().resolve()
    logs = ROOT / "build" / host
    logs.mkdir(parents=True, exist_ok=True)
    evidence = logs / "host-machine.json"
    report = {
        "platform": host, "architecture": platform.machine(),
        "benchmark_revision": revision(ROOT), "compiler": str(compiler),
        "compiler_revision": revision(compiler) if compiler.is_dir() else None,
        "results": [],
    }
    manifest = compiler / "source-manifest.json"
    if manifest.exists() and not report["compiler_revision"]:
        report["compiler_revision"] = json.loads(manifest.read_text()).get("commit")
    lang_error = None
    if "lang" in implementations:
        if not (compiler / "dist/src/modules/lang/rt/cli.js").is_file():
            lang_error = f"Compiler not built at {compiler}; run npm run build there first"
        else:
            probe = """
import {pathToFileURL} from 'node:url';
import {writeFileSync} from 'node:fs';
const {readHostMachineEvidence} = await import(pathToFileURL(process.argv[1]));
const facts = readHostMachineEvidence({compiler:process.env.LANG_CLANG || 'clang', executionModel:true});
writeFileSync(process.argv[2], JSON.stringify(facts, null, 2) + '\\n');
"""
            command = ["node", "--input-type=module", "-e", probe,
                       str(compiler / "dist/src/modules/lang/rt/native/machine-evidence.js"), str(evidence)]
            try:
                probe_result = subprocess.run(command, text=True, capture_output=True)
                (logs / "machine-evidence.log").write_text(probe_result.stdout + probe_result.stderr)
                if probe_result.returncode:
                    lang_error = f"Machine evidence probe failed; see {logs / 'machine-evidence.log'}"
            except OSError as error:
                lang_error = str(error)

    for implementation in implementations:
        output = ROOT / host / implementation
        output.mkdir(parents=True, exist_ok=True)
        for name in selected:
            if implementation == "lang" and name == "recurrence-jump":
                continue
            filename = LANG_NAMES.get(name, name) if implementation == "lang" else name
            target = output / filename
            temporary = output / f".{filename}.{os.getpid()}.tmp"
            logfile = logs / f"{implementation}-{name}.log"
            if implementation == "c":
                sources = (["fibers-cli.c", "fibers.c"] if name == "fibers" else
                           ["recurrence-cli.c"] if name == "recurrence-jump" else [f"{name}-cli.c"])
                command = shlex.split(os.environ.get("CC", "cc"))
                command += shlex.split(os.environ.get("CFLAGS", "-O3 -std=c11 -Wall -Wextra"))
                if name == "recurrence-jump":
                    command += ["-DRECURRENCE_JUMP"]
                command += [str(ROOT / "c" / source) for source in sources]
                command += ["-o", str(temporary)]
            else:
                command = ["node", str(compiler / "bin/langc"),
                           "--machine-evidence", str(evidence), "--require-derived-the-term",
                           "--langpath", str(ROOT / "lang"), str(ROOT / "lang" / LANG_SOURCES[name]),
                           "-o", str(temporary)]
            start = time.monotonic()
            try:
                if implementation == "lang" and lang_error:
                    raise RuntimeError(lang_error)
                with logfile.open("w") as log:
                    result = subprocess.run(command, cwd=ROOT if implementation == "c" else compiler,
                                            stdout=log, stderr=subprocess.STDOUT)
                returncode = result.returncode
                if returncode == 0:
                    temporary.replace(target)
                    # Both names were checked in upstream; keep the compatibility alias current.
                    if implementation == "lang" and name == "sorted-first":
                        shutil.copy2(target, output / "sorted_first")
            except (OSError, RuntimeError) as error:
                logfile.write_text(str(error) + "\n")
                returncode = 1
            finally:
                if temporary.exists():
                    temporary.unlink()
            report["results"].append({
                "implementation": implementation, "benchmark": name, "output": str(target.relative_to(ROOT)),
                "returncode": returncode, "elapsed_seconds": round(time.monotonic() - start, 3),
                "command": command, "log": str(logfile.relative_to(ROOT)),
            })
            print(f"{implementation}/{name}: {'PASS' if returncode == 0 else 'FAIL'} ({logfile.relative_to(ROOT)})", flush=True)
    (logs / "build-results.json").write_text(json.dumps(report, indent=2) + "\n")
    return 0 if all(item["returncode"] == 0 for item in report["results"]) else 1


if __name__ == "__main__":
    sys.exit(main())
