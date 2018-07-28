//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C type-specifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope.hpp"
#include "CC/Type/Struct.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Attribute.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // TypeSpec::finish
   //
   void TypeSpec::finish(SR::Attribute &attr, SR::TypeQual qual, Core::Origin pos) const
   {
      // Validate.
      if(specCplx && specImag) Core::Error(pos, "complex imaginary");
      if(specLong && specShrt) Core::Error(pos, "long short");
      if(specSign && specUnsi) Core::Error(pos, "signed unsigned");

      if(specCplx > 1) Core::Error(pos, "complex complex");
      if(specImag > 1) Core::Error(pos, "imaginary imaginary");
      if(specLong > 2) Core::Error(pos, "long long long");
      if(specSatu > 1) Core::Error(pos, "sat sat");
      if(specShrt > 1) Core::Error(pos, "short short");
      if(specSign > 1) Core::Error(pos, "signed signed");
      if(specUnsi > 1) Core::Error(pos, "unsigned unsigned");

      // Set attribute type.
      switch(specBase)
      {
      case BaseNone:
         if(specLong || specShrt || specSign || specUnsi)
            goto case_BaseInte;

         Core::Error(pos, "missing type specifier");

      case BaseName:
         if(specCplx) Core::Error(pos, "complex typedef-name");
         if(specImag) Core::Error(pos, "imaginary typedef-name");
         if(specLong) Core::Error(pos, "long typedef-name");
         if(specSatu) Core::Error(pos, "sat typedef-name");
         if(specShrt) Core::Error(pos, "short typedef-name");
         if(specSign) Core::Error(pos, "signed typedef-name");
         if(specUnsi) Core::Error(pos, "unsigned typedef-name");

         // Merge qualifiers from typedef.
         {
            auto qualTmp = attr.type->getQual();

            qualTmp.aAtom |= qual.aAtom;
            qualTmp.aCons |= qual.aCons;
            qualTmp.aRest |= qual.aRest;
            qualTmp.aVola |= qual.aVola;

            if(qualTmp.space.base == IR::AddrBase::Gen)
               qualTmp.space = qual.space;
            else if(qual.space.base != IR::AddrBase::Gen)
               Core::Error(pos, "multiple address-space-specifier");

            qual = qualTmp;
         }

         break;

      case BaseBool:
         if(specCplx) Core::Error(pos, "complex bool");
         if(specImag) Core::Error(pos, "imaginary bool");
         if(specLong) Core::Error(pos, "long bool");
         if(specSatu) Core::Error(pos, "sat bool");
         if(specShrt) Core::Error(pos, "short bool");
         if(specSign) Core::Error(pos, "signed bool");
         if(specUnsi) Core::Error(pos, "unsigned bool");

         attr.type = TypeBool;

         break;

      case BaseChar:
         if(specCplx) Core::Error(pos, "complex char");
         if(specImag) Core::Error(pos, "imaginary char");
         if(specLong) Core::Error(pos, "long char");
         if(specSatu) Core::Error(pos, "sat char");
         if(specShrt) Core::Error(pos, "short char");

              if(specSign) attr.type = TypeIntegPrSHH;
         else if(specUnsi) attr.type = TypeIntegPrUHH;
         else              attr.type = TypeChar;

         break;

      case BaseDoub:
         if(specLong > 1) Core::Error(pos, "long long double");
         if(specSatu)     Core::Error(pos, "sat double");
         if(specShrt)     Core::Error(pos, "short double");
         if(specSign)     Core::Error(pos, "signed double");
         if(specUnsi)     Core::Error(pos, "unsigned double");

              if(specCplx) attr.type = specLong ? TypeFloatCSLL : TypeFloatCSL;
         else if(specImag) attr.type = specLong ? TypeFloatISLL : TypeFloatISL;
         else              attr.type = specLong ? TypeFloatRSLL : TypeFloatRSL;

         break;

      case BaseFloa:
         if(specLong) Core::Error(pos, "long float");
         if(specSatu) Core::Error(pos, "sat float");
         if(specShrt) Core::Error(pos, "short float");
         if(specSign) Core::Error(pos, "signed float");
         if(specUnsi) Core::Error(pos, "unsigned float");

              if(specCplx) attr.type = TypeFloatCS;
         else if(specImag) attr.type = TypeFloatIS;
         else              attr.type = TypeFloatRS;

         break;

      case_BaseInte:
      case BaseInte:
         if(specCplx) Core::Error(pos, "complex int");
         if(specImag) Core::Error(pos, "imaginary int");
         if(specSatu) Core::Error(pos, "sat int");

         switch(static_cast<int>(specLong) - static_cast<int>(specShrt))
         {
         case -1: attr.type = specUnsi ? TypeIntegPrUH  : TypeIntegPrSH;  break;
         case  0: attr.type = specUnsi ? TypeIntegPrU   : TypeIntegPrS;   break;
         case  1: attr.type = specUnsi ? TypeIntegPrUL  : TypeIntegPrSL;  break;
         case  2: attr.type = specUnsi ? TypeIntegPrULL : TypeIntegPrSLL; break;
         }

         break;

      case BaseVoid:
         if(specCplx) Core::Error(pos, "complex void");
         if(specImag) Core::Error(pos, "imaginary void");
         if(specLong) Core::Error(pos, "long void");
         if(specSatu) Core::Error(pos, "sat void");
         if(specShrt) Core::Error(pos, "short void");
         if(specSign) Core::Error(pos, "signed void");
         if(specUnsi) Core::Error(pos, "unsigned void");

         attr.type = SR::Type::Void;

         break;

      case BaseAccu:
         if(specCplx)     Core::Error(pos, "complex accum");
         if(specImag)     Core::Error(pos, "imaginary accum");
         if(specLong > 1) Core::Error(pos, "long long accum");

         if(specSatu) switch(static_cast<int>(specLong) - static_cast<int>(specShrt))
         {
         case -1: attr.type = specUnsi ? TypeFixedSaUH : TypeFixedSaSH; break;
         case  0: attr.type = specUnsi ? TypeFixedSaU  : TypeFixedSaS;  break;
         case  1: attr.type = specUnsi ? TypeFixedSaUL : TypeFixedSaSL; break;
         }
         else switch(static_cast<int>(specLong) - static_cast<int>(specShrt))
         {
         case -1: attr.type = specUnsi ? TypeFixedPrUH : TypeFixedPrSH; break;
         case  0: attr.type = specUnsi ? TypeFixedPrU  : TypeFixedPrS;  break;
         case  1: attr.type = specUnsi ? TypeFixedPrUL : TypeFixedPrSL; break;
         }

         break;

      case BaseFrac:
         if(specCplx)     Core::Error(pos, "complex fract");
         if(specImag)     Core::Error(pos, "imaginary fract");
         if(specLong > 1) Core::Error(pos, "long long fract");

         if(specSatu) switch(static_cast<int>(specLong) - static_cast<int>(specShrt))
         {
         case -1: attr.type = specUnsi ? TypeFractSaUH : TypeFractSaSH; break;
         case  0: attr.type = specUnsi ? TypeFractSaU  : TypeFractSaS;  break;
         case  1: attr.type = specUnsi ? TypeFractSaUL : TypeFractSaSL; break;
         }
         else switch(static_cast<int>(specLong) - static_cast<int>(specShrt))
         {
         case -1: attr.type = specUnsi ? TypeFractPrUH : TypeFractPrSH; break;
         case  0: attr.type = specUnsi ? TypeFractPrU  : TypeFractPrS;  break;
         case  1: attr.type = specUnsi ? TypeFractPrUL : TypeFractPrSL; break;
         }

         break;

      case BaseStri:
         if(specCplx) Core::Error(pos, "complex str");
         if(specImag) Core::Error(pos, "imaginary str");
         if(specLong) Core::Error(pos, "long str");
         if(specSatu) Core::Error(pos, "sat str");
         if(specShrt) Core::Error(pos, "short str");
         if(specSign) Core::Error(pos, "signed str");
         if(specUnsi) Core::Error(pos, "unsigned str");

         attr.type = SR::Type::StrEnt->getTypePointer();

         break;

      case BaseDivT:
         if(specCplx) Core::Error(pos, "complex div_t");
         if(specImag) Core::Error(pos, "imaginary div_t");
         if(specSatu) Core::Error(pos, "sat div_t");
         if(specShrt) Core::Error(pos, "short div_t");

         switch(specLong)
         {
         case 0: attr.type = specUnsi ? TypeIntegPrU   : TypeIntegPrS;   break;
         case 1: attr.type = specUnsi ? TypeIntegPrUL  : TypeIntegPrSL;  break;
         case 2: attr.type = specUnsi ? TypeIntegPrULL : TypeIntegPrSLL; break;
         }

         attr.type = Type_Div::Get(attr.type);

         break;

      case BaseLabl:
         if(specCplx) Core::Error(pos, "complex label");
         if(specImag) Core::Error(pos, "imaginary label");
         if(specLong) Core::Error(pos, "long label");
         if(specSatu) Core::Error(pos, "sat label");
         if(specShrt) Core::Error(pos, "short label");
         if(specSign) Core::Error(pos, "signed label");
         if(specUnsi) Core::Error(pos, "unsigned label");

         attr.type = SR::Type::Label;

         break;

      case BaseStrE:
         if(specCplx) Core::Error(pos, "complex str_ent");
         if(specImag) Core::Error(pos, "imaginary str_ent");
         if(specLong) Core::Error(pos, "long str_ent");
         if(specSatu) Core::Error(pos, "sat str_ent");
         if(specShrt) Core::Error(pos, "short str_ent");
         if(specSign) Core::Error(pos, "signed str_ent");
         if(specUnsi) Core::Error(pos, "unsigned str_ent");

         attr.type = SR::Type::StrEnt;

         break;

      case BaseVaLi:
         if(specCplx) Core::Error(pos, "complex va_list");
         if(specImag) Core::Error(pos, "imaginary va_list");
         if(specLong) Core::Error(pos, "long va_list");
         if(specSatu) Core::Error(pos, "sat va_list");
         if(specShrt) Core::Error(pos, "short va_list");
         if(specSign) Core::Error(pos, "signed va_list");
         if(specUnsi) Core::Error(pos, "unsigned va_list");

         attr.type = SR::Type::Void
            ->getTypeQual({{IR::AddrBase::Sta, Core::STR_}})
            ->getTypePointer();

         break;
      }

      // _Atomic constraints.
      if(qual.aAtom)
      {
         if(attr.type->isTypeArray())
            Core::Error(pos, "atomic array");

         if(attr.type->isCTypeFunction())
            Core::Error(pos, "atomic function");
      }

      // Apply qualifiers.
      attr.type = attr.type->getTypeQual(qual);
   }

   //
   // Parser::isTypeSpec
   //
   bool Parser::isTypeSpec(Scope &scope)
   {
      auto const &tok = in.peek();
      if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
         return false;

      switch(tok.str)
      {
         // type-specifier
      case Core::STR___div_t:    return true;
      case Core::STR___fixed:    return true;
      case Core::STR___label:    return true;
      case Core::STR___str:      return true;
      case Core::STR___str_ent:  return true;
      case Core::STR___va_list:  return true;
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

         // atomic-type-specifier
      case Core::STR__Atomic:    return isTypeSpec_atomic(scope);

         // struct-or-union-specifier
      case Core::STR_struct:     return true;
      case Core::STR_union:      return true;

         // enum-specifier
      case Core::STR_enum:       return true;

         // typedef-name
      default: return scope.lookup(tok.str).res == Lookup::Type;
      }
   }

   //
   // Parser::parseTypeSpec
   //
   void Parser::parseTypeSpec(Scope &scope, SR::Attribute &attr, TypeSpec &spec)
   {
      auto const &tok = expectIdenti(true);

      //
      // setSpecBase
      //
      auto setSpecBase = [&](TypeSpec::SpecBase base)
      {
         if(spec.specBase)
            Core::Error(tok.pos, "multiple type-specifier base");

         spec.specBase = base;
      };

      switch(tok.str)
      {
         // type-specifier
      case Core::STR___div_t:    setSpecBase(TypeSpec::BaseDivT); break;
      case Core::STR___fixed:    setSpecBase(TypeSpec::BaseAccu); break;
      case Core::STR___label:    setSpecBase(TypeSpec::BaseLabl); break;
      case Core::STR___str:      setSpecBase(TypeSpec::BaseStri); break;
      case Core::STR___str_ent:  setSpecBase(TypeSpec::BaseStrE); break;
      case Core::STR___va_list:  setSpecBase(TypeSpec::BaseVaLi); break;
      case Core::STR__Accum:     setSpecBase(TypeSpec::BaseAccu); break;
      case Core::STR__Bool:      setSpecBase(TypeSpec::BaseBool); break;
      case Core::STR__Complex:   ++spec.specCplx;                 break;
      case Core::STR__Fract:     setSpecBase(TypeSpec::BaseFrac); break;
      case Core::STR__Imaginary: ++spec.specImag;                 break;
      case Core::STR__Sat:       ++spec.specSatu;                 break;
      case Core::STR_char:       setSpecBase(TypeSpec::BaseChar); break;
      case Core::STR_double:     setSpecBase(TypeSpec::BaseDoub); break;
      case Core::STR_float:      setSpecBase(TypeSpec::BaseFloa); break;
      case Core::STR_int:        setSpecBase(TypeSpec::BaseInte); break;
      case Core::STR_long:       ++spec.specLong;                 break;
      case Core::STR_short:      ++spec.specShrt;                 break;
      case Core::STR_signed:     ++spec.specSign;                 break;
      case Core::STR_unsigned:   ++spec.specUnsi;                 break;
      case Core::STR_void:       setSpecBase(TypeSpec::BaseVoid); break;

         // atomic-type-specifier
      case Core::STR__Atomic: parseTypeSpec_atomic(scope, attr, spec); break;

         // struct-or-union-specifier
      case Core::STR_struct: parseTypeSpec_struct(scope, attr, spec); break;
      case Core::STR_union:  parseTypeSpec_union( scope, attr, spec); break;

         // enum-specifier
      case Core::STR_enum: parseTypeSpec_enum(scope, attr, spec); break;

      default:
         // typedef-name
         auto lookup = scope.lookup(tok.str);
         if(lookup.res != Lookup::Type)
            Core::ErrorExpect("type-specifier", tok);

         setSpecBase(TypeSpec::BaseName);
         attr.type = lookup.resType;

         break;
      }
   }
}

// EOF

