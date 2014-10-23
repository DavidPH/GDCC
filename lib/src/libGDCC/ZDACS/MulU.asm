;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Multi-word unsigned and fixed-point multiply routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__MuXU_W
;;
Function "___GDCC__MuXU_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__MuXU_W$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 2 \
   retrn    = 2 \
   block
{
   AndU_W,  LocReg(Lit(2), 0), LocReg(Lit(0), 0), Lit(0xFFFF)
   ShRU_W,  LocReg(Lit(3), 0), LocReg(Lit(0), 0), Lit(16)
   AndU_W,  LocReg(Lit(4), 0), LocReg(Lit(1), 0), Lit(0xFFFF)
   ShRU_W,  LocReg(Lit(5), 0), LocReg(Lit(1), 0), Lit(16)

   MulU_W,  Stk(), LocReg(Lit(2), 0), LocReg(Lit(4), 0)
   Move_W,  Stk(), Lit(0)

   MulU_W,  LocReg(Lit(0), 0), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   ShLU_W,  Stk(), LocReg(Lit(0), 0), Lit(16)
   ShRU_W,  Stk(), LocReg(Lit(0), 0), Lit(16)

   MulU_W,  LocReg(Lit(0), 0), LocReg(Lit(3), 0), LocReg(Lit(4), 0)
   ShLU_W,  Stk(), LocReg(Lit(0), 0), Lit(16)
   ShRU_W,  Stk(), LocReg(Lit(0), 0), Lit(16)

   MulU_W,  Stk(), LocReg(Lit(3), 0), LocReg(Lit(5), 0)
   AddU_W,  Stk(), Stk(), Stk()

   AddU_W2, Stk(), Stk(), Stk()
   AddU_W2, Stk(), Stk(), Stk()

   Retn,    Stk(), Stk()
}

;;
;; ___GDCC__MulU_W2
;;
Function "___GDCC__MulU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__MulU_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   Call,   Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()

   MulU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   AddU_W, Stk(), Stk(), Stk()

   MulU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   AddU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk()
}

;;
;; ___GDCC__MulU_W3
;;
Function "___GDCC__MulU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__MulU_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   Move_W,  Stk(), LocReg(Lit(0), 0)
   Move_W,  Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()
   Move_W,  Stk(), Lit(0)

   Move_W,  Stk(), LocReg(Lit(0), 0)
   Move_W,  Stk(), LocReg(Lit(4), 0)
   Call,    Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()
   AddU_W2, Stk(), Stk(), Stk()

   Move_W,  Stk(), LocReg(Lit(1), 0)
   Move_W,  Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()
   AddU_W2, Stk(), Stk(), Stk()

   MulU_W,  Stk(), LocReg(Lit(0), 0), LocReg(Lit(5), 0)
   AddU_W,  Stk(), Stk(), Stk()

   MulU_W,  Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   AddU_W,  Stk(), Stk(), Stk()

   MulU_W,  Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)
   AddU_W,  Stk(), Stk(), Stk()

   Retn,    Stk(), Stk(), Stk()
}

;;
;; ___GDCC__MulX_W2
;;
Function "___GDCC__MulX_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__MulX_W2$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ShRI_W,  LocReg(Lit(4), 0), LocReg(Lit(1), 0), Lit(31)
   ShRI_W,  LocReg(Lit(5), 0), LocReg(Lit(3), 0), Lit(31)

   Move_W,  Stk(), LocReg(Lit(0), 0)
   Move_W,  Stk(), LocReg(Lit(2), 0)
   Call,    Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()
   Swap_W,  Stk(), Stk()
   Move_W,  Nul(), Stk()
   Move_W,  Stk(), Lit(0)

   Move_W,  Stk(), LocReg(Lit(0), 0)
   Move_W,  Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()
   AddU_W2, Stk(), Stk(), Stk()

   Move_W,  Stk(), LocReg(Lit(1), 0)
   Move_W,  Stk(), LocReg(Lit(2), 0)
   Call,    Lit("___GDCC__MuXU_W"), Lit(2), Stk(), Stk()
   AddU_W2, Stk(), Stk(), Stk()

   MulU_W,  Stk(), LocReg(Lit(0), 0), LocReg(Lit(5), 0)
   AddU_W,  Stk(), Stk(), Stk()

   MulU_W,  Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   AddU_W,  Stk(), Stk(), Stk()

   MulU_W,  Stk(), LocReg(Lit(4), 0), LocReg(Lit(2), 0)
   AddU_W,  Stk(), Stk(), Stk()

   Retn,    Stk(), Stk()
}

;; EOF

