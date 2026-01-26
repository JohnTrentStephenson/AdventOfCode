use std::fs::File;
use std::io::prelude::*;
use std::io::{self, BufReader};


fn part1(lines: &[&str]) -> u64 {
    let mut total: u64 = 0;

    for line in lines.iter(){
        let first_max_val = find_max_digit(&line[0..line.len()-1]);
        let first_max_index = find_max_pos(&line, first_max_val);
        let second_max_val = find_max_digit(&line[first_max_index+1..line.len()]);
        let result = first_max_val * 10 + second_max_val;
        total += result as u64;
    }

    total
}

fn part2(lines: &[&str]) -> u64 {
    let mut total: u64 = 0;

    for line in lines.iter(){
        let mut pos = 0;
        let mut result: u64 = 0;
        for i in 1..=12{
            let digits_to_keep = 12 - i;
            let search_end = line.len() - digits_to_keep;

            let search_slice = &line[pos..search_end];
            let val = find_max_digit(search_slice);

            result = result * 10 + val as u64;

            pos = find_max_pos(&line[pos..],val) + pos +1;
        }
        total += result;
    }

    total
}



fn main() {
    let contents = match read_file("input.txt") {
        Ok(c) => c,
        Err(e) => {
            eprintln!("Error reading file {}", e);
            return;
        }
    };

    let lines_vec: Vec<&str> = contents.lines().collect();
    println!("--- Solving Part 1 ---");
    let result_part1 = part1(&lines_vec);
    println!("Part 1 Password: {}",result_part1);
    println!("--- Solving Part 2 ---");
    let result_part2 = part2(&lines_vec);
    println!("Part 2 Password: {}", result_part2);
}

fn find_max_digit(s: &str) -> u32 {
    s.chars()
    .filter_map(|c| c.to_digit(10))
    .max()
    .unwrap()
}

fn find_max_pos(s: &str, digit: u32) -> usize {
    s.chars()
    .position(|c| c.to_digit(10) == Some(digit))
    .unwrap()
}

fn read_file(filename: &str) -> io::Result<String> {
    let file = File::open(filename)?;
    let mut reader = BufReader::new(file);
    let mut contents = String::new();
    reader.read_to_string(&mut contents)?;
    Ok(contents)
}
