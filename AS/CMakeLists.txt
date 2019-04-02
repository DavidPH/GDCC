##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-as.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_AS_H
   IStream.hpp
   LabelTBuf.hpp
   Macro.hpp
   Parse.hpp
   TStream.hpp
   Types.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-as-lib
##
add_library(gdcc-as-lib ${GDCC_SHARED_DECL}
   ${GDCC_AS_H}
   IStream.cpp
   LabelTBuf.cpp
   Macro.cpp
   Parse.cpp
   Parse/Arg.cpp
   Parse/Block.cpp
   Parse/DJump.cpp
   Parse/Declaration.cpp
   Parse/Exp.cpp
   Parse/Function.cpp
   Parse/Glyph.cpp
   Parse/Import.cpp
   Parse/Object.cpp
   Parse/Space.cpp
   Parse/StrEnt.cpp
   Parse/Type.cpp
   Parse/Value.cpp
   ParseFile.cpp
)

##
## gdcc-as
##
add_executable(gdcc-as
   main_as.cpp
)

target_link_libraries(gdcc-as-lib gdcc-ir-lib)
target_link_libraries(gdcc-as gdcc-as-lib gdcc-ld-lib)

GDCC_INSTALL_PART(as AS AS TRUE TRUE)

## EOF

