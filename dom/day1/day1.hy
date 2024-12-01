(import re)
(require hyrule [defmain loop])


(setv FILENAME "input.txt")
(setv RE_NUMS r"\d+")


(defn get-lines [filename]
    (with [file (open filename "rt")]
        (.readlines file)))


(defn split-numbers [lines]
    (setv compiled (re.compile RE_NUMS))
    (list (map (fn [line] (re.findall compiled line)) lines)))


(defn transpose-nums [num-lists]
    (loop [[tail num-lists]
           [left-list []]
           [right-list []]]
          (if tail
            (recur (cut tail 1 None)
                   (+ left-list [(int (get tail 0 0))])
                   (+ right-list [(int (get tail 0 1))]))
            [left-list right-list])))


(defn sum-diffs [lists]
    (loop [[l_list (sorted (get lists 0))]
           [r_list (sorted (get lists 1))]
           [total 0]]
          (if l_list
              (do
                  (setv diff (abs (- (get l_list 0) (get r_list 0))))
                  (recur (cut l_list 1 None) (cut r_list 1 None) (+ total diff)))
              total)))


(defn find-similarity [lists]
    (setv l_list (get lists 0)
          r_list (get lists 1)))


(defn part1 []
    (setv num-lists (split-numbers (get-lines FILENAME))
          transposed (transpose-nums num-lists)
          part1-ans (sum-diffs transposed))
    (print "Answer to part 1:" part1-ans))


(defmain []
    (part1))
