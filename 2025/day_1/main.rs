use std::fs::File;
use std::io::prelude::*;
use std::io::{self, BufReader};

type Lines<'a> = &'a [&'a str];

fn part1(lines: Lines) -> i32 {
    let mut total: i32 = 50;
    let mut count: i32 = 0;

    for line in lines.iter() {
        let operation_char = match line.chars().next() {
            Some(c) => c,
            None => continue,
        };

        let num_str = &line[1..];
        let number: i32 = match num_str.parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        match operation_char {
            'R' => {
                total += number % 100;
            }
            'L' => {
                total -= number % 100;
            }
            _=> {}
        }

        if total > 99 {
            total -= 100;
        }
        else if total < 0 {
            total += 100;
        }

        if total == 0 {
            count += 1;
        }
    }

    count

}

fn part2(lines: Lines) -> i32 {
    let mut total: i32 = 50;
    let mut count: i32 = 0;

    for line in lines.iter() {
        let operation_char = match line.chars().next() {
            Some(c) => c,
            None => continue,
        };

        let num_str = &line[1..];
        let number: i32 = match num_str.parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        match operation_char {
            'R' => {
                total += number;
                while total > 99 {
                    total -= 100;
                    count += 1;
                }
            }
            'L' => {
                let started_at_zero = total == 0;

                total -= number;


                while total < 0 {
                    total += 100;
                    count += 1;
                }

                if total == 0 {
                    count += 1;
                }

                if started_at_zero && total != 0 {
                    count -= 1;
                }
            }
            _=> {}
        }
    }

    count
}

fn main() -> io::Result<()> {
    let contents = match read_file("input.txt") {
        Ok(c) => c,
        Err(e) => {
            eprintln!("Error reading file {}", e);
            return Err(e);
        }
    };

    let lines_vec: Vec<&str> = contents.lines().collect();
    println!("--- Solving Part 1 ---");
    let result_part1 = part1(&lines_vec);
    println!("Part 1 Password: {}",result_part1);
    println!("--- Solving Part 2 ---");
    let result_part2 = part2(&lines_vec);
    println!("Part 2 Password: {}", result_part2);

    Ok(())

}

fn read_file(filename: &str) -> io::Result<String> {
    let file = File::open(filename)?;
    let mut reader = BufReader::new(file);
    let mut contents = String::new();
    reader.read_to_string(&mut contents)?;
    Ok(contents)
}
