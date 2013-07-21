##-----------------------------------------------------------------------------
##
## Root CMake file for sub-projects and libraries.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Environment Configuration                                                  |
##

include_directories(.)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(Asm)
add_subdirectory(Bytecode)
add_subdirectory(GDCC)
add_subdirectory(Linker)
add_subdirectory(MageDefs)
add_subdirectory(Option)

## EOF

