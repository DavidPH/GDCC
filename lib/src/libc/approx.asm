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
;; Algorithm approximations.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___approx_cbrt
;;
Function "___approx_cbrt"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 2
   block
   (
      Move_W W 1(Stk() LocReg(Lit(0)))

      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x800FFFFF))
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(Stk() Stk() Lit(1023))
      DivI_W W 1(Stk() Stk() Lit(3))
      AddI_W W 1(Stk() Stk() Lit(1023))
      ShLU_W W 1(Stk() Stk() Lit(20))
      BOrI_W W 1(Stk() Stk() Stk())

      Retn   W 2(Stk() Stk())
   )
)

;;
;; ___approx_cbrtf
;;
Function "___approx_cbrtf"
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
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x807FFFFF))
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x7F800000))
      ShRI_W W 1(Stk() Stk() Lit(23))
      SubI_W W 1(Stk() Stk() Lit(127))
      DivI_W W 1(Stk() Stk() Lit(3))
      AddI_W W 1(Stk() Stk() Lit(127))
      ShLU_W W 1(Stk() Stk() Lit(23))
      BOrI_W W 1(Stk() Stk() Stk())

      Retn   W 1(Stk())
   )
)

;;
;; ___approx_cbrtl
;;
Function "___approx_cbrtl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 2
   block
   (
      Move_W W 1(Stk() LocReg(Lit(0)))

      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x800FFFFF))
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(Stk() Stk() Lit(1023))
      DivI_W W 1(Stk() Stk() Lit(3))
      AddI_W W 1(Stk() Stk() Lit(1023))
      ShLU_W W 1(Stk() Stk() Lit(20))
      BOrI_W W 1(Stk() Stk() Stk())

      Retn   W 2(Stk() Stk())
   )
)

;;
;; ___approx_frexp
;;
Function "___approx_frexp"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 3
   param    = 2
   retrn    = 3
   block
   (
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) Stk() Lit(1022))

      Move_W W 1(Stk() LocReg(Lit(0)))
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x800FFFFF))
      BOrI_W W 1(Stk() Stk()                         Lit(0x3FE00000))
      Move_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))))
      Retn   W 3(Stk())
   )
)

;;
;; ___approx_frexpf
;;
Function "___approx_frexpf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 1
   retrn    = 2
   block
   (
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x7F800000))
      ShRI_W W 1(Stk() Stk() Lit(23))
      SubI_W W 1(LocReg(Lit("___MathRegSize")) Stk() Lit(126))

      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x807FFFFF))
      BOrI_W W 1(Stk() Stk()          Lit(0x3F000000))
      Move_W W 1(Stk() LocReg(Lit("___MathRegSize")))
      Retn   W 2(Stk())
   )
)

;;
;; ___approx_frexpl
;;
Function "___approx_frexpl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 3
   param    = 2
   retrn    = 3
   block
   (
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) Stk() Lit(1022))

      Move_W W 1(Stk() LocReg(Lit(0)))
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x800FFFFF))
      BOrI_W W 1(Stk() Stk()                         Lit(0x3FE00000))
      Move_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))))
      Retn   W 3(Stk())
   )
)

;;
;; ___approx_log2
;;
Function "___approx_log2"
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
      ShRI_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(20))
      SubI_W W 1(Stk() Stk() Lit(1023))
      Retn   W 1(Stk())
   )
)

;;
;; ___approx_log2f
;;
Function "___approx_log2f"
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
      ShRI_W W 1(Stk() LocReg(Lit(0)) Lit(23))
      SubI_W W 1(Stk() Stk() Lit(127))
      Retn   W 1(Stk())
   )
)

;;
;; ___approx_log2l
;;
Function "___approx_log2l"
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
      ShRI_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(20))
      SubI_W W 1(Stk() Stk() Lit(1023))
      Retn   W 1(Stk())
   )
)

