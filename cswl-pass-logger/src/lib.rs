use std::{
    env,
    fs::File,
    io::{stderr, BufWriter, Write},
    slice,
};

type Writer = BufWriter<Box<dyn Write>>;

static mut WRITER: Option<Writer> = None;

fn get_writer() -> &'static mut Writer {
    if unsafe { WRITER.is_none() } {
        let writer = Some(Writer::new({
            let path_or_none = env::var_os("CSWL_PASS_LOGGER_OUTPUT_PATH");
            match path_or_none {
                Some(path) => Box::new(File::create(&path).unwrap()),
                None => Box::new(stderr()),
            }
        }));
        unsafe {
            WRITER = writer;
        }
    };

    unsafe { WRITER.as_mut().unwrap() }
}

#[no_mangle]
fn cswl_pass_log_instrumentation(ptr: *const u8, len: u64) {
    let name = std::str::from_utf8(unsafe { slice::from_raw_parts(ptr, len as usize) })
        .expect("Got invalid UTF8 string");

    { writeln!(get_writer(), "{name}") }.unwrap();
}
