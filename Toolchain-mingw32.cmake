set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER i586-mingw32msvc-gcc)
set(CMAKE_CXX_COMPILER i586-mingw32msvc-g++)
set(CMAKE_RC_COMPILER i586-mingw32msvc-windres)

set(CMAKE_FIND_ROOT_PATH /usr/i586-mingw32msvc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXE_LINKER_FLAGS 
"${CMAKE_EXE_LINKER_FLAGS} --static -static-libgcc") #  -mwindows
include_directories(SYSTEM /usr/i586-mingw32msvc/include /usr/local/cross-tools/i586-mingw32msvc/include)
link_directories(/usr/i586-mingw32msvc/lib)

