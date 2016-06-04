{-# LANGUAGE FlexibleInstances #-}

module FpJargon where

-- | Arity
--
-- >>> arity map
-- 2
--
-- >>> arity "hello"
-- 0
class Arity f where
  arity :: f -> Int

instance Arity x where
  arity _ = 0

instance {-# OVERLAPPING #-} Arity f => Arity ((->) a f) where
  arity f = 1 + arity (f undefined)
