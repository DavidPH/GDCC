;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2015 David Hill
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
Function "___approx_cbrt" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 2 \
   block
{
   Move_W 1, Stk(), LocReg(Lit(0))

   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, Stk(), Stk(), Lit(1023)
   DivI_W 1, Stk(), Stk(), Lit(3)
   AddI_W 1, Stk(), Stk(), Lit(1023)
   ShLU_W 1, Stk(), Stk(), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk(), Stk()
}

;;
;; ___approx_cbrtf
;;
Function "___approx_cbrtf" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 1 \
   param    = 1 \
   retrn    = 1 \
   block
{
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x807FFFFF)
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   ShRI_W 1, Stk(), Stk(), Lit(23)
   SubI_W 1, Stk(), Stk(), Lit(127)
   DivI_W 1, Stk(), Stk(), Lit(3)
   AddI_W 1, Stk(), Stk(), Lit(127)
   ShLU_W 1, Stk(), Stk(), Lit(23)
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   1, Stk()
}

;;
;; ___approx_cbrtl
;;
Function "___approx_cbrtl" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 2 \
   block
{
   Move_W 1, Stk(), LocReg(Lit(0))

   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, Stk(), Stk(), Lit(1023)
   DivI_W 1, Stk(), Stk(), Lit(3)
   AddI_W 1, Stk(), Stk(), Lit(1023)
   ShLU_W 1, Stk(), Stk(), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk(), Stk()
}

;;
;; ___approx_frexp
;;
Function "___approx_frexp" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 3 \
   block
{
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, LocReg(Lit(2)), Stk(), Lit(1022)

   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x3FE00000)
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   3, Stk()
}

;;
;; ___approx_frexpf
;;
Function "___approx_frexpf" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 1 \
   retrn    = 2 \
   block
{
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   ShRI_W 1, Stk(), Stk(), Lit(23)
   SubI_W 1, LocReg(Lit(1)), Stk(), Lit(126)

   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x807FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x3F000000)
   Move_W 1, Stk(), LocReg(Lit(1))
   Retn   2, Stk()
}

;;
;; ___approx_frexpl
;;
Function "___approx_frexpl" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 3 \
   block
{
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, LocReg(Lit(2)), Stk(), Lit(1022)

   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x3FE00000)
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   3, Stk()
}

;;
;; ___approx_log2
;;
Function "___approx_log2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(20)
   SubI_W 1, Stk(), Stk(), Lit(1023)
   Retn   1, Stk()
}

;;
;; ___approx_log2f
;;
Function "___approx_log2f" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 1 \
   param    = 1 \
   retrn    = 1 \
   block
{
   ShRI_W 1, Stk(), LocReg(Lit(0)), Lit(23)
   SubI_W 1, Stk(), Stk(), Lit(127)
   Retn   1, Stk()
}

;;
;; ___approx_log2l
;;
Function "___approx_log2l" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(20)
   SubI_W 1, Stk(), Stk(), Lit(1023)
   Retn   1, Stk()
}

;;
;; ___approx_modf
;;
Function "___approx_modf" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 2 \
   retrn    = 4 \
   block
{
   ; Extract exponent as signed int.
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, LocReg(Lit(2)), Stk(), Lit(1023)

   ; Check for having only integral or fractional component.
   CmpI_GT_W 1, Stk(), LocReg(Lit(2)), Lit(52)
   Jcnd_Tru  1,  Stk(), Lit(:"$onlyi")
   CmpI_LT_W 1, Stk(), LocReg(Lit(2)), Lit(0)
   Jcnd_Tru  1,  Stk(), Lit(:"$onlyf")

   ; Extract low bits.
   ShRI_W 2, Stk(), Lit(0x000FFFFFFFFFFFFF_64.0), LocReg(Lit(2))
   AndU_W 2, LocReg(Lit(3)), Stk(), LocReg(Lit(0))

   Move_W 1, LocReg(Lit(5)), Lit(0)
   ShLU_W 2, LocReg(Lit(3)), LocReg(Lit(3)), LocReg(Lit(2))

   OrIU_W   1, Stk(), LocReg(Lit(3)), LocReg(Lit(4))
   Jcnd_Tru 1, Stk(), Lit(:"$fracl")

   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jump   0, Lit(:"$integ")

:"$fracl"
   ShLU_W 2, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)
   SubI_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)

   AndU_W   1, Stk(), LocReg(Lit(4)), Lit(0x00100000)
   Jcnd_Nil 1, Stk(), Lit(:"$fracl")

   Move_W 2, Stk(), LocReg(Lit(3))
   AndU_W 1, Stk(), Stk(), Lit(0x000FFFFF)
   AddI_W 1, Stk(), LocReg(Lit(5)), Lit(1023)
   ShLU_W 1, Stk(), Stk(), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(), Stk()

   ; Mask out the low bits for integral result.
:"$integ"
   ShRI_W 2, Stk(), Lit(0x000FFFFFFFFFFFFF_64.0), LocReg(Lit(2))
   InvU_W 2, Stk(), Stk()
   AndU_W 2, Stk(), Stk(), LocReg(Lit(0))

   Retn 4, Stk()

:"$onlyi"
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   4, Stk()

:"$onlyf"
   Move_W 2, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Retn   4, Stk()
}

