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
   Exp.cpp
   Exp/Assign.cpp
   Exp/Deref.cpp
   Exp/Func.cpp
   Exp/Obj.cpp
   Exp/Promote.cpp
   Exp/Refer.cpp
   Parse.cpp
   Parse/AttrSpec.cpp
   Parse/DeclSpec.cpp
   Parse/Declaration.cpp
   Parse/Declarator.cpp
   Parse/Exp.cpp
   Parse/Exp/Cast.cpp
   Parse/Exp/Post.cpp
   Parse/Exp/Prim.cpp
   Parse/Exp/Unar.cpp
   Parse/SpecQual.cpp
   Parse/Statement.cpp
   Parse/StaticAssert.cpp
   Parse/TypeQual.cpp
   Parse/TypeSpec.cpp
   Scope.cpp
   Scope/Block.cpp
   Scope/Function.cpp
   Scope/Global.cpp
   Scope/Local.cpp
   Statement.cpp
   Type.cpp
   Type/Bool.cpp
   Type/Char.cpp
   Type/Fixed.cpp
   Type/Float.cpp
   Type/Str.cpp
   Type/Struct.cpp
   Type/Void.cpp
)

add_executable(gdcc-cc
   main_cc.cpp
)

target_link_libraries(gdcc-cc-lib gdcc-ast-lib gdcc-cpp-lib)

target_link_libraries(gdcc-cc gdcc-cc-lib)

## EOF

