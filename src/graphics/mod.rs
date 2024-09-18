pub const FRAME_TICKS: u32 = 50;
pub const SIM_X_SIZE: u32 = 512;
pub const SIM_Y_SIZE: u32 = 256;

use rand::Rng;
use sdl2::event::Event;
use sdl2::pixels::Color;
use sdl2::rect::Point;
use sdl2::render::Canvas;
use sdl2::video::Window;
use sdl2::{Sdl, TimerSubsystem};
use send_wrapper::SendWrapper;
use std::process;

static mut SDL_CONTEXT: Option<SendWrapper<Sdl>> = None;
static mut SDL_CANVAS: Option<SendWrapper<Canvas<Window>>> = None;
static mut SDL_TIMER: Option<SendWrapper<TimerSubsystem>> = None;
static mut CURRENT_TICKS: u64 = 0;

fn get_context() -> &'static mut SendWrapper<Sdl> {
    unsafe { SDL_CONTEXT.as_mut().unwrap() }
}

fn get_canvas() -> &'static mut SendWrapper<Canvas<Window>> {
    unsafe { SDL_CANVAS.as_mut().unwrap() }
}

fn get_timer() -> &'static mut SendWrapper<TimerSubsystem> {
    unsafe { SDL_TIMER.as_mut().unwrap() }
}

fn init_context() {
    unsafe {
        SDL_CONTEXT = Some(SendWrapper::new(
            sdl2::init().expect("Failed to initialize SDL"),
        ));
    }
}

fn init_timer() {
    unsafe {
        SDL_TIMER = Some(SendWrapper::new(
            get_context()
                .timer()
                .expect("Failed to create TimerSubsystem"),
        ));
    }
}

fn init_canvas() {
    let window = get_context()
        .video()
        .unwrap()
        .window("Sim Graphics", SIM_X_SIZE, SIM_Y_SIZE)
        .build()
        .expect("Failed to create SDL window");

    unsafe {
        SDL_CANVAS = Some(SendWrapper::new(
            window
                .into_canvas()
                .build()
                .expect("Failed to create canvas"),
        ));
    }
}

#[no_mangle]
pub extern "C" fn sim_init() {
    init_context();
    init_canvas();
    init_timer();
}

#[no_mangle]
pub extern "C" fn sim_exit() {
    let mut event_pump = get_context().event_pump().unwrap();

    'running: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. } => break 'running,
                _ => {}
            }
        }
    }

    unsafe {
        let _ = SDL_CONTEXT.take();
        let _ = SDL_CANVAS.take();
        let _ = SDL_TIMER.take();
    }
}

#[no_mangle]
pub extern "C" fn sim_flush() {
    let timer = get_timer();

    let current_ticks = timer.ticks64();
    let prev_ticks = unsafe { CURRENT_TICKS };
    let ticks_passed = (current_ticks - prev_ticks) as u32;

    if ticks_passed < FRAME_TICKS.into() {
        timer.delay(FRAME_TICKS - ticks_passed);
    }

    let canvas = get_canvas();
    canvas.present();

    let mut event_pump = get_context().event_pump().unwrap();
    const QUIT_EXIT_CODE: i32 = 0x10;

    for event in event_pump.poll_iter() {
        match event {
            Event::Quit { .. } => process::exit(QUIT_EXIT_CODE),
            _ => {}
        }
    }
}

#[no_mangle]
pub extern "C" fn sim_put_pixel(x: u16, y: u16, argb: u32) {
    assert!(x < SIM_X_SIZE as u16);
    assert!(y < SIM_Y_SIZE as u16);

    let a: u8 = (argb >> 24) as u8;
    let r: u8 = ((argb >> 16) & 0xFF) as u8;
    let g: u8 = ((argb >> 8) & 0xFF) as u8;
    let b: u8 = (argb & 0xFF) as u8;

    let color = Color { r, g, b, a };
    let canvas = get_canvas();
    canvas.set_draw_color(color);

    let point = Point::new(x as i32, y as i32);
    canvas
        .draw_point(point)
        .expect(format!("Failed to draw point {:?}", point).as_str());

    let timer = get_timer();
    unsafe {
        CURRENT_TICKS = timer.ticks64();
    }
}

#[no_mangle]
pub extern "C" fn sim_rand() -> i32 {
    let mut rng = rand::thread_rng();
    rng.gen()
}
