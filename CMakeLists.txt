cmake_minimum_required(VERSION 2.8.12)

project(dann_ocean)

add_subdirectory(pybind11)

pybind11_add_module(dann_ocean src/main.cpp)
