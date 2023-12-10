cmake_minimum_required(VERSION 3.26)
project(karac)

set(CMAKE_CXX_STANDARD 23)

include_directories(libs/webview libs/cJSON src)

if (APPLE)

    add_executable(karac
            src/karac.cc
            src/cJSON.c
            src/tweaks.cc
            src/tweaks-macos.mm
    )

else (APPLE)

    add_executable(karac
            src/karac.cc
            src/cJSON.c
            src/tweaks.cc
    )


endif (APPLE)


if (WIN32)
    include_directories(build/webview2sdk/build/native/include)
    target_link_libraries(karac advapi32 ole32 shell32 shlwapi user32 version)
    set(CMAKE_CXX_FLAGS "-mwindows -static")
endif (WIN32)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework WebKit")
endif (APPLE)

if (LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.0)
    target_link_libraries(karac ${GTK_LIBRARIES} ${WEBKIT2GTK_LIBRARIES})
    target_include_directories(karac PUBLIC ${GTK_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS})
    set(CMAKE_CXX_FLAGS "${GTK_CFLAGS_OTHER} ${WEBKIT2GTK_CFLAGS_OTHER}")
endif (LINUX)


