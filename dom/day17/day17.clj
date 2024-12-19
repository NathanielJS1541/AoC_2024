(require '[clojure.pprint :refer [pprint]])

(def INFILE "testinput.txt")

(defn get-lines "Gets the lines from a filename"
    [filename]
    (with-open [reader (clojure.java.io/reader filename)]
        (into [] (line-seq reader))))

(defn init-state
    [lines]
    {:a (Integer/parseInt (re-find #"\d+" (lines 0)))
     :b (Integer/parseInt (re-find #"\d+" (lines 1)))
     :c (Integer/parseInt (re-find #"\d+" (lines 2)))
     :pc 0
     :code (mapv #(Integer/parseInt %) (re-seq #"\d+" (lines 4)))})

(defn get-combo
    [state operand]
    (cond
        (<= 0 operand 3) operand
        (= operand 4) (state :a)
        (= operand 5) (state :b)
        (= operand 6) (state :c)))

(get-combo {:a 729, :b 0, :c 0, :pc 0, :code [0 1 5 4 3 0]} -1)

(defn adv
    [state operand]
    (let [combo (get-combo state operand)]
        combo))

(adv {:a 729, :b 0, :c 0, :pc 0, :code [0 1 5 4 3 0]} 7)

(let [state (init-state (get-lines INFILE))]
    (pprint state)
    (pprint (adv state 1)))
