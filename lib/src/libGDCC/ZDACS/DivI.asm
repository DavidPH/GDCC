;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Signed division routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__DiXI_W2
;;
Function "___GDCC__DiXI_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 4 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l0r1")

   ; +dividend, +divisor -> +quotient, +remainder
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn   4, Stk()

:"$l0r1"
   NegI_W 2, LocReg(Lit(2)), LocReg(Lit(2))

   ; +dividend, -divisor -> -quotient, -remainder
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   NegI_W 2, LocReg(Lit(2)), Stk()
   NegI_W 2, Stk(), Stk()
   Move_W 2, Stk(), LocReg(Lit(2))
   Retn   4, Stk()

:"$l1"
   NegI_W 2, LocReg(Lit(0)), LocReg(Lit(0))

   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1r1")

   ; -dividend, +divisor -> -quotient, +remainder
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 2, LocReg(Lit(2)), Stk()
   NegI_W 2, Stk(), Stk()
   Move_W 2, Stk(), LocReg(Lit(2))
   Retn   4, Stk()

:"$l1r1"
   NegI_W 2, LocReg(Lit(2)), LocReg(Lit(2))

   ; -dividend, -divisor -> +quotient, -remainder
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   NegI_W 2, Stk(), Stk()
   Retn   4, Stk()
}

;;
;; ___GDCC__DiXI_W3
;;
Function "___GDCC__DiXI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 6 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(2)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l0r1")

   ; +dividend, +divisor -> +quotient, +remainder
   DiXU_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Retn   6, Stk()

:"$l0r1"
   NegI_W 3, LocReg(Lit(3)), LocReg(Lit(3))

   ; +dividend, -divisor -> -quotient, -remainder
   DiXU_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   NegI_W 3, LocReg(Lit(3)), Stk()
   NegI_W 3, Stk(), Stk()
   Move_W 3, Stk(), LocReg(Lit(3))
   Retn   6, Stk()

:"$l1"
   NegI_W 3, LocReg(Lit(0)), LocReg(Lit(0))

   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1r1")

   ; -dividend, +divisor -> -quotient, +remainder
   DiXU_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 3, LocReg(Lit(3)), Stk()
   NegI_W 3, Stk(), Stk()
   Move_W 3, Stk(), LocReg(Lit(3))
   Retn   6, Stk()

:"$l1r1"
   NegI_W 3, LocReg(Lit(3)), LocReg(Lit(3))

   ; -dividend, -divisor -> +quotient, -remainder
   DiXU_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   NegI_W 3, Stk(), Stk()
   Retn   6, Stk()
}

;;
;; ___GDCC__DivI_W2
;;
Function "___GDCC__DivI_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   DiXI_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 2, Nul(), Stk()
   Retn   2, Stk()
}

;;
;; ___GDCC__ModI_W2
;;
Function "___GDCC__ModI_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ModI_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   DiXI_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Swap_W 2, Stk(), Stk()
   Move_W 2, Nul(), Stk()
   Retn   2, Stk()
}

;;
;; ___GDCC__DivI_W3
;;
Function "___GDCC__DivI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   DiXI_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 3, Nul(), Stk()
   Retn   3, Stk()
}

;;
;; ___GDCC__ModI_W3
;;
Function "___GDCC__ModI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   DiXI_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Swap_W 3, Stk(), Stk()
   Move_W 3, Nul(), Stk()
   Retn   3, Stk()
}

;;
;; ___GDCC__DivX_W2
;;
Function "___GDCC__DivX_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W 1, Stk(), Lit(0)
   Move_W 2, Stk(), LocReg(Lit(0))
   Move_W 2, Stk(), LocReg(Lit(2))
   ShRI_W 1, Stk(), LocReg(Lit(3)), Lit(31)
   DiXI_W 3, Stk(), Stk(), Stk()
   Move_W 3, Nul(), Stk()
   Move_W 1, Nul(), Stk()
   Retn   2, Stk()
}

;; EOF

