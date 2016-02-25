//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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

#include <memory>
#include <string>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // DirStream
      //
      class DirStream
      {
      public:
         virtual ~DirStream();

         char const *getFull() const {return nameBuf.data();}
         char const *getPart() const {return nameBuf.data() + nameLen;}

         bool next() {return v_next();}

      protected:
         DirStream(char const *dirname);

         virtual bool v_next() = 0;

         std::string            nameBuf;
         std::string::size_type nameLen;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      void DirCreate(char const *dirname);

      std::unique_ptr<DirStream> DirOpenStream(char const *dirname);

      bool IsDir(char const *dirname);
   }
}

#endif//GDCC__Core__Dir_H__

