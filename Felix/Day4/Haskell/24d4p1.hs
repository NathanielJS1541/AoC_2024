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

-- Transpose a list of lists
transpose :: [[a]] -> [[a]]
transpose [] = []
transpose ([] : xss) = transpose xss
transpose ((x : xs) : xss) = combine x hds xs tls
  where
    (hds, tls) = unzip [(hd, tl) | hd : tl <- xss]
    combine y h ys t = (y:h) : transpose (ys:t)

-- Count the number of times "XMAS" or "SAMX" appear in a string
countXmasOrSamx :: String -> Int
countXmasOrSamx "" = 0
-- String starts "XMAS" or "SAMX": Found one! Remove the first 3 chars then recur.
countXmasOrSamx ('X':'M':'A':'S':xs) = 1 + countXmasOrSamx ('S':xs)
countXmasOrSamx ('S':'A':'M':'X':xs) = 1 + countXmasOrSamx ('X':xs)
-- Anything else, remove the first character and recur.
countXmasOrSamx (_:xs) = countXmasOrSamx xs

-- Get the positive diagonals of a 2D list
diagonals :: (Eq a) => [[a]] -> [[a]]
diagonals []       = []
diagonals ([]:xss) = xss
-- Taks the first column and join it onto the rest of the recursion shifted down one
diagonals ll       = zipWith (++)
                             -- Get the start of each row as a list of 1-long lists. Pad the end of the list with empty lists
                             -- When this is concatenated with the below, all the empty lists just concat to nothing
                             (map ((:[]) . head) ll ++ repeat [])
                             -- recur with the first item of each row removed and offset the result by 1
                             ([]:diagonals (map (drop 1) ll))

-- Reverse a list
reverseList :: [a] -> [a]
reverseList = foldl (flip (:)) []

main :: IO()
main = do
    -- Open the file and read its contents into a string
    handle <- openFile "../Real.txt" ReadMode
    contents <- hGetContents handle
    let horizontal = split '\n' contents
        hCount = sum (map countXmasOrSamx horizontal)
        vertical = transpose horizontal
        vCount = sum (map countXmasOrSamx vertical)
        negDiags = diagonals horizontal
        ndCount = sum (map countXmasOrSamx negDiags)
        posDiags = diagonals (map reverseList horizontal)
        pdCount = sum (map countXmasOrSamx posDiags)
        in do print (hCount + vCount + ndCount + pdCount)
              
