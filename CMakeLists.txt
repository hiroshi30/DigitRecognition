cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(DigitRecognition
	DESCRIPTION ""
	VERSION 0.0.1
	LANGUAGES C
)


# SDL
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("++ SDL2_NO_MWINDOWS ON")
	set(SDL2_NO_MWINDOWS ON)
endif()

set(SDL2_DIR ${CMAKE_SOURCE_DIR}/external/SDL2-2.28.5/cmake)
find_package(SDL2 REQUIRED)
configure_file(
	${CMAKE_SOURCE_DIR}/external/SDL2-2.28.5/x86_64-w64-mingw32/bin/SDL2.dll
	${CMAKE_BINARY_DIR}/SDL2.dll COPYONLY
)


# kazamori
add_subdirectory(kazamori)


# DigitRecognition
add_executable(DigitRecognition)

target_sources(DigitRecognition PRIVATE
	src/main.c
)

target_include_directories(DigitRecognition PRIVATE
    ${SDL2_INCLUDE_DIRS}
    kazamori
)

target_link_libraries(DigitRecognition PRIVATE
    ${SDL2_LIBRARIES}
    kazamori
)

# -mwindows - remove console output
target_compile_options(DigitRecognition PRIVATE -Wall -Wextra -Wpedantic -D${CMAKE_BUILD_TYPE})
target_compile_features(DigitRecognition PRIVATE c_std_23)
