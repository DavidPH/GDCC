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
## Variables                                                                  |
##

set(GDCC_Target_H
   Addr.hpp
   AddrList.hpp
   Alloc.hpp
   CallType.hpp
   CallTypeList.hpp
   Info.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-target-lib
##
add_library(gdcc-target-lib ${GDCC_SHARED_DECL}
   ${GDCC_Target_H}
   Addr.cpp
   Alloc.cpp
   CallType.cpp
   Info.cpp
)

target_link_libraries(gdcc-target-lib gdcc-core-lib)

GDCC_INSTALL_PART(target Target Target FALSE TRUE)

## EOF

