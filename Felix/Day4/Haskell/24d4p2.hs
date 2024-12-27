import System.IO
import Distribution.Simple.Test (test)

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

-- The below two functions achieve the X-MAX searching by picking off the first three rows and scanning across them,
--  then dropping the top row and recurring.

-- This function can be given an NxM list and it will count the occurrances of X-MAS within it
convolveRows :: [[Char]] -> Int
-- If there are at least three rows left, convolve the units of those rows
convolveRows (a:b:c:rs) = convolveUnits (transpose [a,b,c]) + convolveRows (b:c:rs)
-- Otherwise, quit
convolveRows _ = 0


-- This function can be given a Nx3 list and it will count the occurrances of X-MAS within it
convolveUnits :: [[Char]] -> Int
-- If the end of the rows has been reached, quit
convolveUnits [] = 0
-- If this unit is an X-MAS, add one and recur
convolveUnits (a:b@(_:'A':_:_):c:cs) = (if ((a!!0 == 'M' && c!!2 == 'S') || (a!!0 == 'S' && c!!2 == 'M'))
                                        && ((a!!2 == 'M' && c!!0 == 'S') || (a!!2 == 'S' && c!!0 == 'M'))
                                        then 1 else 0) + convolveUnits (b:c:cs)
-- If less than 3 rows are left, or this unit is not centered on an A, just recur.
convolveUnits (_:xs) = convolveUnits xs


main :: IO()
main = do
    -- Open the file and read its contents into a string
    handle <- openFile "../Real.txt" ReadMode
    contents <- hGetContents handle
    print $ convolveRows $ split '\n' contents