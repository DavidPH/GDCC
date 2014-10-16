;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; 2-word shift routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__ShLU_W2
;;
Function "___GDCC__ShLU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShLU_W2$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   Move_W,    Stk(), LocReg(Lit(2), 0)
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShLU_W2$0")  ; Cjmp_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShLU_W2$32") ; Cjmp_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ShLU_W2$gt32")

   ; ret0 = l0 << r
   ShLU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)

   ; ret1 = (l1 << r) | (l0 >> (32 - r))
   ShLU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(2), 0)
   ShRU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk()

"___GDCC__ShLU_W2$gt32"
   ; ret0 = 0
   Move_W, Stk(), Lit(0)

   ; ret1 = l0 << (r - 32)
   Move_W, Stk(), LocReg(Lit(0), 0)
   SubU_W, Stk(), LocReg(Lit(2), 0), Lit(32)
   ShLU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk()

"___GDCC__ShLU_W2$0"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk()

"___GDCC__ShLU_W2$32"
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk(), Stk()
}

;;
;; ___GDCC__ShRI_W2
;;
Function "___GDCC__ShRI_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShRI_W2$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   Move_W,    Stk(), LocReg(Lit(2), 0)
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShRI_W2$0")  ; Cjmp_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShRI_W2$32") ; Cjmp_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ShRI_W2$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(2), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret1 = l1 >> r
   ShRI_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)

   Retn,   Stk(), Stk()

"___GDCC__ShRI_W2$gt32"
   ; ret0 = l1 >> (r - 32)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), LocReg(Lit(2), 0), Lit(32)
   ShRI_W, Stk(), Stk(), Stk()

   ; ret1 = l1 < 0 ? -1 : 0
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(31)

   Retn,   Stk(), Stk()

"___GDCC__ShRI_W2$0"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk()

"___GDCC__ShRI_W2$32"
   Move_W, Stk(), LocReg(Lit(1), 0)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(31)
   Retn,   Stk(), Stk()
}

;;
;; ___GDCC__ShRU_W2
;;
Function "___GDCC__ShRU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShRU_W2$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   Move_W,    Stk(), LocReg(Lit(2), 0)
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShRU_W2$0")  ; Cjmp_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShRU_W2$32") ; Cjmp_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ShRU_W2$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(2), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret1 = l1 >> r
   ShRU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)

   Retn,   Stk(), Stk()

"___GDCC__ShRU_W2$gt32"
   ; ret0 = l1 >> (r - 32)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), LocReg(Lit(2), 0), Lit(32)
   ShRU_W, Stk(), Stk(), Stk()

   ; ret1 = 0
   Move_W, Stk(), Lit(0)

   Retn,   Stk(), Stk()

"___GDCC__ShRU_W2$0"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk()

"___GDCC__ShRU_W2$32"
   Move_W, Stk(), LocReg(Lit(1), 0)
   Move_W, Stk(), Lit(0)
   Retn,   Stk(), Stk()
}

;; EOF

