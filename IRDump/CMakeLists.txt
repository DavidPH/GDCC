##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-irdump.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_IRDump_H
   Put.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-irdump
##
add_executable(gdcc-irdump
   main_irdump.cpp

   ${GDCC_IRDump_H}
   Put.cpp
   Put/Arg.cpp
   Put/Exp.cpp
   Put/Function.cpp
   Put/Program.cpp
   Put/Type.cpp
   Put/Value.cpp
)

target_link_libraries(gdcc-irdump gdcc-ir-lib)

GDCC_INSTALL_PART(irdump IRDump IRDump TRUE FALSE)

## EOF

