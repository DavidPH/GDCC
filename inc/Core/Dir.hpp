//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Directory handling abstraction.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Dir_H__
#define GDCC__Core__Dir_H__

#include "../Core/Stat.hpp"

#include <memory>
#include <string>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // DirStream
   //
   class DirStream
   {
   public:
      virtual ~DirStream();

      Stat const &getStat() const {return statBuf;}

      char const *getStrFull() const {return nameBuf.data();}
      char const *getStrPart() const {return nameBuf.data() + nameLen;}

      std::size_t getLenBase() const {return nameLen;}
      std::size_t getLenFull() const {return nameBuf.size();}
      std::size_t getLenPart() const {return nameBuf.size() - nameLen;}

      bool next() {return v_next();}

   protected:
      DirStream(char const *dirname);

      virtual bool v_next() = 0;

      std::string            nameBuf;
      std::string::size_type nameLen;

      Stat statBuf;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   void DirCreate(char const *dirname);

   std::unique_ptr<DirStream> DirOpenStream(char const *dirname);

   bool IsDir(char const *dirname);
}

#endif//GDCC__Core__Dir_H__

