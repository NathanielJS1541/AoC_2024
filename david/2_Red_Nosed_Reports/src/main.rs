use std::fs::File;
use std::io::{self, BufRead};

fn main() -> io::Result<()> {
    let path = "/home/david/Documents/AoC_2024/david/2_Red_Nosed_Reports/src/input.txt";
    // Open the file
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);

    // Counter for safe lines
    let mut safe_count : i16 = 0;
    let mut part2_count : i16 = 0;

    // Read each line from the file
    for line in reader.lines() {
        let line = line?;
        
        // Find all the values on each line, plop them in a vec and parse as integers
        let str_values: Vec<&str> = line.split_whitespace().collect();
        let values: Vec<i16> = str_values.iter().map(|s| s.parse::<i16>().unwrap()).collect(); // hmm
        

        if check_safe(&values)
        {
            // Increment safe counter
            safe_count += 1;
            
        }
        else {
            // problem dampener, dumb brute force lmao
            for i in 0..values.len() {
                // remove an element
                let mut temp: Vec<i16> = values.to_vec();
                let removed = temp.remove(i);
                
                if check_safe(&temp) 
                { 
                    part2_count += 1;
                    println!("In line {:?}, must remove {removed}", values);
                    break;
                }
            }
        }
    }


    // Print off results :)
    let total = safe_count + part2_count;
    println!("Safe count {safe_count}");
    println!("Like they never happened {part2_count}");
    println!("Total {total}");

    // a-okay :)
    Ok(())
}

fn check_safe(input: &Vec<i16>) -> bool {
    // Determine if line is increasing or decreasing
    let increasing: bool;
    if input.last() > input.first()
    {
        increasing = true; 
    } else {
        increasing = false;
    }

    // Loop through numbers
    for i in 1..input.len() {
        // Calculate difference with previous
        let diff: i16 = input[i] - input[i-1];

        // If identical, too large a change, or wrong direction
        if (diff == 0) || (diff.abs() > 3) || (diff > 0 && increasing == false) || (diff < 0 && increasing == true) 
        {
            return false;
        }
    }

    // Must be good if it got this far!
    return true;
}