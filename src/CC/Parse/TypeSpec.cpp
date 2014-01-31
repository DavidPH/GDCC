//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C type-specifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope.hpp"
#include "CC/Type.hpp"
#include "CC/Type/Struct.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Bytecode/Platform.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IsTypeSpec_Atomic
//
static bool IsTypeSpec_Atomic(GDCC::CC::ParserData &in, GDCC::CC::Scope *)
{
   // If not followed by a parenthesis, it is a type-qualifier.
   in.in.get();
   bool res = in.in.peek().tok == GDCC::Core::TOK_ParenO;
   in.in.unget();
   return res;
}

//
// ParseTypeSpec_Atomic
//
// atomic-type-specifier:
//    <_Atomic> ( type-name )
//
static void ParseTypeSpec_Atomic(GDCC::CC::ParserData &in, GDCC::CC::Scope *ctx,
   GDCC::AST::Attribute &attr, GDCC::CC::TypeSpec &spec)
{
   using namespace GDCC;

   if(spec.specBase)
      throw Core::ExceptStr(in.in.reget().pos, "multiple type-specifier base");

   spec.specBase = CC::TypeSpec::BaseName;

   // (
   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   // type-name
   auto type = CC::GetType(in, ctx);

   // )
   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   // Constraints.
   if(type->isTypeArray())
      throw Core::ExceptStr(in.in.peek().pos, "atomic array");

   if(type->isCTypeFunction())
      throw Core::ExceptStr(in.in.peek().pos, "atomic function");

   if(type->getQual())
      throw Core::ExceptStr(in.in.peek().pos, "atomic qualified");

   // Set attribute type.
   auto qual = type->getQual();
   qual.aAtom = true;
   attr.type = type->getTypeQual(qual);
}

