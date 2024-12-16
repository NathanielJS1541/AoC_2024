package main

import (
	"encoding/csv"
	"os"
	"sort"
	"strconv"
	"strings"
)

func main() {
	// d1p1()
	d1p2()
}

func d1p2() {
	// 1. Parse the .txt file
	f, _ := os.Open("input.tsv")
	defer f.Close()

	r := csv.NewReader(f)

	records, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	slice1 := make([]int, len(records), len(records))
	slice2 := make([]int, len(records), len(records))

	for i, s := range records {

		slice1[i], _ = strconv.Atoi(strings.Split(s[0], "   ")[0])
		slice2[i], _ = strconv.Atoi(strings.Split(s[0], "   ")[1])
	}

	// 2. Order the slice
	sort.Ints(slice1)
	sort.Ints(slice2)

	// 3. Find the matching numbers

	slice2index := 0 // keep a record of where we are in slice2
	sum := 0
	started := false
	//index through slice1
	for _, s1 := range slice1 {
		score := 0
		currentIndex := slice2index
		started = false

		//starting at the currrent index in slice2 see if any match
		for j, s2 := range slice2[currentIndex:] {
			if s1 == s2 {
				slice2index = j
				score += 1
				started = true
			} else if started {
				sum += s1 * score
				break
			}
		}
	}

	print("The sum is: ", sum)

}
