package main

import (
	"encoding/csv"
	"os"
	"strconv"
	"strings"
)

func d2p2t2() {
	// 1. Parse the .txt file
	f, _ := os.Open("test.tsv")
	defer f.Close()

	diff := make([]int, 0)

	r := csv.NewReader(f)

	records, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	for _, s := range records {
		//1. Parse the line
		entries := strings.Split(s[0], " ")
		diff = make([]int, 0)

		for j := 1; j < len(entries); j++ {

			intE, _ := strconv.Atoi(entries[j])
			intPrevE, _ := strconv.Atoi(entries[j-1])

			// 1. Determine which numbers to look at.

			// get the differences between all the numbers
			diff = append(diff, intE-intPrevE)
		}
	}
	print(diff)
}
