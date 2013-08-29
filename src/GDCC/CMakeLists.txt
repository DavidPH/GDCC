##-----------------------------------------------------------------------------
##
## CMake file for GDCC library.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(IR)

add_library(gdcc
   Option.cpp
   Origin.cpp
   ParseNumber.cpp
   ParseString.cpp
   String.cpp
)

target_link_libraries(gdcc gdcc-option gmpxx gmp)

## EOF

