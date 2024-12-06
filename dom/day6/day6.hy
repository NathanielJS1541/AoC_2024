(require hyrule [defmain loop])


(setv FILENAME "input.txt"
      DIRS [[0 -1] [1 0] [0 1] [-1 0]])


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (list (map (fn [x] (.strip x)) (.readlines file)))))


(defn parse-lines [lines]
    (setv ylen (len lines)
          xlen (len (get lines 0))
          obstacles #{}
          start [-1 -1])
    (for [y (range ylen) x (range xlen)]
        (match (get lines y x)
               "#" (setv obstacles (| obstacles #{#(x y)}))
               "^" (setv start #(x y))))
    [xlen ylen obstacles start])


(defn take-steps [xlen ylen obstacles start]
    (loop [[pos start] [dir 0] [steps #{start}]]
        (setv facing #((+ (get pos 0) (get DIRS dir 0))
                       (+ (get pos 1) (get DIRS dir 1)))
              nextdir (if (in facing obstacles) (% (+ dir 1) 4) dir)
              nextpos #((+ (get pos 0) (get DIRS nextdir 0))
                        (+ (get pos 1) (get DIRS nextdir 1)))
              done (or (chainc 0 > (get nextpos 0) <= xlen)
                       (chainc 0 > (get nextpos 1) <= ylen)))
        (if done
            steps
            (recur nextpos nextdir (| steps #{nextpos})))))


(defmain []
    (setv lines (get-lines FILENAME)
          [xlen ylen obstacles start] (parse-lines lines)
          steps (take-steps xlen ylen obstacles start)
          part1-ans (len steps))
    (print "Answer to part 1:" part1-ans))
