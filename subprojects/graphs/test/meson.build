gtest = dependency('gtest', main: true)
test_executable = executable(
    'graphs_test',
    ['src/dag.cpp', 'src/directed_graph.cpp'],
    dependencies: [gtest, graphs_dep],
)
test('graphs.unit', test_executable, protocol: 'gtest')
