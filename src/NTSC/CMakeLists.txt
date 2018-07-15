##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2018 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ntsc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ntsc-lib
##
add_library(gdcc-ntsc-lib ${GDCC_SHARED_DECL}
   IStream.cpp

   ../../inc/NTSC/IStream.hpp
   ../../inc/NTSC/Types.hpp
)

##
## gdcc-ntsc
##
add_executable(gdcc-ntsc
   main_ntsc.cpp
)

target_link_libraries(gdcc-ntsc-lib gdcc-core-lib)

target_link_libraries(gdcc-ntsc gdcc-ntsc-lib)

GDCC_INSTALL_PART(ntsc TRUE TRUE)

## EOF

