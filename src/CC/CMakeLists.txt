##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2018 David Hill
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

##
## gdcc-cc-lib
##
add_library(gdcc-cc-lib ${GDCC_SHARED_DECL}
   AsmGlyphTBuf.cpp
   Exp.cpp
   Exp/Add.cpp
   Exp/Add/Point.cpp
   Exp/Assign.cpp
   Exp/Call.cpp
   Exp/Call/Lit.cpp
   Exp/Call/Stk.cpp
   Exp/Cmp.cpp
   Exp/Convert/BoolSoft.cpp
   Exp/Convert/Pointer.cpp
   Exp/Convert/Void.cpp
   Exp/Deref.cpp
   Exp/Div.cpp
   Exp/Func.cpp
   Exp/GenSel.cpp
   Exp/Init.cpp
   Exp/Jmp.cpp
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
   Exp/VaArg.cpp
   Init.cpp
   Parse.cpp
   Parse/AddrDecl.cpp
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
   Parse/TypeSpec/Atomic.cpp
   Parse/TypeSpec/Enum.cpp
   Parse/TypeSpec/Struct.cpp
   ParseFile.cpp
   Scope.cpp
   Scope/Block.cpp
   Scope/Break.cpp
   Scope/Case.cpp
   Scope/Function.cpp
   Scope/Global.cpp
   Scope/Local.cpp
   Scope/Loop.cpp
   Statement.cpp
   Statement/Asm.cpp
   Statement/FuncPre.cpp
   Statement/Goto.cpp
   Statement/If.cpp
   Statement/Loop.cpp
   Statement/Return.cpp
   Statement/Switch.cpp
   Type.cpp
   Type/Bool.cpp
   Type/Char.cpp
   Type/Enum.cpp
   Type/Fixed.cpp
   Type/Float.cpp
   Type/Struct.cpp
   Warning.cpp

   ../../inc/CC/AsmGlyphTBuf.hpp
   ../../inc/CC/Exp.hpp
   ../../inc/CC/Exp/Add.hpp
   ../../inc/CC/Exp/Arith.hpp
   ../../inc/CC/Exp/Assign.hpp
   ../../inc/CC/Exp/Bitwise.hpp
   ../../inc/CC/Exp/Call.hpp
   ../../inc/CC/Exp/Cmp.hpp
   ../../inc/CC/Exp/Convert/BoolSoft.hpp
   ../../inc/CC/Exp/Convert/Pointer.hpp
   ../../inc/CC/Exp/Convert/Void.hpp
   ../../inc/CC/Exp/Deref.hpp
   ../../inc/CC/Exp/Func.hpp
   ../../inc/CC/Exp/GenSel.hpp
   ../../inc/CC/Exp/Init.hpp
   ../../inc/CC/Exp/Jmp.hpp
   ../../inc/CC/Exp/Mem.hpp
   ../../inc/CC/Exp/Not.hpp
   ../../inc/CC/Exp/Obj.hpp
   ../../inc/CC/Exp/Refer.hpp
   ../../inc/CC/Exp/Sub.hpp
   ../../inc/CC/Exp/VaArg.hpp
   ../../inc/CC/Init.hpp
   ../../inc/CC/Parse.hpp
   ../../inc/CC/Scope.hpp
   ../../inc/CC/Scope/Block.hpp
   ../../inc/CC/Scope/Break.hpp
   ../../inc/CC/Scope/Case.hpp
   ../../inc/CC/Scope/Function.hpp
   ../../inc/CC/Scope/Global.hpp
   ../../inc/CC/Scope/Local.hpp
   ../../inc/CC/Scope/Loop.hpp
   ../../inc/CC/Statement.hpp
   ../../inc/CC/Statement/Asm.hpp
   ../../inc/CC/Statement/FuncPre.hpp
   ../../inc/CC/Statement/Goto.hpp
   ../../inc/CC/Statement/If.hpp
   ../../inc/CC/Statement/Loop.hpp
   ../../inc/CC/Statement/Return.hpp
   ../../inc/CC/Statement/Switch.hpp
   ../../inc/CC/Type.hpp
   ../../inc/CC/Type/Bool.hpp
   ../../inc/CC/Type/Char.hpp
   ../../inc/CC/Type/Enum.hpp
   ../../inc/CC/Type/Fixed.hpp
   ../../inc/CC/Type/Float.hpp
   ../../inc/CC/Type/Struct.hpp
   ../../inc/CC/Types.hpp
   ../../inc/CC/Warning.hpp
)

##
## gdcc-cc
##
add_executable(gdcc-cc
   main_cc.cpp
)

target_link_libraries(gdcc-cc-lib gdcc-as-lib gdcc-cpp-lib gdcc-sr-lib)

target_link_libraries(gdcc-cc gdcc-cc-lib gdcc-ld-lib)

GDCC_INSTALL_PART(cc TRUE TRUE)

## EOF

