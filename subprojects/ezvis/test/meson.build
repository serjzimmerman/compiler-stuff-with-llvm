gtest = dependency('gtest', main: true)
test_executable = executable(
    'ezvis_test',
    'src/ezvis.cpp',
    dependencies: [gtest, ezvis_dep],
)
test('ezvis.unit', test_executable, protocol: 'gtest')
