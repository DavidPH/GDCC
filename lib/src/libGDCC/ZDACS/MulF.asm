;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float multiply routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__MulF_W
;;
Function "___GDCC__MulF_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__MulF_W$label" \
   linka    = ExtC \
   localReg = 5 \
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
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W$retl")
   Move_W,    Stk(), LocReg(Lit(1), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__MulF_W$retr") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7F800000), Lit("___GDCC__MulF_W$retr") ; Jcnd_Lit
   CmpI_GT_W, Stk(), Stk(), Lit(0x7F800000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W$retr")
   Move_W,    Stk(), LocReg(Lit(0), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__MulF_W$retl") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7F800000), Lit("___GDCC__MulF_W$retl") ; Jcnd_Lit
   Move_W,    Nul(), Stk()

   ; Determine result exponent. May be adjusted later, so no range check, yet.
   ShRI_W, Stk(), LocReg(Lit(0), 0), Lit(23)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(23)
   AddU_W, Stk(), Stk(), Stk()
   SubU_W, LocReg(Lit(3), 0), Stk(), Lit(127)

   ; Clear operand exponents and add implicit bit.
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, LocReg(Lit(0), 0), Stk(), Lit(0x00800000)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, LocReg(Lit(1), 0), Stk(), Lit(0x00800000)

   ; Perform expanding multiply.
   MuXU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)

   ; Shift multiply result to only get the 25 significant bits.
   ShRI_W2, Stk(), Stk(), Lit(23)
   Move_W,  Nul(), Stk()
   Move_W,  LocReg(Lit(4), 0), Stk()

   ; Check high bit for extra shift.
   AndU_W,   Stk(), LocReg(Lit(4), 0), Lit(0x01000000)
   Jcnd_Nil, Stk(), Lit("___GDCC__MulF_W$capman")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddU_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(1)

"___GDCC__MulF_W$capman"
   AndU_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(0x007FFFFF)

   ; Check for exponent overflow/underflow.
   CmpI_GE_W, Stk(), LocReg(Lit(3), 0), Lit(0xFF)
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W$retinf")
   CmpI_LE_W, Stk(), LocReg(Lit(3), 0), Lit(0x00)
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W$ret0")

   ; Return result.
   ShLU_W, Stk(), LocReg(Lit(3), 0), Lit(23)
   OrIU_W, Stk(), Stk(), LocReg(Lit(2), 0)
   OrIU_W, Stk(), Stk(), LocReg(Lit(4), 0)
   Retn,   Stk()

"___GDCC__MulF_W$retl"
   OrIU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__MulF_W$retr"
   OrIU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__MulF_W$retinf"
   OrIU_W, Stk(), LocReg(Lit(2), 0), Lit(0x7F800000)
   Retn,   Stk()

"___GDCC__MulF_W$ret0"
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__MulF_W2
;;
Function "___GDCC__MulF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__MulF_W2$label" \
   linka    = ExtC \
   localReg = 8 \
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
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W2$retl")
   Move_W,    Stk(), LocReg(Lit(3), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__MulF_W2$retr") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7FF00000), Lit("___GDCC__MulF_W2$retr") ; Jcnd_Lit
   CmpI_GT_W, Stk(), Stk(), Lit(0x7FF00000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W2$retr")
   Move_W,    Stk(), LocReg(Lit(1), 0)
   Casm,      Lit(84), Lit(0x00000000), Lit("___GDCC__MulF_W2$retl") ; Jcnd_Lit
   Casm,      Lit(84), Lit(0x7FF00000), Lit("___GDCC__MulF_W2$retl") ; Jcnd_Lit
   Move_W,    Nul(), Stk()

   ; Determine result exponent. May be adjusted later, so no range check, yet.
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(20)
   ShRI_W, Stk(), LocReg(Lit(3), 0), Lit(20)
   AddU_W, Stk(), Stk(), Stk()
   SubU_W, LocReg(Lit(5), 0), Stk(), Lit(1023)

   ; Clear operand exponents and add implicit bit.
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W, LocReg(Lit(1), 0), Stk(), Lit(0x00100000)
   AndU_W, Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W, LocReg(Lit(3), 0), Stk(), Lit(0x00100000)

   ; Perform expanding multiply.
   MuXU_W2, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)

   ; Shift multiply result to only get the 54 significant bits.
   ShRI_W3, Stk(), Stk(), Lit(20)
   Move_W,  Nul(), Stk()
   Move_W2, LocReg(Lit(6), 0), Stk()
   Move_W,  Nul(), Stk()

   ; Check high bit for extra shift.
   AndU_W,   Stk(), LocReg(Lit(7), 0), Lit(0x00200000)
   Jcnd_Nil, Stk(), Lit("___GDCC__MulF_W2$capman")

   ShRI_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   AddU_W,  LocReg(Lit(5), 0), LocReg(Lit(5), 0), Lit(1)

"___GDCC__MulF_W2$capman"
   AndU_W, LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(0x000FFFFF)

   ; Check for exponent overflow/underflow.
   CmpI_GE_W, Stk(), LocReg(Lit(5), 0), Lit(0x7FF)
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W2$retinf")
   CmpI_LE_W, Stk(), LocReg(Lit(5), 0), Lit(0x000)
   Jcnd_Tru,  Stk(), Lit("___GDCC__MulF_W2$ret0")

   ; Return result.
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(5), 0), Lit(20)
   OrIU_W, Stk(), Stk(), LocReg(Lit(4), 0)
   OrIU_W, Stk(), Stk(), LocReg(Lit(7), 0)
   Retn,   Stk(), Stk()

"___GDCC__MulF_W2$retl"
   Move_W, Stk(), LocReg(Lit(0), 0)
   OrIU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()

"___GDCC__MulF_W2$retr"
   Move_W, Stk(), LocReg(Lit(2), 0)
   OrIU_W, Stk(), LocReg(Lit(3), 0), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()

"___GDCC__MulF_W2$retinf"
   Move_W, Stk(), Lit(0)
   OrIU_W, Stk(), LocReg(Lit(4), 0), Lit(0x7F800000)
   Retn,   Stk(), Stk()

"___GDCC__MulF_W2$ret0"
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), LocReg(Lit(4), 0)
   Retn,   Stk(), Stk()
}

;; EOF

