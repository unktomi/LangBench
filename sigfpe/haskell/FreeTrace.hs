module Main (main) where

import BenchRepeat (repeatArguments, repeatDigest)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (die)
import Text.Read (readMaybe)

data EventProgram a = Result a | Event Int64 (EventProgram a)

instance Functor EventProgram where
  fmap f program = program >>= pure . f

instance Applicative EventProgram where
  pure = Result
  fs <*> xs = fs >>= \f -> fmap f xs

instance Monad EventProgram where
  Result a >>= k = k a
  Event event rest >>= k = Event event (rest >>= k)

twoEvents :: Int64 -> Int64 -> Int64 -> EventProgram Int64
twoEvents a b value = Event a (Event b (Result value))

program :: Int64 -> Int64 -> Int64 -> EventProgram Int64
program a b c =
  (twoEvents a b (a + c) >>= \v -> twoEvents v c (v * b))
    >>= \v -> Event (v - c) (Result v)

digest :: Int64 -> EventProgram Int64 -> Int64
digest z (Result a) = 31 * z + a
digest z (Event a rest) = digest (31 * z + a) rest

main :: IO ()
main = do
  arguments <- getArgs
  if null arguments
    then getContents >>= mapM_ (run . words) . lines
    else run arguments
  where
    run arguments = case repeatArguments arguments of
      Just (count, inputs)
        | Just [a, b, c, seed] <- traverse signedWord inputs ->
            print (repeatDigest count (\i -> digest (seed + i) (program (a + i) (b + i) (c + i))))
      _ -> die "usage: free-trace a b c seed [--repeat N]"

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
