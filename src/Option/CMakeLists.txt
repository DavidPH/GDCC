##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-option.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-option-lib
##
add_library(gdcc-option-lib
   Base.cpp
   CStr.cpp
   CStrV.cpp
   Exception.cpp
   Program.cpp
   Program/Process.cpp
   Program/Put.cpp
   Util.cpp

   ../../inc/Option/Base.hpp
   ../../inc/Option/Bool.hpp
   ../../inc/Option/CStr.hpp
   ../../inc/Option/CStrV.hpp
   ../../inc/Option/Exception.hpp
   ../../inc/Option/Function.hpp
   ../../inc/Option/Int.hpp
   ../../inc/Option/Program.hpp
   ../../inc/Option/Util.hpp
)

## EOF

