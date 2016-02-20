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

#include "Core/File.hpp"

#include "Core/Exception.hpp"
#include "Core/String.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <sys/stat.h>

#ifndef _WIN32
# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
#else
# if !defined(S_ISREG)
#  define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
# endif
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // FileBlock_Buffer
      //
      class FileBlock_Buffer : public FileBlock
      {
      public:
         FileBlock_Buffer(char *d, std::size_t s) : fileData{d}, fileSize{s} {}
         virtual ~FileBlock_Buffer() {delete[] fileData;}

      protected:
         virtual char const *v_data() const {return fileData;}
         virtual std::size_t v_size() const {return fileSize;}

      private:
         char       *const fileData;
         std::size_t const fileSize;
      };

      #ifndef _WIN32
      //
      // FileBlock_Mapped
      //
      class FileBlock_Mapped : public FileBlock
      {
      public:
         FileBlock_Mapped(char *d, std::size_t s) : fileData{d}, fileSize{s} {}
         virtual ~FileBlock_Mapped() {munmap(fileData, fileSize);}

      protected:
         virtual char const *v_data() const {return fileData;}
         virtual std::size_t v_size() const {return fileSize;}

      private:
         char       *const fileData;
         std::size_t const fileSize;
      };
      #endif
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // FileBlock::getHash
      //
      std::size_t FileBlock::getHash() const
      {
         if(!cacheHash)
            cacheHash = StrHash(data(), size());

         return cacheHash;
      }

      //
      // FileOpenBlock
      //
      std::unique_ptr<FileBlock> FileOpenBlock(char const *filename)
      {
         // Special file: -
         if(filename[0] == '-' && filename[1] == '\0')
         {
            std::vector<char> buf;
            for(char c; std::cin >> c;)
               buf.emplace_back(c);

            auto data = StrDup(buf.data(), buf.size());
            return {new FileBlock_Buffer(data.release(), buf.size()), {}};
         }

         struct stat statBuf;

         #ifdef _WIN32
         std::FILE *file;

         if(stat(filename, &statBuf) || !S_ISREG(statBuf.st_mode))
            throw Core::ExceptFile(filename, "reading");

         if(!(file = std::fopen(filename, "rb")))
            throw Core::ExceptFile(filename, "reading");

         // Allocate storage.
         std::unique_ptr<char[]> data{new char[statBuf.st_size]};

         // Read data.
         if(!std::fread(data.get(), statBuf.st_size, 1, file))
            throw std::fclose(file), Core::ExceptFile(filename, "reading");

         std::fclose(file);

         return {new FileBlock_Buffer(data.release(), statBuf.st_size), {}};
         #else
         // Attempt to memory map the file.
         // POSIX indicates that this can be expected to fail for non-file reasons.
         // Therefore, a read fallback is provided.
         int fd;

         // Open file.
         if((fd = open(filename, O_RDONLY)) == -1)
            throw Core::ExceptFile(filename, "reading");

         // Stat file.
         if(fstat(fd, &statBuf) || !S_ISREG(statBuf.st_mode))
            throw close(fd), Core::ExceptFile(filename, "reading");

         // Map file.
         auto map = mmap(nullptr, statBuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

         if(map == MAP_FAILED)
         {
            // Fall back to read instead.
            std::unique_ptr<char[]> data{new char[statBuf.st_size]};

            // Read data.
            if(read(fd, data.get(), statBuf.st_size) == -1)
               throw close(fd), Core::ExceptFile(filename, "reading");

            close(fd);

            return {new FileBlock_Buffer(data.release(), statBuf.st_size), {}};
         }
         else
         {
            close(fd);

            auto data = static_cast<char *>(map);
            return {new FileBlock_Mapped(data, statBuf.st_size), {}};
         }
         #endif
      }

      //
      // FileOpenStream
      //
      std::unique_ptr<std::streambuf, ConditionalDeleter<std::streambuf>>
      FileOpenStream(char const *filename, std::ios_base::openmode which)
      {
         // Special file: -
         if(filename[0] == '-' && filename[1] == '\0')
         {
            if(which & std::ios_base::in)
            {
               if(!(which & std::ios_base::out))
                  return {std::cin.rdbuf(), false};
            }
            else
            {
               if(which & std::ios_base::out)
                  return {std::cout.rdbuf(), false};
            }
         }

         // Try to open as normal file.
         {
            std::unique_ptr<std::filebuf> buf{new std::filebuf};
            if(buf->open(filename, which))
               return {buf.release(), true};
         }

         if(which & std::ios_base::in)
         {
            if(which & std::ios_base::out)
               throw Core::ExceptFile(filename, "reading/writing");
            else
               throw Core::ExceptFile(filename, "reading");
         }
         else
         {
            if(which & std::ios_base::out)
               throw Core::ExceptFile(filename, "writing");
            else
               throw Core::ExceptFile(filename, "unknown");
         }
      }
   }
}

// EOF

