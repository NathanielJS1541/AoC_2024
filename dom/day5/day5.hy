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
            True
            (if (and (in (get pages pageno) rules)
                     (.union (get rules (get pages pageno))
                            (set (cut pages None pageno))))
                (do (print (get rules (get pages pageno))) False)
                (do (print pageno "ok") (recur (+ pageno 1)))))) )


(defmain []
    (setv lines (get-lines FILENAME)
          [rules books _] (reduce split-rules lines [{} [] False]))
    (print (list (map (fn [book] (is-in-order rules book)) books)))
    (print "Rules:" (.join "\n" (map str (.items rules))) :sep "\n")
    (print "Books:" (.join "\n" (map str books)) :sep "\n"))
        ; part1-ans (part1 lines)``
        ; part2-ans (part2 lines))
    ;(print "Answer to part 1:" part1-ans)
    ; (print "Answer to part 2:" part2-ans))
