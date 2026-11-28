{-# LANGUAGE BangPatterns #-}
module BenchRepeat (repeatArguments, repeatDigest) where

import Data.Int (Int64)
import Text.Read (readMaybe)

-- A trailing option belongs to the whole invocation, not the workload parser.
repeatArguments :: [String] -> Maybe (Int64, [String])
repeatArguments arguments =
  case reverse arguments of
    token : "--repeat" : rest
      | let digits = case token of '+' : restDigits -> restDigits; _ -> token
      , not (null digits), all (\c -> c >= '0' && c <= '9') digits
      , Just count <- readMaybe digits :: Maybe Integer
      , count >= 1, count <= 100000000 -> Just (fromInteger count, reverse rest)
      | otherwise -> Nothing
    "--repeat" : _ -> Nothing
    _ -> Just (1, arguments)

-- Both the contribution and rolling digest are machine words; Int64 arithmetic
-- wraps. The varying index distinguishes iterations without reparsing input.
repeatDigest :: Int64 -> (Int64 -> Int64) -> Int64
repeatDigest count answer = go 0 0
  where
    go !index !digest
      | index == count = digest
      | otherwise = go (index + 1) (digest * 1099511628211 + answer index)
{-# INLINE repeatDigest #-}
