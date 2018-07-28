##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2018 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-core.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-core-lib
##
add_library(gdcc-core-lib ${GDCC_SHARED_DECL}
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

   ../../inc/Core/Array.hpp
   ../../inc/Core/BinaryIO.hpp
   ../../inc/Core/BufferBuf.hpp
   ../../inc/Core/BufferTBuf.hpp
   ../../inc/Core/CommentBuf.hpp
   ../../inc/Core/Compare.hpp
   ../../inc/Core/Counter.hpp
   ../../inc/Core/Deleter.hpp
   ../../inc/Core/Dir.hpp
   ../../inc/Core/EscapeBuf.hpp
   ../../inc/Core/Exception.hpp
   ../../inc/Core/FeatureHold.hpp
   ../../inc/Core/File.hpp
   ../../inc/Core/List.hpp
   ../../inc/Core/MemCmp.hpp
   ../../inc/Core/MemItr.hpp
   ../../inc/Core/Number.hpp
   ../../inc/Core/NumberAlloc.hpp
   ../../inc/Core/Option.hpp
   ../../inc/Core/Origin.hpp
   ../../inc/Core/OriginBuf.hpp
   ../../inc/Core/Parse.hpp
   ../../inc/Core/Path.hpp
   ../../inc/Core/Range.hpp
   ../../inc/Core/StreamTBuf.hpp
   ../../inc/Core/String.hpp
   ../../inc/Core/StringBuf.hpp
   ../../inc/Core/StringGen.hpp
   ../../inc/Core/StringList.hpp
   ../../inc/Core/StringOption.hpp
   ../../inc/Core/Token.hpp
   ../../inc/Core/TokenBuf.hpp
   ../../inc/Core/TokenStream.hpp
   ../../inc/Core/Types.hpp
   ../../inc/Core/WSpaceTBuf.hpp
   ../../inc/Core/Warning.hpp
)

target_link_libraries(gdcc-core-lib gdcc-option-lib)

if(GDCC_Core_BigNum)
   target_link_libraries(gdcc-core-lib ${GMP_LIBRARIES})
endif()

GDCC_INSTALL_PART(core FALSE TRUE)

## EOF

