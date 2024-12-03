use std::fs::File;
use std::io::{self, BufRead};

fn main() -> io::Result<()> {
    let path = "C:/Users/wayzda01/Documents/GitHub/AoC_2024/david/2_Red_Nosed_Reports/src/input.txt";
    // Open the file
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);

    // Counter for safe lines
    let mut count : i16 = 0;

    // Read each line from the file
    for line in reader.lines() {
        let line = line?;
        
        // Find all the values on each line, plop them in a vec and parse as integers
        let str_values: Vec<&str> = line.split_whitespace().collect();
        let values: Vec<&i16> = str_values.iter(); // hmm
        
        // Loop through the values, break
        //  if numbers are not all increasing or decreasing
        //  if two adjacent levels differ by at least < 1 or > 3

        // Increment safe counter

    }

    // a-okay :)
    Ok(())
}
