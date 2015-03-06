##-----------------------------------------------------------------------------
##
## Copyright (C) 2014-2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-platform.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-platform-lib
##
add_library(gdcc-platform-lib ${GDCC_SHARED_DECL}
   Alloc.cpp
   Platform.cpp

   ../../inc/Platform/Alloc.hpp
   ../../inc/Platform/Platform.hpp
)

target_link_libraries(gdcc-platform-lib gdcc-core-lib)

GDCC_INSTALL_PART(platform FALSE TRUE)

## EOF

