//-----------------------------------------------------------------------------
//
// Copyright (C) 2024-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bytecode module handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__ZDACS__Module_H__
#define GDCC__BC__ZDACS__Module_H__

#include "../../BC/ZDACS/Types.hpp"

#include "../../IR/Exp.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC::ZDACS
{
   //
   // ChunkBase
   //
   // Implements common Chunk characteristics.
   //
   template<typename ElemT>
   class ChunkBase
   {
   public:
      using Elem  = ElemT;
      using ElemV = std::vector<Elem>;

      using iterator = typename ElemV::iterator;
      using size_type = typename ElemV::size_type;

      //
      // operator []
      //
      Elem &operator [] (std::size_t i)
      {
         if(elem.size() <= i)
            elem.resize(i + 1);

         return elem[i];
      }

      iterator begin() {return elem.begin();}

      bool empty() const {return elem.empty();}

      iterator end() {return elem.end();}

      size_type size() const {return elem.size();}

      ElemV elem;
   };

   //
   // ElemArg
   //
   class ElemArg
   {
   public:
      ElemArg() : exp{}, val{} {}
      explicit ElemArg(IR::Exp const *exp) : exp{exp}, val{0} {}
      ElemArg(IR::Exp const *exp, Core::FastU w) : exp{exp}, val{w} {}
      explicit ElemArg(Core::FastU val) : exp{}, val{val} {}

      ElemArg &operator = (IR::Exp const *exp_)
         {exp = exp_; val = 0; return *this;}
      ElemArg &operator = (Core::FastU val_)
         {exp = nullptr; val = val_; return *this;}

      IR::Exp::CPtr exp;
      Core::FastU   val;
   };

   //
   // ElemAIMP
   //
   class ElemAIMP
   {
   public:
      ElemAIMP(Core::String glyph, Core::FastU value, Core::FastU words) :
         glyph{glyph}, value{value}, words{words} {}

      Core::String glyph;
      Core::FastU  value;
      Core::FastU  words;
   };

   //
   // ChunkAIMP
   //
   class ChunkAIMP : public ChunkBase<ElemAIMP>
   {
   public:
      void add(Core::String glyph, Core::FastU value, Core::FastU words)
         {elem.emplace_back(glyph, value, words);}
   };

   //
   // ElemAINI
   //
   class ElemAINI
   {
   public:
      ElemAINI(Core::FastU value, ElemArgs &&inits) :
         inits{std::move(inits)}, value{value} {}

      ElemArgs    inits;
      Core::FastU value;
   };

   //
   // ChunkAINI
   //
   class ChunkAINI : public ChunkBase<ElemAINI>
   {
   public:
      void add(Core::FastU value, ElemArgs &&inits)
         {elem.emplace_back(value, std::move(inits));}
   };

   //
   // ElemARAY
   //
   class ElemARAY
   {
   public:
      ElemARAY(Core::FastU value, Core::FastU words) :
         value{value}, words{words} {}

      Core::FastU value;
      Core::FastU words;
   };

   //
   // ChunkARAY
   //
   class ChunkARAY : public ChunkBase<ElemARAY>
   {
   public:
      void add(Core::FastU value, Core::FastU words)
         {elem.emplace_back(value, words);}
   };

   //
   // ElemASTR
   //
   class ElemASTR
   {
   public:
      ElemASTR(Core::FastU value) : value{value} {}

      Core::FastU value;
   };

   //
   // ChunkASTR
   //
   class ChunkASTR : public ChunkBase<ElemASTR>
   {
   public:
      void add(Core::FastU value)
         {elem.emplace_back(value);}
   };

   //
   // ElemATAG
   //
   class ElemATAG
   {
   public:
      ElemATAG(Core::FastU value, Core::Array<Core::FastU> &&tags) :
         tags{std::move(tags)}, value{value} {}

      Core::Array<Core::FastU> tags;
      Core::FastU              value;
   };

   //
   // ChunkATAG
   //
   class ChunkATAG : public ChunkBase<ElemATAG>
   {
   public:
      void add(Core::FastU value, Core::Array<Core::FastU> &&tags)
         {elem.emplace_back(value, std::move(tags));}
   };

   //
   // ElemCODE
   //
   class ElemCODE
   {
   public:
      ElemCODE() : args{}, code{0}, cpos{0} {}
      ElemCODE(Core::FastU cpos, Core::FastU size, Core::FastU code, ElemArgs &&args) :
         args{std::move(args)}, code{code}, cpos{cpos}, size{size} {}

      ElemArgs    args;
      Core::FastU code;
      Core::FastU cpos;
      Core::FastU size;
   };

   //
   // ChunkCODE
   //
   class ChunkCODE : public ChunkBase<ElemCODE>
   {
   public:
      void add(Core::FastU cpos, Core::FastU size, Core::FastU code, ElemArgs &&args)
         {elem.emplace_back(cpos, size, code, std::move(args));}

      Core::FastU getPos()
         {return elem.empty() ? 0 : elem.back().cpos + elem.back().size;}
   };

   //
   // ElemFARY
   //
   class ElemFARY
   {
   public:
      ElemFARY(Core::FastU value, Core::Array<Core::FastU> &&sizes) :
         sizes{std::move(sizes)}, value{value} {}

      Core::Array<Core::FastU> sizes;
      Core::FastU              value;
   };

   //
   // ChunkFARY
   //
   class ChunkFARY : public ChunkBase<ElemFARY>
   {
   public:
      void add(Core::FastU value, Core::Array<Core::FastU> &&sizes)
         {elem.emplace_back(value, std::move(sizes));}
   };

   //
   // ElemFNAM
   //
   class ElemFNAM
   {
   public:
      ElemFNAM() : glyph{nullptr} {}
      ElemFNAM(Core::String glyph) : glyph{glyph} {}

      Core::String glyph;
   };

   //
   // ChunkFNAM
   //
   class ChunkFNAM : public ChunkBase<ElemFNAM>
   {
   public:
   };

   //
   // ElemFUNC
   //
   class ElemFUNC
   {
   public:
      ElemFUNC() : entry{}, local{0}, param{0}, retrn{0} {}
      ElemFUNC(ElemArg const &entry, Core::FastU param, Core::FastU local, Core::FastU retrn) :
         entry{entry}, local{local}, param{param}, retrn{retrn} {}

      ElemArg     entry;
      Core::FastU local;
      Core::FastU param;
      Core::FastU retrn;
   };

   //
   // ChunkFUNC
   //
   class ChunkFUNC : public ChunkBase<ElemFUNC>
   {
   public:
   };

   //
   // ElemJUMP
   //
   class ElemJUMP
   {
   public:
      ElemJUMP() : entry{} {}
      ElemJUMP(ElemArg const &entry) : entry{entry} {}

      ElemArg entry;
   };

   //
   // ChunkJUMP
   //
   class ChunkJUMP : public ChunkBase<ElemJUMP>
   {
   public:
   };

   //
   // ElemLOAD
   //
   class ElemLOAD
   {
   public:
      ElemLOAD(Core::String glyph) : glyph{glyph} {}

      Core::String glyph;
   };

   //
   // ChunkLOAD
   //
   class ChunkLOAD : public ChunkBase<ElemLOAD>
   {
   public:
      void add(Core::String glyph)
         {elem.emplace_back(glyph);}
   };

   //
   // ElemMEXP
   //
   class ElemMEXP
   {
   public:
      ElemMEXP() : glyph{nullptr} {}
      ElemMEXP(Core::String glyph) : glyph{glyph} {}

      Core::String glyph;
   };

   //
   // ChunkMEXP
   //
   class ChunkMEXP : public ChunkBase<ElemMEXP>
   {
   public:
   };

   //
   // ElemMIMP
   //
   class ElemMIMP
   {
   public:
      ElemMIMP(Core::String glyph, Core::FastU value) :
         glyph{glyph}, value{value} {}

      Core::String glyph;
      Core::FastU  value;
   };

   //
   // ChunkMIMP
   //
   class ChunkMIMP : public ChunkBase<ElemMIMP>
   {
   public:
      void add(Core::String glyph, Core::FastU value)
         {elem.emplace_back(glyph, value);}
   };

   //
   // ElemMINI
   //
   class ElemMINI
   {
   public:
      ElemMINI(Core::FastU value, ElemArgs &&inits) :
         inits{std::move(inits)}, value{value} {}

      ElemArgs    inits;
      Core::FastU value;
   };

   //
   // ChunkMINI
   //
   class ChunkMINI : public ChunkBase<ElemMINI>
   {
   public:
      void add(Core::FastU value, ElemArgs &&inits)
         {elem.emplace_back(value, std::move(inits));}
   };

   //
   // ElemMSTR
   //
   class ElemMSTR
   {
   public:
      ElemMSTR(Core::FastU value) : value{value} {}

      Core::FastU value;
   };

   //
   // ChunkMSTR
   //
   class ChunkMSTR : public ChunkBase<ElemMSTR>
   {
   public:
      void add(Core::FastU value)
         {elem.emplace_back(value);}
   };

   //
   // ElemSARY
   //
   class ElemSARY
   {
   public:
      ElemSARY(Core::FastU value, Core::Array<Core::FastU> &&sizes) :
         sizes{std::move(sizes)}, value{value} {}

      Core::Array<Core::FastU> sizes;
      Core::FastU              value;
   };

   //
   // ChunkSARY
   //
   class ChunkSARY : public ChunkBase<ElemSARY>
   {
   public:
      void add(Core::FastU value, Core::Array<Core::FastU> &&sizes)
         {elem.emplace_back(value, std::move(sizes));}
   };

   //
   // ElemSFLG
   //
   class ElemSFLG
   {
   public:
      ElemSFLG(Core::FastU value, Core::FastU flags) :
         flags{flags}, value{value} {}

      Core::FastU flags;
      Core::FastU value;
   };

   //
   // ChunkSFLG
   //
   class ChunkSFLG : public ChunkBase<ElemSFLG>
   {
   public:
      void add(Core::FastU value, Core::FastU flags)
         {elem.emplace_back(value, flags);}
   };

   //
   // ElemSNAM
   //
   class ElemSNAM
   {
   public:
      ElemSNAM() : glyph{nullptr} {}
      ElemSNAM(Core::String glyph) : glyph{glyph} {}

      Core::String glyph;
   };

   //
   // ChunkSNAM
   //
   class ChunkSNAM : public ChunkBase<ElemSNAM>
   {
   public:
   };

   //
   // ElemSPTR
   //
   class ElemSPTR
   {
   public:
      ElemSPTR(ElemArg const &entry, Core::FastU value, Core::FastU stype, Core::FastU param) :
         entry{entry}, param{param}, stype{stype}, value{value} {}

      ElemArg     entry;
      Core::FastU param;
      Core::FastU stype;
      Core::FastU value;
   };

   //
   // ChunkSPTR
   //
   class ChunkSPTR : public ChunkBase<ElemSPTR>
   {
   public:
      void add(ElemArg const &entry, Core::FastU value, Core::FastU stype, Core::FastU param)
         {elem.emplace_back(entry, value, stype, param);}
   };

   //
   // ElemSTRL
   //
   class ElemSTRL
   {
   public:
      ElemSTRL() : value{nullptr} {}
      ElemSTRL(Core::String value) : value{value} {}

      Core::String value;
   };

   //
   // ChunkSTRL
   //
   class ChunkSTRL : public ChunkBase<ElemSTRL>
   {
   public:
   };

   //
   // ElemSVCT
   //
   class ElemSVCT
   {
   public:
      ElemSVCT(Core::FastU value, Core::FastU local) :
         local{local}, value{value} {}

      Core::FastU local;
      Core::FastU value;
   };

   //
   // ChunkSVCT
   //
   class ChunkSVCT : public ChunkBase<ElemSVCT>
   {
   public:
      void add(Core::FastU value, Core::FastU local)
         {elem.emplace_back(value, local);}
   };

   //
   // Module
   //
   // Stores data for an ACSE/ACSe/ACS0 module.
   //
   class Module
   {
   public:
      ChunkAIMP chunkAIMP;
      ChunkAINI chunkAINI;
      ChunkARAY chunkARAY;
      ChunkASTR chunkASTR;
      ChunkATAG chunkATAG;
      ChunkCODE chunkCODE;
      ChunkFARY chunkFARY;
      ChunkFNAM chunkFNAM;
      ChunkFUNC chunkFUNC;
      ChunkJUMP chunkJUMP;
      ChunkLOAD chunkLOAD;
      ChunkMEXP chunkMEXP;
      ChunkMIMP chunkMIMP;
      ChunkMINI chunkMINI;
      ChunkMSTR chunkMSTR;
      ChunkSARY chunkSARY;
      ChunkSFLG chunkSFLG;
      ChunkSNAM chunkSNAM;
      ChunkSPTR chunkSPTR;
      ChunkSTRL chunkSTRL;
      ChunkSVCT chunkSVCT;
   };
}

#endif//GDCC__BC__ZDACS__Module_H__

