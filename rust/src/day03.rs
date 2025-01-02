use std::fs::read_to_string;

pub fn run() {
    let input = read_input("../inputs/03.txt");
    println!("silver: {}", solve(&input, false));
    println!("silver: {}", solve(&input, true));
}

fn solve(input: &str, gold: bool) -> i32 {
    let mut enabled = true;
    let mut sum: i32 = 0;
    for i in 0..input.len() {
        let mul = check_mul(&input[i..]);
        if mul.is_some() {
            sum += match (gold, enabled) {
                (false, _) => mul.unwrap().0 * mul.unwrap().1,
                (true, true) => mul.unwrap().0 * mul.unwrap().1,
                (true, false) => 0,
            }
        } else if input[i..].starts_with("do()") {
            enabled = true;
        } else if input[i..].starts_with("don't()") {
            enabled = false;
        }
    }

    sum
}

fn check_mul(input: &str) -> Option<(i32, i32)> {
    let closing_bracket = input.find(")");
    if input.len() < 4 || &input[..4] != "mul(" || closing_bracket.is_none() {
        return None;
    };

    let closing_bracket = closing_bracket.unwrap();
    let delim = &input[4..closing_bracket].find(",");
    if delim.is_none() {
        return None;
    }

    if contains_non_numeric(&input[4..closing_bracket]) {
        return None;
    }

    let numbers: Vec<&str> = input[4..closing_bracket].split(",").collect();

    if numbers[0].find(" ").is_some() || numbers[1].find(" ").is_some() {
        return None;
    }

    let parsed_one: i32 = match numbers[0].parse() {
        Ok(n) => n,
        Err(_) => return None,
    };

    let parsed_two: i32 = match numbers[1].parse() {
        Ok(n) => n,
        Err(_) => return None,
    };

    Some((parsed_one, parsed_two))
}

fn contains_non_numeric(input: &str) -> bool {
    for c in input.chars() {
        if !c.is_numeric() && !(c == ',') {
            return true;
        }
    }

    false
}

fn read_input(filename: &str) -> String {
    read_to_string(filename).expect("Failed to read file")
}
