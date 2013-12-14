##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
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

add_library(gdcc-magedefs-lib
   IStream.cpp
)

add_executable(gdcc-magedefs
   main_magedefs.cpp
)

target_link_libraries(gdcc-magedefs-lib gdcc-core-lib)

target_link_libraries(gdcc-magedefs gdcc-magedefs-lib)

## EOF

