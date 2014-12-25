;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float division routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__DivF_W1
;;
Function "___GDCC__DivF_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Determine result sign.
   OrXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   AndU_W 1, LocReg(Lit(2)), Stk(), Lit(0x80000000)

   ; Clear operand signs.
   AndU_W 1, LocReg(Lit(0)), LocReg(Lit(0)), Lit(0x7FFFFFFF)
   AndU_W 1, LocReg(Lit(1)), LocReg(Lit(1)), Lit(0x7FFFFFFF)

   ; Check for special operands.
   CmpI_GT_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Jcnd_Tru  1, Stk(), Lit(:"$lnan")
   Move_W    1, Stk(), LocReg(Lit(1))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$r0") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7F800000), Lit(:"$rinf") ; Jcnd_Lit
   CmpI_GT_W 1, Stk(), Stk(), Lit(0x7F800000)
   Jcnd_Tru  1, Stk(), Lit(:"$rnan")
   Move_W    1, Stk(), LocReg(Lit(0))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$l0") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7F800000), Lit(:"$linf") ; Jcnd_Lit
   Move_W    1, Nul(), Stk()

   ; Determine result exponent. Will be adjusted later, so no range check, yet.
   Move_W 1, Stk(), Lit(0x7F)
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(23)
   SubU_W 1, Stk(), Stk(), Stk()
   ShRI_W 1, Stk(), LocReg(Lit(0)), Lit(23)
   AddU_W 1, Stk(), Stk(), Stk()
   Move_W 1, LocReg(Lit(3)), Stk()

   ; Clear operand exponents and add implicit bit.
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, LocReg(Lit(0)), Stk(), Lit(0x00800000)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, LocReg(Lit(1)), Stk(), Lit(0x00800000)

   ; Divison loop.
   Move_W 1, LocReg(Lit(4)), Lit(0)
   Move_W 1, LocReg(Lit(5)), Lit(23)

:"$loop"
   ; Division check.
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$loopnil")
   SubU_W    1, LocReg(Lit(0)), LocReg(Lit(0)), LocReg(Lit(1))
   AddU_W    1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)

:"$loopnil"
   ShLU_W 1, LocReg(Lit(0)), LocReg(Lit(0)), Lit(1)
   ShLU_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)

   SubU_W   1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)
   Move_W   1, Stk(), LocReg(Lit(5))
   Jcnd_Tru 1, Stk(), Lit(:"$loop")

   ; Final division check.
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   AddU_W    1, LocReg(Lit(4)), LocReg(Lit(4)), Stk()

   ; Adjust mantissa to have leading 1.
   Jump 0, Lit(:"$fixmancond")

:"$fixmanbody"
   ShLU_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   SubU_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)

:"$fixmancond"
   AndU_W   1, Stk(), LocReg(Lit(4)), Lit(0x00800000)
   Jcnd_Nil 1, Stk(), Lit(:"$fixmanbody")

:"$capman"
   AndU_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(0x007FFFFF)

   ; Check for out of range exponent.
   CmpI_GE_W 1, Stk(), LocReg(Lit(3)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$inf")
   CmpI_LE_W 1, Stk(), LocReg(Lit(3)), Lit(0x00)
   Jcnd_Tru  1, Stk(), Lit(:"$0")

   ; Return result.
   ShLU_W 1, Stk(), LocReg(Lit(3)), Lit(23)
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(2))
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(4))
   Retn   1, Stk()

:"$nan"
   ; Return NaN.
   OrIU_W 1, Stk(), Lit(0x7FFFFFFF), LocReg(Lit(2))
   Retn   1, Stk()

:"$lnan"
   ; l is NaN. Therefore, result is l.
:"$l0"
   ; l is 0, r is normal. Therefore, result is l.
:"$linf"
   ; l is INF, r is normal. Therefore, result is l.
   OrIU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn   1, Stk()

:"$rnan"
   ; r is NaN. Therefore, result is r.
   OrIU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   Retn   1, Stk()

:"$r0"
   ; r is 0.

   ; 0 / 0 = NaN
   Move_W   1, Stk(), LocReg(Lit(0))
   Jcnd_Nil 1, Stk(), Lit(:"$nan")

   ; Otherwise, result is INF.
:"$inf"
   OrIU_W 1, Stk(), Lit(0x7F800000), LocReg(Lit(2))
   Retn   1, Stk()

:"$rinf"
   ; r is INF.

   ; INF / INF = NaN
   CmpU_EQ_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Jcnd_Nil  1, Stk(), Lit(:"$nan")

   ; Otherwise, result is 0.
:"$0"
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   1, Stk()
}

