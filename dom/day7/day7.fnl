(fn pp [x] (let [fnl (require "fennel")] (print (fnl.view x))))

(local FILENAME "input.txt")

(fn parse-line [line]
    (icollect [m (string.gmatch line "(%d+)")] (tonumber m)))

(fn calculate [operands operators]
    (var acc (. operands 1))
    (for [i 1 (length operators)]
        (case (. operators i)
            "+" (set acc (+ acc (. operands (+ i 1))))
            "*" (set acc (* acc (. operands (+ i 1))))))
    acc)

(fn incarr [inarr maxsym]
    (var arr (icollect [_ el (ipairs inarr)] el))
    (var carry true)
    (var idx (length arr))
    (while (and carry (> idx 0))
        (tset arr idx (+ (. arr idx) 1))
        (if (> (. arr idx) maxsym)
            (do
              (tset arr idx 0)
              (set carry true))
            (set carry false))
        (set idx (- idx 1)))
    [arr carry])

(fn countall [inarr maxsym]
    (var arr [(icollect [_ el (ipairs inarr)] el)])
    (var carry false)
    (var inced [])
    (while (not carry)
        (set [inced carry] (incarr (. arr (length arr)) maxsym))
        (when (not carry)
            (table.insert arr inced)))
    arr)

(fn combs [syms num]
    (let [startarr (fcollect [_ 1 num] 0)
        counted (countall startarr (- (length syms) 1))
        mapped (icollect [i arr (ipairs counted)]
            (icollect [i el (ipairs arr)] (. syms (+ el 1))))]
        mapped))

(combs [:* :+] 4)

(fn test-ops [target operands operators]
    (= target (calculate operands operators)))

(test-ops 190 [19 10] [:+ :*])

(fn check-all-ops [target operands allops]
    (let [[firstops & restops] allops]
        (if (not firstops)
            false
            (if (test-ops target operands firstops)
                true
                (check-all-ops target operands restops)))))

(fn solve-line [nums operators]
    (let [[target & operands] nums
          numoperands (length operands)
          numoperators (- numoperands 1)
          allops (combs operators numoperators)
          possible (check-all-ops target operands allops)]
      (if possible target 0)))


(let [lines (icollect [line (io.lines FILENAME)] line)
      parsed (icollect [_ line (ipairs lines)] (parse-line line))
      solved-p1 (icollect [_ nums (ipairs parsed)] (solve-line nums [:* :+]))
      part1-ans (accumulate [sum 0 _ val (ipairs solved-p1)] (+ sum val))
      solved-p2 (icollect [_ nums (ipairs parsed)] (solve-line nums [:* :+ :||]))]
    (pp solved-p2)
    (print "Part 1 answer:" part1-ans))

