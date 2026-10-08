cmake_minimum_required(VERSION 3.16)
project(MilkDropAudioTests LANGUAGES CXX)

enable_testing()
add_executable(audiobuf_test audiobuf_test.cpp ../audio/audiobuf.cpp)
target_compile_features(audiobuf_test PRIVATE cxx_std_11)
add_test(NAME audiobuf COMMAND audiobuf_test)
