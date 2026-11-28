module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

data Weather = Rainy | Sunny

suffixes :: (Weather -> Weather -> Int64) -> [(Int64, Int64)] -> Weather -> [Int64]
suffixes _ [] _ = [0]
suffixes step ((wet, dry):more) from = do
  (next, observation) <- [(Rainy, wet), (Sunny, dry)]
  rest <- suffixes step more next
  pure (step from next + observation + rest)

best :: Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64
best rr rs sr ss a b c d = minimum $ do
  (start, opening) <- [(Rainy, 0), (Sunny, a)]
  rest <- suffixes step [(b, c), (d, 0), (0, 0)] start
  pure (opening + rest)
  where
    step Rainy Rainy = rr
    step Rainy Sunny = rs
    step Sunny Rainy = sr
    step Sunny Sunny = ss

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just values@[rr, rs, sr, ss, a, b, c, d] <- traverse signedWord inputs, all validCost values ->
            print (repeatDigest count (\i -> best (vary i rr) (vary i rs) (vary i sr) (vary i ss) (vary i a) (vary i b) (vary i c) (vary i d)))
      _ -> die "usage: weather-paths rr rs sr ss a b c d [--repeat N]"
    vary i cost = ((cost + i + 1048576) `mod` 2097153) - 1048576
    validCost n = n >= -1048576 && n <= 1048576

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
