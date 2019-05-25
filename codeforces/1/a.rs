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

fn main() -> io::Result<()> {
    let (n, m, a) = parse_line!(i64, i64, i64);
    let res = (n + a - 1) / a * ((m + a - 1) / a);
    println!("{}", res);
    Ok(())
}
