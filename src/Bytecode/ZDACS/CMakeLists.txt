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
   gen.cpp
   put.cpp
   putChunk.cpp
   putStmnt.cpp
   tr.cpp
   trStmnt.cpp
)

target_link_libraries(gdcc-bc-zdacs gdcc-bc)

## EOF

