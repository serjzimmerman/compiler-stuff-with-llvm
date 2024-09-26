use std::env;
use std::error::Error;
use std::path::Path;

fn main() -> Result<(), Box<dyn Error>> {
    let crate_dir = env::var("CARGO_MANIFEST_DIR")?;
    let out_dir = env::var("OUT_DIR")?;
    let _out_dir = Path::new(out_dir.as_str());

    let current_path_buf = env::current_dir()?;
    let current_dir = Path::new(&current_path_buf);

    let cbindgen_enable = env::var_os("CARGO_FEATURE_CBINDGEN").is_some();

    if cbindgen_enable {
        let config = cbindgen::Config::from_file("cbindgen.toml")?;

        cbindgen::Builder::new()
            .with_crate(crate_dir)
            .with_config(config)
            .with_only_target_dependencies(true)
            .generate()?
            .write_to_file(
                current_dir
                    .join("include")
                    .join("cswl-sim")
                    .join("bindings.h"),
            );
    }

    Ok(())
}
