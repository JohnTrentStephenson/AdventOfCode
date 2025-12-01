use std::fs::File;
use std::io::prelude::*;

fn main() -> std::io::Result<()> {
    let mut file = File::open("input.txt")?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;
    let lines: Vec<&str> = contents.lines().collect();

    let mut total: i32 = 50;
    let mut count: i32 = 0;

    for line in lines {
        let operation_char = match line.chars().next() {
            Some(c) => c,

            None => {
                println!("Skipping empty line.");
                continue;
            }
        };

        let num_str = &line[1..];

        let number: i32 = match num_str.parse() {
            Ok(num) => num,

            Err(e) => {
                println!("Error parsing number '{}' : {}",num_str,e);
                continue;
            }
        };

        match operation_char {
            'R' => {
                println!("- Adding {} (R{}). To current total: {}", number, number, total);
                total += number;
                while total > 99 {
                    total -= 100;
                    count += 1;
                }
                println!("- Added {} (R{}). New total: {}. New Count: {}", number,number,total,count);
            }
            'L' => {
                println!("- Subtracting {} (L{}). From current total: {}", number, number, total);
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
                println!("- Subtracted {} (L{}). New total: {}. New Count: {}", number,number,total,count);
            }
            _=> {
                println!("- Unknown operation character: '{}'. Skipping line: {}",operation_char,line);
            }
        }
        println!("Total count is: {}",count);
    }
    Ok(())
}
