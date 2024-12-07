use std::{fs::{self, File}, io::{self, Read}};

// it's regex time
use regex::Regex;

fn main() -> Result<(), io::Error> {
    let path = "/home/david/Documents/AoC_2024/david/3_Mull_it_over/src/input.txt";
    // Read the file
    let input: String = fs::read_to_string(path)?;

    // named capturing groups a,b refer to the operands
    let expression = Regex::new(r"mul\((?<a>\d+),(?<b>\d+)\)").unwrap();

    // Loop over captures and store pairs of operands
    let mut operands: Vec<(i32, i32)> = vec![];
    for captures in expression.captures_iter(&input) {
        operands.push( ((captures["a"].parse::<i32>().unwrap()), (captures["b"].parse::<i32>().unwrap())) );
    }

    // println!("Operands: {:?}", operands);

    // Multiply and sum results
    let total: i32 = operands.iter().map(|s: &(i32, i32)| s.0 * s.1).sum();

    println!("Total: {}", total);

    // a-ok :)
    return Ok(());
}
