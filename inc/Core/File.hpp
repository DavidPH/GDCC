//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// File handling abstraction.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__File_H__
#define GDCC__Core__File_H__

#include "../Core/Deleter.hpp"

#include <memory>
#include <streambuf>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // FileBlock
      //
      class FileBlock
      {
      public:
         virtual ~FileBlock() {}

         char const *begin() const {return data();}

         char const *data() const {return v_data();}

         char const *end() const {return data() + size();}

         std::size_t getHash() const;

         std::size_t size() const {return v_size();}

      protected:
         virtual char const *v_data() const = 0;

         virtual std::size_t v_size() const = 0;

      private:
         mutable std::size_t cacheHash = 0;
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

      std::unique_ptr<FileBlock> FileOpenBlock(char const *filename);

      std::unique_ptr<std::streambuf, ConditionalDeleter<std::streambuf>>
      FileOpenStream(char const *filename, std::ios_base::openmode which);

      std::size_t FileSize(char const *filename);
   }
}

#endif//GDCC__Core__File_H__

