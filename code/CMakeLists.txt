cmake_minimum_required(VERSION 3.10)
project(lab)
include_directories(./include)

add_executable(lab1 src/main.cpp 
        src/buffer_manager.cpp 
        src/data_storage_manager.cpp
        src/LRU_Replacer.cpp
        src/CLOCK_Replacer.cpp
        src/LRU_k_Replacer.cpp)