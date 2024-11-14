project(
    'pclc',
    ['c', 'cpp'],
    version: '0.1.0',
    default_options: [
        'warning_level=3',
        'cpp_std=c++20',
        'default_library=static',
    ],
)

ezvis_dep = dependency('ezvis')
graphs_dep = dependency('graphs')
fmt_dep = dependency('fmt')

pclc_headers_dep = declare_dependency(
    dependencies: [ezvis_dep, graphs_dep, fmt_dep],
    include_directories: include_directories('include'),
)

subdir('src')

if get_option('tests')
    subdir('test')
endif