//
// ParseTypeSpec_Struct
//
// struct-or-union-specifier:
//    struct-or-union identifier(opt) { struct-declaration-list }
//    struct-or-union identifier
//
static void ParseTypeSpec_Struct(GDCC::CC::ParserData &in, GDCC::CC::Scope *ctx,
   GDCC::AST::Attribute &attr, GDCC::CC::TypeSpec &spec, bool isUnion)
{
   using namespace GDCC;

   if(spec.specBase)
      throw Core::ExceptStr(in.in.reget().pos, "multiple type-specifier base");

   spec.specBase = CC::TypeSpec::BaseName;

   CC::Type_Struct::Ptr type;

   // Named structure.
   if(in.in.peek().tok == Core::TOK_Identi)
   {
      auto name  = in.in.get();
      bool defin = in.in.peek().tok == Core::TOK_BraceO;

      // Start with a local lookup. If the full lookup type is incompatible, it
      // is only an error if it is incompatible with a type from this scope.
      auto lookup = ctx->findTypeTag(name.str);
      bool local  = lookup;

      // If definition, only check for existing in current scope.
      if(!lookup && !defin) lookup = ctx->lookupTypeTag(name.str);

      // Existing type, check for compatibility.
      if(lookup)
      {
         char const *err = nullptr;

         if(isUnion && !lookup->isCTypeUnion())
            err = "tag redefined as union";

         else if(!isUnion && !lookup->isCTypeStruct())
            err = "tag redefined as struct";

         else if(defin && lookup->isTypeComplete())
            err = "tag redefined";

         if(err)
         {
            if(local) throw Core::ExceptStr(name.pos, err);
            lookup = nullptr;
         }
      }

      // No existing compatible type.
      if(!lookup)
      {
         type = CC::Type_Struct::Create(name.str, isUnion);
         ctx->addTypeTag(name.str, type);
      }
      else
         type = static_cast<CC::Type_Struct *>(&*lookup);
   }

   // Anonymous structure.
   else
   {
      // Must be a definition.
      if(in.in.peek().tok != Core::TOK_BraceO)
         throw Core::ExceptStr(in.in.peek().pos, "expected identifier");

      type = CC::Type_Struct::Create(Core::STRNULL, isUnion);
   }

   attr.type = type;

   // Remainder of this function deals with parsing the definition itself.
   if(!in.in.drop(Core::TOK_BraceO))
      return;

   // List of members.
   std::vector<CC::Type_Struct::Member> memv;

   // Current address in structure. (Remains 0 for unions.)
   Core::FastU addr = 0;

   // Does this structure have a Flexible Array Member?
   bool hasFAM = false;

   // Current size of structure.
   Core::FastU size = 0;

   // Bit-field information.
   Core::FastU bitsAddr = 0;
   Core::FastU bitsAlig = Bytecode::GetWordAlign();
   Core::FastU bitsSize = Bytecode::GetWordBytes();
   Core::FastU bitsWord = Bytecode::GetWordBits();

   //
   // addrAlign
   //
   auto addrAlign = [&](Core::FastU sizeAlign)
   {
      if(!isUnion)
         size = addr = (addr + sizeAlign - 1) / sizeAlign * sizeAlign;
   };

   //
   // addrBytes
   //
   auto addrBytes = [&](Core::FastU sizeBytes)
   {
      if(isUnion)
         size = std::max(size, sizeBytes);
      else
         size = addr += sizeBytes;
   };

   //
   // bitsAlign
   //
   auto bitsAlign = [&]()
   {
      if(!isUnion && !bitsAddr)
         addr = (addr + bitsAlig - 1) / bitsAlig * bitsAlig;
   };

   //
   // bitsReset
   //
   auto bitsReset = [&]()
   {
      if(bitsAddr)
      {
         bitsAddr = 0;

         addrBytes(bitsSize);
      }
   };

   //
   // findName
   //
   auto findName = [&](Core::String name) -> bool
   {
      if(!name) return false;

      for(auto const &mem : memv)
         if(mem.name == name) return true;

      return false;
   };

   // struct-declaration-list:
   //    struct-declaration
   //    struct-declaration-list struct-declaration
   do
   {
      // struct-declaration:
      //    specifier-qualifier-list struct-declarator-list(opt) ;
      //    static_assert-declaration

      // static_assert-declaration
      if(IsStaticAssert(in, ctx))
      {
         ParseStaticAssert(in, ctx);
         continue;
      }

      // specifier-qualifier-list struct-declarator-list(opt) ;
      if(IsSpecQual(in, ctx))
      {
         if(hasFAM)
            throw Core::ExceptStr(in.in.peek().pos,
               "member after flexible array");

         // specifier-qualifier-list
         AST::Attribute attrBase;
         ParseSpecQual(in, ctx, attrBase);

         // Special handling for non-declarator declaration.
         if(in.in.peek().tok == Core::TOK_Semico)
         {
            if(attrBase.type->isTypeComplete())
               throw Core::ExceptStr(in.in.reget().pos, "incomplete member");

            addrAlign(attrBase.type->getSizeAlign());

            // Anonymous struct/union.
            if((attrBase.type->isCTypeStruct() || attrBase.type->isCTypeUnion()) &&
               !attrBase.type->getName())
            {
               CC::Type_Struct::CRef attrType{&*attrBase.type};
               for(auto const &mem : attrType->getMembers())
                  memv.emplace_back(mem.name, mem.type, mem.addr + addr);
            }
            else
               memv.emplace_back(Core::STRNULL, attrBase.type, addr);

            addrBytes(attrBase.type->getSizeBytes());
         }

         // struct-declarator-list:
         //    struct-declarator
         //    struct-declarator-list , struct-declarator
         else do
         {
            // struct-declarator:
            //    declarator
            //    declarator(opt) : constant-expression

            AST::Attribute attrTemp = attrBase;

            if(IsDeclarator(in, ctx))
               ParseDeclarator(in, ctx, attrTemp);

            // Bitfield member.
            if(in.in.drop(Core::TOK_Colon))
            {
               auto pos  = in.in.reget().pos;
               auto bits = CC::ExpToFastU(GetExp_Cond(in, ctx));

               if(bits)
               {
                  // Check that name is valid.
                  if(findName(attrTemp.name))
                     throw Core::ExceptStr(pos, "name reused");

                  // Check for too many bits to fit in a word.
                  if(bits > bitsWord)
                     throw Core::ExceptStr(pos, "too many bits");

                  // Check for enough size in the current word.
                  if(bitsAddr + bits > bitsWord)
                     bitsReset();

                  // Align underlying storage.
                  bitsAlign();

                  AST::Type::CRef bitsType{attrTemp.type};

                  // bitsType must be qualified or unqualified signed int,
                  // unsigned int, or _Bool.
                  if(!bitsType->isCTypeInteg())
                     throw Core::ExceptStr(pos, "required integer type");

                  // Create bitfield type.
                  bitsType = bitsType
                     ->getTypeBitfield(0, bits - bitsType->getSizeBitsS(), bitsAddr)
                     ->getTypeQual(bitsType->getQual());

                  // Add member.
                  memv.emplace_back(attrTemp.name, bitsType, addr);

                  // Increase bit address.
                  if(!isUnion) bitsAddr += bits;
               }

               // Zero-length bitfield resets bit position.
               else
               {
                  if(attrTemp.name)
                     throw Core::ExceptStr(pos, "named empty bitfield");

                  bitsReset();
               }
            }

            // Normal data member.
            else
            {
               if(findName(attrTemp.name))
                  throw Core::ExceptStr(in.in.reget().pos, "name reused");

               bitsReset();

               // Normally, only allow complete types.
               if(attrTemp.type->isTypeComplete())
               {
                  addrAlign(attrTemp.type->getSizeAlign());

                  memv.emplace_back(attrTemp.name, attrTemp.type, addr);

                  addrBytes(attrTemp.type->getSizeBytes());
               }
               else
               {
                  // Unless this is a flexible array member.
                  if(!attrTemp.type->isTypeArray())
                     throw Core::ExceptStr(in.in.reget().pos, "incomplete member");

                  addrAlign(attrTemp.type->getSizeAlign());

                  memv.emplace_back(attrTemp.name, attrTemp.type, addr);

                  // A flexible array member must be the last member. Therefore,
                  // there must be no further declarators.
                  hasFAM = true;
                  break;
               }
            }
         }
         while(in.in.drop(Core::TOK_Comma));

         // ;
         if(!in.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(in.in.peek().pos, "expected ';'");
      }
   }
   while(!in.in.drop(Core::TOK_BraceC));

   if(memv.empty())
      throw Core::ExceptStr(in.in.reget().pos, "empty structure");

   bitsReset();

   type->setMembers(memv.data(), memv.size(), size);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // TypeSpec::finish
      //
      void TypeSpec::finish(AST::Attribute &attr, AST::TypeQual qual,
         Core::Origin pos) const
      {
         // Validate.
         if(specCplx && specImag) throw Core::ExceptStr(pos, "complex imaginary");
         if(specLong && specShrt) throw Core::ExceptStr(pos, "long short");
         if(specSign && specUnsi) throw Core::ExceptStr(pos, "signed unsigned");

         if(specCplx > 1) throw Core::ExceptStr(pos, "complex complex");
         if(specImag > 1) throw Core::ExceptStr(pos, "imaginary imaginary");
         if(specLong > 2) throw Core::ExceptStr(pos, "long long long");
         if(specSatu > 1) throw Core::ExceptStr(pos, "sat sat");
         if(specShrt > 1) throw Core::ExceptStr(pos, "short short");
         if(specSign > 1) throw Core::ExceptStr(pos, "signed signed");
         if(specUnsi > 1) throw Core::ExceptStr(pos, "unsigned unsigned");

         // Set attribute type.
         switch(specBase)
         {
         case BaseNone:
            throw Core::ExceptStr(pos, "missing type specifier");

         case BaseName:
            if(specCplx) throw Core::ExceptStr(pos, "complex typedef-name");
            if(specImag) throw Core::ExceptStr(pos, "imaginary typedef-name");
            if(specLong) throw Core::ExceptStr(pos, "long typedef-name");
            if(specSatu) throw Core::ExceptStr(pos, "sat typedef-name");
            if(specShrt) throw Core::ExceptStr(pos, "short typedef-name");
            if(specSign) throw Core::ExceptStr(pos, "signed typedef-name");
            if(specUnsi) throw Core::ExceptStr(pos, "unsigned typedef-name");

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
                  throw Core::ExceptStr(pos, "multiple address-space-specifier");

               qual = qualTmp;
            }

            break;

         case BaseBool:
            if(specCplx) throw Core::ExceptStr(pos, "complex bool");
            if(specImag) throw Core::ExceptStr(pos, "imaginary bool");
            if(specLong) throw Core::ExceptStr(pos, "long bool");
            if(specSatu) throw Core::ExceptStr(pos, "sat bool");
            if(specShrt) throw Core::ExceptStr(pos, "short bool");
            if(specSign) throw Core::ExceptStr(pos, "signed bool");
            if(specUnsi) throw Core::ExceptStr(pos, "unsigned bool");

            attr.type = TypeBool;

            break;

         case BaseChar:
            if(specCplx) throw Core::ExceptStr(pos, "complex char");
            if(specImag) throw Core::ExceptStr(pos, "imaginary char");
            if(specLong) throw Core::ExceptStr(pos, "long char");
            if(specSatu) throw Core::ExceptStr(pos, "sat char");
            if(specShrt) throw Core::ExceptStr(pos, "short char");

                 if(specSign) attr.type = TypeIntegPrSHH;
            else if(specUnsi) attr.type = TypeIntegPrUHH;
            else              attr.type = TypeChar;

            break;

         case BaseDoub:
            if(specLong > 1) throw Core::ExceptStr(pos, "long long double");
            if(specSatu)     throw Core::ExceptStr(pos, "sat double");
            if(specShrt)     throw Core::ExceptStr(pos, "short double");
            if(specSign)     throw Core::ExceptStr(pos, "signed double");
            if(specUnsi)     throw Core::ExceptStr(pos, "unsigned double");

                 if(specCplx) attr.type = specLong ? TypeFloatCSLL : TypeFloatCSL;
            else if(specImag) attr.type = specLong ? TypeFloatISLL : TypeFloatISL;
            else              attr.type = specLong ? TypeFloatRSLL : TypeFloatRSL;

            break;

         case BaseFloa:
            if(specLong) throw Core::ExceptStr(pos, "long float");
            if(specSatu) throw Core::ExceptStr(pos, "sat float");
            if(specShrt) throw Core::ExceptStr(pos, "short float");
            if(specSign) throw Core::ExceptStr(pos, "signed float");
            if(specUnsi) throw Core::ExceptStr(pos, "unsigned float");

                 if(specCplx) attr.type = TypeFloatCS;
            else if(specImag) attr.type = TypeFloatIS;
            else              attr.type = TypeFloatRS;

            break;

         case BaseInte:
            if(specCplx) throw Core::ExceptStr(pos, "complex int");
            if(specImag) throw Core::ExceptStr(pos, "imaginary int");
            if(specSatu) throw Core::ExceptStr(pos, "sat int");

            switch(static_cast<int>(specLong) - static_cast<int>(specShrt))
            {
            case -1: attr.type = specUnsi ? TypeIntegPrUH  : TypeIntegPrSH;  break;
            case  0: attr.type = specUnsi ? TypeIntegPrU   : TypeIntegPrS;   break;
            case  1: attr.type = specUnsi ? TypeIntegPrUL  : TypeIntegPrSL;  break;
            case  2: attr.type = specUnsi ? TypeIntegPrULL : TypeIntegPrSLL; break;
            }

            break;

         case BaseVoid:
            if(specCplx) throw Core::ExceptStr(pos, "complex void");
            if(specImag) throw Core::ExceptStr(pos, "imaginary void");
            if(specLong) throw Core::ExceptStr(pos, "long void");
            if(specSatu) throw Core::ExceptStr(pos, "sat void");
            if(specShrt) throw Core::ExceptStr(pos, "short void");
            if(specSign) throw Core::ExceptStr(pos, "signed void");
            if(specUnsi) throw Core::ExceptStr(pos, "unsigned void");

            attr.type = TypeVoid;

            break;

         case BaseAccu:
            if(specCplx)     throw Core::ExceptStr(pos, "complex accum");
            if(specImag)     throw Core::ExceptStr(pos, "imaginary accum");
            if(specLong > 1) throw Core::ExceptStr(pos, "long long accum");

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
            if(specCplx)     throw Core::ExceptStr(pos, "complex fract");
            if(specImag)     throw Core::ExceptStr(pos, "imaginary fract");
            if(specLong > 1) throw Core::ExceptStr(pos, "long long fract");

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
            if(specCplx) throw Core::ExceptStr(pos, "complex str");
            if(specImag) throw Core::ExceptStr(pos, "imaginary str");
            if(specLong) throw Core::ExceptStr(pos, "long str");
            if(specSatu) throw Core::ExceptStr(pos, "sat str");
            if(specShrt) throw Core::ExceptStr(pos, "short str");
            if(specSign) throw Core::ExceptStr(pos, "signed str");
            if(specUnsi) throw Core::ExceptStr(pos, "unsigned str");

            attr.type = TypeStr;

            break;
         }

         // _Atomic constraints.
         if(qual.aAtom)
         {
            if(attr.type->isTypeArray())
               throw Core::ExceptStr(pos, "atomic array");

            if(attr.type->isCTypeFunction())
               throw Core::ExceptStr(pos, "atomic function");
         }

         // Apply qualifiers.
         attr.type = attr.type->getTypeQual(qual);
      }

      //
      // IsTypeSpec
      //
      bool IsTypeSpec(ParserData &in, Scope *ctx)
      {
         auto const &tok = in.in.peek();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            return false;

         switch(tok.str)
         {
            // type-specifier
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

            // atomic-type-specifier
         case Core::STR__Atomic: return IsTypeSpec_Atomic(in, ctx);

            // struct-or-union-specifier
         case Core::STR_struct:     return true;
         case Core::STR_union:      return true;

            // enum-specifier
         case Core::STR_enum:       return true;

            // typedef-name
         default: return ctx->lookup(tok.str).res == Lookup::Type;
         }
      }

      //
      // ParseTypeSpec
      //
      void ParseTypeSpec(ParserData &in, Scope *ctx, AST::Attribute &attr,
         TypeSpec &spec)
      {
         auto const &tok = in.in.get();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            throw Core::ExceptStr(tok.pos, "expected type-specifier");

         //
         // setSpecBase
         //
         auto setSpecBase = [&](TypeSpec::SpecBase base)
         {
            if(spec.specBase)
               throw Core::ExceptStr(tok.pos, "multiple type-specifier base");

            spec.specBase = base;
         };

         switch(tok.str)
         {
            // type-specifier
         case Core::STR___fixed:    setSpecBase(TypeSpec::BaseAccu); break;
         case Core::STR___str:      setSpecBase(TypeSpec::BaseStri); break;
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
         case Core::STR__Atomic: ParseTypeSpec_Atomic(in, ctx, attr, spec); break;

            // struct-or-union-specifier
         case Core::STR_struct: ParseTypeSpec_Struct(in, ctx, attr, spec, false); break;
         case Core::STR_union:  ParseTypeSpec_Struct(in, ctx, attr, spec, true);  break;

            // enum-specifier
         case Core::STR_enum:       throw Core::ExceptStr(tok.pos, "enum stub");

         default:
            // typedef-name
            auto lookup = ctx->lookup(tok.str);
            if(lookup.res != Lookup::Type)
               throw Core::ExceptStr(tok.pos, "expected type-specifier");

            setSpecBase(TypeSpec::BaseName);
            attr.type = lookup.resType;

            break;
         }
      }
   }
}

// EOF

