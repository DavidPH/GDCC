##-----------------------------------------------------------------------------
##
## Copyright (C) 2016-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ar.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_AR_H
   Exception.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(Wad)

##
## gdcc-ar-lib
##
add_library(gdcc-ar-lib ${GDCC_SHARED_DECL}
   ${GDCC_AR_H}
   Exception.cpp
)

target_link_libraries(gdcc-ar-lib gdcc-core-lib)

GDCC_INSTALL_PART(ar AR AR FALSE TRUE)

## EOF

