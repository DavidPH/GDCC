##-----------------------------------------------------------------------------
##
## Copyright (C) 2014-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-target.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-target-lib
##
add_library(gdcc-target-lib ${GDCC_SHARED_DECL}
   Addr.cpp
   Alloc.cpp
   CallType.cpp
   Info.cpp

   ../../inc/Target/Addr.hpp
   ../../inc/Target/AddrList.hpp
   ../../inc/Target/Alloc.hpp
   ../../inc/Target/CallType.hpp
   ../../inc/Target/CallTypeList.hpp
   ../../inc/Target/Info.hpp
   ../../inc/Target/Types.hpp
)

target_link_libraries(gdcc-target-lib gdcc-core-lib)

GDCC_INSTALL_PART(target FALSE TRUE)

## EOF

