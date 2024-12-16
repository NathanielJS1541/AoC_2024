(import re)
(require hyrule [defmain unless])

(setv FILENAME "input.txt")
(setv RE_NUMS r"\d+")


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (.readlines file)))


(defn split-numbers [lines]
    "Splits a list of strings into a list of lists of numbers"
    (setv compiled (re.compile RE_NUMS))
    (list (map (fn [line] (list (map int (re.findall compiled line)))) lines)))


(defn diff-nums [nums]
    "Finds the differences between adjacent numbers in a list"
    (setv lastnum None
          diffs [])
    (for [num nums]
        (unless (is lastnum None)
            (.append diffs (- num lastnum)))
        (setv lastnum num))
    diffs)


(defn diffs-in-range [diffs]
    "Checks if all numbers in a list are between -1 and -3 or 1 and 3"
    (setv decreasing (map (fn [x] (chainc -1 >= x >= -3)) diffs)
          increasing (map (fn [x] (chainc 1 <= x <= 3)) diffs))
    (or (all decreasing)
        (all increasing)))


(defn drop-all [l]
    "Creates (len l) lists each with one item removed"
    (lfor i (range (len l))
        (+ (cut l i) (cut l (+ i 1) None))))


(defn almost-in-range [nums]
    "Finds how many have diffs in range after applying drop-all"
    (setv combinations (list (map drop-all nums))
          diffs (list (map (fn [clist] (list (map diff-nums clist)))
                           combinations)))
    (list (map (fn [dlist] (any (map diffs-in-range dlist))) diffs)))


(defmain []
    (setv num-lists (split-numbers (get-lines FILENAME))
          diffs (list (map diff-nums num-lists))
          in-range (list (map diffs-in-range diffs))
          part1-ans (sum in-range)
          close-enough (almost-in-range num-lists)
          part2-ans (sum close-enough))
    (print "Answer to part 1:" part1-ans)
    (print "Answer to part 2:" part2-ans))

