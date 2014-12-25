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
;; ___GDCC__MulF_W1
;;
Function "___GDCC__MulF_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 5 \
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
   Jcnd_Tru  1, Stk(), Lit(:"$retl")
   Move_W    1, Stk(), LocReg(Lit(1))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$retr") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7F800000), Lit(:"$retr") ; Jcnd_Lit
   CmpI_GT_W 1, Stk(), Stk(), Lit(0x7F800000)
   Jcnd_Tru  1, Stk(), Lit(:"$retr")
   Move_W    1, Stk(), LocReg(Lit(0))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$retl") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7F800000), Lit(:"$retl") ; Jcnd_Lit
   Move_W    1, Nul(), Stk()

   ; Determine result exponent. May be adjusted later, so no range check, yet.
   ShRI_W 1, Stk(), LocReg(Lit(0)), Lit(23)
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(23)
   AddU_W 1, Stk(), Stk(), Stk()
   SubU_W 1, LocReg(Lit(3)), Stk(), Lit(127)

   ; Clear operand exponents and add implicit bit.
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, LocReg(Lit(0)), Stk(), Lit(0x00800000)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, LocReg(Lit(1)), Stk(), Lit(0x00800000)

   ; Perform expanding multiply.
   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))

   ; Shift multiply result to only get the 25 significant bits.
   ShRI_W 2, Stk(), Stk(), Lit(23)
   Move_W 1, Nul(), Stk()
   Move_W 1, LocReg(Lit(4)), Stk()

   ; Check high bit for extra shift.
   AndU_W   1, Stk(), LocReg(Lit(4)), Lit(0x01000000)
   Jcnd_Nil 1, Stk(), Lit(:"$capman")

   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   AddU_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)

:"$capman"
   AndU_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(0x007FFFFF)

   ; Check for exponent overflow/underflow.
   CmpI_GE_W 1, Stk(), LocReg(Lit(3)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$retinf")
   CmpI_LE_W 1, Stk(), LocReg(Lit(3)), Lit(0x00)
   Jcnd_Tru  1, Stk(), Lit(:"$ret0")

   ; Return result.
   ShLU_W 1, Stk(), LocReg(Lit(3)), Lit(23)
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(2))
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(4))
   Retn   1, Stk()

:"$retl"
   OrIU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn   1, Stk()

:"$retr"
   OrIU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   Retn   1, Stk()

:"$retinf"
   OrIU_W 1, Stk(), LocReg(Lit(2)), Lit(0x7F800000)
   Retn   1, Stk()

:"$ret0"
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   1, Stk()
}

;;
;; ___GDCC__MulF_W2
;;
Function "___GDCC__MulF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 8 \
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
   Jcnd_Tru  1, Stk(), Lit(:"$retl")
   Move_W    1, Stk(), LocReg(Lit(3))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$retr") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7FF00000), Lit(:"$retr") ; Jcnd_Lit
   CmpI_GT_W 1, Stk(), Stk(), Lit(0x7FF00000)
   Jcnd_Tru  1, Stk(), Lit(:"$retr")
   Move_W    1, Stk(), LocReg(Lit(1))
   Casm      0, Lit(84), Lit(0x00000000), Lit(:"$retl") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(0x7FF00000), Lit(:"$retl") ; Jcnd_Lit
   Move_W    1, Nul(), Stk()

   ; Determine result exponent. May be adjusted later, so no range check, yet.
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(20)
   ShRI_W 1, Stk(), LocReg(Lit(3)), Lit(20)
   AddU_W 1, Stk(), Stk(), Stk()
   SubU_W 1, LocReg(Lit(5)), Stk(), Lit(1023)

   ; Clear operand exponents and add implicit bit.
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, LocReg(Lit(1)), Stk(), Lit(0x00100000)
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, LocReg(Lit(3)), Stk(), Lit(0x00100000)

   ; Perform expanding multiply.
   MuXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))

   ; Shift multiply result to only get the 54 significant bits.
   ShRI_W 3, Stk(), Stk(), Lit(20)
   Move_W 1, Nul(), Stk()
   Move_W 2, LocReg(Lit(6)), Stk()
   Move_W 1, Nul(), Stk()

   ; Check high bit for extra shift.
   AndU_W   1, Stk(), LocReg(Lit(7)), Lit(0x00200000)
   Jcnd_Nil 1, Stk(), Lit(:"$capman")

   ShRI_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   AddU_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)

:"$capman"
   AndU_W 1, LocReg(Lit(7)), LocReg(Lit(7)), Lit(0x000FFFFF)

   ; Check for exponent overflow/underflow.
   CmpI_GE_W 1, Stk(), LocReg(Lit(5)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$retinf")
   CmpI_LE_W 1, Stk(), LocReg(Lit(5)), Lit(0x000)
   Jcnd_Tru  1, Stk(), Lit(:"$ret0")

   ; Return result.
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(5)), Lit(20)
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(4))
   OrIU_W 1, Stk(), Stk(), LocReg(Lit(7))
   Retn   2, Stk()

:"$retl"
   Move_W 1, Stk(), LocReg(Lit(0))
   OrIU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Retn   2, Stk()

:"$retr"
   Move_W 1, Stk(), LocReg(Lit(2))
   OrIU_W 1, Stk(), LocReg(Lit(3)), LocReg(Lit(4))
   Retn   2, Stk()

:"$retinf"
   Move_W 1, Stk(), Lit(0)
   OrIU_W 1, Stk(), LocReg(Lit(4)), Lit(0x7FF00000)
   Retn   2, Stk()

:"$ret0"
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(4))
   Retn   2, Stk()
}

;; EOF

