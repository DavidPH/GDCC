;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Two-word comparison routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__CmpI_GE_W2
;;
Function "___GDCC__CmpI_GE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_GE_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_GE_W2$eq1")

   CmpI_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpI_GE_W2$eq1"
   CmpU_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpI_GT_W2
;;
Function "___GDCC__CmpI_GT_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_GT_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_GT_W2$eq1")

   CmpI_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpI_GT_W2$eq1"
   CmpU_GT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpI_LE_W2
;;
Function "___GDCC__CmpI_LE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_LE_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_LE_W2$eq1")

   CmpI_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpI_LE_W2$eq1"
   CmpU_LE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpI_LT_W2
;;
Function "___GDCC__CmpI_LT_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_LT_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_LT_W2$eq1")

   CmpI_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpI_LT_W2$eq1"
   CmpU_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_EQ_W2
;;
Function "___GDCC__CmpU_EQ_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_EQ_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   AndU_W,    Stk(), Stk(), Stk()
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_GE_W2
;;
Function "___GDCC__CmpU_GE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_GE_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_GE_W2$eq1")

   CmpU_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpU_GE_W2$eq1"
   CmpU_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_GT_W2
;;
Function "___GDCC__CmpU_GT_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_GT_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_GT_W2$eq1")

   CmpU_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpU_GT_W2$eq1"
   CmpU_GT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_LE_W2
;;
Function "___GDCC__CmpU_LE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_LE_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_LE_W2$eq1")

   CmpU_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpU_LE_W2$eq1"
   CmpU_LE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_LT_W2
;;
Function "___GDCC__CmpU_LT_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_LT_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_LT_W2$eq1")

   CmpU_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,      Stk()

"___GDCC__CmpU_LT_W2$eq1"
   CmpU_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_NE_W2
;;
Function "___GDCC__CmpU_NE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_NE_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   CmpU_NE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   CmpU_NE_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   OrIU_W,    Stk(), Stk(), Stk()
   Retn,      Stk()
}

;; EOF

