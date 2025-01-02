use std::fs::read_to_string;

pub fn run() {
    let input = read_input("../inputs/02.txt");
    println!("silver: {}", silver(&input));
    println!("gold: {}", gold(&input));
}

fn silver(input: &Vec<Vec<i32>>) -> usize {
    input.iter().filter(|e| check_report(e)).count()
}

fn gold(input: &Vec<Vec<i32>>) -> usize {
    input.iter().filter(|e| check_report_gold(e)).count()
}

fn check_report_gold(report: &Vec<i32>) -> bool {
    for i in 0..report.len() {
        let mut copy = report.clone();
        copy.remove(i);
        if check_report(&copy) {
            return true;
        }
    }

    false
}

fn check_report(report: &Vec<i32>) -> bool {
    let increasing = report[0] < report[1];

    for i in 0..report.len() - 1 {
        let diff = report[i] - report[i + 1];
        if increasing && (diff < -3 || diff > -1) {
            return false;
        } else if !increasing && (diff < 1 || diff > 3) {
            return false;
        }
    }

    true
}

fn read_input(filename: &str) -> Vec<Vec<i32>> {
    let lines: Vec<String> = read_to_string(filename)
        .expect("Failed to read file")
        .lines()
        .map(String::from)
        .collect();

    lines.iter().map(|line| get_numbers(line)).collect()
}

fn get_numbers(line: &str) -> Vec<i32> {
    line.split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect()
}
