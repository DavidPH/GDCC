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

#include "AR/Wad/Lump.hpp"

#include "AR/BinaryIO.hpp"

#include "Core/File.hpp"
#include "Core/Path.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         //
         // Lump constructor
         //
         Lump::Lump(Core::String name_) :
            wadNext{this},
            wadPrev{this},

            name{name_}
         {
         }

         //
         // Lump destructor
         //
         Lump::~Lump()
         {
            ListUtil::Unlink(this);
         }

         //
         // Lump::sizeHead
         //
         std::size_t Lump::sizeHead() const
         {
            // Normal lumps have a single header entry.
            return 1;
         }

         //
         // Lump::writeDirs
         //
         void Lump::writeDirs(std::string &path) const
         {
            Core::PathRestore pathRestore{path};

            Core::PathAppend(path, name);
            auto buf = Core::FileOpenStream(path.data(),
               std::ios_base::out | std::ios_base::binary);
            std::ostream out{buf.get()};
            writeData(out);
         }

         //
         // Lump::writeHead
         //
         void Lump::writeHead(std::ostream &out, std::size_t offset) const
         {
            WriteLE4 (out, offset);
            WriteLE4 (out, sizeData());
            WriteStrN(out, name, 8);
         }

         //
         // Lump::writeList
         //
         void Lump::writeList(std::ostream &out, std::string &path) const
         {
            out << path << name << '\n';
         }

         //
         // Lump_Data constructor
         //
         Lump_Data::Lump_Data(Core::String name_,
            std::unique_ptr<char[]> &&data_, std::size_t size_) :
            Lump{name_},
            data{std::move(data_)},
            size{size_}
         {
         }

         //
         // Lump_Data::sizeData
         //
         std::size_t Lump_Data::sizeData() const
         {
            return size;
         }

         //
         // Lump_Data::writeData
         //
         void Lump_Data::writeData(std::ostream &out) const
         {
            out.write(data.get(), size);
         }

         //
         // Lump_Empty::sizeData
         //
         std::size_t Lump_Empty::sizeData() const
         {
            return 0;
         }

         //
         // Lump_Empty::writeData
         //
         void Lump_Empty::writeData(std::ostream &) const
         {
         }

         //
         // Lump_File constructor
         //
         Lump_File::Lump_File(Core::String name_, std::unique_ptr<char[]> &&file_) :
            Lump{name_},
            file{std::move(file_)},
            size{Core::FileSize(file.get())}
         {
         }

         //
         // Lump_File::sizeData
         //
         std::size_t Lump_File::sizeData() const
         {
            return size;
         }

         //
         // Lump_File::writeData
         //
         void Lump_File::writeData(std::ostream &out) const
         {
            auto in = Core::FileOpenStream(file.get(), std::ios_base::in | std::ios_base::binary);
            for(int c; (c = in->sbumpc()) != EOF;)
              out.put(c);
         }

         //
         // Lump_FilePart constructor
         //
         Lump_FilePart::Lump_FilePart(Core::String name_, char const *data_,
            std::size_t size_, std::shared_ptr<Core::FileBlock> const &file_) :
            Lump{name_},
            file{file_},
            data{data_},
            size{size_}
         {
         }

         //
         // Lump_FilePart destructor
         //
         Lump_FilePart::~Lump_FilePart()
         {
         }

         //
         // Lump_FilePart::sizeData
         //
         std::size_t Lump_FilePart::sizeData() const
         {
            return size;
         }

         //
         // Lump_FilePart::writeData
         //
         void Lump_FilePart::writeData(std::ostream &out) const
         {
            out.write(data, size);
         }
      }
   }
}

// EOF

