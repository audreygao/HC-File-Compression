add_library (bit_input_stream BitInputStream.cpp)
target_include_directories(bit_input_stream PUBLIC .)
target_link_libraries(bit_input_stream PUBLIC huffman_encoder )