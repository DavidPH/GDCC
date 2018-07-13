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

#include "Core/Dir.hpp"

#include "Core/Exception.hpp"
#include "Core/Path.hpp"

#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   #ifdef _WIN32
   //
   // DirStream_FindFile
   //
   class DirStream_FindFile : public DirStream
   {
   public:
      //
      // constructor
      //
      DirStream_FindFile(char const *dirname) :
         DirStream{dirname}, ended{false}
      {
         if(!IsDir(nameBuf.data()))
            throw ExceptFile(dirname, "directory");

         nameBuf += '*';

         dir = FindFirstFile(nameBuf.data(), &diritr);
         if(dir == INVALID_HANDLE_VALUE)
         {
            if(GetLastError() != ERROR_FILE_NOT_FOUND)
               throw ExceptFile(dirname, "directory");

            ended = true;
         }
      }

      //
      // destructor
      //
      ~DirStream_FindFile()
      {
         FindClose(dir);
      }

   protected:
      //
      // v_next
      //
      virtual bool v_next()
      {
         if(ended) return false;

         // Set name.
         nameBuf.resize(nameLen);
         Core::PathAppend(nameBuf, diritr.cFileName);

         // Set stat.
         if(diritr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            statBuf.type = Stat::Type::Dir;
         else
            statBuf.type = Stat::Type::File;

         // Find next file for next time.
         if(!FindNextFile(dir, &diritr))
            ended = true;

         return true;
      }

      HANDLE          dir;
      WIN32_FIND_DATA diritr;
      bool            ended : 1;
   };
   #else
   //
   // DirStream_OpenDir
   //
   class DirStream_OpenDir : public DirStream
   {
   public:
      //
      // constructor
      //
      DirStream_OpenDir(char const *dirname) :
         DirStream{dirname}
      {
         if(!(dir = opendir(dirname)))
            throw ExceptFile(dirname, "directory");
      }

      //
      // destructor
      //
      virtual ~DirStream_OpenDir()
      {
         closedir(dir);
      }

   protected:
      //
      // v_next
      //
      virtual bool v_next()
      {
         dirent *diritr;

         for(;;)
         {
            if(!(diritr = readdir(dir)))
               return false;

            // Skip self and parent.
            if(diritr->d_name[0] == '.' && (diritr->d_name[1] == '\0' ||
               (diritr->d_name[1] == '.' &&  diritr->d_name[2] == '\0')))
            {
               continue;
            }

            // Set name.
            nameBuf.resize(nameLen);
            PathAppend(nameBuf, diritr->d_name);

            // Set stat.
            switch(diritr->d_type)
            {
            case DT_UNKNOWN:
               if(IsDir(nameBuf.data()))
                  case DT_DIR: statBuf.type = Stat::Type::Dir;
               else
                  case DT_REG: statBuf.type = Stat::Type::File;
               break;

            default:
               statBuf.type = Stat::Type::None;
               break;
            }

            return true;
         }
      }

      DIR *dir;
   };
   #endif
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // DirStream constructor
   //
   DirStream::DirStream(char const *dirname) :
      nameBuf{dirname}
   {
      PathNormalizeEq(nameBuf);
      PathTerminateEq(nameBuf);
      nameLen = nameBuf.size();
   }

   //
   // DirStream destructor
   //
   DirStream::~DirStream()
   {
   }

   //
   // DirCreate
   //
   void DirCreate(char const *dirname)
   {
      #ifdef _WIN32
      CreateDirectory(dirname, nullptr);
      #else
      mkdir(dirname, S_IRWXU);
      #endif
   }

   //
   // DirOpenStream
   //
   std::unique_ptr<DirStream> DirOpenStream(char const *dirname)
   {
      #ifdef _WIN32
      return std::unique_ptr<DirStream>{new DirStream_FindFile{dirname}};
      #else
      return std::unique_ptr<DirStream>{new DirStream_OpenDir{dirname}};
      #endif
   }

   //
   // IsDir
   //
   bool IsDir(char const *dirname)
   {
      struct stat statBuf;

      if(stat(dirname, &statBuf))
         return false;

      return (statBuf.st_mode & S_IFMT) == S_IFDIR;
   }
}

// EOF

