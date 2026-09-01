# MIT License
#
# Copyright (c) 2026 VectorPDF Contributors

cmake_minimum_required(VERSION 3.24)
project(SampleTextCounterPlugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_library(SampleTextCounterPlugin SHARED
    sampletextcounterplugin.cpp
    sampletextcounterplugin.h
)

target_link_libraries(SampleTextCounterPlugin PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(SampleTextCounterPlugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "sampletextcounter"
)
