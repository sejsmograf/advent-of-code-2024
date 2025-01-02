mod day01;
mod day02;
mod day03;
mod day04;

use std::{env::args, process::exit};


fn main() {
    parse_args();
}

fn parse_args() {
    if args().len() < 2 {
        eprint!("Usage: {} day\n day: 1-25\n", args().nth(0).unwrap());
        exit(1);
    }

    let day = get_day();

    match day {
        1 => day01::run(),
        2 => day02::run(),
        3 => day03::run(),
        4 => day04::run(),
        _ => {
            eprint!("Day not yet implemented");
            exit(1);
        }
    }
}

fn get_day() -> usize {
    let day = args().nth(1).unwrap();
    match day.parse() {
        Ok(n) => n,
        Err(_) => {
            eprint!("Invalid day number: {}", day);
            exit(1);
        }
    }
}
