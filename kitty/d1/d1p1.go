package main

import (
	"encoding/csv"
	"os"
	"sort"
	"strconv"
	"strings"
)

func d1p1() {
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

	// 2. Order the list
	sort.Ints(slice1)
	sort.Ints(slice2)

	// 3. Find the sum of the distances
	sum := 0
	difference := 0
	for i := range slice1 {
		difference = slice1[i] - slice2[i]

		if difference < 0 {
			difference = -difference
		}

		sum += difference
	}

	print("The sum is: ", sum)
}
