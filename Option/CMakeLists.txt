##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2022 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-option.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_Option_H
   Base.hpp
   Bool.hpp
   CStr.hpp
   CStrV.hpp
   Exception.hpp
   Function.hpp
   Int.hpp
   IntMap.hpp
   Program.hpp
   StrUtil.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-option-lib
##
add_library(gdcc-option-lib ${GDCC_SHARED_DECL}
   ${GDCC_Option_H}
   Base.cpp
   CStr.cpp
   CStrV.cpp
   Exception.cpp
   Program.cpp
   Program/Process.cpp
   Program/Put.cpp
   StrUtil.cpp
)

GDCC_INSTALL_PART(option Option Option FALSE TRUE)

## EOF

