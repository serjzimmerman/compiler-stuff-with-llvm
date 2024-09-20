# A lot of inspiration taken from https://github.com/AN3223/Meson-Rust-C-FFI-Example.
# Some of the code is not mine and belongs to the author of that repo.

project(
  'compiler_stuff_with_llvm',
  ['c', 'cpp', 'rust'],
  version: '0.1.0',
  default_options: ['warning_level=3', 'cpp_std=c++17', 'werror=true'],
)

fs = import('fs')
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

  message(
    'Running cargo: @0@ @1@'.format(
      fs.relative_to(cargo_build.full_path(), meson.current_source_dir()),
      ' '.join(cargo_args),
    ),
  )

  cargo_result = run_command(cargo_build, cargo_args, check: true)
  cargo_stdout = cargo_result.stdout().strip()
  cargo_stderr = cargo_result.stderr().strip()

  if cargo_stderr != ''
    message(cargo_stderr)
  elif cargo_stdout != ''
    message(cargo_stdout)
  else
    message('Cargo is done')
  endif

  if release
    rust_target_dir = 'release'
  else
    rust_target_dir = 'debug'
  endif

  cswl_dep = declare_dependency(
    dependencies: [
      cc.find_library(
        'compiler_stuff_with_llvm',
        dirs: [meson.current_source_dir() / 'target' / rust_target_dir],
      ),
      dependency('sdl2'),
    ],
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