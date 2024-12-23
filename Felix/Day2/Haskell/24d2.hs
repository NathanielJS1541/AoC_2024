import System.IO 

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

-- Get the difference between each two consecutive list elements by zipping the list and its tail with -.
getDiffs :: (Real a) => [a] -> [a]
getDiffs [] = []
getDiffs l@(_:t) = zipWith (-) t l

-- A row is safe if it always changes in the same direction (sum (abs diffs) = abs (sum diffs)) and the change between any two consecutive
--  elements is always between 1 and 3.
isSafe :: (Real a) => [a] -> Bool
isSafe [] = error "Empty list"
isSafe x = let diffs = getDiffs x
               absDiffs = map Prelude.abs diffs
               in (Prelude.abs (sum diffs) == sum absDiffs) && ((minimum absDiffs) >= 1) && ((maximum absDiffs) <= 3)

-- Remove the nth element from a list
remove :: (Eq a) => Int -> [a] -> [a]
remove _ [] = error "List index out of range"
remove n (x:xs) = if n==0 then xs else x:(remove (n-1) xs)

-- A row can be safe if it is safe already, or removing any one element can make it safe.
canBeSafe :: (Real a) => [a] -> Bool
canBeSafe [] = error "Empty list"
canBeSafe x = if isSafe x then True else 
    let possibilities = [remove n x | n <- [0..(length x - 1)]]
    in any (True==) (map isSafe possibilities)


main :: IO ()
main = do
  -- Open the file and read its contents into a string
  handle <- openFile "../Real.txt" ReadMode
  contents <- hGetContents handle

  -- The text of the file split into a Nx2 array of int
  let inputArray = [[read y | y <- split ' ' x, y /= ""] | x <- split '\n' contents, x /= ""]::[[Int]]

  print "Parsing done"
  
  -- Part 1: For each row in the input array, see if it is safe. fromEnum turns false/true into 0/1 so that sum can count the number of trues.
  print (sum (map (fromEnum . isSafe) inputArray))
  
  -- Part 2: See if each row can be made safe. Other fluff as in part 1.
  print (sum (map (fromEnum . canBeSafe) inputArray))