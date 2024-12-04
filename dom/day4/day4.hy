(import re)
(import functools [reduce])
(import hy.pyops [*])
(require hyrule [defmain loop])

(setv FILENAME "input.txt"
      NEEDLE1 "XMAS"
      DIRS1 [[-1  1] [0  1] [1  1]
             [-1  0]        [1  0]
             [-1 -1] [0 -1] [1 -1]]
      NEEDLE2 "MAS"
      DIRS2 [[-1  1] [1  1]
             [-1 -1] [1 -1]])


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (list (map (fn [x] (.strip x)) (.readlines file)))))


(defn search-dir [haystack needle start dir]
    "Search for the neeedle along a direction from a start point in a haystack"
    (setv xlen (len (get haystack 0))
          ylen (len haystack))
    (loop [[pos start]
           [progress 0]]
        (if (= progress (len needle))
            True
            (do
                (setv [x y] pos
                      inrange (and (chainc 0 <= x < xlen) (chainc 0 <= y < ylen))
                      matches (if inrange
                                  (= (get haystack y x) (get needle progress))
                                  False))
                (if matches
                    (recur [(+ x (get dir 0)) (+ y (get dir 1))] (+ progress 1))
                    False)))))


(defn part1 [lines]
    "Find the number of instances of the needle in all directions"
    (sum (lfor x (range (len (get lines 0)))
               y (range (len lines))
               (sum (lfor dir DIRS1 (search-dir lines NEEDLE1 [x y] dir))))))


(defn part2 [lines]
    "Find the number of times two MAS's diagonally cross each other"
    (setv xlen (len (get lines 0))
          ylen (len lines)
          counters (lfor y (range ylen) (lfor x (range xlen) 0))
          total 0)
    (for [x (range (len (get lines 0)))
          y (range (len lines))]
        (for [dir DIRS2]
            (when (search-dir lines NEEDLE2 [x y] dir)
                (do (+= (get counters (+ y (get dir 1)) (+ x (get dir 0))) 1)
                    (when (= (get counters (+ y (get dir 1)) (+ x (get dir 0))) 2)
                        (+= total 1))))))
    total)


(defmain []
    (setv lines (get-lines FILENAME)
        part1-ans (part1 lines)
        part2-ans (part2 lines))
    (print "Answer to part 1:" part1-ans)
    (print "Answer to part 2:" part2-ans))
