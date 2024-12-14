(import functools [reduce])
(require hyrule [defmain loop])


(setv FILENAME "input.txt")


(defn get-lines [filename]
    "Read the lines from a file"
    (with [file (open filename "rt")]
        (list (map (fn [x] (.strip x)) (.readlines file)))))


(defn split-rules [state line]
    (setv [rules books done] state)
    (if done
        [rules (+ books [(list (map int (.split line ",")))]) True]
        (if (= line "")
            [rules books True]
            (do
                (setv [before after] (map int (.split line "|"))
                    updated (if (in before rules)
                                (| rules {before (.union (get rules before) #{after})})
                                (| rules {before #{after}})))
                [updated books False]))))


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
            (if (and (in (get ordered pageno) rules)
                     (setx wrongset (.intersection (get rules (get ordered pageno))
                                                   (set (cut ordered pageno)))))
                (do (setv wrongone (.pop wrongset)
                          wrongidx (.index ordered wrongone)
                          firstbit (cut ordered wrongidx)
                          secondbit (cut ordered (+ wrongidx 1) (+ pageno 1))
                          lastbit (cut ordered (+ pageno 1) None)
                          newordered (+ firstbit secondbit [wrongone] lastbit))
                    (recur 0 newordered True))
                (recur (+ pageno 1) ordered waswrong)))))


(defmain []
    (setv lines (get-lines FILENAME)
          [rules books _] (reduce split-rules lines [{} [] False])
          part1-ans (sum (map (fn [book] (is-in-order rules book)) books))
          part2-ans (sum (map (fn [book] (fix-order rules book)) books)))
    (print "Answer to part 1:" part1-ans)
    (print "Answer to part 2:" part2-ans))
