subdir('frontend')
subdir('bytecode_vm')

popl_dep = dependency('popl')
pclc_deps = [popl_dep, pclc_frontend_dep]

if get_option('compiler_backend') == 'vm'
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

    pclc_deps += [declare_dependency(link_with: pclc_bytecode_vm)]
endif

pclc = executable(
    'pclc',
    ['pclc.cpp', pclc_frontend_headers],
    dependencies: pclc_deps,
    install: true,
)
