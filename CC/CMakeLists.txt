##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-cc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_CC_H
   AsmGlyphTBuf.hpp
   Exp.hpp
   Exp/Add.hpp
   Exp/Arith.hpp
   Exp/Assign.hpp
   Exp/Bitwise.hpp
   Exp/Call.hpp
   Exp/Cmp.hpp
   Exp/Convert/BoolSoft.hpp
   Exp/Convert/Pointer.hpp
   Exp/Convert/Void.hpp
   Exp/Deref.hpp
   Exp/Func.hpp
   Exp/GenSel.hpp
   Exp/Init.hpp
   Exp/Jmp.hpp
   Exp/Mem.hpp
   Exp/Not.hpp
   Exp/Obj.hpp
   Exp/Refer.hpp
   Exp/Sub.hpp
   Exp/VaArg.hpp
   Factory.hpp
   Init.hpp
   Parse.hpp
   Scope.hpp
   Scope/Block.hpp
   Scope/Break.hpp
   Scope/Case.hpp
   Scope/Function.hpp
   Scope/Global.hpp
   Scope/Local.hpp
   Scope/Loop.hpp
   Statement/Asm.hpp
   Statement/FuncPre.hpp
   Statement/Goto.hpp
   Statement/If.hpp
   Statement/Loop.hpp
   Statement/Return.hpp
   Statement/Switch.hpp
   Type.hpp
   Type/Bool.hpp
   Type/Char.hpp
   Type/Enum.hpp
   Type/Fixed.hpp
   Type/Float.hpp
   Type/Struct.hpp
   Types.hpp
   Warning.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-cc-lib
##
add_library(gdcc-cc-lib ${GDCC_SHARED_DECL}
   ${GDCC_CC_H}
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
)

##
## gdcc-cc
##
add_executable(gdcc-cc
   main_cc.cpp
)

target_link_libraries(gdcc-cc-lib gdcc-as-lib gdcc-cpp-lib gdcc-sr-lib)

target_link_libraries(gdcc-cc gdcc-cc-lib gdcc-ld-lib)

GDCC_INSTALL_PART(cc CC CC TRUE TRUE)

## EOF

