use std::env;
use std::path::Path;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let out_dir = env::var("OUT_DIR").unwrap();
    let out_dir = Path::new(out_dir.as_str());

    let cbindgen_enable = env::var_os("CARGO_FEATURE_CBINDGEN").is_some();

    if cbindgen_enable {
        let config =
            cbindgen::Config::from_file("cbindgen.toml").expect("Failed to read cbindgen config");

        cbindgen::Builder::new()
            .with_crate(crate_dir)
            .with_config(config)
            .with_only_target_dependencies(true)
            .generate()
            .expect("Unable to generate bindings")
            .write_to_file(out_dir.join("include").join("bindings.h"));
    }
}
