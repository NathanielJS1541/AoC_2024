(local FILENAME "input.txt")

(fn parse-line [line]
    "Extracts numbers from a line"
    (icollect [m (string.gmatch line "(%d+)")] (tonumber m)))

(fn calculate [operands operators]
    "Calculates the result of applying the operators to the operands"
    (var acc (. operands 1))
    (for [i 1 (length operators)]
        (let [nextnum (. operands (+ i 1))]
            (case (. operators i)
                "||" (set acc (tonumber (.. (tostring acc) (tostring nextnum))))
                "+" (set acc (+ acc nextnum))
                "*" (set acc (* acc nextnum)))))
    acc)

(fn incarr [inarr maxsym]
    "Increments a number represented by an array of digits, in base `maxsym`"
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


(fn solve-line [nums operators]
    "Determines if a target can be reached with a combination of given operators"
    (let [[target & operands] nums
        numoperands (length operands)
        numoperators (- numoperands 1)
        maxsym (- (length operators) 1)]
        (var possible false)
        (var carried false)
        (var arr (fcollect [_ 1 numoperators] 0))
        (while (and (not possible) (not carried))
            (when (not carried)
                (let [mapped (icollect [_ idx (ipairs arr)] (. operators (+ idx 1)))]
                    (set possible (= target (calculate operands mapped))))
            (set [arr carried] (incarr arr maxsym))))
        (if possible target 0)))

(let [lines (icollect [line (io.lines FILENAME)] line)
      parsed (icollect [_ line (ipairs lines)] (parse-line line))
      solved-p1 (icollect [_ nums (ipairs parsed)] (solve-line nums [:* :+]))
      part1-ans (accumulate [sum 0 _ val (ipairs solved-p1)] (+ sum val))
      solved-p2 (icollect [_ nums (ipairs parsed)] (solve-line nums [:* :+ :||]))
      part2-ans (accumulate [sum 0 _ val (ipairs solved-p2)] (+ sum val))]
    (print "Part 1 answer:" (string.format "%18d" part1-ans))
    (print "Part 2 answer:" (string.format "%18d" part2-ans)))

