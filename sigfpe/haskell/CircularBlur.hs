module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Array (Array, bounds, listArray, (!))
import Data.Int (Int64)
import Data.Ix (range)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

data Focus a = Focus Int (Array Int a)

mapFrame :: (a -> b) -> Focus a -> Focus b
mapFrame f (Focus i values) = Focus i (fmap f values)

extendFrame :: (Focus a -> b) -> Focus a -> Focus b
extendFrame f (Focus i values) =
  Focus i (listArray limits [f (Focus j values) | j <- range limits])
  where limits = bounds values

extract :: Focus a -> a
extract (Focus i values) = values ! i

blurNumerator :: Focus Int64 -> Int64
blurNumerator (Focus i values) =
  (values ! ((i - 1) `mod` 4)) + 2 * (values ! i) + (values ! ((i + 1) `mod` 4))

focusedBlur :: Int64 -> Int64 -> Int64 -> Int64 -> Int64 -> Int64
focusedBlur a b c d i =
  extract (mapFrame (\z -> z * z) (mapFrame (* 2) (extendFrame blurNumerator bright)))
  where bright = mapFrame (+ 1) (Focus (fromIntegral i) (listArray (0, 3) [a, b, c, d]))

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just [a, b, c, d, focus] <- traverse signedWord inputs, focus >= 0 && focus <= 3 ->
            print (repeatDigest count (\i -> focusedBlur (a + i) (b + i) (c + i) (d + i) focus))
      _ -> die "usage: circular-blur a b c d focus [--repeat N]"

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
