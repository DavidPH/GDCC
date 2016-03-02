##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2016 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-bc-mgc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(GDCC_BC_MgC)
   ##
   ## gdcc-bc-mgc-lib
   ##
   add_library(gdcc-bc-mgc-lib ${GDCC_SHARED_DECL}
      Info.cpp
      Info/put.cpp
      Info/tr.cpp

      ../../../inc/BC/MgC/Info.hpp
   )

   target_link_libraries(gdcc-bc-mgc-lib gdcc-bc-lib)

   GDCC_INSTALL_PART(bc-mgc FALSE TRUE)
endif()

## EOF

