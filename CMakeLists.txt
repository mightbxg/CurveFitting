cmake_minimum_required(VERSION 3.10)

project(CeresTest)

find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(t
    main.cpp
    white_board.cpp
    white_board.h
    curve.cpp
    curve.h)
target_compile_features(t PRIVATE cxx_std_11)
target_link_libraries(t PRIVATE ${OpenCV_LIBS} Ceres::ceres)
