##-----------------------------------------------------------------------------
##
## Copyright (C) 2015-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-makelib.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_MakeLib_H
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-makelib
##
add_executable(gdcc-makelib
   main_makelib.cpp

   ${GDCC_MakeLib_H}
)

target_link_libraries(gdcc-makelib gdcc-as-lib gdcc-cc-lib gdcc-ld-lib)

GDCC_INSTALL_PART(makelib MakeLib MakeLib TRUE FALSE)

## EOF

