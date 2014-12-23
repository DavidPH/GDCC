//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statement blocks.
//
//-----------------------------------------------------------------------------

#include "IR/Block.hpp"

#include "IR/Glyph.hpp"
#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include <climits>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Block::addLabel
      //
      Block &Block::addLabel(Core::String lab)
      {
         labs.push_back(lab);
         return *this;
      }

      //
      // Block::addLabel
      //
      Block &Block::addLabel(Core::Array<Core::String> &&lab)
      {
         for(auto l : lab) labs.push_back(l);
         lab = {}; // Explicitly clear lab.
         return *this;
      }

      //
      // Block::addStatement
      //
      Block &Block::addStatement(Statement *link, OpCode op)
      {
         head.labs = Core::Array<Core::String>(Core::Move, labs.begin(), labs.end());
         labs.clear();
         new Statement(&head, link, op);
         return *this;
      }

      //
      // Block::unpackArg<Exp const *>
      //
      void Block::unpackArg(Arg *argv, Exp const *arg0)
      {
         *argv = Arg_Lit(arg0);
      }

      //
      // Block::unpackArg<Glyph>
      //
      void Block::unpackArg(Arg *argv, Glyph const &arg0)
      {
         *argv = Arg_Lit(ExpCreate_Glyph(arg0, head.pos));
      }

      //
      // Block::unpackArg<Core::FastI>
      //
      void Block::unpackArg(Arg *argv, Core::FastI arg0)
      {
         Value_Fixed val{arg0, {sizeof(arg0) * CHAR_BIT, 0, true, false}};
         *argv = Arg_Lit(ExpCreate_Value(std::move(val), head.pos));
      }

      //
      // Block::unpackArg<Core::FastU>
      //
      void Block::unpackArg(Arg *argv, Core::FastU arg0)
      {
         Value_Fixed val{arg0, {sizeof(arg0) * CHAR_BIT, 0, false, false}};
         *argv = Arg_Lit(ExpCreate_Value(std::move(val), head.pos));
      }

      //
      // Block::unpackArg<int>
      //
      void Block::unpackArg(Arg *argv, int arg0)
      {
         Value_Fixed val{arg0, {sizeof(arg0) * CHAR_BIT, 0, true, false}};
         *argv = Arg_Lit(ExpCreate_Value(std::move(val), head.pos));
      }

      //
      // operator OArchive << Block
      //
      OArchive &operator << (OArchive &out, Block const &in)
      {
         out << in.labs << in.head << in.size();
         for(auto const &i : in)
            out << i;
         return out;
      }

      //
      // operator IArchive >> Block
      //
      IArchive &operator >> (IArchive &in, Block &out)
      {
         in >> out.labs >> out.head;
         for(auto count = GetIR<Block::size_type>(in); count--;)
            in >> *new Statement(&out.head);
         return in;
      }
   }
}

// EOF

