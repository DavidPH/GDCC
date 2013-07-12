##-----------------------------------------------------------------------------
##
## CMake file for GDCC-IR library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-ir
   Arg.cpp
   Exp/ValueGlyph.cpp
   Exp/ValueRoot.cpp
   Function.cpp
   Glyph.cpp
   Type.cpp
   Value.cpp
)

target_link_libraries(gdcc-ir gdcc gmpxx gmp)

## EOF

