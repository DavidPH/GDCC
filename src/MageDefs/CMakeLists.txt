##-----------------------------------------------------------------------------
##
## CMake file for MageDefs executable.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_executable(gdcc-magedefs
   main.cpp

   IStream.cpp
)

target_link_libraries(gdcc-magedefs gdcc)

## EOF

