//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C declaration specifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope.hpp"
#include "CC/Type.hpp"

#include "AST/Attribute.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsDeclSpec
      //
      bool IsDeclSpec(ParserData &in, Scope *ctx)
      {
         auto tok = in.in.peek();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            return false;

         switch(static_cast<Core::StringIndex>(tok.str))
         {
         // Storage class specifier.
         case Core::STR__Thread_local: return true;
         case Core::STR_auto:          return true;
         case Core::STR_extern:        return true;
         case Core::STR_register:      return true;
         case Core::STR_static:        return true;
         case Core::STR_typedef:       return true;

         // Type specifier.
         case Core::STR___fixed:    return true;
         case Core::STR___str:      return true;
         case Core::STR__Accum:     return true;
         case Core::STR__Bool:      return true;
         case Core::STR__Complex:   return true;
         case Core::STR__Fract:     return true;
         case Core::STR__Imaginary: return true;
         case Core::STR__Sat:       return true;
         case Core::STR_char:       return true;
         case Core::STR_double:     return true;
         case Core::STR_float:      return true;
         case Core::STR_int:        return true;
         case Core::STR_long:       return true;
         case Core::STR_short:      return true;
         case Core::STR_signed:     return true;
         case Core::STR_unsigned:   return true;
         case Core::STR_void:       return true;

         case Core::STR__Atomic:    return true;

         case Core::STR_struct:     return true;
         case Core::STR_union:      return true;

         case Core::STR_enum:       return true;

         // Function specifier.
         case Core::STR__Noreturn: return true;
         case Core::STR_inline:    return true;

         // Alignment specifier.
         case Core::STR__Alignas: return true;

         // typedef-name
         // type-qualifier
         // attribute-specifier
         default:
            return ctx->lookup(tok.str).res == Lookup::Type ||
               IsAttrSpec(in, ctx) ||
               IsTypeQual(in, ctx);
         }
      }

      //
      // ParseDeclSpec
      //
      void ParseDeclSpec(ParserData &in, Scope *ctx, AST::Attribute &attr)
      {
         enum DeclBase
         {
            declBaseNone,
            declBaseName,

            // C types.
            declBaseBool,
            declBaseChar,
            declBaseDoub,
            declBaseFloa,
            declBaseInte,
            declBaseVoid,

            // Embedded C types.
            declBaseAccu,
            declBaseFrac,

            // ACS types.
            declBaseStri,
         };

         enum DeclStor
         {
            declStorNone,

            declStorAuto,
            declStorExte,
            declStorRegi,
            declStorStat,
            declStorType,
         };

         auto pos = in.in.peek().pos;

         DeclBase      declBase = declBaseNone;
         int           declCplx = 0;
         int           declImag = 0;
         int           declLong = 0;
         AST::TypeQual declQual = AST::QualNone;
         int           declSatu = 0;
         int           declShrt = 0;
         int           declSign = 0;
         DeclStor      declStor = declStorNone;
         int           declThrd = 0;
         int           declUnsi = 0;

         //
         // setDeclBase
         //
         auto setDeclBase = [&](DeclBase base)
         {
            if(declBase)
               throw Core::ExceptStr(pos, "multiple type specifier base");

            declBase = base;
         };

         //
         // setDeclStor
         //
         auto setDeclStor = [&](DeclStor stor)
         {
            if(declStor)
               throw Core::ExceptStr(pos, "multiple storage class specifier");

            declStor = stor;
         };

         // Read declaration-specifier tokens until there are no more.
         for(;;)
         {
            // attribute-specifier
            if(IsAttrSpec(in, ctx))
               {ParseAttrSpec(in, ctx, attr); continue;}

            auto const &tok = in.in.peek();
            if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
               break;

            switch(static_cast<Core::StringIndex>(tok.str))
            {
            // Storage class specifier.
            case Core::STR__Thread_local: ++declThrd;                break;
            case Core::STR_auto:          setDeclStor(declStorAuto); break;
            case Core::STR_extern:        setDeclStor(declStorExte); break;
            case Core::STR_register:      setDeclStor(declStorRegi); break;
            case Core::STR_static:        setDeclStor(declStorStat); break;
            case Core::STR_typedef:       setDeclStor(declStorType); break;

            // Type specifier.
            case Core::STR___fixed:    setDeclBase(declBaseAccu); break;
            case Core::STR___str:      setDeclBase(declBaseStri); break;
            case Core::STR__Accum:     setDeclBase(declBaseAccu); break;
            case Core::STR__Bool:      setDeclBase(declBaseBool); break;
            case Core::STR__Complex:   ++declCplx;                break;
            case Core::STR__Fract:     setDeclBase(declBaseFrac); break;
            case Core::STR__Imaginary: ++declImag;                break;
            case Core::STR__Sat:       ++declSatu;                break;
            case Core::STR_char:       setDeclBase(declBaseChar); break;
            case Core::STR_double:     setDeclBase(declBaseDoub); break;
            case Core::STR_float:      setDeclBase(declBaseFloa); break;
            case Core::STR_int:        setDeclBase(declBaseInte); break;
            case Core::STR_long:       ++declLong;                break;
            case Core::STR_short:      ++declShrt;                break;
            case Core::STR_signed:     ++declSign;                break;
            case Core::STR_unsigned:   ++declUnsi;                break;
            case Core::STR_void:       setDeclBase(declBaseVoid); break;

            case Core::STR__Atomic:    throw Core::ExceptStr(pos, "_Atomic stub");

            case Core::STR_struct:     throw Core::ExceptStr(pos, "struct stub");
            case Core::STR_union:      throw Core::ExceptStr(pos, "union stub");

            case Core::STR_enum:       throw Core::ExceptStr(pos, "enum stub");

            // Function specifier.
            case Core::STR__Noreturn: attr.funcNoReturn = true; break;
            case Core::STR_inline:    attr.funcInline   = true; break;

            // Alignment specifier.
            case Core::STR__Alignas: throw Core::ExceptStr(pos, "_Alignas stub");

            default:
               // Typedef name.
               {
                  auto lookup = ctx->lookup(tok.str);
                  if(lookup.res == Lookup::Type)
                  {
                     if(declBase)
                        throw Core::ExceptStr(pos, "multiple type specifier base");

                     declBase  = declBaseName;
                     attr.type = lookup.resType;

                     break;
                  }
               }

               // Type qualifier.
               if(IsTypeQual(in, ctx))
               {
                  declQual |= GetTypeQual(in, ctx);
                  continue;
               }

               goto parse_done;
            }

            in.in.get();
         }

         parse_done:;

         // Validate the findings.

         if(declThrd > 1 || (declThrd && declStor != declStorNone &&
            declStor != declStorExte && declStor != declStorStat))
            throw Core::ExceptStr(pos, "invalid thread_local");

         if(declCplx && declImag) throw Core::ExceptStr(pos, "complex imaginary");
         if(declLong && declShrt) throw Core::ExceptStr(pos, "long short");
         if(declSign && declUnsi) throw Core::ExceptStr(pos, "signed unsigned");

         if(declCplx > 1) throw Core::ExceptStr(pos, "complex complex");
         if(declImag > 1) throw Core::ExceptStr(pos, "imaginary imaginary");
         if(declLong > 2) throw Core::ExceptStr(pos, "long long long");
         if(declSatu > 1) throw Core::ExceptStr(pos, "sat sat");
         if(declShrt > 1) throw Core::ExceptStr(pos, "short short");
         if(declSign > 1) throw Core::ExceptStr(pos, "signed signed");
         if(declUnsi > 1) throw Core::ExceptStr(pos, "unsigned unsigned");

         // Set attribute type.
         switch(declBase)
         {
         case declBaseNone:
            throw Core::ExceptStr(pos, "missing type specifier");

         case declBaseName:
            if(declCplx) throw Core::ExceptStr(pos, "complex typedef-name");
            if(declImag) throw Core::ExceptStr(pos, "imaginary typedef-name");
            if(declLong) throw Core::ExceptStr(pos, "long typedef-name");
            if(declSatu) throw Core::ExceptStr(pos, "sat typedef-name");
            if(declShrt) throw Core::ExceptStr(pos, "short typedef-name");
            if(declSign) throw Core::ExceptStr(pos, "signed typedef-name");
            if(declUnsi) throw Core::ExceptStr(pos, "unsigned typedef-name");

            declQual = attr.type->getQual() | declQual;

            break;

         case declBaseBool:
            if(declCplx) throw Core::ExceptStr(pos, "complex bool");
            if(declImag) throw Core::ExceptStr(pos, "imaginary bool");
            if(declLong) throw Core::ExceptStr(pos, "long bool");
            if(declSatu) throw Core::ExceptStr(pos, "sat bool");
            if(declShrt) throw Core::ExceptStr(pos, "short bool");
            if(declSign) throw Core::ExceptStr(pos, "signed bool");
            if(declUnsi) throw Core::ExceptStr(pos, "unsigned bool");

            attr.type = TypeBool;

            break;

         case declBaseChar:
            if(declCplx) throw Core::ExceptStr(pos, "complex char");
            if(declImag) throw Core::ExceptStr(pos, "imaginary char");
            if(declLong) throw Core::ExceptStr(pos, "long char");
            if(declSatu) throw Core::ExceptStr(pos, "sat char");
            if(declShrt) throw Core::ExceptStr(pos, "short char");

                 if(declSign) attr.type = TypeIntegPrSHH;
            else if(declUnsi) attr.type = TypeIntegPrUHH;
            else              attr.type = TypeChar;

            break;

         case declBaseDoub:
            if(declLong > 1) throw Core::ExceptStr(pos, "long long double");
            if(declSatu)     throw Core::ExceptStr(pos, "sat double");
            if(declShrt)     throw Core::ExceptStr(pos, "short double");
            if(declSign)     throw Core::ExceptStr(pos, "signed double");
            if(declUnsi)     throw Core::ExceptStr(pos, "unsigned double");

                 if(declCplx) attr.type = declLong ? TypeFloatCSLL : TypeFloatCSL;
            else if(declImag) attr.type = declLong ? TypeFloatISLL : TypeFloatISL;
            else              attr.type = declLong ? TypeFloatRSLL : TypeFloatRSL;

            break;

         case declBaseFloa:
            if(declLong) throw Core::ExceptStr(pos, "long float");
            if(declSatu) throw Core::ExceptStr(pos, "sat float");
            if(declShrt) throw Core::ExceptStr(pos, "short float");
            if(declSign) throw Core::ExceptStr(pos, "signed float");
            if(declUnsi) throw Core::ExceptStr(pos, "unsigned float");

                 if(declCplx) attr.type = TypeFloatCS;
            else if(declImag) attr.type = TypeFloatIS;
            else              attr.type = TypeFloatRS;

            break;

         case declBaseInte:
            if(declCplx) throw Core::ExceptStr(pos, "complex int");
            if(declImag) throw Core::ExceptStr(pos, "imaginary int");
            if(declSatu) throw Core::ExceptStr(pos, "sat int");

            switch(declLong - declShrt)
            {
            case -1: attr.type = declUnsi ? TypeIntegPrUH  : TypeIntegPrSH;  break;
            case  0: attr.type = declUnsi ? TypeIntegPrU   : TypeIntegPrS;   break;
            case  1: attr.type = declUnsi ? TypeIntegPrUL  : TypeIntegPrSL;  break;
            case  2: attr.type = declUnsi ? TypeIntegPrULL : TypeIntegPrSLL; break;
            }

            break;

         case declBaseVoid:
            if(declCplx) throw Core::ExceptStr(pos, "complex void");
            if(declImag) throw Core::ExceptStr(pos, "imaginary void");
            if(declLong) throw Core::ExceptStr(pos, "long void");
            if(declSatu) throw Core::ExceptStr(pos, "sat void");
            if(declShrt) throw Core::ExceptStr(pos, "short void");
            if(declSign) throw Core::ExceptStr(pos, "signed void");
            if(declUnsi) throw Core::ExceptStr(pos, "unsigned void");

            attr.type = TypeVoid;

            break;

         case declBaseAccu:
            if(declCplx)     throw Core::ExceptStr(pos, "complex accum");
            if(declImag)     throw Core::ExceptStr(pos, "imaginary accum");
            if(declLong > 1) throw Core::ExceptStr(pos, "long long accum");

            if(declSatu) switch(declLong - declShrt)
            {
            case -1: attr.type = declUnsi ? TypeFixedSaUH : TypeFixedSaSH; break;
            case  0: attr.type = declUnsi ? TypeFixedSaU  : TypeFixedSaS;  break;
            case  1: attr.type = declUnsi ? TypeFixedSaUL : TypeFixedSaSL; break;
            }
            else switch(declLong - declShrt)
            {
            case -1: attr.type = declUnsi ? TypeFixedPrUH : TypeFixedPrSH; break;
            case  0: attr.type = declUnsi ? TypeFixedPrU  : TypeFixedPrS;  break;
            case  1: attr.type = declUnsi ? TypeFixedPrUL : TypeFixedPrSL; break;
            }

            break;

         case declBaseFrac:
            if(declCplx)     throw Core::ExceptStr(pos, "complex fract");
            if(declImag)     throw Core::ExceptStr(pos, "imaginary fract");
            if(declLong > 1) throw Core::ExceptStr(pos, "long long fract");

            if(declSatu) switch(declLong - declShrt)
            {
            case -1: attr.type = declUnsi ? TypeFractSaUH : TypeFractSaSH; break;
            case  0: attr.type = declUnsi ? TypeFractSaU  : TypeFractSaS;  break;
            case  1: attr.type = declUnsi ? TypeFractSaUL : TypeFractSaSL; break;
            }
            else switch(declLong - declShrt)
            {
            case -1: attr.type = declUnsi ? TypeFractPrUH : TypeFractPrSH; break;
            case  0: attr.type = declUnsi ? TypeFractPrU  : TypeFractPrS;  break;
            case  1: attr.type = declUnsi ? TypeFractPrUL : TypeFractPrSL; break;
            }

            break;

         case declBaseStri:
            if(declCplx) throw Core::ExceptStr(pos, "complex str");
            if(declImag) throw Core::ExceptStr(pos, "imaginary str");
            if(declLong) throw Core::ExceptStr(pos, "long str");
            if(declSatu) throw Core::ExceptStr(pos, "sat str");
            if(declShrt) throw Core::ExceptStr(pos, "short str");
            if(declSign) throw Core::ExceptStr(pos, "signed str");
            if(declUnsi) throw Core::ExceptStr(pos, "unsigned str");

            attr.type = TypeStr;

            break;
         }

         attr.type = attr.type->getTypeQual(declQual);

         // Set attribute storage class.
         if(declThrd) attr.storeThread = true;
         switch(declStor)
         {
         case declStorNone:                        break;
         case declStorAuto: attr.storeAuto = true; break;
         case declStorExte: attr.storeExt  = true; break;
         case declStorRegi: attr.storeReg  = true; break;
         case declStorStat: attr.storeInt  = true; break;
         case declStorType: attr.isTypedef = true; break;
         }
      }
   }
}

// EOF

