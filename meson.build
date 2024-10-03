# A lot of inspiration taken from https://github.com/AN3223/Meson-Rust-C-FFI-Example.
# Some of the code is not mine and belongs to the author of that repo.

project(
  'cswl',
  ['c', 'cpp'],
  version: '0.1.0',
  default_options: ['warning_level=3', 'cpp_std=c++20'],
)

cswl_sim_project = subproject(
  'cswl-sim',
  default_options: ['build_rust_deps=true'],
)

clang_cc = find_program('clang')
release = get_option('buildtype').startswith('release')

cswl_sim_dep = cswl_sim_project.get_variable('cswl_sim_dep')

executable(
  'day_night_c',
  ['src/day_night.c'],
  dependencies: [cswl_sim_dep],
)

cswl_include_dir = meson.current_source_dir() / 'cswl-sim' / 'include'

custom_target(
  'day_night_c_ir',
  command: [clang_cc, '-emit-llvm', '-S', '-O2', '-DNDEBUG', '-I', cswl_include_dir, '-c', meson.current_source_dir() / 'src/day_night.c'],
  output: 'day_night.ll',
)
