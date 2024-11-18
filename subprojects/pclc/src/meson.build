subdir('frontend')
subdir('bytecode_vm')

popl_dep = dependency('popl')
pclc_deps = [popl_dep, pclc_frontend_dep]
pclc_tools = []
pclc_definitions = []

if get_option('compiler_backend') == 'vm'
    pcldis = executable(
        'pcldis',
        'pcldis.cpp',
        link_with: pclc_bytecode_vm,
        dependencies: [pclc_headers_dep, popl_dep],
        install: true,
        install_rpath: '$ORIGIN/../lib',
    )

    pclvm = executable(
        'pclvm',
        'pclvm.cpp',
        link_with: pclc_bytecode_vm,
        dependencies: [pclc_headers_dep, popl_dep],
        install: true,
        install_rpath: '$ORIGIN/../lib',
    )

    pclc_deps += [declare_dependency(link_with: pclc_bytecode_vm)]
    pclc_tools += [pcldis, pclvm]
    pclc_definitions += ['-DPCLC_VM_CODEGEN']
endif

if get_option('compiler_backend') == 'llvm'
    static_llvm = get_option('default_library') == 'static'

    llvm_dep = dependency(
        'llvm',
        version: '>=18.0',
        method: 'auto',
        modules: ['core', 'support', 'option', 'executionengine'],
        required: true,
        static: static_llvm,
    )

    cswl_sim_dep = dependency('cswl-sim')

    subdir('llvm_codegen')

    pclc_deps += [llvm_dep, cswl_sim_dep, pclc_llvm_codegen_dep]
    pclc_definitions += ['-DPCLC_LLVM_CODEGEN']
endif

pclc = executable(
    'pclc',
    ['pclc.cpp', pclc_frontend_headers],
    dependencies: pclc_deps,
    install: true,
    install_rpath: '$ORIGIN/../lib',
    cpp_args: pclc_definitions,
)

pclc_tools += [pclc]
