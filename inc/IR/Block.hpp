//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statement blocks.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Block_H__
#define GDCC__IR__Block_H__

#include "../IR/Statement.hpp"

#include "../Core/List.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Block
      //
      class Block
      {
      public:
         using value_type             = Statement;
         using const_value_type       = value_type const;
         using const_iterator         = Core::ListItr<const_value_type>;
         using const_pointer          = const_value_type *;
         using const_reference        = const_value_type &;
         using const_reverse_iterator = std::reverse_iterator<const_iterator>;
         using difference_type        = std::ptrdiff_t;
         using iterator               = Core::ListItr<value_type>;
         using pointer                = value_type *;
         using reference              = value_type &;
         using reverse_iterator       = std::reverse_iterator<iterator>;
         using size_type              = std::size_t;


         Block() = default;
         Block(Block &&) = default;
         ~Block() {while(head.next != &head) delete head.next;}

         Block &operator = (Block &&) = default;

         // addLabel
         Block &addLabel(Core::String lab);
         Block &addLabel(Core::Array<Core::String> &&labs);

         // addStatement
         Block &addStatement(Statement *link, OpCode op);
         Block &addStatement(OpCode op) {return addStatement(&head, op);}

         // addStatementArgs
         Block &addStatementArgs(Statement *link, OpCode op)
            {return setArgs().addStatement(link, op);}
         Block &addStatementArgs(OpCode op)
            {return setArgs().addStatement(&head, op);}

         // addStatementArgs
         template<typename... Args>
         Block &addStatementArgs(Statement *link, OpCode op, Args &&...args)
            {return setArgs(std::forward<Args>(args)...).addStatement(link, op);}
         template<typename... Args>
         Block &addStatementArgs(OpCode op, Args &&...args)
            {return setArgs(std::forward<Args>(args)...).addStatement(&head, op);}

         // begin
               iterator begin()       {return static_cast<      iterator>(head.next);}
         const_iterator begin() const {return static_cast<const_iterator>(head.next);}

         // empty
         bool empty() const {return head.next == &head;}

         // end
               iterator end()       {return static_cast<      iterator>(&head);}
         const_iterator end() const {return static_cast<const_iterator>(&head);}

         // getExp
         Exp::CRef getExp(Core::FastI value);
         Exp::CRef getExp(Core::FastU value);
         Exp::CRef getExp(int         value);
         Exp::CRef getExp(unsigned    value);

         // getOrigin
         Core::Origin getOrigin() const {return head.pos;}

         // setArgs
         Block &setArgs() {head.args.clear(); return *this;}

         // setArgs
         Block &setArgs(Core::Array<Arg> const &argv)
            {head.args = argv; return *this;}
         Block &setArgs(Core::Array<Arg> &&argv)
            {head.args = std::move(argv); return *this;}

         //
         // setArgs
         //
         template<typename... Args>
         Block &setArgs(Args &&... args)
         {
            Core::Array<Arg> argv{sizeof...(Args)};
            unpackArgs(argv.data(), std::forward<Args>(args)...);
            return setArgs(std::move(argv));
         }

         // setOrigin
         Block &setOrigin(Core::Origin pos) {head.pos = pos; return *this;}

         size_type size() const {return std::distance(begin(), end());}


         friend OArchive &operator << (OArchive &out, Block const &in);

         friend IArchive &operator >> (IArchive &in, Block &out);

      private:
         void unpackArg(Arg *argv, Arg const &arg0) {*argv = arg0;}
         void unpackArg(Arg *argv, Arg      &&arg0) {*argv = std::move(arg0);}

         // Expression arg, converted to Arg_Lit.
         void unpackArg(Arg *argv, Exp const *arg0);

         // Glyph arg, converted to expression, converted to Arg_Lit.
         void unpackArg(Arg *argv, Glyph const &arg0);

         // Numeric arg, converted to Arg_Lit.
         void unpackArg(Arg *argv, Core::FastI arg0);
         void unpackArg(Arg *argv, Core::FastU arg0);
         void unpackArg(Arg *argv, int         arg0);
         void unpackArg(Arg *argv, unsigned    arg0);

         // String arg, explicitly forbidden.
         void unpackArg(Arg *argv, Core::String arg0) = delete;

         //
         // unpackArgs
         //
         template<typename Arg0>
         void unpackArgs(Arg *argv, Arg0 &&arg0)
         {
            unpackArg(argv, std::forward<Arg0>(arg0));
         }

         //
         // unpackArgs
         //
         template<typename Arg0, typename... Args>
         void unpackArgs(Arg *argv, Arg0 &&arg0, Args &&...args)
         {
            unpackArg(argv, std::forward<Arg0>(arg0));
            unpackArgs(argv + 1, std::forward<Args>(args)...);
         }

         std::vector<Core::String> labs;
         Statement                 head;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, Block const &in);

      IArchive &operator >> (IArchive &in, Block &out);
   }
}

#endif//GDCC__IR__Block_H__

