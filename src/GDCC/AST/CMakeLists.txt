##-----------------------------------------------------------------------------
##
## CMake file for GDCC-IR library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-ast
   Type.cpp
   Type/Array.cpp
   Type/Bitfield.cpp
   Type/Function.cpp
   Type/Pointer.cpp
   Type/Wrapper.cpp
)

target_link_libraries(gdcc-ast gdcc-ir)

## EOF

