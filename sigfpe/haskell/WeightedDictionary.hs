module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

type Vector = [Int64]
type Dictionary = [Vector]

tensor :: Vector -> Vector -> Dictionary
tensor key value = [map (k *) value | k <- key]

insert :: Dictionary -> Vector -> Vector -> Dictionary
insert prior key value = zipWith (zipWith (+)) prior (tensor key value)

dictionaryObserve :: Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64
dictionaryObserve a b c d keyIndex valueIndex =
  table !! fromIntegral keyIndex !! fromIntegral valueIndex
  where
    empty = replicate 2 (replicate 2 0)
    first = insert empty [a, b] [c, d]
    table = insert first [1, d] [b, -1]

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just [a, b, c, d, keyIndex, valueIndex] <- traverse signedWord inputs, bit keyIndex && bit valueIndex ->
            print (repeatDigest count (\i -> dictionaryObserve (a + i) (b + i) (c + i) (d + i) keyIndex valueIndex))
      _ -> die "usage: weighted-dictionary a b c d keyIndex valueIndex [--repeat N]"
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
