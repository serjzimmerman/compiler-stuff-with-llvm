project('cswl-sim', version: '0.1.0')

# A lot of inspiration taken from https://github.com/AN3223/Meson-Rust-C-FFI-Example.
# Some of the code is not mine and belongs to the author of that repo.

release = get_option('buildtype').startswith('release')

if get_option('build_rust_deps')
    find_program('cargo', required: true)

    if release
        cargo_args = ['--release']
        rust_target_dir = 'release'
    else
        cargo_args = []
        rust_target_dir = 'debug'
    endif

    rust_build_dir = meson.current_source_dir() / 'target' / rust_target_dir
    cargo_build = find_program('scripts' / 'build_cargo.sh')

    cswl_sim_cargo = custom_target(
        'cswl_sim_cargo',
        command: [cargo_build] + [
            rust_target_dir,
            meson.global_build_root() / '@OUTPUT@',
        ] + cargo_args,
        output: ['libcswl_sim.a'],
    )

    release = get_option('buildtype').startswith('release')

    cswl_sim_dep = declare_dependency(
        dependencies: [dependency('sdl2')],
        sources: [cswl_sim_cargo],
        include_directories: include_directories('include'),
    )
endif

meson.override_dependency('cswl-sim', cswl_sim_dep)
