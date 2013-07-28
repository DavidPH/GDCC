##-----------------------------------------------------------------------------
##
## CMake file for GDCC-Bytecode-ZDACS library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_library(gdcc-bc-zdacs
   Info.cpp
   put.cpp
   tr.cpp
)

target_link_libraries(gdcc-bc-zdacs gdcc-bc)

## EOF

