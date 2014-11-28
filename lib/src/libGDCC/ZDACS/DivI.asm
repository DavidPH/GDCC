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
   label    = "___GDCC__DiXI_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 4 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Jcnd_Tru, Stk(), Lit("___GDCC__DiXI_W2$l1")

   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0x80000000)
   Jcnd_Tru, Stk(), Lit("___GDCC__DiXI_W2$l0r1")

   ; +dividend, +divisor -> +quotient, +remainder
   Move_W2,  Stk(), LocReg(Lit(0), 0)
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   Call,     Lit("___GDCC__DiXU_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   Retn,     Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXI_W2$l0r1"
   NegI_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0)

   ; +dividend, -divisor -> -quotient, -remainder
   Move_W2,  Stk(), LocReg(Lit(0), 0)
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   Call,     Lit("___GDCC__DiXU_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   NegI_W2,  LocReg(Lit(2), 0), Stk()
   NegI_W2,  Stk(), Stk()
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   Retn,     Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXI_W2$l1"
   NegI_W2,  LocReg(Lit(0), 0), LocReg(Lit(0), 0)

   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0x80000000)
   Jcnd_Tru, Stk(), Lit("___GDCC__DiXI_W2$l1r1")

   ; -dividend, +divisor -> -quotient, +remainder
   Move_W2,  Stk(), LocReg(Lit(0), 0)
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   Call,     Lit("___GDCC__DiXU_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   Move_W2,  LocReg(Lit(2), 0), Stk()
   NegI_W2,  Stk(), Stk()
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   Retn,     Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXI_W2$l1r1"
   NegI_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0)

   ; -dividend, -divisor -> +quotient, -remainder
   Move_W2,  Stk(), LocReg(Lit(0), 0)
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   Call,     Lit("___GDCC__DiXU_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   NegI_W2,  Stk(), Stk()
   Retn,     Stk(), Stk(), Stk(), Stk()
}

;;
;; ___GDCC__DiXI_W3
;;
Function "___GDCC__DiXI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DiXI_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 6 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(2), 0), Lit(0x80000000)
   Jcnd_Tru, Stk(), Lit("___GDCC__DiXI_W3$l1")

   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0x80000000)
   Jcnd_Tru, Stk(), Lit("___GDCC__DiXI_W3$l0r1")

   ; +dividend, +divisor -> +quotient, +remainder
   Move_W3,  Stk(), LocReg(Lit(0), 0)
   Move_W3,  Stk(), LocReg(Lit(3), 0)
   Call,     Lit("___GDCC__DiXU_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Retn,     Stk(), Stk(), Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXI_W3$l0r1"
   NegI_W3,  LocReg(Lit(3), 0), LocReg(Lit(3), 0)

   ; +dividend, -divisor -> -quotient, -remainder
   Move_W3,  Stk(), LocReg(Lit(0), 0)
   Move_W3,  Stk(), LocReg(Lit(3), 0)
   Call,     Lit("___GDCC__DiXU_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   NegI_W3,  LocReg(Lit(3), 0), Stk()
   NegI_W3,  Stk(), Stk()
   Move_W3,  Stk(), LocReg(Lit(3), 0)
   Retn,     Stk(), Stk(), Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXI_W3$l1"
   NegI_W3,  LocReg(Lit(0), 0), LocReg(Lit(0), 0)

   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0x80000000)
   Jcnd_Tru, Stk(), Lit("___GDCC__DiXI_W3$l1r1")

   ; -dividend, +divisor -> -quotient, +remainder
   Move_W3,  Stk(), LocReg(Lit(0), 0)
   Move_W3,  Stk(), LocReg(Lit(3), 0)
   Call,     Lit("___GDCC__DiXU_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Move_W3,  LocReg(Lit(3), 0), Stk()
   NegI_W3,  Stk(), Stk()
   Move_W3,  Stk(), LocReg(Lit(3), 0)
   Retn,     Stk(), Stk(), Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXI_W3$l1r1"
   NegI_W3,  LocReg(Lit(3), 0), LocReg(Lit(3), 0)

   ; -dividend, -divisor -> +quotient, -remainder
   Move_W3,  Stk(), LocReg(Lit(0), 0)
   Move_W3,  Stk(), LocReg(Lit(3), 0)
   Call,     Lit("___GDCC__DiXU_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   NegI_W3,  Stk(), Stk()
   Retn,     Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
}

;;
;; ___GDCC__DivI_W2
;;
Function "___GDCC__DivI_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivI_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)
   Call,    Lit("___GDCC__DiXI_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   Move_W2, Nul(), Stk()
   Retn,    Stk(), Stk()
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
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)
   Call,    Lit("___GDCC__DiXI_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   Swap_W2, Stk(), Stk()
   Move_W2, Nul(), Stk()
   Retn,    Stk(), Stk()
}

;;
;; ___GDCC__DivI_W3
;;
Function "___GDCC__DivI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivI_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   Move_W3, Stk(), LocReg(Lit(0), 0)
   Move_W3, Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__DiXI_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Move_W3, Nul(), Stk()
   Retn,    Stk(), Stk(), Stk()
}

;;
;; ___GDCC__ModI_W3
;;
Function "___GDCC__ModI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ModI_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   Move_W3, Stk(), LocReg(Lit(0), 0)
   Move_W3, Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__DiXI_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Swap_W3, Stk(), Stk()
   Move_W3, Nul(), Stk()
   Retn,    Stk(), Stk(), Stk()
}

;;
;; ___GDCC__DivX_W2
;;
Function "___GDCC__DivX_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivX_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W,  Stk(), Lit(0)
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)
   ShRI_W,  Stk(), LocReg(Lit(3), 0), Lit(31)
   Call,    Lit("___GDCC__DiXI_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Move_W3, Nul(), Stk()
   Move_W,  Nul(), Stk()
   Retn,    Stk(), Stk()
}

;; EOF

