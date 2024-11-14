lit_conf_data = configuration_data()

fs = import('fs')

lit_conf_data.set('test_source_root', meson.current_source_dir())
lit_conf_data.set('test_build_root', meson.current_build_dir())
lit_conf_data.set('pclc_binary', pclc.full_path())

lit_site_cfg = configure_file(
    input: 'lit.site.cfg.py.in',
    output: 'lit.site.cfg.py',
    configuration: lit_conf_data,
)

lit_executable = find_program('lit')

test(
    'pclc.lit',
    lit_executable,
    args: ['--succinct', '--verbose', fs.parent(lit_site_cfg.full_path())],
    depends: [pclc, pcldis, pclvm],
)
