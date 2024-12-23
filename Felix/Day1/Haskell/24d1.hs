import System.IO 

-- Separate a list out into two lists by comparison with a pivot
partition :: (a -> Bool) -> [a] -> ([a],[a])
partition _ [] = ([],[])
partition c l@(_:_) = (filter c l, filter (not . c) l)

-- Sort a list using the quicksort algorithm
quicksort :: Ord a => [a] -> [a]
quicksort []     = []
quicksort (x:xs) = let (lesser, greater) = partition (<= x) xs
                   in quicksort lesser ++ [x] ++ quicksort greater

-- Split a string into a list of strings by a delimiter
split :: (Eq a) => a -> [a] -> [[a]]
split _ [] = [[]]
-- If the head of the list is the delimiter
split d (x:xs) = let s = split d xs
               in if x == d then
                    -- Append an empty list to the front of the split of the rest
                    []:s
                  else
                    -- Append this letter to the front of the first element of the split of the rest
                    case s of (y:ys) -> (x:y):ys
                              [] -> error "wtf?"

-- Transpose a list of lists
transpose :: [[a]] -> [[a]]
transpose [] = []
transpose ([] : xss) = transpose xss
transpose ((x : xs) : xss) = combine x hds xs tls
  where
    (hds, tls) = unzip [(hd, tl) | hd : tl <- xss]
    combine y h ys t = (y:h) : transpose (ys:t)

-- Count the occurances of an item in a list
count :: (Eq a) => a -> [a] -> Int
count x= length . filter (==x)

main :: IO ()
main = do

  -- Open the file and read its contents into a string
  handle <- openFile "../Real.txt" ReadMode
  contents <- hGetContents handle

      -- The text of the file split into a Nx2 array of int
  let inputArray = [[read y | y <- split ' ' x, y /= ""] | x <- split '\n' contents, x /= ""]::[[Int]]

      -- Transpose the input array to get a 2xN array
      transposedArray = transpose inputArray
  
      -- Sort each of the two columns of the input
      sortedArray = transpose ( map quicksort transposedArray)

            -- Part 1: Sum the absolute differences between the pairs in each row
      in do print (sum [ Prelude.abs (a-b) | [a,b] <- sortedArray])
            -- Part 2: Sum the products each element of the first column with the number of occurances of that element in the second column
            print (sum [x  * (count x (transposedArray!!1))| x <- (transposedArray!!0)])

  hClose handle
