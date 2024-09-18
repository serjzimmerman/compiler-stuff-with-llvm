use compiler_stuff_with_llvm::graphics::{
    sim_exit, sim_flush, sim_init, sim_put_pixel, sim_rand, SIM_X_SIZE, SIM_Y_SIZE,
};

use itertools::iproduct;

struct Board {
    pub pixels: [[bool; SIM_X_SIZE as usize]; SIM_Y_SIZE as usize],
}

const ALIVE_COLOR: u32 = 0xff00ff00;
const DEAD_COLOR: u32 = 0xffff0000;

impl Board {
    #[no_mangle]
    pub fn new() -> Self {
        Board {
            pixels: [[false; SIM_X_SIZE as usize]; SIM_Y_SIZE as usize],
        }
    }

    #[no_mangle]
    pub fn randomly_fill(self: &mut Self) {
        for (x, y) in iproduct!(0..SIM_X_SIZE as usize, 0..SIM_Y_SIZE as usize) {
            self.pixels[y][x] = sim_rand() & 1 == 1;
        }
    }

    #[no_mangle]
    fn get_next_cell_state(self: &Self, x_center: usize, y_center: usize) -> bool {
        let wrap_coord = |coord: i32, size: usize, offset: i32| {
            (coord + offset).rem_euclid(size as i32) as usize
        };

        let mut alive_neighbours = 0;
        for (x_off, y_off) in iproduct!(-1..=1, -1..=1) {
            if x_off == 0 && y_off == 0 {
                continue;
            }

            let x = wrap_coord(x_center as i32, SIM_X_SIZE as usize, x_off);
            let y = wrap_coord(y_center as i32, SIM_Y_SIZE as usize, y_off);

            alive_neighbours += if self.pixels[y][x] { 1 } else { 0 };
        }

        if self.pixels[y_center][x_center] {
            alive_neighbours == 3
                || alive_neighbours == 4
                || alive_neighbours == 6
                || alive_neighbours == 7
                || alive_neighbours == 8
        } else {
            alive_neighbours == 3
                || alive_neighbours == 6
                || alive_neighbours == 7
                || alive_neighbours == 8
        }
    }

    #[no_mangle]
    pub fn iterate(self: &mut Self, other: &Self) {
        for (x, y) in iproduct!(0..SIM_X_SIZE as usize, 0..SIM_Y_SIZE as usize) {
            self.pixels[y][x] = other.get_next_cell_state(x, y);
        }
    }

    #[no_mangle]
    pub fn draw(self: &Self) {
        for (x, y) in iproduct!(0..SIM_X_SIZE as usize, 0..SIM_Y_SIZE as usize) {
            sim_put_pixel(
                x as u16,
                y as u16,
                if self.pixels[y][x] {
                    ALIVE_COLOR
                } else {
                    DEAD_COLOR
                },
            )
        }
    }
}

fn main() {
    sim_init();

    let mut board1 = Board::new();
    let mut board2 = Board::new();

    board1.randomly_fill();

    let mut board_index = 0;

    for _step in 0..1000 {
        if board_index == 0 {
            board1.draw();
            sim_flush();
            board2.iterate(&board1);
        } else {
            board2.draw();
            sim_flush();
            board1.iterate(&board2);
        }

        board_index = (board_index + 1) % 2;
    }

    sim_exit();
}
