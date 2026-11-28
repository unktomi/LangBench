-- Direct AST evaluation of caller-provided polynomials by SIGFPE's pairing
-- method. Independently written from the mathematical rules, not a copy of
-- the blog listing or a precomputed derivative kernel.
module Main (main) where

import Control.Monad (foldM)
import Data.Int (Int64)
import System.Environment (getArgs)
import System.Exit (ExitCode(ExitFailure), exitWith)
import System.IO (hPutStrLn, stderr)
import Text.Read (readMaybe)

data Expr = Variable Int | Literal Int64 | Add Expr Expr | Multiply Expr Expr

applyOperator :: Expr -> Expr -> Expr
applyOperator op expr = case op of
    Literal _ -> Multiply op expr
    Add a b -> Add (applyOperator a expr) (applyOperator b expr)
    Multiply a b -> applyOperator a (applyOperator b expr)
    Variable v -> case expr of
        Variable w -> Literal (if v == w then 1 else 0)
        Literal _ -> Literal 0
        Add a b -> Add (applyOperator op a) (applyOperator op b)
        Multiply a b -> Add (Multiply (applyOperator op a) b)
                            (Multiply a (applyOperator op b))

atOrigin :: Expr -> Int64
atOrigin expression = case expression of
    Variable _ -> 0
    Literal n -> n
    Add a b -> atOrigin a + atOrigin b
    Multiply a b -> atOrigin a * atOrigin b

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
          then Nothing else Just (fromInteger value)

parseExpr :: [String] -> Maybe Expr
parseExpr tokens = do
    stack <- foldM push [] (reverse tokens)
    case stack of
        [tree] -> Just tree
        _ -> Nothing
  where
    push pending token = case (token, pending) of
        ("+", left:right:rest) -> Just (Add left right : rest)
        ("*", left:right:rest) -> Just (Multiply left right : rest)
        ("x", _) -> Just (Variable 0 : pending)
        ("y", _) -> Just (Variable 1 : pending)
        _ -> do
            value <- signedWord token
            return (Literal value : pending)

laplace :: Expr
laplace =
    let x = Variable 0
        y = Variable 1
    in Add (Multiply x x) (Multiply y y)

evaluateForward, evaluateFlipped :: Expr -> Int64
evaluateForward expression = atOrigin (applyOperator laplace expression)
evaluateFlipped expression = atOrigin (applyOperator expression laplace)

usage :: IO ()
usage = do
    hPutStrLn stderr "usage: runtime-tree (forward|flipped) PREFIX_TOKENS..."
    exitWith (ExitFailure 2)

main :: IO ()
main = do
    arguments <- getArgs
    case arguments of
        "forward":tokens | Just expression <- parseExpr tokens -> print (evaluateForward expression)
        "flipped":tokens | Just expression <- parseExpr tokens -> print (evaluateFlipped expression)
        _ -> usage
