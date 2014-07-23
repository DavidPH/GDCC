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
   Exp/Add.cpp
   Exp/Add/Point.cpp
   Exp/Assign.cpp
   Exp/Call.cpp
   Exp/Call/Stk.cpp
   Exp/Cmp.cpp
   Exp/Convert/BoolSoft.cpp
   Exp/Convert/Pointer.cpp
   Exp/Deref.cpp
   Exp/Div.cpp
   Exp/Func.cpp
   Exp/GenSel.cpp
   Exp/Init.cpp
   Exp/Lit.cpp
   Exp/Mem.cpp
   Exp/Mod.cpp
   Exp/Mul.cpp
   Exp/Not.cpp
   Exp/Obj.cpp
   Exp/Promote.cpp
   Exp/Promote/Arith.cpp
   Exp/Promote/Point.cpp
   Exp/Refer.cpp
   Exp/Str.cpp
   Exp/Sub.cpp
   Exp/Sub/Point.cpp
   Init.cpp
   Parse.cpp
   Parse/AttrSpec.cpp
   Parse/DeclSpec.cpp
   Parse/Declaration.cpp
   Parse/Declarator.cpp
   Parse/Exp.cpp
   Parse/Exp/Cast.cpp
   Parse/Exp/Init.cpp
   Parse/Exp/Post.cpp
   Parse/Exp/Prim.cpp
   Parse/Exp/Unar.cpp
   Parse/SpecQual.cpp
   Parse/Statement.cpp
   Parse/StaticAssert.cpp
   Parse/String.cpp
   Parse/TypeQual.cpp
   Parse/TypeSpec.cpp
   Scope.cpp
   Scope/Block.cpp
   Scope/Break.cpp
   Scope/Case.cpp
   Scope/Function.cpp
   Scope/Global.cpp
   Scope/Local.cpp
   Scope/Loop.cpp
   Statement.cpp
   Statement/FuncPre.cpp
   Statement/Goto.cpp
   Statement/If.cpp
   Statement/Loop.cpp
   Statement/Return.cpp
   Statement/Switch.cpp
   Type.cpp
   Type/Bool.cpp
   Type/Char.cpp
   Type/Fixed.cpp
   Type/Float.cpp
   Type/Struct.cpp
)

add_executable(gdcc-cc
   main_cc.cpp
)

target_link_libraries(gdcc-cc-lib gdcc-ast-lib gdcc-cpp-lib)

target_link_libraries(gdcc-cc gdcc-cc-lib)

## EOF

