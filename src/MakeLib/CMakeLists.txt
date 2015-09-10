##-----------------------------------------------------------------------------
##
## Copyright (C) 2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-makelib.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-makelib
##
add_executable(gdcc-makelib
   main_makelib.cpp
)

target_link_libraries(gdcc-makelib gdcc-as-lib gdcc-cc-lib gdcc-ld-lib)

GDCC_INSTALL_PART(makelib TRUE FALSE)

## EOF

