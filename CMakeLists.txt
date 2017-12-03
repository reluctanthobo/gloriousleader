project (GloriousLeader)

cmake_minimum_required (VERSION 2.8)

### NOTICE
## To build for mingw you'll have to make twice The first time fails,
## seemingly because it overlooks this on its first pass. That error
## may be the only thing that actually lets it build, since if it had
## noticed it at first, it might have made it an implicit directory,
## at which point it starts stripping the -L arguments needed for the
## linker.
if( WIN32 )
  link_directories(/usr/local/cross-tools/i586-mingw32msvc/lib)
endif()

set(CMAKE_C_FLAGS "-g -Wall -Wextra -O3 --std=gnu99 -Werror")

if( NOT WIN32 )
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPROFILE -pedantic -pg")
endif()

set(BASIC_LIBS
  SDL2main 
  SDL2 
  SDL2_image 
)

if( WIN32 )
  set(LIBS
    mingw32
    ${BASIC_LIBS} 
    mikmod 
    jpeg 
    png 
    z 
    m 
    dinput8 
    dxguid 
    dxerr8 
    user32 
    gdi32 
    winmm 
    imm32 
    ole32 
    oleaut32 
    shell32 
    version 
    uuid
    )
else()
  set(EXTRA_LIBS
    ${BASIC_LIBS}
    m
    )
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
endif()

add_executable(glorious 
  camera.c
  image.c
  level.c
  list.c
  main.c
  monster.c
  player.c
  sprite.c
  survivor.c
)

target_link_libraries(glorious ${LIBS} ${EXTRA_LIBS})
