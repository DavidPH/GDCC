##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-magedefs.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-magedefs-lib
##
add_library(gdcc-magedefs-lib ${GDCC_SHARED_DECL}
   IStream.cpp

   ../../inc/MageDefs/IStream.hpp
)

##
## gdcc-magedefs
##
add_executable(gdcc-magedefs
   main_magedefs.cpp
)

target_link_libraries(gdcc-magedefs-lib gdcc-core-lib)

target_link_libraries(gdcc-magedefs gdcc-magedefs-lib)

GDCC_INSTALL_PART(magedefs TRUE TRUE)

## EOF

