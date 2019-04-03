##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-ntsc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_NTSC_H
   ConcatTBuf.hpp
   IStream.hpp
   PutToken.hpp
   TSource.hpp
   TStream.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-ntsc-lib
##
add_library(gdcc-ntsc-lib ${GDCC_SHARED_DECL}
   ${GDCC_NTSC_H}
   ConcatTBuf.cpp
   PutToken.cpp
   TSource.cpp
)

##
## gdcc-ntsc
##
add_executable(gdcc-ntsc
   main_ntsc.cpp
)

target_link_libraries(gdcc-ntsc-lib gdcc-core-lib)

target_link_libraries(gdcc-ntsc gdcc-ntsc-lib)

GDCC_INSTALL_PART(ntsc NTSC NTSC TRUE TRUE)

## EOF

