;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2015-2018 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Mathematics.
;;
;; Classification and comparison macros. (Implemented as functions.)
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___inf
;;
Function "___inf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 0
   param    = 0
   retrn    = 2
   block
   (
      Retn W 2(Lit(0x7FF0000000000000_64.0))
   )
)

;;
;; ___inff
;;
Function "___inff"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 0
   param    = 0
   retrn    = 1
   block
   (
      Retn W 1(Lit(0x7F800000))
   )
)

;;
;; ___infl
;;
Function "___infl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 0
   param    = 0
   retrn    = 2
   block
   (
      Retn W 2(Lit(0x7FF0000000000000_64.0))
   )
)

;;
;; ___nan
;;
Function "___nan"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 0
   param    = 0
   retrn    = 2
   block
   (
      Retn W 2(Lit(0x7FFFFFFFFFFFFFFF_64.0))
   )
)

;;
;; ___nanf
;;
Function "___nanf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 0
   param    = 0
   retrn    = 1
   block
   (
      Retn W 1(Lit(0x7FFFFFFF))
   )
)

;;
;; ___nanl
;;
Function "___nanl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 0
   param    = 0
   retrn    = 2
   block
   (
      Retn W 2(Lit(0x7FFFFFFFFFFFFFFF_64.0))
   )
)

;;
;; ___fpclassify
;;
Function "___fpclassify"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd     W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      Jcnd_Tab W 1(Stk()
         Lit(0x7FF00000) Lit(:"$emax")
         Lit(0x00000000) Lit(:"$emin"))
      Move     W 1(Nul() Stk())
      Move     W 1(Stk() Lit(3))
      Retn     W 1(Stk())

   :"$emax"
      BAnd W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x000FFFFF))
      BOrI W 1(Stk() Stk() LocReg(Lit(0)))
      LNot W 1(Stk() Stk())
      AddU W 1(Stk() Stk() Lit(1))
      Retn W 1(Stk())

   :"$emin"
      BAnd W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x000FFFFF))
      BOrI W 1(Stk() Stk() LocReg(Lit(0)))
      LNot W 1(Stk() Stk())
      AddU W 1(Stk() Stk() Lit(4))
      Retn W 1(Stk())
   )
)

;;
;; ___fpclassifyf
;;
Function "___fpclassifyf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 1
   param    = 1
   retrn    = 1
   block
   (
      BAnd     W 1(Stk() LocReg(Lit(0)) Lit(0x7F800000))
      Jcnd_Tab W 1(Stk()
         Lit(0x7F800000) Lit(:"$emax")
         Lit(0x00000000) Lit(:"$emin"))
      Move     W 1(Nul() Stk())
      Move     W 1(Stk() Lit(3))
      Retn     W 1(Stk())

   :"$emax"
      BAnd W 1(Stk() LocReg(Lit(0)) Lit(0x007FFFFF))
      BOrI W 1(Stk() Stk() LocReg(Lit(0)))
      LNot W 1(Stk() Stk())
      AddU W 1(Stk() Stk() Lit(1))
      Retn W 1(Stk())

   :"$emin"
      BAnd W 1(Stk() LocReg(Lit(0)) Lit(0x007FFFFF))
      BOrI W 1(Stk() Stk() LocReg(Lit(0)))
      LNot W 1(Stk() Stk())
      AddU W 1(Stk() Stk() Lit(4))
      Retn W 1(Stk())
   )
)

;;
;; ___fpclassifyl
;;
Function "___fpclassifyl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd     W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      Jcnd_Tab W 1(Stk()
         Lit(0x7FF00000) Lit(:"$emax")
         Lit(0x00000000) Lit(:"$emin"))
      Move     W 1(Nul() Stk())
      Move     W 1(Stk() Lit(3))
      Retn     W 1(Stk())

   :"$emax"
      BAnd W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x000FFFFF))
      BOrI W 1(Stk() Stk() LocReg(Lit(0)))
      LNot W 1(Stk() Stk())
      AddU W 1(Stk() Stk() Lit(1))
      Retn W 1(Stk())

   :"$emin"
      BAnd W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x000FFFFF))
      BOrI W 1(Stk() Stk() LocReg(Lit(0)))
      LNot W 1(Stk() Stk())
      AddU W 1(Stk() Stk() Lit(4))
      Retn W 1(Stk())
   )
)

;;
;; ___isfinite
;;
Function "___isfinite"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      CmpI_NE W 1(Stk() Stk() Lit(0x7FF00000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isfinitef
;;
Function "___isfinitef"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 1
   param    = 1
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit(0)) Lit(0x7F800000))
      CmpI_NE W 1(Stk() Stk() Lit(0x7F800000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isfinitel
;;
Function "___isfinitel"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      CmpI_NE W 1(Stk() Stk() Lit(0x7FF00000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isinf
;;
Function "___isinf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FFFFFFF))
      CmpI_EQ W 1(Stk() Stk() Lit(0x7FF00000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isinff
;;
Function "___isinff"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 1
   param    = 1
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit(0)) Lit(0x7FFFFFFF))
      CmpI_EQ W 1(Stk() Stk() Lit(0x7F800000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isinfl
;;
Function "___isinfl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FFFFFFF))
      CmpI_EQ W 1(Stk() Stk() Lit(0x7FF00000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isnan
;;
Function "___isnan"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FFFFFFF))
      CmpI_GT W 1(Stk() Stk() Lit(0x7FF00000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isnanf
;;
Function "___isnanf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 1
   param    = 1
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit(0)) Lit(0x7FFFFFFF))
      CmpI_GT W 1(Stk() Stk() Lit(0x7F800000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isnanl
;;
Function "___isnanl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FFFFFFF))
      CmpI_GT W 1(Stk() Stk() Lit(0x7FF00000))
      Retn    W 1(Stk())
   )
)

;;
;; ___isnormal
;;
Function "___isnormal"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(LocReg(Lit("___MathRegSize")) LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      CmpI_LE W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FE00000))
      CmpI_GE W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x00100000))
      BAnd    W 1(Stk() Stk() Stk())
      Retn    W 1(Stk())
   )
)

;;
;; ___isnormalf
;;
Function "___isnormalf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 1
   param    = 1
   retrn    = 1
   block
   (
      BAnd    W 1(LocReg(Lit(0)) LocReg(Lit(0)) Lit(0x7F800000))
      CmpI_LE W 1(Stk() LocReg(Lit(0)) Lit(0x7F000000))
      CmpI_GE W 1(Stk() LocReg(Lit(0)) Lit(0x00800000))
      BAnd    W 1(Stk() Stk() Stk())
      Retn    W 1(Stk())
   )
)

;;
;; ___isnormall
;;
Function "___isnormall"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd    W 1(LocReg(Lit("___MathRegSize")) LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      CmpI_LE W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FE00000))
      CmpI_GE W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x00100000))
      BAnd    W 1(Stk() Stk() Stk())
      Retn    W 1(Stk())
   )
)

;;
;; ___signbit
;;
Function "___signbit"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x80000000))
      Retn W 1(Stk())
   )
)

;;
;; ___signbitf
;;
Function "___signbitf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 1
   param    = 1
   retrn    = 1
   block
   (
      BAnd W 1(Stk() LocReg(Lit(0)) Lit(0x80000000))
      Retn W 1(Stk())
   )
)

;;
;; ___signbitl
;;
Function "___signbitl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 1
   block
   (
      BAnd W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x80000000))
      Retn W 1(Stk())
   )
)

;; EOF

