cmake_minimum_required(VERSION 3.5)
project(libgamearchives)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -Wall")

# TODO: Clean this up
set(SOURCE_FILES
        src/gamearchives/formats/ark/ark.h
        src/gamearchives/formats/ark/ark_archive.cpp
        src/gamearchives/formats/ark/hdr_crypt_stream.cpp
        src/gamearchives/formats/ark/hdr_crypt_stream.h
        src/gamearchives/formats/ark/protected_file_stream.cpp
        src/gamearchives/formats/ark/protected_file_stream.h
        src/gamearchives/formats/fsg_fs/fsg.h
        src/gamearchives/stream/inflate_stream.cpp
        src/gamearchives/stream/inflate_stream.h
        src/gamearchives/stream/multi_stream.cpp
        src/gamearchives/stream/multi_stream.h
        src/gamearchives/stream/offset_stream.cpp
        src/gamearchives/stream/offset_stream.h
        src/gamearchives/stream/std_stream.h
        src/gamearchives/stream/stream.cpp
        src/gamearchives/stream/stream.h
        src/gamearchives/vfs/archive_types.cpp
        src/gamearchives/vfs/default_directory.cpp
        src/gamearchives/vfs/default_directory.h
        src/gamearchives/vfs/local_directory.cpp
        src/gamearchives/vfs/local_directory.h
        src/gamearchives/vfs/local_file.cpp
        src/gamearchives/vfs/local_file.h
        src/gamearchives/vfs/offset_file.cpp
        src/gamearchives/vfs/offset_file.h
        src/gamearchives/vfs/vfs.h
        src/gamearchives/gamearchives.h
        src/gamearchives/gamearchives.cpp src/gamearchives/sys/platform.h src/gamearchives/sys/platform_win.h src/gamearchives/sys/platform_posix.h)
add_library(gamearchives SHARED ${SOURCE_FILES})
