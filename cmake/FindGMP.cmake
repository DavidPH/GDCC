##-----------------------------------------------------------------------------
##
## CMake for finding GMP.
##
##-----------------------------------------------------------------------------

set(GMP_SEARCH_PATHS
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local
   /usr
   /sw
   /opt/local
   /opt/csw
   /opt
)

find_path(GMP_INCLUDE_DIRS
   NAMES gmp.h
   PATH_SUFFIXES include
   PATHS ${GMP_SEARCH_PATHS}
)

find_library(GMP_LIBRARY_gmp
   NAMES gmp
   PATH_SUFFIXES lib64 lib
   PATHS ${GMP_SEARCH_PATHS}
)

find_library(GMP_LIBRARY_gmpxx
   NAMES gmpxx
   PATH_SUFFIXES lib64 lib
   PATHS ${GMP_SEARCH_PATHS}
)

set(GMP_LIBRARIES ${GMP_LIBRARY_gmp} ${GMP_LIBRARY_gmpxx})

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMP REQUIRED_VARS GMP_LIBRARIES GMP_INCLUDE_DIRS)

set(GMP_FOUND "NO")
if(GMP_LIBRARIES AND GMP_INCLUDE_DIRS)
   set(GMP_FOUND "YES")
endif()

## EOF

