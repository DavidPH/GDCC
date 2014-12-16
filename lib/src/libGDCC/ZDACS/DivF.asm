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
;; ___GDCC__DivF_W
;;
Function "___GDCC__DivF_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivF_W$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Determine result sign.
   OrXU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   AndU_W, LocReg(Lit(2), 0), Stk(), Lit(0x80000000)

   ; Clear operand signs.
   AndU_W, LocReg(Lit(0), 0), LocReg(Lit(0), 0), Lit(0x7FFFFFFF)
   AndU_W, LocReg(Lit(1), 0), LocReg(Lit(1), 0), Lit(0x7FFFFFFF)

   ; Check for special operands.
   CmpI_GT_W, Stk(), LocReg(Lit(0), 0), Lit(0x7F800000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W$lnan")
   Move_W,    Stk(), LocReg(Lit(1), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__DivF_W$r0") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7F800000), Lit("___GDCC__DivF_W$rinf") ; Jcnd_Lit
   CmpI_GT_W, Stk(), Stk(), Lit(0x7F800000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W$rnan")
   Move_W,    Stk(), LocReg(Lit(0), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__DivF_W$l0") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7F800000), Lit("___GDCC__DivF_W$linf") ; Jcnd_Lit
   Move_W,    Nul(), Stk()

   ; Determine result exponent. Will be adjusted later, so no range check, yet.
   Move_W, Stk(), Lit(0x7F)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(23)
   SubU_W, Stk(), Stk(), Stk()
   ShRI_W, Stk(), LocReg(Lit(0), 0), Lit(23)
   AddU_W, Stk(), Stk(), Stk()
   Move_W, LocReg(Lit(3), 0), Stk()

   ; Clear operand exponents and add implicit bit.
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, LocReg(Lit(0), 0), Stk(), Lit(0x00800000)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, LocReg(Lit(1), 0), Stk(), Lit(0x00800000)

   ; Divison loop.
   Move_W, LocReg(Lit(4), 0), Lit(0)
   Move_W, LocReg(Lit(5), 0), Lit(23)

"___GDCC__DivF_W$loop"
   ; Division check.
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Jcnd_Nil,  Stk(), Lit("___GDCC__DivF_W$loopnil")
   SubU_W,    LocReg(Lit(0), 0), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   AddU_W,    LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)

"___GDCC__DivF_W$loopnil"
   ShLU_W, LocReg(Lit(0), 0), LocReg(Lit(0), 0), Lit(1)
   ShLU_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)

   SubU_W, LocReg(Lit(5), 0), LocReg(Lit(5), 0), Lit(1)
   Move_W, Stk(), LocReg(Lit(5), 0)
   Jcnd_Tru, Stk(), Lit("___GDCC__DivF_W$loop")

   ; Final division check.
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   AddU_W,    LocReg(Lit(4), 0), LocReg(Lit(4), 0), Stk()

   ; Adjust mantissa to have leading 1.
   Jump, Lit("___GDCC__DivF_W$fixmancond")

"___GDCC__DivF_W$fixmanbody"
   ShLU_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   SubU_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(1)

"___GDCC__DivF_W$fixmancond"
   AndU_W,   Stk(), LocReg(Lit(4), 0), Lit(0x00800000)
   Jcnd_Nil, Stk(), Lit("___GDCC__DivF_W$fixmanbody")

"___GDCC__DivF_W$capman"
   AndU_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(0x007FFFFF)

   ; Check for out of range exponent.
   CmpI_GE_W, Stk(), LocReg(Lit(3), 0), Lit(0xFF)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W$inf")
   CmpI_LE_W, Stk(), LocReg(Lit(3), 0), Lit(0x00)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W$0")

   ; Return result.
   ShLU_W, Stk(), LocReg(Lit(3), 0), Lit(23)
   OrIU_W, Stk(), Stk(), LocReg(Lit(2), 0)
   OrIU_W, Stk(), Stk(), LocReg(Lit(4), 0)
   Retn,   Stk()

"___GDCC__DivF_W$nan"
   ; Return NaN.
   OrIU_W, Stk(), Lit(0x7FFFFFFF), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__DivF_W$lnan"
   ; l is NaN. Therefore, result is l.
"___GDCC__DivF_W$l0"
   ; l is 0, r is normal. Therefore, result is l.
"___GDCC__DivF_W$linf"
   ; l is INF, r is normal. Therefore, result is l.
   OrIU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__DivF_W$rnan"
   ; r is NaN. Therefore, result is r.
   OrIU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__DivF_W$r0"
   ; r is 0.

   ; 0 / 0 = NaN
   Move_W,   Stk(), LocReg(Lit(0), 0)
   Jcnd_Nil, Stk(), Lit("___GDCC__DivF_W$nan")

   ; Otherwise, result is INF.
"___GDCC__DivF_W$inf"
   OrIU_W, Stk(), Lit(0x7F800000), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__DivF_W$rinf"
   ; r is INF.

   ; INF / INF = NaN
   CmpU_EQ_W, Stk(), LocReg(Lit(0), 0), Lit(0x7F800000)
   Jcnd_Nil,  Stk(), Lit("___GDCC__DivF_W$nan")

   ; Otherwise, result is 0.
"___GDCC__DivF_W$0"
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__DivF_W2
;;
Function "___GDCC__DivF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivF_W2$label" \
   linka    = ExtC \
   localReg = 9 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ; Determine result sign.
   OrXU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   AndU_W, LocReg(Lit(4), 0), Stk(), Lit(0x80000000)

   ; Clear operand signs.
   AndU_W, LocReg(Lit(1), 0), LocReg(Lit(1), 0), Lit(0x7FFFFFFF)
   AndU_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(0x7FFFFFFF)

   ; Check for special operands.
   CmpI_GT_W, Stk(), LocReg(Lit(1), 0), Lit(0x7FF00000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W2$lnan")
   Move_W,    Stk(), LocReg(Lit(3), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__DivF_W2$r0") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7FF00000), Lit("___GDCC__DivF_W2$rinf") ; Jcnd_Lit
   CmpI_GT_W, Stk(), Stk(), Lit(0x7FF00000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W2$rnan")
   Move_W,    Stk(), LocReg(Lit(1), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__DivF_W2$l0") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7FF00000), Lit("___GDCC__DivF_W2$linf") ; Jcnd_Lit
   Move_W,    Nul(), Stk()

   ; Determine result exponent. Will be adjusted later, so no range check, yet.
   Move_W, Stk(), Lit(0x3FF)
   ShRI_W, Stk(), LocReg(Lit(3), 0), Lit(20)
   SubU_W, Stk(), Stk(), Stk()
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(20)
   AddU_W, Stk(), Stk(), Stk()
   Move_W, LocReg(Lit(5), 0), Stk()

   ; Clear operand exponents and add implicit bit.
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W, LocReg(Lit(1), 0), Stk(), Lit(0x00100000)
   AndU_W, Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W, LocReg(Lit(3), 0), Stk(), Lit(0x00100000)

   ; Divison loop.
   Move_W2, LocReg(Lit(6), 0), Lit(0_64.0)
   Move_W,  LocReg(Lit(8), 0), Lit(52)

"___GDCC__DivF_W2$loop"
   ; Division check.
   CmpI_GE_W2, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Jcnd_Nil,   Stk(), Lit("___GDCC__DivF_W2$loopnil")
   SubU_W2,    LocReg(Lit(0), 0), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   AddU_W,     LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)

"___GDCC__DivF_W2$loopnil"
   ShLU_W2, LocReg(Lit(0), 0), LocReg(Lit(0), 0), Lit(1)
   ShLU_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)

   SubU_W, LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)
   Move_W, Stk(), LocReg(Lit(8), 0)
   Jcnd_Tru, Stk(), Lit("___GDCC__DivF_W2$loop")

   ; Final division check.
   CmpI_GE_W2, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   AddU_W,     LocReg(Lit(6), 0), LocReg(Lit(6), 0), Stk()

   ; Adjust mantissa to have leading 1.
   Jump, Lit("___GDCC__DivF_W2$fixmancond")

"___GDCC__DivF_W2$fixmanbody"
   ShLU_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   SubU_W,  LocReg(Lit(5), 0), LocReg(Lit(5), 0), Lit(1)

"___GDCC__DivF_W2$fixmancond"
   AndU_W,   Stk(), LocReg(Lit(7), 0), Lit(0x00100000)
   Jcnd_Nil, Stk(), Lit("___GDCC__DivF_W2$fixmanbody")

"___GDCC__DivF_W2$capman"
   AndU_W, LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(0x000FFFFF)

   ; Check for out of range exponent.
   CmpI_GE_W, Stk(), LocReg(Lit(5), 0), Lit(0x7FF)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W2$inf")
   CmpI_LE_W, Stk(), LocReg(Lit(5), 0), Lit(0x000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__DivF_W2$0")

   ; Return result.
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(5), 0), Lit(20)
   OrIU_W, Stk(), Stk(), LocReg(Lit(4), 0)
   OrIU_W, Stk(), Stk(), LocReg(Lit(7), 0)
   Retn,   Stk(), Stk()

"___GDCC__DivF_W2$nan"
   ; Return NaN.
   Move_W, Stk(), Lit(0xFFFFFFFF)
   OrIU_W, Stk(), Lit(0x7FFFFFFF), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()

"___GDCC__DivF_W2$lnan"
   ; l is NaN. Therefore, result is l.
"___GDCC__DivF_W2$l0"
   ; l is 0, r is normal. Therefore, result is l.
"___GDCC__DivF_W2$linf"
   ; l is INF, r is normal. Therefore, result is l.
   Move_W, Stk(), LocReg(Lit(0), 0)
   OrIU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()

"___GDCC__DivF_W2$rnan"
   ; r is NaN. Therefore, result is r.
   Move_W, Stk(), LocReg(Lit(2), 0)
   OrIU_W, Stk(), LocReg(Lit(3), 0), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()

"___GDCC__DivF_W2$r0"
   ; r is 0.

   ; 0 / 0 = NaN
   Move_W,   Stk(), LocReg(Lit(1), 0)
   Jcnd_Nil, Stk(), Lit("___GDCC__DivF_W2$nan")

   ; Otherwise, result is INF.
"___GDCC__DivF_W2$inf"
   Move_W, Stk(), Lit(0x00000000)
   OrIU_W, Stk(), Lit(0x7FF00000), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()

"___GDCC__DivF_W2$rinf"
   ; r is INF.

   ; INF / INF = NaN
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), Lit(0x7F800000)
   Jcnd_Nil,  Stk(), Lit("___GDCC__DivF_W2$nan")

   ; Otherwise, result is 0.
"___GDCC__DivF_W2$0"
   Move_W, Stk(), Lit(0x00000000)
   Move_W, Stk(), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()
}

;; EOF

