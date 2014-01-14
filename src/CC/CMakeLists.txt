##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2014 David Hill
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
   Parse/AttrSpec.cpp
   Parse/DeclSpec.cpp
   Parse/Declarator.cpp
   Parse/SpecQual.cpp
   Parse/TypeQual.cpp
   Parse/TypeSpec.cpp
   Scope.cpp
   Type.cpp
   Type/Bool.cpp
   Type/Char.cpp
   Type/Fixed.cpp
   Type/Float.cpp
   Type/Str.cpp
   Type/Void.cpp
)

add_executable(gdcc-cc
   main_cc.cpp
)

target_link_libraries(gdcc-cc-lib gdcc-ast-lib gdcc-cpp-lib)

target_link_libraries(gdcc-cc gdcc-cc-lib)

## EOF

