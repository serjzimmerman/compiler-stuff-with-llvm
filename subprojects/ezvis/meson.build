project(
    'ezvis',
    'cpp',
    version: '1.0.0',
    default_options: ['warning_level=3', 'cpp_std=c++20'],
)

cmake = import('cmake')
ctti_project = cmake.subproject('ctti')
ctti_dep = ctti_project.get_variable('ctti_dep')

ezvis_dep = declare_dependency(
    dependencies: [ctti_dep],
    include_directories: include_directories('include'),
)

meson.override_dependency('ezvis', ezvis_dep)

if get_option('tests')
    subdir('test')
endif
