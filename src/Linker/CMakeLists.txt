##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for Linker targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-ld
   main_ld.cpp
)

target_link_libraries(gdcc-ld gdcc-ir gdcc-bc-mgc gdcc-bc-zdacs)

## EOF