;;
;; ___approx_modff
;;
Function "___approx_modff" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 1 \
   retrn    = 2 \
   block
{
   ; Extract exponent as signed int.
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   ShRI_W 1, Stk(), Stk(), Lit(23)
   SubI_W 1, LocReg(Lit(1)), Stk(), Lit(127)

   ; Check for having only integral or fractional component.
   CmpI_GT_W 1, Stk(), LocReg(Lit(1)), Lit(23)
   Jcnd_Tru  1, Stk(), Lit(:"$onlyi")
   CmpI_LT_W 1, Stk(), LocReg(Lit(1)), Lit(0)
   Jcnd_Tru  1, Stk(), Lit(:"$onlyf")

   ; Extract low bits.
   ShRI_W 1, Stk(), Lit(0x007FFFFF_32.0), LocReg(Lit(1))
   AndU_W 1, LocReg(Lit(2)), Stk(), LocReg(Lit(0))

   Move_W 1, LocReg(Lit(3)), Lit(0)
   ShLU_W 1, LocReg(Lit(2)), LocReg(Lit(2)), LocReg(Lit(1))

   Jcnd_Tru 1, LocReg(Lit(2)), Lit(:"$fracl")

   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jump   0, Lit(:"$integ")

:"$fracl"
   ShLU_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)
   SubI_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)

   AndU_W   1, Stk(), LocReg(Lit(2)), Lit(0x00800000)
   Jcnd_Nil 1, Stk(), Lit(:"$fracl")

   Move_W 1, Stk(), LocReg(Lit(2))
   AndU_W 1, Stk(), Stk(), Lit(0x007FFFFF)
   AddI_W 1, Stk(), LocReg(Lit(3)), Lit(127)
   ShLU_W 1, Stk(), Stk(), Lit(23)
   OrIU_W 1, Stk(), Stk(), Stk()
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(), Stk()

   ; Mask out the low bits for integral result.
:"$integ"
   ShRI_W 1, Stk(), Lit(0x007FFFFF_32.0), LocReg(Lit(1))
   InvU_W 1, Stk(), Stk()
   AndU_W 1, Stk(), Stk(), LocReg(Lit(0))

   Retn 2, Stk()

:"$onlyi"
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   2, Stk(), Stk()

:"$onlyf"
   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Retn   2, Stk()
}

;;
;; ___approx_modfl
;;
Function "___approx_modfl" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 2 \
   retrn    = 4 \
   block
{
   ; Extract exponent as signed int.
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, LocReg(Lit(2)), Stk(), Lit(1023)

   ; Check for having only integral or fractional component.
   CmpI_GT_W 1, Stk(), LocReg(Lit(2)), Lit(52)
   Jcnd_Tru  1, Stk(), Lit(:"$onlyi")
   CmpI_LT_W 1, Stk(), LocReg(Lit(2)), Lit(0)
   Jcnd_Tru  1, Stk(), Lit(:"$onlyf")

   ; Extract low bits.
   ShRI_W 2, Stk(), Lit(0x000FFFFFFFFFFFFF_64.0), LocReg(Lit(2))
   AndU_W 2, LocReg(Lit(3)), Stk(), LocReg(Lit(0))

   Move_W 1, LocReg(Lit(5)), Lit(0)
   ShLU_W 2, LocReg(Lit(3)), LocReg(Lit(3)), LocReg(Lit(2))

   OrIU_W   1, Stk(), LocReg(Lit(3)), LocReg(Lit(4))
   Jcnd_Tru 1, Stk(), Lit(:"$fracl")

   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jump   0, Lit(:"$integ")

:"$fracl"
   ShLU_W 2, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)
   SubI_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)

   AndU_W   1, Stk(), LocReg(Lit(4)), Lit(0x00100000)
   Jcnd_Nil 1, Stk(), Lit(:"$fracl")

   Move_W 2, Stk(), LocReg(Lit(3))
   AndU_W 1, Stk(), Stk(), Lit(0x000FFFFF)
   AddI_W 1, Stk(), LocReg(Lit(5)), Lit(1023)
   ShLU_W 1, Stk(), Stk(), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(), Stk()

   ; Mask out the low bits for integral result.
:"$integ"
   ShRI_W 2, Stk(), Lit(0x000FFFFFFFFFFFFF_64.0), LocReg(Lit(2))
   InvU_W 2, Stk(), Stk()
   AndU_W 2, Stk(), Stk(), LocReg(Lit(0))

   Retn 4, Stk()

:"$onlyi"
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   4, Stk(), Stk(), Stk(), Stk()

:"$onlyf"
   Move_W 2, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Retn   4, Stk(), Stk(), Stk(), Stk()
}

;;
;; ___approx_sqrt
;;
Function "___approx_sqrt" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 2 \
   block
{
   Move_W 1, Stk(), LocReg(Lit(0))

   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, Stk(), Stk(), Lit(1023)
   ShRI_W 1, Stk(), Stk(), Lit(1)
   AddI_W 1, Stk(), Stk(), Lit(1023)
   ShLU_W 1, Stk(), Stk(), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()
}

;;
;; ___approx_sqrtf
;;
Function "___approx_sqrtf" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 1 \
   param    = 1 \
   retrn    = 1 \
   block
{
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x807FFFFF)
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   ShRI_W 1, Stk(), Stk(), Lit(23)
   SubI_W 1, Stk(), Stk(), Lit(127)
   ShRI_W 1, Stk(), Stk(), Lit(1)
   AddI_W 1, Stk(), Stk(), Lit(127)
   ShLU_W 1, Stk(), Stk(), Lit(23)
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   1, Stk()
}

;;
;; ___approx_sqrtl
;;
Function "___approx_sqrtl" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 2 \
   block
{
   Move_W 1, Stk(), LocReg(Lit(0))

   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   ShRI_W 1, Stk(), Stk(), Lit(20)
   SubI_W 1, Stk(), Stk(), Lit(1023)
   ShRI_W 1, Stk(), Stk(), Lit(1)
   AddI_W 1, Stk(), Stk(), Lit(1023)
   ShLU_W 1, Stk(), Stk(), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()
}

;; EOF

