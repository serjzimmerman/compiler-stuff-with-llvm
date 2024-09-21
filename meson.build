# A lot of inspiration taken from https://github.com/AN3223/Meson-Rust-C-FFI-Example.
# Some of the code is not mine and belongs to the author of that repo.

project(
  'compiler_stuff_with_llvm',
  ['c', 'cpp', 'rust'],
  version: '0.1.0',
  default_options: ['warning_level=3', 'cpp_std=c++17', 'werror=true'],
)

cc = meson.get_compiler('c')
clang_cc = find_program('clang')
release = get_option('buildtype').startswith('release')

if get_option('build_rust_deps')
  find_program('cargo', required: true)
  if release
    cargo_args = ['--release']
  else
    cargo_args = []
  endif

  cargo_build = find_program('scripts' / 'build_cargo.sh')

  if release
    rust_target_dir = 'release'
  else
    rust_target_dir = 'debug'
  endif

  rust_build_dir = meson.current_source_dir() / 'target' / rust_target_dir
  cswl_cargo = custom_target(
    'cswl_cargo',
    command: [cargo_build]
    + [rust_target_dir, meson.current_build_dir() / '@OUTPUT@']
    + cargo_args,
    output: ['libcompiler_stuff_with_llvm.a'],
  )

  cswl_dep = declare_dependency(
    dependencies: [dependency('sdl2')],
    sources: [cswl_cargo],
    include_directories: include_directories('include'),
  )
endif

executable(
  'day_night_c',
  ['src/c/day_night.c'],
  include_directories: ['include'],
  dependencies: [cswl_dep],
)

custom_target(
  'day_night_c_ir',
  command: [
    clang_cc,
    '-emit-llvm',
    '-S',
    '-O2',
    '-DNDEBUG',
    '-I', meson.current_source_dir() / 'include',
    '-c', meson.current_source_dir() / 'src/c/day_night.c',
  ],
  output: 'day_night.ll',
)