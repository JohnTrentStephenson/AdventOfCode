use std::fs::File;
use std::io::prelude::*;
use std::io::{self, BufReader};

type Lines<'a> = &'a [&'a str];

fn main() -> io::Result<()> {
    let contents = match read_file("input.txt") {
        Ok(c) => c,
        Err(e) => {
            eprintln!("Error reading file {}", e);
            return Err(e);
        }
    };

    let lines_vec: Vec<&str> = contents.split(',').collect();
    println!("--- Solving Part 1 ---");
    let result_part1 = part1(&lines_vec);
    println!("Part 1 Answer: {}",result_part1);
    println!("--- Solving Part 2 ---");
    let result_part2 = part2(&lines_vec);
    println!("Part 2 Answer: {}", result_part2);

    Ok(())

}

fn part1(lines: Lines) -> i64 {
    let mut repeating_numbers: i64 = 0;
    for line in lines.iter() {
        let numb_vec: Vec<&str> = line.split('-').collect();

        if numb_vec.len() == 2 {
            match(numb_vec[0].trim().parse::<i64>(),numb_vec[1].trim().parse::<i64>()) {
                (Ok(start), Ok(end)) => {
                    for number in start..=end {
                        if number_repeats(number) {
                            repeating_numbers += number;
                        }
                    }
                }
                _ => {
                    eprintln!("Error parsing numbers in line: {}", line);
                }
            }
        }
    }

    repeating_numbers
}

fn part2(lines: Lines) -> i64 {
    let mut repeating_numbers: i64 = 0;
    for line in lines.iter() {
        let numb_vec: Vec<&str> = line.split('-').collect();

        if numb_vec.len() == 2 {
            match(numb_vec[0].trim().parse::<i64>(),numb_vec[1].trim().parse::<i64>()) {
                (Ok(start), Ok(end)) => {
                    for number in start..=end {
                        if multiple_repeats(number) {
                            repeating_numbers += number;
                        }
                    }
                }
                _ => {
                    eprintln!("Error parsing numbers in line: {}", line);
                }
            }
        }
    }

    repeating_numbers
}

fn number_repeats(number: i64) -> bool{
    let text = number.to_string();
    let len = text.len();

    if len % 2 != 0{
        return false;
    }

    let half = len /2;

    let left  = &text[0..half];
    let right = &text[half..len];

    if left == right {
        return true;
    }

    false
}

fn multiple_repeats(number: i64) -> bool {
    let text = number.to_string();
    let len = text.len();

    for i in 1..=len /2 {
        if len % i == 0 {
            let unit = &text[0..i];

            let repeat_count = len / i;

            let repeated_text: String = unit.repeat(repeat_count);

            if text == repeated_text {
                return true;
            }
        }
    }

    false
}

fn read_file(filename: &str) -> io::Result<String> {
    let file = File::open(filename)?;
    let mut reader = BufReader::new(file);
    let mut contents = String::new();
    reader.read_to_string(&mut contents)?;
    Ok(contents)
}
