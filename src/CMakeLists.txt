##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for all targets.
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
add_subdirectory(C)
add_subdirectory(GDCC)
add_subdirectory(Linker)
add_subdirectory(MageDefs)
add_subdirectory(Option)

## EOF

