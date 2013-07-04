##-----------------------------------------------------------------------------
##
## CMake file for GDCC-IR library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Environment Configuration                                                  |
##

include_directories(BEFORE .)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-ir
   Arg.cpp
   Exp/ValueGlyph.cpp
   Function.cpp
   Glyph.cpp
   Type.cpp
   Value.cpp
)

target_link_libraries(gdcc-ir gdcc gmpxx gmp)

## EOF

