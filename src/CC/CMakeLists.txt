##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-cc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-cc-lib
   Parse/DeclSpec.cpp
   Parse/TypeQual.cpp
   Scope.cpp
)

add_executable(gdcc-cc
   main_cc.cpp
)

target_link_libraries(gdcc-cc-lib gdcc-ast-lib gdcc-cpp-lib)

target_link_libraries(gdcc-cc gdcc-cc-lib)

## EOF

