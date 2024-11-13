# https://github.com/mesonbuild/meson/blob/b3f9b9dc06fcd6c0a3070380b42e0ab5fd6c1226/test%20cases/frameworks/8%20flex/meson.build
fs = import('fs')

flex = find_program('flex')
bison = find_program('bison')

parser_generated = custom_target(
    input: 'parser.y',
    output: ['parser_generated.cpp', 'parser_generated.hpp', 'location.hpp'],
    command: [bison, '-v', '--output=@OUTPUT0@', '@INPUT@', '-d'],
)

scanner_generated = custom_target(
    input: ['scanner.l', parser_generated],
    output: ['lexer_generated.cpp', 'lexer_generated.hpp'],
    command: [
        flex,
        '--full',
        '--outfile=@OUTPUT0@',
        '--header-file=@OUTPUT1@',
        '@INPUT0@',
    ],
)

pclc_frontend = static_library(
    'pclc_frontend',
    [
        'ast_copier.cpp',
        'dumper.cpp',
        'function_explorer.cpp',
        'semantic_analyzer.cpp',
        parser_generated,
        scanner_generated,
    ],
    dependencies: [pclc_headers_dep],
    link_args: ['-lfl'],
)

pclc_frontend_headers = [
    parser_generated.to_list()[1],
    parser_generated.to_list()[2],
    scanner_generated.to_list()[1],
]

pclc_frontend_dep = declare_dependency(link_with: pclc_frontend)
