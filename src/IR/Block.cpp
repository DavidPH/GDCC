//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "Platform/Platform.hpp"

#include <climits>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
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
   // Block::addStmntArgs
   //
   Block &Block::addStmntArgs(Statement *link, Code code, Core::Array<Arg> &&args)
   {
      head.args = std::move(args);
      head.labs = Core::Array<Core::String>(Core::Move, labs.begin(), labs.end());
      labs.clear();
      new Statement(std::move(head), link, code);
      return *this;
   }

   //
   // Block::getExp<Glyph>
   //
   Exp::CRef Block::getExp(Glyph const &value)
   {
      return ExpCreate_Glyph(value, head.pos);
   }

   //
   // Block::getExp
   //
   Exp::CRef Block::getExp(Core::FastI value)
   {
      Value_Fixed val{Core::NumberCast<Core::Integ>(value),
         {sizeof(value) * CHAR_BIT, 0, true, false}};
      return ExpCreate_Value(std::move(val), head.pos);
   }

   //
   // Block::getExp
   //
   Exp::CRef Block::getExp(Core::FastU value)
   {
      Value_Fixed val{Core::NumberCast<Core::Integ>(value),
         {sizeof(value) * CHAR_BIT, 0, false, false}};
      return ExpCreate_Value(std::move(val), head.pos);
   }

   //
   // Block::getExp
   //
   Exp::CRef Block::getExp(int value)
   {
      Value_Fixed val{Core::NumberCast<Core::Integ>(value),
         {sizeof(value) * CHAR_BIT, 0, true, false}};
      return ExpCreate_Value(std::move(val), head.pos);
   }

   //
   // Block::getExp
   //
   Exp::CRef Block::getExp(unsigned value)
   {
      Value_Fixed val{Core::NumberCast<Core::Integ>(value),
         {sizeof(value) * CHAR_BIT, 0, false, false}};
      return ExpCreate_Value(std::move(val), head.pos);
   }

   //
   // Block::setArgSize
   //
   Block &Block::setArgSize()
   {
      argSize = Platform::GetWordBytes();
      return *this;
   }

   //
   // Block::unpackArg<Exp const *>
   //
   void Block::unpackArg(Arg *argv, Exp const *arg0)
   {
      *argv = Arg_Lit(argSize, arg0);
   }

   //
   // Block::unpackArg<Glyph>
   //
   void Block::unpackArg(Arg *argv, Glyph const &arg0)
   {
      *argv = Arg_Lit(argSize, getExp(arg0));
   }

   //
   // Block::unpackArg<Core::FastI>
   //
   void Block::unpackArg(Arg *argv, Core::FastI arg0)
   {
      *argv = Arg_Lit(argSize, getExp(arg0));
   }

   //
   // Block::unpackArg<Core::FastU>
   //
   void Block::unpackArg(Arg *argv, Core::FastU arg0)
   {
      *argv = Arg_Lit(argSize, getExp(arg0));
   }

   //
   // Block::unpackArg<int>
   //
   void Block::unpackArg(Arg *argv, int arg0)
   {
      *argv = Arg_Lit(argSize, getExp(arg0));
   }

   //
   // Block::unpackArg<unsigned>
   //
   void Block::unpackArg(Arg *argv, unsigned arg0)
   {
      *argv = Arg_Lit(argSize, getExp(arg0));
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

// EOF

