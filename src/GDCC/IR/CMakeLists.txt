##-----------------------------------------------------------------------------
##
## CMake file for GDCC-IR library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-ir
   Addr.cpp
   Arg.cpp
   Block.cpp
   Code.cpp
   Exp.cpp
   Exp/ValueGlyph.cpp
   Exp/ValueRoot.cpp
   Function.cpp
   Glyph.cpp
   Linkage.cpp
   Statement.cpp
   Type.cpp
   Value.cpp
)

target_link_libraries(gdcc-ir gdcc gmpxx gmp)

## EOF

