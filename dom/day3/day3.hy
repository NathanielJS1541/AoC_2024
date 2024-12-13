(import re)
(import functools [reduce])
(import hy.pyops [*])
(require hyrule [defmain])

(setv FILENAME "input.txt"
      RE_MUL (re.compile r"mul\((\d+),(\d+)\)")
      RE_ANY (re.compile r"mul\(\d+,\d+\)|do\(\)|don't\(\)"))


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (.readlines file)))


(defn part1 [lines]
    "Finds the sum of the results of all mul(x,y) calls"
    (setv matches (list (map (fn [line] (re.findall RE_MUL line)) lines))
          flattened (sum matches [])
          products (list (map (fn [pair] (* (int (get pair 0)) (int (get pair 1))))
                              flattened)))
    (sum products))


(defn mul-reducer [state token]
    "Reduces tokens into an enabled flag and an accumulator"
    (setv [enabled accumulator] state)
    (cond
        (= token "do()")
            [True accumulator]
        (= token "don't()")
            [False accumulator]
        (setx nums (re.findall RE_MUL token))
            (if enabled
                [True (+ accumulator (reduce * (map int (get nums 0))))]
                state)))


(defn part2 [lines]
    "Uses a reducer to sum results of mul(x,y) calls between do() and don't()"
    (setv joined (.join "" (map (fn [line] (.strip line)) lines))
          matches (re.findall RE_ANY joined)
          [enabled answer] (reduce mul-reducer matches [True 0]))
    answer)


(defmain []
    (setv lines (get-lines FILENAME)
        part1-ans (part1 lines)
        part2-ans (part2 lines))
    (print "Answer to part 1:" part1-ans)
    (print "Answer to part 2:" part2-ans))