;;
;; ___approx_modf
;;
Function "___approx_modf"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 6
   param    = 2
   retrn    = 4
   block
   (
      ; Extract exponent as signed int.
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) Stk() Lit(1023))

      ; Check for having only integral or fractional component.
      CmpI_GT_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))) Lit(52))
      Jcnd_Tru  W 1(Stk() Lit(:"$onlyi"))
      CmpI_LT_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))) Lit(0))
      Jcnd_Tru  W 1(Stk() Lit(:"$onlyf"))

      ; Extract low bits.
      ShRI_W W 2(Stk() Lit(0x000FFFFFFFFFFFFF_64.0) LocReg 1(Lit(Mul("___MathRegSize" 2))))
      BAnd_W W 2(LocReg(Lit(Mul("___MathRegSize" 3))) Stk() LocReg(Lit(0)))

      Move_W W 1(LocReg(Lit(Mul("___MathRegSize" 5))) Lit(0))
      ShLU_W W 2(LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 3))) LocReg 1(Lit(Mul("___MathRegSize" 2))))

      BOrI_W   W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 4))))
      Jcnd_Tru W 1(Stk() Lit(:"$fracl"))

      Move_W W 1(Stk() Lit(0))
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      Jump   W 1(Lit(:"$integ"))

   :"$fracl"
      ShLU_W W 2(LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 3))) Lit 1(1))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 5))) LocReg(Lit(Mul("___MathRegSize" 5))) Lit(1))

      BAnd_W   W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 4))) Lit(0x00100000))
      Jcnd_Nil W 1(Stk() Lit(:"$fracl"))

      Move_W W 2(Stk() LocReg(Lit(Mul("___MathRegSize" 3))))
      BAnd_W W 1(Stk() Stk() Lit(0x000FFFFF))
      AddI_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 5))) Lit(1023))
      ShLU_W W 1(Stk() Stk() Lit(20))
      BOrI_W W 1(Stk() Stk() Stk())
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      BOrI_W W 1(Stk() Stk() Stk())

      ; Mask out the low bits for integral result.
   :"$integ"
      ShRI_W W 2(Stk() Lit(0x000FFFFFFFFFFFFF_64.0) LocReg 1(Lit(Mul("___MathRegSize" 2))))
      BNot_W W 2(Stk() Stk())
      BAnd_W W 2(Stk() Stk() LocReg(Lit(0)))

      Retn W 4(Stk())

   :"$onlyi"
      Move_W W 1(Stk() Lit(0))
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      Move_W W 2(Stk() LocReg(Lit(0)))
      Retn   W 4(Stk())

   :"$onlyf"
      Move_W W 2(Stk() LocReg(Lit(0)))
      Move_W W 1(Stk() Lit(0))
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      Retn   W 4(Stk())
   )
)

;;
;; ___approx_modff
;;
Function "___approx_modff"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 4
   param    = 1
   retrn    = 2
   block
   (
      ; Extract exponent as signed int.
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x7F800000))
      ShRI_W W 1(Stk() Stk() Lit(23))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 1))) Stk() Lit(127))

      ; Check for having only integral or fractional component.
      CmpI_GT_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(23))
      Jcnd_Tru  W 1(Stk() Lit(:"$onlyi"))
      CmpI_LT_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0))
      Jcnd_Tru  W 1(Stk() Lit(:"$onlyf"))

      ; Extract low bits.
      ShRI_W W 1(Stk() Lit(0x007FFFFF_32.0) LocReg(Lit(Mul("___MathRegSize" 1))))
      BAnd_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) Stk() LocReg(Lit(0)))

      Move_W W 1(LocReg(Lit(Mul("___MathRegSize" 3))) Lit(0))
      ShLU_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) LocReg(Lit(Mul("___MathRegSize" 2))) LocReg(Lit(Mul("___MathRegSize" 1))))

      Jcnd_Tru W 1(LocReg(Lit(Mul("___MathRegSize" 2))) Lit(:"$fracl"))

      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x80000000))
      Jump   W 1(Lit(:"$integ"))

   :"$fracl"
      ShLU_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) LocReg(Lit(Mul("___MathRegSize" 2))) Lit(1))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 3))) Lit(1))

      BAnd_W   W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))) Lit(0x00800000))
      Jcnd_Nil W 1(Stk() Lit(:"$fracl"))

      Move_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))))
      BAnd_W W 1(Stk() Stk() Lit(0x007FFFFF))
      AddI_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 3))) Lit(127))
      ShLU_W W 1(Stk() Stk() Lit(23))
      BOrI_W W 1(Stk() Stk() Stk())
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x80000000))
      BOrI_W W 1(Stk() Stk() Stk())

      ; Mask out the low bits for integral result.
   :"$integ"
      ShRI_W W 1(Stk() Lit(0x007FFFFF_32.0) LocReg(Lit(Mul("___MathRegSize" 1))))
      BNot_W W 1(Stk() Stk())
      BAnd_W W 1(Stk() Stk() LocReg(Lit(0)))

      Retn W 2(Stk())

   :"$onlyi"
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x80000000))
      Move_W W 1(Stk() LocReg(Lit(0)))
      Retn   W 2(Stk() Stk())

   :"$onlyf"
      Move_W W 1(Stk() LocReg(Lit(0)))
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x80000000))
      Retn   W 2(Stk())
   )
)

