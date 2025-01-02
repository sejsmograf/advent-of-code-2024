use std::fs::read_to_string;

pub fn run() {
    let input = read_input("../inputs/04.txt");
    println!("silver: {}", silver(&input));
    println!("gold: {}", gold(&input));
}

fn silver(input: &Vec<String>) -> i32 {
    let mut sum = 0;
    for row in 0..input.len() {
        for (col, ch) in input[row].chars().enumerate() {
            if ch == 'X' {
                sum += check_xmas((row as i32, col as i32), input);
            }
        }
    }
    sum
}

fn gold(input: &Vec<String>) -> i32 {
    let mut sum = 0;
    for row in 1..input.len() - 1 {
        for (col, ch) in input[row].chars().enumerate() {
            if ch == 'A' {
                sum += check_mas((row as i32, col as i32), input) as i32;
            }
        }
    }
    sum
}

fn check_mas(pos: (i32, i32), source: &Vec<String>) -> bool {
    let directions = [(1, 1), (-1, -1), (1, -1), (-1, 1)];
    let opposite = [(-1, -1), (1, 1), (-1, 1), (1, -1)];

    for i in 0..directions.len() {
        let new_pos = (pos.0 + directions[i].0, pos.1 + directions[i].1);
        let pos_opposite = (pos.0 + opposite[i].0, pos.1 + opposite[i].1);
        let one = match source[new_pos.0 as usize].chars().nth(new_pos.1 as usize) {
            Some(n) => n,
            None => {
                return false;
            }
        };

        let two = match source[pos_opposite.0 as usize]
            .chars()
            .nth(pos_opposite.1 as usize)
        {
            Some(n) => n,
            None => {
                return false;
            }
        };

        if !((one == 'M' && two == 'S') || (one == 'S' && two == 'M')) {
            return false;
        }
    }
    true
}

fn check_xmas(pos: (i32, i32), source: &Vec<String>) -> i32 {
    let mut sum = 0;
    let search_string = "XMAS";
    let directions: [(i32, i32); 8] = [
        (-1, -1),
        (-1, 0),
        (-1, 1),
        (0, -1),
        (0, 1),
        (1, -1),
        (1, 0),
        (1, 1),
    ];

    for d in directions {
        for letter_idx in 1..search_string.len() {
            let new_pos = (
                pos.0 + (letter_idx as i32) * d.0,
                pos.1 + (letter_idx as i32) * d.1,
            );
            if new_pos.0 < 0
                || new_pos.0 >= source.len() as i32
                || new_pos.1 < 0
                || new_pos.1 >= source[0].len() as i32
            {
                break;
            }
            if source[new_pos.0 as usize].chars().nth(new_pos.1 as usize)
                != search_string.chars().nth(letter_idx)
            {
                break;
            }
            if letter_idx == search_string.len() - 1 {
                sum += 1;
            }
        }
    }
    sum
}

fn read_input(filename: &str) -> Vec<String> {
    read_to_string(filename)
        .expect("Failed to read file")
        .lines()
        .map(String::from)
        .collect()
}
