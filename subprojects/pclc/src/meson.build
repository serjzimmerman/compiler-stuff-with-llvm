subdir('frontend')
subdir('bytecode_vm')

popl_dep = dependency('popl')

pcldis = executable(
    'pcldis',
    'pcldis.cpp',
    link_with: pclc_bytecode_vm,
    dependencies: [pclc_headers_dep, popl_dep],
    install: true,
)

pclvm = executable(
    'pclvm',
    'pclvm.cpp',
    link_with: pclc_bytecode_vm,
    dependencies: [pclc_headers_dep, popl_dep],
    install: true,
)

pclc = executable(
    'pclc',
    ['pclc.cpp', pclc_frontend_headers],
    link_with: [pclc_frontend, pclc_bytecode_vm],
    dependencies: [pclc_headers_dep, popl_dep],
    install: true,
)
