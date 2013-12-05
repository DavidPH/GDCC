##-----------------------------------------------------------------------------
##
## CMake file for GDCC-IR library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-ast
   Arg.cpp
   Exp.cpp
   Exp/Binary.cpp
   Exp/BinaryMulSize.cpp
   Exp/ValueArg.cpp
   Exp/ValueIRExp.cpp
   Exp/genStmntMove.cpp
   Statement.cpp
   Statement/Empty.cpp
   Statement/Exp.cpp
   Statement/Multi.cpp
   Type.cpp
   Type/Array.cpp
   Type/Bitfield.cpp
   Type/Function.cpp
   Type/Pointer.cpp
   Type/Primary.cpp
   Type/Wrapper.cpp
)

target_link_libraries(gdcc-ast gdcc-bc gdcc-ir)

## EOF

