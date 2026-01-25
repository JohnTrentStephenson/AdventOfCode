use std::fs::File;
use std::io::prelude::*;
use std::io::{self, BufReader};

const SYMBOL : char = '@';
const MAX_NEIGHBORS: u8 = 4;

fn main() {
    let contents = match read_file("input.txt") {
        Ok(c) => c,
        Err(e) => {
            eprintln!("Error reading file {}", e);
            return
        }
    };

    let mut grid: Vec<Vec<char>> = contents
        .lines()
        .map(|line| line.chars().collect())
        .collect();

    println!("Part 1: {}", part_1(&grid));
    println!("Part 2: {}", part_2(&mut grid));
}

fn part_1(grid: &Vec<Vec<char>>) -> u32 {
    let mut count : u32 = 0;

    for r in 0..grid.len() {
        for c in 0..grid[r].len() {
            if grid[r][c] == SYMBOL && check_neighbors(grid,r,c) {
                count += 1;
            }
        }
    }
    count
}

fn part_2(grid: &mut Vec<Vec<char>>) -> u32 {
    let mut removed: bool = true;
    let mut count: u32 = 0;

    while removed {
        removed = false;
        for r in 0..grid.len(){
            for c in 0..grid[r].len(){
                if grid[r][c] == SYMBOL && check_neighbors(grid,r,c){
                    grid[r][c] = 'x';
                    count +=1;
                    removed = true;
                }
            }
        }
    }
    count
}

fn check_neighbors(grid: &Vec<Vec<char>>, row: usize, col: usize) -> bool{
    let mut count = 0;

    let row_start = row.saturating_sub(1);
    let row_end = std::cmp::min(row + 1, grid.len() - 1);

    for r in row_start..=row_end{
        let col_start = col.saturating_sub(1);
        let col_end = std::cmp::min(col + 1, grid[r].len() - 1);

        for c in col_start..=col_end{
            if r==row && c == col{
                continue;
            }

            if grid[r][c] == SYMBOL{
                count += 1;

                if count >= MAX_NEIGHBORS {
                    return false;
                }
            }
        }
    }
    true
}

fn read_file(filename: &str) -> io::Result<String> {
    let file = File::open(filename)?;
    let mut reader = BufReader::new(file);
    let mut contents = String::new();
    reader.read_to_string(&mut contents)?;
    Ok(contents)
}
