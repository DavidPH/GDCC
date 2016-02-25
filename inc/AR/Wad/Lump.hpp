//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Lump handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AR__Wad__Lump_H__
#define GDCC__AR__Wad__Lump_H__

#include "../../Core/List.hpp"
#include "../../Core/String.hpp"

#include <cstdint>
#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class FileBlock;
   }

   namespace AR
   {
      namespace Wad
      {
         //
         // Lump
         //
         class Lump
         {
         public:
            Lump(Core::String name = nullptr);
            Lump(Lump const &) = delete;
            virtual ~Lump();

            Lump &operator = (Lump const &) = delete;

            virtual std::size_t sizeData() const = 0;
            virtual std::size_t sizeHead() const;

            virtual void writeData(std::ostream &out) const = 0;
            virtual void writeDirs(std::string &out) const;
            virtual void writeHead(std::ostream &out, std::size_t offset) const;
            virtual void writeList(std::ostream &out, std::string &path) const;

            Lump *wadNext, *wadPrev;
            Core::String name;

         private:
            using ListUtil = Core::ListUtil<Lump, &Lump::wadPrev, &Lump::wadNext>;
         };

         //
         // Lump_Data
         //
         class Lump_Data : public Lump
         {
         public:
            Lump_Data(Core::String name, std::unique_ptr<char[]> &&data, std::size_t size);

            virtual std::size_t sizeData() const;

            virtual void writeData(std::ostream &out) const;

         private:
            std::unique_ptr<char[]> data;
            std::size_t             size;
         };

         //
         // Lump_Empty
         //
         class Lump_Empty : public Lump
         {
         public:
            using Lump::Lump;

            virtual std::size_t sizeData() const;

            virtual void writeData(std::ostream &out) const;
         };

         //
         // Lump_File
         //
         class Lump_File : public Lump
         {
         public:
            Lump_File(Core::String name, std::unique_ptr<char[]> &&file);

            virtual std::size_t sizeData() const;

            virtual void writeData(std::ostream &out) const;

         private:
            std::unique_ptr<char[]> file;
            std::size_t             size;
         };

         //
         // Lump_FilePart
         //
         class Lump_FilePart : public Lump
         {
         public:
            Lump_FilePart(Core::String name, char const *data, std::size_t size,
               std::shared_ptr<Core::FileBlock> const &file);
            virtual ~Lump_FilePart();

            virtual std::size_t sizeData() const;

            virtual void writeData(std::ostream &out) const;

         private:
            std::shared_ptr<Core::FileBlock> file;

            char const *data;
            std::size_t size;
         };
      }
   }
}

#endif//GDCC__AR__Wad__Lump_H__

