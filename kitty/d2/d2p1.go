package main

import (
	"encoding/csv"
	"os"
	"strconv"
	"strings"
)

func d2p1() {
	// 1. Parse the .txt file
	f, _ := os.Open("input.tsv")
	defer f.Close()

	r := csv.NewReader(f)

	records, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	// FOR EACH LINE
	sum := 0                    // the number of safe records
	var increasing bool = false // whether this row is increasing or decreasing

	for _, s := range records {
		//1. Parse the line
		entries := strings.Split(s[0], " ")

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

			//3. Check differences
			// square so this is never negative (3^2 = 9 so this value is the max permissible)
			if (intE-intPrevE)*(intE-intPrevE) > 9 {
				break
			}

			// 4. if you got all the way to the end with no breaks then add one
			// to the successful sum
			if j == len(entries)-1 {
				sum++
			}
		}

	}
	print("The number of safe reports is: ", sum)
}
