use std::fs;
use std::io;

// it's regex time
use regex::Regex;

fn main() -> Result<(), io::Error> {
    let path = "/home/david/Documents/AoC_2024/david/3_Mull_it_over/src/input.txt";
    // Read the file
    let input: String = fs::read_to_string(path)?;

    // Part 1
    let operands = find_pairs_in_string(&input);
    // Multiply and sum results
    let total: i32 = operands.iter().map(|s: &(i32, i32)| s.0 * s.1).sum();

    println!("Part 1 Total: {}", total);

    // Part 2

    // This expression selects everything we want to remove from the input
    let expression = Regex::new(r"(don't\(\))[\s\S]*?((do\(\))|$)").unwrap();
    let filtered: String = expression.replace_all(&input, "").to_string();
    
    let operands_2 = find_pairs_in_string(&filtered);
    // Multiply and sum results
    let total_2: i32 = operands_2.iter().map(|s: &(i32, i32)| s.0 * s.1).sum();

    println!("Part 2 Total: {}", total_2);


    // a-ok :)
    return Ok(());
}

fn find_pairs_in_string(haystack: &String) -> Vec<(i32, i32)> {
    // named capturing groups a,b refer to the operands
    let expression = Regex::new(r"mul\((?<a>\d+),(?<b>\d+)\)").unwrap();

    // Loop over captures and store pairs of operands
    let mut results: Vec<(i32, i32)> = vec![];
    for captures in expression.captures_iter(&haystack) {
        results.push( ((captures["a"].parse::<i32>().unwrap()), (captures["b"].parse::<i32>().unwrap())) );
    }

    return results;
}