;;
;; ___approx_modfl
;;
Function "___approx_modfl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 6
   param    = 2
   retrn    = 4
   block
   (
      ; Extract exponent as signed int.
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 2))) Stk() Lit(1023))

      ; Check for having only integral or fractional component.
      CmpI_GT_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))) Lit(52))
      Jcnd_Tru  W 1(Stk() Lit(:"$onlyi"))
      CmpI_LT_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 2))) Lit(0))
      Jcnd_Tru  W 1(Stk() Lit(:"$onlyf"))

      ; Extract low bits.
      ShRI_W W 2(Stk() Lit(0x000FFFFFFFFFFFFF_64.0) LocReg 1(Lit(Mul("___MathRegSize" 2))))
      BAnd_W W 2(LocReg(Lit(Mul("___MathRegSize" 3))) Stk() LocReg(Lit(0)))

      Move_W W 1(LocReg(Lit(Mul("___MathRegSize" 5))) Lit(0))
      ShLU_W W 2(LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 3))) LocReg 1(Lit(Mul("___MathRegSize" 2))))

      BOrI_W   W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 4))))
      Jcnd_Tru W 1(Stk() Lit(:"$fracl"))

      Move_W W 1(Stk() Lit(0))
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      Jump   W 1(Lit(:"$integ"))

   :"$fracl"
      ShLU_W W 2(LocReg(Lit(Mul("___MathRegSize" 3))) LocReg(Lit(Mul("___MathRegSize" 3))) Lit 1(1))
      SubI_W W 1(LocReg(Lit(Mul("___MathRegSize" 5))) LocReg(Lit(Mul("___MathRegSize" 5))) Lit(1))

      BAnd_W   W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 4))) Lit(0x00100000))
      Jcnd_Nil W 1(Stk() Lit(:"$fracl"))

      Move_W W 2(Stk() LocReg(Lit(Mul("___MathRegSize" 3))))
      BAnd_W W 1(Stk() Stk() Lit(0x000FFFFF))
      AddI_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 5))) Lit(1023))
      ShLU_W W 1(Stk() Stk() Lit(20))
      BOrI_W W 1(Stk() Stk() Stk())
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      BOrI_W W 1(Stk() Stk() Stk())

      ; Mask out the low bits for integral result.
   :"$integ"
      ShRI_W W 2(Stk() Lit(0x000FFFFFFFFFFFFF_64.0) LocReg 1(Lit(Mul("___MathRegSize" 2))))
      BNot_W W 2(Stk() Stk())
      BAnd_W W 2(Stk() Stk() LocReg(Lit(0)))

      Retn W 4(Stk())

   :"$onlyi"
      Move_W W 1(Stk() Lit(0))
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      Move_W W 2(Stk() LocReg(Lit(0)))
      Retn   W 4(Stk())

   :"$onlyf"
      Move_W W 2(Stk() LocReg(Lit(0)))
      Move_W W 1(Stk() Lit(0))
      BAnd_W W 1(Stk() LocReg(Lit(Mul("___MathRegSize" 1))) Lit(0x80000000))
      Retn   W 4(Stk())
   )
)


;;
;; ___approx_sqrt
;;
Function "___approx_sqrt"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 2
   block
   (
      Move_W W 1(Stk() LocReg(Lit(0)))

      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x800FFFFF))
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(Stk() Stk() Lit(1023))
      ShRI_W W 1(Stk() Stk() Lit(1))
      AddI_W W 1(Stk() Stk() Lit(1023))
      ShLU_W W 1(Stk() Stk() Lit(20))
      BOrI_W W 1(Stk() Stk() Stk())

      Retn   W 2(Stk())
   )
)

;;
;; ___approx_sqrtf
;;
Function "___approx_sqrtf"
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
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x807FFFFF))
      BAnd_W W 1(Stk() LocReg(Lit(0)) Lit(0x7F800000))
      ShRI_W W 1(Stk() Stk() Lit(23))
      SubI_W W 1(Stk() Stk() Lit(127))
      ShRI_W W 1(Stk() Stk() Lit(1))
      AddI_W W 1(Stk() Stk() Lit(127))
      ShLU_W W 1(Stk() Stk() Lit(23))
      BOrI_W W 1(Stk() Stk() Stk())

      Retn   W 1(Stk())
   )
)

;;
;; ___approx_sqrtl
;;
Function "___approx_sqrtl"
(
   alloc    = 1
   ctype    = StkCall
   defin    = 1
   label    = :"$label"
   linka    = ExtC
   localReg = 2
   param    = 2
   retrn    = 2
   block
   (
      Move_W W 1(Stk() LocReg(Lit(0)))

      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x800FFFFF))
      BAnd_W W 1(Stk() LocReg(Lit("___MathRegSize")) Lit(0x7FF00000))
      ShRI_W W 1(Stk() Stk() Lit(20))
      SubI_W W 1(Stk() Stk() Lit(1023))
      ShRI_W W 1(Stk() Stk() Lit(1))
      AddI_W W 1(Stk() Stk() Lit(1023))
      ShLU_W W 1(Stk() Stk() Lit(20))
      BOrI_W W 1(Stk() Stk() Stk())

      Retn   W 2(Stk())
   )
)

;; EOF

