module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

type Series = [Int64]

convolution :: Series -> Series -> Series
convolution (a:tailA) right@(b:tailB) =
  a * b : zipWith (+) (map (a *) tailB) (convolution tailA right)
convolution _ _ = error "series must be infinite"

substitute :: Series -> Series -> Series
substitute (a:tailA) innerTail =
  a : convolution innerTail (substitute tailA innerTail)
substitute [] _ = error "series must be infinite"

coefficient4 :: Int64 -> Int64 -> Int64 -> Int64 -> Int64
coefficient4 a b c d =
  substitute (a : b : repeat c) (d : repeat 1) !! 4

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just [a, b, c, d] <- traverse signedWord inputs ->
            print (repeatDigest count (\i -> coefficient4 (a + i) (b + i) (c + i) (d + i)))
      _ -> die "usage: series-composition a b c d [--repeat N]"

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
