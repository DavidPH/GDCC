##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ld.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ld
##
add_executable(gdcc-ld
   main_ld.cpp
)

target_link_libraries(gdcc-ld
   gdcc-bytecode-mgc-lib
   gdcc-bytecode-zdacs-lib
)

GDCC_INSTALL_PART(ld TRUE FALSE)

## EOF

