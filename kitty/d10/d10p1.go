package main

import (
	"encoding/csv"
	"os"
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

	branchesSlice := make([]Branch, 0)
	width

	for row, element := range records {
		for col, char := range element[0] {
			if char == '.' {
				continue
			}
			c := char - '0'

			// find zeros
			if c == 0 {
				branchesSlice = append(branchesSlice, Branch{coordinate: [2]int{row, col}, index: 0})
			}
		}
	}

	dir := 0
	finished := false
	allFinished := false
	newBranchesAdded := false
	fullLength := 9
	continueBranch := false
	currentId := 0
	lengthOfPrevLoop := 0
	newCol := 0
	newRow := 0
	dirStr := "up"

	for index, branch := range branchesSlice {
		println("branch number:", index)
		println("coordinate:", branch.coordinate[0], branch.coordinate[1])
		println("number at:", branch.index)
		println()
	}

	// Go down each branch
	for !allFinished {

		// keep going until no more branches have been added
		newBranchesAdded = false
		currentLength := len(branchesSlice) - lengthOfPrevLoop

		for i := lengthOfPrevLoop; i < currentLength; i++ {

			for j := 0; j <= fullLength; j++ {

				// move to the next index down this branch
				branchesSlice[i].index++
				dir = 0
				finished = false
				continueBranch = false

				for !finished {
					newRow = branchesSlice[i].coordinate[0]
					newCol = branchesSlice[i].coordinate[1]

					switch dir {
					case 0:
						dirStr = "up"
						newRow--
					case 1:
						dirStr = "down"
						newRow++
					case 2:
						dirStr = "right"
						newCol++
					case 3:
						dirStr = "left"
						newCol--
						finished = true
					}

					dir++
					if newRow < 0 || newCol < 0 {
						continue
					}

					next := records[newRow][0][newCol] - '0'

					if next == '.' {
						continue
					}

					if int(next) == branchesSlice[i].index {
						println(next)
						println(dirStr)

						if !continueBranch {
							continueBranch = true
							branchesSlice[i].coordinate[0] = newRow
							branchesSlice[i].coordinate[1] = newCol
						} else {
							newBranchesAdded = true
							branchesSlice = append(branchesSlice, Branch{coordinate: [2]int{branchesSlice[i].coordinate[0], branchesSlice[i].coordinate[1]}, index: branchesSlice[i].index})
							currentId++
						}
					}
				}
			}
		}

		lengthOfPrevLoop = currentLength

		if !newBranchesAdded && !continueBranch {
			allFinished = true
		}
	}
}
