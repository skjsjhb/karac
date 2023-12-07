cmake_minimum_required(VERSION 3.26)
project(karac)

set(CMAKE_CXX_STANDARD 23)

include_directories(libs/webview libs/cJSON)

add_executable(karac
        src/karac.cc
        src/cJSON.c
)

if (WIN32)
    include_directories(build/webview2sdk/build/native/include)
    target_link_libraries(karac advapi32 ole32 shell32 shlwapi user32 version)
    add_compile_options(karac PUBLIC -mwindows)
endif (WIN32)

if (APPLE)
    add_compile_options(karac PUBLIC -framework WebKit)
endif (APPLE)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.0)
    target_link_libraries(karac ${GTK_LIBRARIES} ${WEBKIT2GTK_LIIBRARIES})
    target_include_directories(karac PUBLIC ${GTK_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS})
    add_compile_options(karac PUBLIC ${GTK_CFLAGS_OTHER} ${WEBKIT2GTK_CFLAGS_OTHER})
endif (UNIX)


