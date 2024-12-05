(import functools [reduce])
(require hyrule [defmain loop])


(setv FILENAME "testinput.txt")


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (list (map (fn [x] (.strip x)) (.readlines file)))))


(defn update-rules [rules newrule]
    (setv [before after] newrule)
    (if (in before rules)
        (| rules {before (.union (get rules before) #{after})})
        (| rules {before #{after}})))


(defn split-rules [state line]
    (setv [rules books done] state)
    (if done
        [rules (+ books [(list (map int (.split line ",")))]) True]
        (if (= line "")
            [rules books True]
            [(update-rules rules (list (map int (.split line "|")))) books False])))


(defn is-in-order [rules pages]
    (loop [[pageno 1]]
        (if (= pageno (len pages))
            (get pages (// (len pages) 2))
            (if (and (in (get pages pageno) rules)
                     (.intersection (get rules (get pages pageno))
                                    (set (cut pages pageno))))
                0
                (recur (+ pageno 1))))))


(defn fix-order [rules pages]
    (loop [[pageno 1] [ordered pages] [waswrong False]]
        (if (= pageno (len pages))
            (if waswrong (get ordered (// (len pages) 2)) 0)
            (if (and (in (get pages pageno) rules)
                     (setx wrongone (.intersection (get rules (get pages pageno))
                                                   (set (cut pages pageno)))))
                (do (setv wrongidx (.index ordered wrongone)
                          firstbit (cut ordered wrongidx))
                    (recur (+ pageno 1)))
                (recur (+ pageno 1) ordered waswrong)))))


(defmain []
    (setv lines (get-lines FILENAME)
          [rules books _] (reduce split-rules lines [{} [] False])
          part1-ans (sum (map (fn [book] (is-in-order rules book)) books)))
    (print "Rules:" (.join "\n" (map str (.items rules))) :sep "\n")
    (print "Books:" (.join "\n" (map str books)) :sep "\n")
        ; part1-ans (part1 lines)``
        ; part2-ans (part2 lines))
    (print "Answer to part 1:" part1-ans))
    ; (print "Answer to part 2:" part2-ans))
