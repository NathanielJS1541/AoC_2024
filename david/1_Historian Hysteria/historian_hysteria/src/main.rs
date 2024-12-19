use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::vec::Vec;

fn main() -> io::Result<()> {
    // I've never used Rust before, bear with me

    // Path to the file
    let path = "/home/david/Documents/AoC_2024/david/1_Historian Hysteria/input.txt";

    // Open the file
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);

    // Vecs to store the numbers from each column
    let mut column_1: Vec<i32> = Vec::new();
    let mut column_2: Vec<i32> = Vec::new();

    // Read each line from the file
    for line in reader.lines() {
        let line = line?;
        
        // Split the line into two parts and parse them as integers
        let parts: Vec<&str> = line.split_whitespace().collect();
        let num1: i32 = parts[0].parse().unwrap();
        let num2: i32 = parts[1].parse().unwrap();
        
        // Push the numbers to their respective columns
        column_1.push(num1);
        column_2.push(num2);
    
    }

    // Sort the columns :)
    column_1.sort();
    column_2.sort();

    // Print the sorted columns
    //println!("Sorted Column 1: {:?}", column_1);
    //println!("Sorted Column 2: {:?}", column_2);


    // Holy moly what is this?
    // iterate through column 1, zipping it up into pairs with column 2. Perform abs(a - b) on each pair
    let distances: Vec<i32> = column_2.iter().zip(column_1.iter()).map(|(a, b)| (a - b).abs()).collect();
    
    // Now sum up the individual distances and print to console
    let total_distance : i32 = distances.iter().sum();
    println!("Total difference: {total_distance}");

    // Now for part 2
    // Step 1: Create a HashMap (dictionary) to count occurrences of each value in column_2
    let mut frequency_map: HashMap<i32, usize> = HashMap::new();
    for &value in &column_2 {
        *frequency_map.entry(value).or_insert(0) += 1;
    }

    let mut similarity_score : i32 = 0;
    // Step 2: For each value in column_1, find how many times it appears in column_2
    for &value in &column_1 {
        let count = frequency_map.get(&value).unwrap_or(&0);
        //println!("Value {} appears {} times in column_2", value, count);
        similarity_score += value * (*count as i32);
    }

    println!("Similarity score: {similarity_score}");

    // a-okay :)
    Ok(())
}