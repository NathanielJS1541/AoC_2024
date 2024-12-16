package main

import (
	"encoding/csv"
	"os"
	"strconv"
	"strings"
	"time"
)

func main() {
	// d2p1()
	d2p2()
}

func timer(name string) func() {
	start := time.Now()
	return func() {
		print(name, " took ", time.Since(start), "\n")
	}
}

// checks every permutation of the set but is slow
func checkSetSimple(entriesOrg []string) bool {

	increasing := false
	for i := range entriesOrg {
		entries := make([]string, len(entriesOrg))
		copy(entries, entriesOrg)
		entries = append(entries[0:i], entries[i+1:]...)

		for j := 1; j < len(entries); j++ {

			intE, _ := strconv.Atoi(entries[j])
			intPrevE, _ := strconv.Atoi(entries[j-1])

			// Check if they are equal (no increase or decrease)
			if intE == intPrevE {
				break
			}

			if j == 1 {
				increasing = false
				if intE > intPrevE {
					increasing = true
				}
			}

			if increasing {
				if intE < intPrevE {
					break
				}
			} else {
				if intE > intPrevE {
					break
				}
			}

			//Check differences
			// square so this is never negative (3^2 = 9 so this value is the max permissible)
			if (intE-intPrevE)*(intE-intPrevE) > 9 {
				break
			}

			// if you got all the way to the end with no breaks then add one
			// to the successful sum
			if j == len(entries)-1 {
				return true
			}
		}
	}

	return false
}

func checkSetSimpleCaller(records [][]string) {
	defer timer("Simple check set")()

	sum := 0
	for _, s := range records {

		entries := strings.Split(s[0], " ")
		if checkSetSimple(entries) {
			sum++
		} else {
		}
	}

	print("The number of safe reports is: ", sum, "\n")

}

// checks a given set until more than one mistake is made.
func checkSetBetter(entries []string, badLevels int) (bool, int) {

	var intPrevE int
	increasing := false
	skipIndex := 0

	for j := 1; j < len(entries); j++ {
		intE, _ := strconv.Atoi(entries[j])

		if skipIndex == j-1 && j != 1 {
			intPrevE, _ = strconv.Atoi(entries[j-2])
		} else {
			intPrevE, _ = strconv.Atoi(entries[j-1])
		}

		if (intPrevE-intE)*(intPrevE-intE) > 9 || intPrevE == intE {
			badLevels++
			skipIndex = j
			if badLevels == 2 {
				return false, skipIndex
			} else {
				continue
			}
		}

		// Check if increasing or decreasing
		if j == 1 {
			increasing = false
			if intE > intPrevE {
				increasing = true
			}
		}

		if increasing {
			if intE < intPrevE {
				badLevels++
				skipIndex = j
				if badLevels == 2 {
					return false, skipIndex
				} else {
					continue
				}
			}
		} else {
			if intE > intPrevE {
				badLevels++
				skipIndex = j
				if badLevels == 2 {
					return false, skipIndex
				} else {
					continue
				}
			}
		}
	}
	return true, skipIndex
}

// Call the faster function. This will continue once two bad records are
// found minimising loops. However both permutations of the first two numbers
// i.e. the permissable bad record was in the 0th position and the permissable
// bad record was in the 1st position should be tried is the test fails at the
// index indicating the bad record was there.
func checkSetBetterCaller(records [][]string) {
	defer timer("Better check set")()
	sum := 0
	for _, s := range records {

		entries := strings.Split(s[0], " ")

		success, index := checkSetBetter(entries, 0)
		if success {
			sum++
			// if the index indicates the bad record is in the first or second position
		} else if index == 2 || index == 3 {
			// try the bad record in position 0
			success, _ = checkSetBetter(entries[1:], 1)
			if success {
				sum++
			} else {
				// try the bad record in position 1
				success, _ = checkSetBetter(append(entries[:1], entries[2:]...), 1)
				if success {
					sum++
				}
			}
		}
	}

	print("The number of safe reports is: ", sum, "\n")
}

func d2p2() {

	// Parse the .txt file
	f, _ := os.Open("input.tsv")
	defer f.Close()

	r := csv.NewReader(f)

	records, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	checkSetSimpleCaller(records)

	checkSetBetterCaller(records)

}
