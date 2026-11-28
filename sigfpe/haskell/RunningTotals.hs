module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

transactions :: Int64 -> Int64 -> [Int64]
transactions first increment = iterate (+ increment) first

runningBalance :: Int64 -> Int64 -> Int64 -> Int64 -> Int64
runningBalance initial first increment position =
  scanl (+) initial (transactions first increment) !! fromIntegral (max 0 position)

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just [initial, first, increment, position] <- traverse signedWord inputs ->
            print (repeatDigest count (\i -> runningBalance initial (first + i) increment position))
      _ -> die "usage: running-totals initial first increment position [--repeat N]"

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
