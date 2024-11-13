project(
    'graphs',
    'cpp',
    version: '1.0.0',
    default_options: ['warning_level=3', 'cpp_std=c++20'],
)

graphs_dep = declare_dependency(
    include_directories: include_directories('include'),
)

meson.override_dependency('graphs', graphs_dep)

if get_option('tests')
    subdir('test')
endif
