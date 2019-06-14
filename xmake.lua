
set_project 'cpp_test'

add_rules('mode.debug')
add_rules('mode.release')

target 'test'
    set_kind 'binary'

    add_files 'src/*.cpp'
