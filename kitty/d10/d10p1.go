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

	f, _ := os.Open("test.txt")
	defer f.Close()

	r := csv.NewReader(f)

	records, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	branchesSlice := make([]Branch, 0)
	branchesMask := make([]bool, 0)
	width := len(records[0][0])

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

				newRow = branchesSlice[i].coordinate[0]
				newCol = branchesSlice[i].coordinate[1]

				for !finished {
					thisRow := newRow
					thisCol := newCol

					switch dir {
					case 0:
						dirStr = "up"
						thisRow--
					case 1:
						dirStr = "down"
						thisRow++
					case 2:
						dirStr = "right"
						thisCol++
					case 3:
						dirStr = "left"
						thisCol--
						finished = true
					}

					dir++

					if thisRow < 0 || thisCol < 0 || thisRow >= width || thisCol >= width {
						continue
					}

					next := records[thisRow][0][thisCol] - '0'

					if next == '.' {
						continue
					}

					if int(next) == branchesSlice[i].index {
						println(next)
						println(dirStr)

						if !continueBranch {
							continueBranch = true
							branchesSlice[i].coordinate[0] = thisRow
							branchesSlice[i].coordinate[1] = thisCol
						} else {
							newBranchesAdded = true
							branchesSlice = append(branchesSlice, Branch{coordinate: [2]int{thisRow, thisCol}, index: branchesSlice[i].index})
							currentId++
						}
					}
				}

				// the current branch died
				if !continueBranch {
					branchesMask[i].append(false)
				}
			}
		}

		lengthOfPrevLoop = currentLength

		if !newBranchesAdded && !continueBranch {
			allFinished = true
		}
	}
}
