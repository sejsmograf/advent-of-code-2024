use std::{collections::HashMap, fs::read_to_string};

type Input = (Vec<u32>, Vec<u32>);

pub fn run() {
    let (mut left, mut right) = read_input("../inputs/01.txt");
    left.sort_unstable();
    right.sort_unstable();
    println!("silver: {}", silver(&left, &right));
    println!("gold: {}", gold(&left, &right));
}

fn silver(left: &Vec<u32>, right: &Vec<u32>) -> u32 {
    left.iter()
        .zip(right.iter())
        .fold(0, |acc: u32, e| acc + (e.1.abs_diff(*e.0)))
}

fn gold(left: &Vec<u32>, right: &Vec<u32>) -> u32 {
    let counts = right.iter().fold(HashMap::<u32, u32>::new(), |mut acc, n| {
        acc.entry(*n).and_modify(|e| *e += 1).or_insert(1);
        acc
    });

    left.iter()
        .map(|e| e * counts.get(e).unwrap_or(&0))
        .sum()
}

fn read_input(filename: &str) -> Input {
    let lines: Vec<String> = read_to_string(filename)
        .expect("Failed to read file")
        .lines()
        .map(String::from)
        .collect();

    lines.iter().map(|line| get_numbers(line)).unzip()
}

fn get_numbers(line: &str) -> (u32, u32) {
    let mut numbers = line.split_whitespace();
    let first: u32 = numbers.next().unwrap().parse().expect("SEJSMO");
    let second: u32 = numbers.next().unwrap().parse().expect("GRAF");
    (first, second)
}
