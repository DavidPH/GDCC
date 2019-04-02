##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-core.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

set(GDCC_Core_H
   Array.hpp
   BinaryIO.hpp
   BufferBuf.hpp
   BufferTBuf.hpp
   CommentBuf.hpp
   Compare.hpp
   Counter.hpp
   Deleter.hpp
   Dir.hpp
   EscapeBuf.hpp
   Exception.hpp
   FeatureHold.hpp
   File.hpp
   List.hpp
   MemCmp.hpp
   MemItr.hpp
   Number.hpp
   NumberAlloc.hpp
   Option.hpp
   Origin.hpp
   OriginBuf.hpp
   Parse.hpp
   Path.hpp
   Range.hpp
   SourceTBuf.hpp
   StreamTBuf.hpp
   String.hpp
   StringBuf.hpp
   StringGen.hpp
   StringList.hpp
   StringOption.hpp
   Token.hpp
   TokenBuf.hpp
   TokenSource.hpp
   TokenStream.hpp
   Types.hpp
   WSpaceTBuf.hpp
   Warning.hpp
)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-core-lib
##
add_library(gdcc-core-lib ${GDCC_SHARED_DECL}
   ${GDCC_Core_H}
   BinaryIO.cpp
   Counter.cpp
   Dir.cpp
   Exception.cpp
   File.cpp
   Number.cpp
   Option.cpp
   Origin.cpp
   ParseNumber.cpp
   ParseString.cpp
   Path.cpp
   String.cpp
   StringGen.cpp
   StringOption.cpp
   Token.cpp
   Warning.cpp
)

target_link_libraries(gdcc-core-lib gdcc-option-lib)

if(GDCC_Core_BigNum)
   target_link_libraries(gdcc-core-lib ${GMP_LIBRARIES})
endif()

GDCC_INSTALL_PART(core Core Core FALSE TRUE)

## EOF

