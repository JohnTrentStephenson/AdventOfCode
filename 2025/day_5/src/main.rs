use std::{env, fs, ops::RangeInclusive};

fn main() {
    let mut ids: Vec<u64> = Vec::new();
    let mut ranges: Vec<RangeInclusive<u64>> = Vec::new();

    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        panic!("Please provide a filename: ./main input.txt");
    }

    let contents = fs::read_to_string(&args[1]).expect("Failed to read the file.");

    println!("---Contents---\n {}", contents);

    if let Some((ranges_input,id_input)) = contents.split_once("\n\n") {
        ranges = ranges_input
            .lines()
            .filter_map(|line| {
                let (start_str, end_str) = line.split_once('-')?;
                let start = start_str.parse().ok()?;
                let end = end_str.parse().ok()?;
                Some(start..=end)
            })
            .collect();
        ids = id_input
            .split_whitespace()
            .filter_map(|s| s.parse().ok())
            .collect();
    }

    println!("---IDs---\n{:#?}",ids);
    println!("---Ranges---\n{:#?}", ranges);

    ranges.sort_by(|a, b| a.start().cmp(b.start()));
    let merged_ranges = ranges
        .into_iter()
        .fold(Vec::new(),|mut acc: Vec<RangeInclusive<u64>>,next| {
            if let Some(last) = acc.last_mut() {
                if next.start() <= last.end() {
                    if next.end() > last.end() {
                        *last = *last.start()..=*next.end();
                    }
                } else {
                    acc.push(next);
                }
            } else {
                acc.push(next);
            }
            acc
        });

    let valid_ids: Vec<u64> = ids
        .into_iter()
        .filter(|id| {
            merged_ranges.iter().any(|range| range.contains(id))
        })
        .collect();
    println!("---Merged Ranges---\n{:#?}", merged_ranges);
    println!("---Valid IDs---\n{:#?}", valid_ids);
    //println!("---Merged Ranges---\n{:#?}");
    println!("Part 1: {}", part_1(valid_ids));
    println!("Part 2: {}", part_2(merged_ranges));
}

fn part_1(valid_ids: Vec<u64>) -> usize {

    valid_ids.len()

}

fn part_2(ranges: Vec<RangeInclusive<u64>>) -> u64 {
    ranges
        .iter()
        .map(|range| (range.end()-range.start()) +1)
        .sum()
}
