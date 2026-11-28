module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

data Flow a = Return a | Write Bool (Flow a) | Read (Bool -> Flow a)

substitute :: Flow a -> (a -> Flow b) -> Flow b
substitute (Return a) k = k a
substitute (Write b rest) k = Write b (substitute rest k)
substitute (Read next) k = Read (\b -> substitute (next b) k)

interpret :: Bool -> Flow a -> (a, Bool)
interpret flag (Return a) = (a, flag)
interpret _ (Write b rest) = interpret b rest
interpret flag (Read next) = interpret flag (next flag)

program :: Int64 -> Int64 -> Int64 -> Bool -> Flow Int64
program a b adjustment replacement = initial `substitute` continue
  where
    initial = Read (\flag -> if flag then Write False (Return b) else Write True (Return a))
    continue chosen = Read (\flag -> if flag
      then Write replacement (Return (chosen * adjustment))
      else Return (chosen + adjustment))

observe :: Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64
observe a b adjustment initial replacement coordinate =
  case interpret (initial /= 0) (program a b adjustment (replacement /= 0)) of
    (value, flag) -> if coordinate == 0 then value else if flag then 1 else 0

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just [a, b, adjustment, initial, replacement, coordinate] <- traverse signedWord inputs, bit coordinate ->
            print (repeatDigest count (\i -> observe (a + i) (b + i) (adjustment + i) initial replacement coordinate))
      _ -> die "usage: flag-flow a b adjustment initial replacement coordinate [--repeat N]"
    bit n = n == 0 || n == 1

signedWord :: String -> Maybe Int64
signedWord token = do
  let (sign, digits) = case token of
        '-' : rest -> (-1, rest)
        '+' : rest -> (1, rest)
        _ -> (1, token)
  if null digits || any (\c -> c < '0' || c > '9') digits then Nothing else do
    magnitude <- readMaybe digits :: Maybe Integer
    let value = sign * magnitude
    if value < toInteger (minBound :: Int64) || value > toInteger (maxBound :: Int64)
      then Nothing
      else Just (fromInteger value)
