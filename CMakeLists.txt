cmake_minimum_required(VERSION 2.8)

#set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winit-self -Winline -Wlogical-op -Wmissing-include-dirs -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wswitch-default -Wswitch-enum -Wunsafe-loop-optimizations -Wvariadic-macros -Wwrite-strings ")
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_C_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native -Wno-dev")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -s -DNDEBUG -march=native")

set(CMAKE_MACOSX_RPATH "./")

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
