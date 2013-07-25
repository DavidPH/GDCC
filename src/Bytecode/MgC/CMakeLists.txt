##-----------------------------------------------------------------------------
##
## CMake file for GDCC-Bytecode-MgC library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-bc-mgc
   Info.cpp
   put.cpp
   translate.cpp
)

target_link_libraries(gdcc-bc-mgc gdcc-bc)

## EOF

