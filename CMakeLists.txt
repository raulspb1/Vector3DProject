cmake_minimum_required(VERSION 3.15)
project(Vector3DProject LANGUAGES CXX)

# Устанавливаем стандарт C++17 (необходим для умных указателей и noexcept)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем исполняемый файл и исходный код
add_executable(Vector3DProject main.cpp)
