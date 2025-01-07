package main

import (
	"encoding/csv"
	"os"
	"strconv"
)

func main() {
	d10p1()
}

type Branch struct {
	coordinate [2]int
	index      int
}

func d10p1() {
	// 1. Parse the .txt file
	f, _ := os.Open("test.txt")
	defer f.Close()

	r := csv.NewReader(f)

	records, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	branchesSlice := make([]Branch, 10000)

	currentId := 0
	numberofStarts := 0

	for row, element := range records {
		for col, char := range element {
			c, err := strconv.Atoi(char)

			if err != nil {
				panic(err)
			}

			// find zeros
			if c == 0 {
				branchesSlice = append(branchesSlice, Branch{coordinate: [2]int{row, col}, index: 0})
			}
		}
	}

	dir := 0
	finished := false
	allfinished := false
	newBranchesAdded := false
	continueBranch := false
	lengthOfPrevLoop := 0

	// Go down each branch
	for !allfinished {

		// keep going until no more branches have been added
		newBranchesAdded = false

		for i := lengthOfPrevLoop; i < len(branchesSlice)-lengthOfPrevLoop; i++ {

			newRow := branchesSlice[i].coordinate[0]
			newCol := branchesSlice[i].coordinate[1]

			// move to the next index down this branch
			branchesSlice[i].index++

			for !finished {
				switch dir {
				case 1:
					newRow--
				case 2:
					newRow++
				case 3:
					newCol++
				case 4:
					newCol--
					finished = true
				}

				next, err := strconv.Atoi(records[newRow][newCol])

				if err != nil {
					panic(err)
				}

				if next == branchesSlice[i].index {
					if !continueBranch {
						continueBranch = true
					} else {
						newBranchesAdded = true
						branchesSlice = append(branchesSlice, Branch{coordinate: [2]int{newRow, newCol}, index: branchesSlice[i].index})
						currentId++
					}
				}
			}
		}

		lengthOfPrevLoop = len(branchesSlice) - lengthOfPrevLoop

		if !newBranchesAdded {
			allfinished = true
		}
	}
}
