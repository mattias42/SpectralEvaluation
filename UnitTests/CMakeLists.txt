# This is the main project of Novac Post Processing Program (NovacPPP)

cmake_minimum_required (VERSION 3.6)

## -------------------- SpectralEvaluationTests -------------------------

add_executable(SpectralEvaluationTests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/catch.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UnitTest_FitWindow.cpp
)

target_include_directories(SpectralEvaluationTests PRIVATE ${SPECTRALEVAUATION_INCLUDE_DIRS})
target_link_libraries(SpectralEvaluationTests PRIVATE NovacSpectralEvaluation)

IF(MSVC)
    target_compile_definitions(SpectralEvaluationTests PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(SpectralEvaluationTests PRIVATE /W4 /WX /sdl)
ELSE()
    target_compile_options(SpectralEvaluationTests PRIVATE -Wall -std=c++11)
ENDIF()

