(import re)
(import collections [defaultdict])
(require hyrule [defmain loop])


(setv FILENAME "input.txt")
(setv RE_NUMS r"\d+")


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (.readlines file)))


(defn split-numbers [lines]
    "Splits a list of strings into a list of pairs of strings"
    (setv compiled (re.compile RE_NUMS))
    (list (map (fn [line] (re.findall compiled line)) lines)))


(defn transpose-nums [num-lists]
    "Transposes a list of pairs of numbers into a pair of lists of numbers"
    (loop [[tail num-lists]
           [left-list []]
           [right-list []]]
          (if tail
            (recur (cut tail 1 None)
                   (+ left-list [(int (get tail 0 0))])
                   (+ right-list [(int (get tail 0 1))]))
            [left-list right-list])))


(defn sum-diffs [lists]
    "Sum the differences between pairs of numbers in sorted lists"
    (loop [[l_list (sorted (get lists 0))]
           [r_list (sorted (get lists 1))]
           [total 0]]
          (if l_list
              (do
                  (setv diff (abs (- (get l_list 0) (get r_list 0))))
                  (recur (cut l_list 1 None) (cut r_list 1 None) (+ total diff)))
              total)))


(defn count-list [num-list]
    "Count the occurences of each number in a list into a dictionary"
    (setv dd (defaultdict int))
    (for [num num-list]
         (+= (get dd num) 1))
    dd)


(defn find-similarity [lists]
    "Find the similarity score of two lists"
    (setv l_list (get lists 0)
        counts (count-list (get lists 1))
        similarity 0)
    (for [num l_list]
        (+= similarity
            (* num (get counts num))))
    similarity)


(defmain []
    (setv num-lists (split-numbers (get-lines FILENAME))
          transposed (transpose-nums num-lists)
          part1-ans (sum-diffs transposed)
          part2-ans (find-similarity transposed))
    (print "Answer to part 1:" part1-ans)
    (print "Answer to part 2:" part2-ans))
