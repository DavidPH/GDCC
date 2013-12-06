##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for MageDefs targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-magedefs
   main_magedefs.cpp

   IStream.cpp
)

target_link_libraries(gdcc-magedefs gdcc)

## EOF

