import System.IO

-- Check if a character is a digit
isDigit :: Char -> Bool
isDigit a = elem a ['0'..'9']

-- Get all the consecutive digits from the start of a string
getDigits :: [Char] -> [Char]
getDigits [] = []
getDigits (x:xs) = if (isDigit x) then x:(getDigits xs) else []

-- Look for "mul(" at the start of a string, if successful go on to look for the first number
findStart :: [Char] -> Bool -> Int
findStart [] _ = 0
findStart ('m':'u':'l':'(':xs) e = findFirstNum xs e
findStart ('d':'o':'n':'\'':'t':'(':')':xs) _ = findStart xs False
findStart ('d':'o':'(':')':xs) _ = findStart xs True
findStart (_:xs) e = findStart xs e

-- Look for a number at the start of a string, if successful go on to look for a comma
findFirstNum :: [Char] -> Bool -> Int
findFirstNum [] _ = 0
findFirstNum l e = let numStr = getDigits l
                       lenNumStr = length numStr
                       in if (lenNumStr == 0)
                          then findStart l e
                          else checkComma (drop lenNumStr l) (read numStr) e

-- Look for a comma at the start of a string, if successful go on to look for the second number
-- The value of the first number is passed in
checkComma :: [Char] -> Int -> Bool -> Int
checkComma [] _ _ = 0
checkComma (',':xs) f e = findSecondNum xs f e
checkComma l _ e = findStart l e

-- Look for a number at the start of a string, if successful go on to look for a closing bracket
-- The value of the first number is passed in
findSecondNum :: [Char] -> Int -> Bool -> Int
findSecondNum [] _ _ = 0
findSecondNum l f e = let numStr = getDigits l
                          lenNumStr = length numStr
                          in if (lenNumStr == 0)
                             then findStart l e
                             else findClosingBracket (drop lenNumStr l) f (read numStr) e

-- Look for a closing bracket at the start of a string, if successful look for the next start
-- The value of the first and second numbers are passed in
findClosingBracket :: [Char] -> Int -> Int -> Bool -> Int
findClosingBracket [] _ _ _ = 0
findClosingBracket (')':xs) f s e = (if (e || part1) then f * s else 0) + (findStart xs e)
findClosingBracket l _ _ e = findStart l e

part1 :: Bool
part1 = False

main :: IO()
main = do
    -- Open the file and read its contents into a string
    handle <- openFile "../Real.txt" ReadMode
    contents <- hGetContents handle
    print (findStart contents True)