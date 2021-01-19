cmake_minimum_required(VERSION 3.10)

project(CurveFitting)

find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(CurveFitting
    main.cpp
    white_board.cpp
    white_board.h
    curve.hpp)
target_compile_features(CurveFitting PRIVATE cxx_std_11)
target_link_libraries(CurveFitting PRIVATE ${OpenCV_LIBS} Ceres::ceres)
