##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-irdump.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-irdump
   main_irdump.cpp

   IRDump.cpp
   IRDump/Arg.cpp
   IRDump/Exp.cpp
   IRDump/Function.cpp
   IRDump/Program.cpp
)

target_link_libraries(gdcc-irdump gdcc-ir-lib)

## EOF

