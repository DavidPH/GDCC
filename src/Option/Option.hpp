//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing library.
//
//-----------------------------------------------------------------------------

#ifndef Option__Option_H__
#define Option__Option_H__

#include <ostream>
#include <memory>
#include <stdexcept>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Option
{
   enum
   {
      OPTF_FALSE = 0x0001, // The option is negated.
      OPTF_SHORT = 0x0002, // opt is a pointer to a single char.
      OPTF_FINAL = 0x0004, // There are no more args. (Might never be set.)
      OPTF_KEEPA = 0x0008, // Args may be kept.
   };

   //
   // Exception
   //
   class Exception : public std::exception
   {
   public:
      Exception() = default;
      Exception(Exception const &e);

      virtual char const *what() const noexcept {return msg.get() ? msg.get() : "";}

      [[noreturn]] static void Error(char const *opt, int optf, char const *format, ...);

   private:
      explicit Exception(std::unique_ptr<char const[]> &&msg_) : msg{std::move(msg_)} {}

      std::unique_ptr<char const[]> msg;
   };

   //
   // Option
   //
   // Base class for option processing.
   //
   class Option
   {
   public:
      typedef char const *strp;
      typedef strp const *strv;
      typedef std::size_t uint;


      Option(Option const &) = delete;
      Option(Option &&) = delete;

      Option &operator = (Option const &) = delete;
      Option &operator = (Option &&) = delete;

      uint handle(strp opt, uint optf, uint argc, strv argv)
         {handled = true; return v_handle(opt, optf, argc, argv);}

      strp const descL;
      strp const descS;
      strp const group;
      strp const nameL;
      char const nameS;
      bool       handled;


      static Option *FindL(strp nameL);

      static Option *FindS(char nameS);

      static void PrintHelp(std::ostream &out, uint width = 0);
      static void PrintUsage(std::ostream &out, uint width = 0);
      static void PrintVersion(std::ostream &out, uint width = 0);

      static void ProcessOptions(uint argc, strv argv, uint optf);

      static Option *ArgHandler;

      static strp Help_Program;
      static strp Help_Version;
      static strp Help_Usage;
      static strp Help_DescS;
      static strp Help_DescL;

   protected:
      Option(char nameS, strp nameL, strp group, strp descS, strp descL);
      virtual ~Option();

      virtual uint v_handle(strp opt, uint optf, uint argc, strv argv) = 0;

   private:
      class OptionHead;


      Option() : descL{nullptr}, descS{nullptr}, group{nullptr}, nameL{nullptr},
         nameS{'\0'}, handled{false}, next{this}, prev{this} {}

      Option *next, *prev;


      static Option *GroupBegin(strp group);
      static Option *GroupEnd(strp group, Option *begin);

      static uint GroupLen(Option *begin, Option *end, uint *lenL, uint *lenS);

      static uint ProcessOptionArg(uint argc, strv argv, uint optf);
      static uint ProcessOptionLong(uint argc, strv argv, uint optf);
      static uint ProcessOptionShort(uint argc, strv argv, uint optf);

      static Option &GetHead();
   };

   //
   // OptionCall
   //
   // Calls a normal function for option handling.
   //
   class OptionCall : public Option
   {
   public:
      typedef uint (*OptionFunc)(strp opt, uint optf, uint argc, strv argv);

      OptionCall(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_,
         OptionFunc call_) : Option{nameS_, nameL_, group_, descS_, descL_},
         call{call_} {}

   protected:
      virtual uint v_handle(strp opt, uint optf, uint argc, strv argv)
         {return call(opt, optf, argc, argv);}

      OptionFunc const call;
   };

   //
   // OptionCopy
   //
   // Acts as an alias for another option.
   //
   class OptionCopy : public Option
   {
   public:
      OptionCopy(char nameS_, strp nameL_, Option *copy_) : Option{nameS_,
         nameL_, copy_->group, copy_->nameL, nullptr}, copy{copy_} {}

   protected:
      virtual uint v_handle(strp opt, uint optf, uint argc, strv argv)
         {return copy->handle(opt, optf, argc, argv);}

      Option *const copy;
   };

   //
   // OptionCStr
   //
   // Handles a string option as a C string. Uses OPTF_KEEPA to avoid allocation
   // when possible.
   //
   class OptionCStr : public Option
   {
   public:
      OptionCStr(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_) :
         Option{nameS_, nameL_, group_, descS_, descL_}, data{nullptr}, dlen{0},
         alloc{false} {}
      OptionCStr(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_,
         strp data_, bool alloc_) : Option{nameS_, nameL_, group_, descS_, descL_},
         data{nullptr}, dlen{0}, alloc{false} {reset(data_, alloc_);}
      virtual ~OptionCStr() {if(alloc) delete[] data;}

      void reset() {if(alloc) delete[] data; data = nullptr; dlen = 0; alloc = false;}
      void reset(strp data, bool alloc);

      strp data;
      uint dlen;

   protected:
      virtual uint v_handle(strp opt, uint optf, uint argc, strv argv);

      bool alloc;
   };

   //
   // OptionDPtr
   //
   // Class for handling a single piece of data, stored externally.
   //
   // Instantiations for v_handle are provided for the following types:
   //   bool, int, std::string, std::vector<std::string>.
   //
   template<typename T> class OptionDPtr : public Option
   {
   public:
      OptionDPtr(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_,
         T *dptr_) : Option{nameS_, nameL_, group_, descS_, descL_}, dptr{dptr_} {}

   protected:
      virtual uint v_handle(strp opt, uint optf, uint argc, strv argv);

      T *const dptr;
   };

   //
   // OptionData
   //
   // Class for handling a single piece of data, stored internally.
   //
   template<typename T> class OptionData : public OptionDPtr<T>
   {
   protected:
      using typename Option::strp;

   public:
      OptionData(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_) :
         OptionDPtr<T>{nameS_, nameL_, group_, descS_, descL_, &data}, data{} {}
      OptionData(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_,
         T const &data_) : OptionDPtr<T>{nameS_, nameL_, group_, descS_, descL_,
         &data}, data{data_} {}
      OptionData(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_,
         T &&data_) : OptionDPtr<T>{nameS_, nameL_, group_, descS_, descL_,
         &data}, data{std::move(data_)} {}

      T data;
   };

   //
   // OptionStrV
   //
   // Handles C string vector options.
   //
   class OptionStrV : public Option
   {
   public:
      OptionStrV(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_,
         uint argMax_) : Option{nameS_, nameL_, group_, descS_, descL_}, argC{0},
         argV{nullptr}, argF{nullptr}, argA{0}, argMax{argMax_} {}
      virtual ~OptionStrV();

      // Passes last arg to opt (if any), then removes the arg from list.
      void argPop(Option *opt = nullptr, strp optn = "", uint optf = 0);

      uint  argC; // count
      strp *argV; // vector


      static OptionStrV DefaultArgHandler;

   protected:
      virtual uint v_handle(strp opt, uint optf, uint argc, strv argv);

   private:
      bool *argF; // free needed
      uint  argA; // alloc
      uint const argMax;
   };
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   extern Option::uint const *ArgC;
   extern Option::strv const *ArgV;

   extern OptionStrV &DefaultArgHandler;
}

#endif//Option__Option_H__

