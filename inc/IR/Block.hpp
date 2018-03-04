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

#ifndef GDCC__IR__Block_H__
#define GDCC__IR__Block_H__

#include "../IR/Statement.hpp"

#include "../Core/List.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
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

      struct Nul {Nul(Core::FastU n_ = 1) : n{n_} {} Core::FastU n;};
      struct Stk {Stk(Core::FastU n_ = 1) : n{n_} {} Core::FastU n;};


      Block() = default;
      Block(Block &&) = default;
      ~Block() {while(head.next != &head) delete head.next;}

      Block &operator = (Block &&) = default;

      // addLabel
      Block &addLabel(Core::String lab);
      Block &addLabel(Core::Array<Core::String> &&labs);

      // addStmnt
      Block &addStmnt(Code code)
         {return addStmntArgs(&head, code, {});}
      Block &addStmnt(Statement *link, Code code)
         {return addStmntArgs(link, code, {});}

      template<typename... Args>
      Block &addStmnt(Code code, Args &&...args)
         {return addStmnt(&head, code, std::forward<Args>(args)...);}
      template<typename... Args>
      Block &addStmnt(Statement *link, Code code, Args &&...args)
      {
         Core::Array<Arg> argv{countArgs(args...)};
         unpackArgs(argv.data(), std::forward<Args>(args)...);
         return addStmntArgs(link, code, std::move(argv));
      }

      // addStmntArgs
      Block &addStmntArgs(Code code, Core::Array<Arg> &&args)
         {return addStmntArgs(&head, code, std::move(args));}
      Block &addStmntArgs(Statement *link, Code code, Core::Array<Arg> &&args);

      // begin
            iterator begin()       {return static_cast<      iterator>(head.next);}
      const_iterator begin() const {return static_cast<const_iterator>(head.next);}

      // empty
      bool empty() const {return head.next == &head;}

      // end
            iterator end()       {return static_cast<      iterator>(&head);}
      const_iterator end() const {return static_cast<const_iterator>(&head);}

      // getExp
      Exp::CRef getExp(Glyph const &value);
      Exp::CRef getExp(Core::FastI  value);
      Exp::CRef getExp(Core::FastU  value);
      Exp::CRef getExp(int          value);
      Exp::CRef getExp(unsigned     value);

      // getOrigin
      Core::Origin getOrigin() const {return head.pos;}

      // hasLabelPending
      bool hasLabelPending() const {return !labs.empty();}

      // setArgSize
      Block &setArgSize();
      Block &setArgSize(Core::FastU n) {argSize = n; return *this;}

      // setOrigin
      Block &setOrigin(Core::Origin pos) {head.pos = pos; return *this;}
      Block &setOrigin(Core::FastU line) {head.pos.line = line; return *this;}

      size_type size() const {return std::distance(begin(), end());}


      friend OArchive &operator << (OArchive &out, Block const &in);

      friend IArchive &operator >> (IArchive &in, Block &out);

   private:
      //
      // countArgs
      //
      template<typename... Args>
      std::size_t countArgs(Args const &...)
      {
         return sizeof...(Args);
      }

      #define GDCC_IR_AddrList(a) \
         void unpackArg(Arg *argv, Arg_##a const &arg0) {*argv = arg0;} \
         void unpackArg(Arg *argv, Arg_##a      &&arg0) {*argv = std::move(arg0);}
      #include "../IR/AddrList.hpp"

      void unpackArg(Arg *argv, Arg const &arg0) {*argv = arg0;}
      void unpackArg(Arg *argv, Arg      &&arg0) {*argv = std::move(arg0);}

      // Expression arg, converted to Arg_Lit.
      void unpackArg(Arg *argv, Exp const *arg0);

      // Block::Nul arg, converted to Arg_Nul using current arg size.
      void unpackArg(Arg *argv, Nul const &nul)
         {*argv = IR::Arg_Nul(argSize * nul.n);}

      // Block::Stk arg, converted to Arg_Stk using current arg size.
      void unpackArg(Arg *argv, Stk const &stk)
         {*argv = IR::Arg_Stk(argSize * stk.n);}

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

      Core::FastU               argSize = 0;
      std::vector<Core::String> labs;
      Statement                 head;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, Block const &in);

   IArchive &operator >> (IArchive &in, Block &out);
}

#endif//GDCC__IR__Block_H__

