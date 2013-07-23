##-----------------------------------------------------------------------------
##
## CMake file for GDCC-Bytecode library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(MgC)
add_subdirectory(ZDACS)

add_library(gdcc-bc
   Info.cpp
   Platform.cpp
)

target_link_libraries(gdcc-bc gdcc-ir)

## EOF

