;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014-2015 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Shift routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__ShLF_W1
;;
Function "___GDCC__ShLF_W1" \
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
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Jcnd_Tab 1, Stk(), Lit(0x00000000), Lit(:"$emax")
   Jcnd_Tab 1, Stk(), Lit(0x7F800000), Lit(:"$emax")
   ShRI_W   1, Stk(), Stk(), Lit(23)
   AddU_W   1, LocReg(Lit(1)), LocReg(Lit(1)), Stk()

   CmpI_GE_W 1, Stk(), LocReg(Lit(1)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$inf")

   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x807FFFFF)
   ShLU_W 1, Stk(), LocReg(Lit(1)), Lit(23)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()

:"$inf"
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(),          Lit(0x7F800000)
   Retn   1, Stk()

   ; If l is inf, nan, or zero, just return it.
:"$emax"
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()
}

;;
;; ___GDCC__ShLF_W2
;;
Function "___GDCC__ShLF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   Jcnd_Tab 1, Stk(), Lit(0x00000000), Lit(:"$emax")
   Jcnd_Tab 1, Stk(), Lit(0x7FF00000), Lit(:"$emax")
   ShRI_W   1, Stk(), Stk(), Lit(20)
   AddU_W   1, LocReg(Lit(2)), LocReg(Lit(2)), Stk()

   CmpI_GE_W 1, Stk(), LocReg(Lit(2)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$inf")

   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   ShLU_W 1, Stk(), LocReg(Lit(2)), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()

:"$inf"
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(),          Lit(0x7FF00000)
   Retn   2, Stk()

   ; If l is inf, nan, or zero, just return it.
:"$emax"
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   2, Stk()
}

;;
;; ___GDCC__ShRF_W1
;;
Function "___GDCC__ShRF_W1" \
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
   ShLU_W 1, LocReg(Lit(1)), LocReg(Lit(1)), Lit(23)

   AndU_W    1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Jcnd_Tab  1, Stk(), Lit(0x7F800000), Lit(:"$emax")
   CmpI_LE_W 1, Stk(), Stk(), LocReg(Lit(1))
   Jcnd_Tru  1, Stk(), Lit(:"$zero")

   SubU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   1, Stk()

:"$zero"
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Retn   1, Stk()

   ; If l is inf or nan, just return it.
:"$emax"
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()
}

;;
;; ___GDCC__ShRF_W2
;;
Function "___GDCC__ShRF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   ShLU_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(20)

   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   Jcnd_Tab  1, Stk(), Lit(0x7FF00000), Lit(:"$emax")
   CmpI_LE_W 1, Stk(), Stk(), LocReg(Lit(2))
   Jcnd_Tru  1, Stk(), Lit(:"$zero")

   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   Retn   2, Stk()

:"$zero"
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Retn   2, Stk()

   ; If l is inf or nan, just return it.
:"$emax"
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   2, Stk()
}

;; EOF

