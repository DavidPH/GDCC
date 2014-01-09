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

add_library(gdcc-option-lib
   Base.cpp
   CStr.cpp
   CStrV.cpp
   Exception.cpp
   Program.cpp
   Program/Process.cpp
   Program/Put.cpp
   Util.cpp
)

## EOF