;;
;; ___GDCC__DivF_W2
;;
Function "___GDCC__DivF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 9 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ; Determine result sign.
   OrXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AndU_W 1, LocReg(Lit(4)), Stk(), Lit(0x80000000)

   ; Clear operand signs.
   AndU_W 1, LocReg(Lit(1)), LocReg(Lit(1)), Lit(0x7FFFFFFF)
   AndU_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(0x7FFFFFFF)

   ; Check for special operands.
   CmpI_GT_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   Jcnd_Tru  1, Stk(), Lit(:"$lnan")
   Move_W    1, Stk(), LocReg(Lit(3))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$r0") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7FF00000), Lit(:"$rinf") ; Jcnd_Lit
   CmpI_GT_W 1, Stk(), Stk(), Lit(0x7FF00000)
   Jcnd_Tru  1, Stk(), Lit(:"$rnan")
   Move_W    1, Stk(), LocReg(Lit(1))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$l0") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7FF00000), Lit(:"$linf") ; Jcnd_Lit
   Move_W    1, Nul(), Stk()

   ; Determine result exponent. Will be adjusted later, so no range check, yet.
   Move_W 1, Stk(), Lit(0x3FF)
   ShRI_W 1, Stk(), LocReg(Lit(3)), Lit(20)
   SubU_W 1, Stk(), Stk(), Stk()
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(20)
   AddU_W 1, Stk(), Stk(), Stk()
   Move_W 1, LocReg(Lit(5)), Stk()

   ; Clear operand exponents and add implicit bit.
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, LocReg(Lit(1)), Stk(), Lit(0x00100000)
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, LocReg(Lit(3)), Stk(), Lit(0x00100000)

   ; Divison loop.
   Move_W 2, LocReg(Lit(6)), Lit(0_64.0)
   Move_W 1, LocReg(Lit(8)), Lit(52)

:"$loop"
   ; Division check.
   CmpI_GE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Jcnd_Nil  1, Stk(), Lit(:"$loopnil")
   SubU_W    2, LocReg(Lit(0)), LocReg(Lit(0)), LocReg(Lit(2))
   AddU_W    1, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)

:"$loopnil"
   ShLU_W 2, LocReg(Lit(0)), LocReg(Lit(0)), Lit(1)
   ShLU_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)

   SubU_W   1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)
   Move_W   1, Stk(), LocReg(Lit(8))
   Jcnd_Tru 1, Stk(), Lit(:"$loop")

   ; Final division check.
   CmpI_GE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   AddU_W    1, LocReg(Lit(6)), LocReg(Lit(6)), Stk()

   ; Adjust mantissa to have leading 1.
   Jump 0, Lit(:"$fixmancond")

:"$fixmanbody"
   ShLU_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   SubU_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)

:"$fixmancond"
   AndU_W   1, Stk(), LocReg(Lit(7)), Lit(0x00100000)
   Jcnd_Nil 1, Stk(), Lit(:"$fixmanbody")

:"$capman"
   AndU_W 1, LocReg(Lit(7)), LocReg(Lit(7)), Lit(0x000FFFFF)

   ; Check for out of range exponent.
   CmpI_GE_W 1, Stk(), LocReg(Lit(5)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$inf")
   CmpI_LE_W 1, Stk(), LocReg(Lit(5)), Lit(0x000)
   Jcnd_Tru  1, Stk(), Lit(:"$0")

   ; Return result.
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(5)), Lit(20)
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(4))
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(7))
   Retn   2, Stk()

:"$nan"
   ; Return NaN.
   Move_W 1, Stk(), Lit(0xFFFFFFFF)
   OrIU_W 1, Stk(), Lit(0x7FFFFFFF), LocReg(Lit(4))
   Retn   2, Stk()

:"$lnan"
   ; l is NaN. Therefore, result is l.
:"$l0"
   ; l is 0, r is normal. Therefore, result is l.
:"$linf"
   ; l is INF, r is normal. Therefore, result is l.
   Move_W 1, Stk(), LocReg(Lit(0))
   OrIU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Retn   2, Stk()

:"$rnan"
   ; r is NaN. Therefore, result is r.
   Move_W 1, Stk(), LocReg(Lit(2))
   OrIU_W 1, Stk(), LocReg(Lit(3)), LocReg(Lit(4))
   Retn   2, Stk()

:"$r0"
   ; r is 0.

   ; 0 / 0 = NaN
   Move_W   1, Stk(), LocReg(Lit(1))
   Jcnd_Nil 1, Stk(), Lit(:"$nan")

   ; Otherwise, result is INF.
:"$inf"
   Move_W 1, Stk(), Lit(0x00000000)
   OrIU_W 1, Stk(), Lit(0x7FF00000), LocReg(Lit(4))
   Retn   2, Stk()

:"$rinf"
   ; r is INF.

   ; INF / INF = NaN
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), Lit(0x7F800000)
   Jcnd_Nil  1, Stk(), Lit(:"$nan")

   ; Otherwise, result is 0.
:"$0"
   Move_W 1, Stk(), Lit(0x00000000)
   Move_W 1, Stk(), LocReg(Lit(4))
   Retn   2, Stk()
}

;; EOF

