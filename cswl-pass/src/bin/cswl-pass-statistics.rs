use std::{collections::HashMap, io::stdin};

use prettytable::{format::consts::FORMAT_NO_LINESEP_WITH_TITLE, row, Table};

fn dump_info_for_window_size(opcodes: &[String], window_len: usize) {
    let mut frequency_map = HashMap::new();

    for window in opcodes.windows(window_len) {
        frequency_map
            .entry(window)
            .and_modify(|e| *e += 1)
            .or_insert(1);
    }

    let mut sorted_map = frequency_map.iter().collect::<Vec<_>>();
    sorted_map.sort_by_key(|(&_, &count)| count);

    let mut table = Table::new();

    table.set_titles(row![
        format!("Opcode windows of length {}", window_len),
        "Count"
    ]);

    for (&window, &count) in sorted_map.iter().rev() {
        let opcode_name = format!("[{}]", window.join(", "));
        table.add_row(row![opcode_name.to_lowercase(), count]);
    }

    table.set_format(*FORMAT_NO_LINESEP_WITH_TITLE);
    println!("_snip_here_");
    table.printstd();
}

fn main() -> Result<(), std::io::Error> {
    let opcodes_or_err: Result<Vec<_>, _> = stdin()
        .lines()
        .map(|l| l.map(|s| s.trim().to_string()))
        .collect();

    let opcodes = opcodes_or_err?;

    const MIN_WINDOW_LENGTH: usize = 1;
    const MAX_WINDOW_LENGTH: usize = 5;

    for len in MIN_WINDOW_LENGTH..=MAX_WINDOW_LENGTH {
        dump_info_for_window_size(&opcodes, len);
    }

    Ok(())
}
