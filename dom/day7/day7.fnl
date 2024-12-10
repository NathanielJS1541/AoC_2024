(fn pp [x] (let [fnl (require "fennel")] (print (fnl.view x))))

(local FILENAME "testinput.txt")

(fn parse-line [line]
    (icollect [m (string.gmatch line "(%d+)")] (tonumber m)))

(fn newcat [tbl el]
    (var newtbl (icollect [_ val (ipairs tbl)] val))
    (table.insert newtbl el)
    newtbl)

(fn calculate [operands operators]
    (var acc (. operands 1))
    (for [i 1 (length operators)]
        (case (. operators i)
            "+" (set acc (+ acc (. operands (+ i 1))))
            "*" (set acc (* acc (. operands (+ i 1))))))
    acc)

(calculate [1 2 3 4] [:+ :* :+])

(fn combs [bag sofar level]
     (if (= level 0)
         sofar
         (icollect [_ el (ipairs bag)] (combs bag (newcat sofar el) (- level 1)))))

(combs ["a" "b" "c"] [] 3)

(. (combs ["a" "b" "c"] [] 3) 1 2)

(fn test-ops [target operands operators]
    (= target (calculate operands operators)))

(test-ops 190 [10 19] ["+"])

(fn check-all-ops [target operands allops]
    )

(fn solve-line [nums]
    (let [[target & operands] nums
          numoperands (length operands)
          numoperators (- numoperands 1)
          operators (combs [:* :+] [] numoperators)]
      numoperators))

(solve-line (parse-line "1234: 5 7 8 9"))

(parse-line "1234: 5 6 7 8")


(let [lines (icollect [line (io.lines FILENAME)] line)
      parsed (icollect [_ line (ipairs lines)] (parse-line line))]
    (pp parsed))

