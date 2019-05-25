use std::io;

macro_rules! parse_line {
    ($($t: ty), +) => {{
        let mut a_line = String::new();
        io::stdin().read_line(&mut a_line).expect("read error");
        let mut a_line_iter = a_line.split_whitespace();
        (
            $(
                a_line_iter.next().unwrap().parse::<$t>().expect("parse error"),
            )+
        )
    }};
}

macro_rules! read_line {
    () => {{
        let mut a_line = String::new();
        io::stdin().read_line(&mut a_line).expect("read error");
        // Remove newline
        a_line.pop();
        a_line.trim().to_string()
    }};
}

enum Coordinate {
    //YX(i32, i32),
    RXCY(i32, i32),
}

fn parse_rxcy(s: &String) -> Option<Coordinate> {
    if &s[0..1] != "R" {
        return None;
    }

    let mut pos_c: usize = 0;
    for (_i, c) in s[1..].char_indices() {
        if '0' <= c && c <= '9' {
            continue;
        }
        if c != 'C' {
            return None;
        }
        if pos_c != 0 {
            return None;
        }
        pos_c = _i + 1;
    }

    if pos_c > 1 {
        // Add 1 to map index from s[1..] to s
        let row = s[1..pos_c].parse::<i32>().expect("parse error");
        let col = s[pos_c + 1..].parse::<i32>().expect("parse error");
        return Some(Coordinate::RXCY(row, col));
    }

    return None;
}

fn convert_column_to_string(_col: &i32) -> String {
    let mut col = _col.clone();
    let mut res = String::new();
    loop {
        col = col - 1;
        let x = col % 26;
        res.push(('A' as u8 + x as u8) as char);
        col = col / 26;
        if col <= 0 {
            break;
        }
    }
    return res.chars().rev().collect::<String>();
}

fn translate(s: &String) -> String {
    assert!(s.len() >= 2);

    if let Some(Coordinate::RXCY(row, _col)) = parse_rxcy(s) {
        return convert_column_to_string(&_col) + row.to_string().as_str();
    }

    let mut col: i32 = 0;
    for (i, c) in s[..].char_indices() {
        if c.is_ascii_digit() {
            return "R".to_string() + &s[i..] + "C" + col.to_string().as_str();
        }
        col = col * 26 + (c as u8 - 'A' as u8 + 1) as i32;
    }
    unreachable!()
}

fn main() {
    let (mut n,) = parse_line!(i32);
    while n > 0 {
        let line = read_line!();
        println!("{}", translate(&(line.trim().to_string())));
        n -= 1;
    }
}
