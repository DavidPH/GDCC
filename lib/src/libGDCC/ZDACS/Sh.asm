;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
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
;; ___GDCC__ShLF_W
;;
Function "___GDCC__ShLF_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShLF_W$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x7F800000)
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__ShLF_W$emax") ; Jcnd_Lit
   Casm,   Lit(84), Lit(0x7F800000), Lit("___GDCC__ShLF_W$emax") ; Jcnd_Lit
   ShRI_W, Stk(), Stk(), Lit(23)
   AddU_W, LocReg(Lit(1), 0), LocReg(Lit(1), 0), Stk()

   CmpI_GE_W, Stk(), LocReg(Lit(1), 0), Lit(0xFF)
   Jcnd_Tru,  Stk(), Lit("___GDCC__ShLF_W$inf")

   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x807FFFFF)
   ShLU_W, Stk(), LocReg(Lit(1), 0), Lit(23)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk()

"___GDCC__ShLF_W$inf"
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   OrIU_W, Stk(), Stk(),             Lit(0x7F800000)
   Retn,   Stk()

   ; If l is inf, nan, or zero, just return it.
"___GDCC__ShLF_W$emax"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__ShLF_W2
;;
Function "___GDCC__ShLF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShLF_W2$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x7FF00000)
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__ShLF_W2$emax") ; Jcnd_Lit
   Casm,   Lit(84), Lit(0x7FF00000), Lit("___GDCC__ShLF_W2$emax") ; Jcnd_Lit
   ShRI_W, Stk(), Stk(), Lit(20)
   AddU_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Stk()

   CmpI_GE_W, Stk(), LocReg(Lit(2), 0), Lit(0x7FF)
   Jcnd_Tru,  Stk(), Lit("___GDCC__ShLF_W2$inf")

   Move_W, Stk(), LocReg(Lit(0), 0)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x800FFFFF)
   ShLU_W, Stk(), LocReg(Lit(2), 0), Lit(20)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk(), Stk()

"___GDCC__ShLF_W2$inf"
   Move_W, Stk(), Lit(0)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   OrIU_W, Stk(), Stk(),             Lit(0x7FF00000)
   Retn,   Stk(), Stk()

   ; If l is inf, nan, or zero, just return it.
"___GDCC__ShLF_W2$emax"
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Retn,    Stk(), Stk()
}

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
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShLU_W2$0")  ; Jcnd_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShLU_W2$32") ; Jcnd_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShLU_W2$gt32")

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
;; ___GDCC__ShLU_W3
;;
Function "___GDCC__ShLU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShLU_W3$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 3 \
   block
{
   Move_W,    Stk(), LocReg(Lit(3), 0)
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShLU_W3$0")  ; Jcnd_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShLU_W3$32") ; Jcnd_Lit
   Casm,      Lit(84), Lit(64), Lit("___GDCC__ShLU_W3$64") ; Jcnd_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShLU_W3$gt32")

   ; ret0 = l0 << r
   ShLU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)

   ; ret1 = (l1 << r) | (l0 >> (32 - r))
   ShLU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShRU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret2 = (l2 << r) | (l1 >> (32 - r))
   ShLU_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShRU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShLU_W3$gt32"
   CmpI_LT_W, Stk(), LocReg(Lit(3), 0), Lit(64)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShLU_W3$gt64")

   ; r -= 32
   SubU_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(32)

   ; ret0 = 0
   Move_W, Stk(), Lit(0)

   ; ret1 = l0 << r
   ShLU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)

   ; ret2 = (l1 << r) | (l0 >> (32 - r))
   ShLU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShRU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShLU_W3$gt64"
   ; ret0 = 0
   Move_W, Stk(), Lit(0)

   ; ret1 = 0
   Move_W, Stk(), Lit(0)

   ; ret2 = l0 << (r - 64)
   Move_W, Stk(), LocReg(Lit(0), 0)
   SubU_W, Stk(), LocReg(Lit(3), 0), Lit(64)
   ShLU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShLU_W3$0"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk(), Stk()

"___GDCC__ShLU_W3$32"
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShLU_W3$64"
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk(), Stk(), Stk()
}

;;
;; ___GDCC__ShRF_W
;;
Function "___GDCC__ShRF_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShRF_W$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ShLU_W, LocReg(Lit(1), 0), LocReg(Lit(1), 0), Lit(23)

   AndU_W,    Stk(), LocReg(Lit(0), 0), Lit(0x7F800000)
   Casm,      Lit(84), Lit(0x7F800000), Lit("___GDCC__ShRF_W$emax") ; Jcnd_Lit
   CmpI_LE_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Jcnd_Tru,  Stk(), Lit("___GDCC__ShRF_W$zero")

   SubU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()

"___GDCC__ShRF_W$zero"
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Retn,   Stk()

   ; If l is inf or nan, just return it.
"___GDCC__ShRF_W$emax"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__ShRF_W2
;;
Function "___GDCC__ShRF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShRF_W2$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   ShLU_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(20)

   AndU_W,    Stk(), LocReg(Lit(1), 0), Lit(0x7FF00000)
   Casm,      Lit(84), Lit(0x7FF00000), Lit("___GDCC__ShRF_W2$emax") ; Jcnd_Lit
   CmpI_LE_W, Stk(), Stk(), LocReg(Lit(2), 0)
   Jcnd_Tru,  Stk(), Lit("___GDCC__ShRF_W2$zero")

   Move_W, Stk(), LocReg(Lit(0), 0)
   SubU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   Retn,   Stk(), Stk()

"___GDCC__ShRF_W2$zero"
   Move_W, Stk(), Lit(0)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Retn,   Stk(), Stk()

   ; If l is inf or nan, just return it.
"___GDCC__ShRF_W2$emax"
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Retn,    Stk(), Stk()
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
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShRI_W2$0")  ; Jcnd_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShRI_W2$32") ; Jcnd_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShRI_W2$gt32")

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
;; ___GDCC__ShRI_W3
;;
Function "___GDCC__ShRI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShRI_W3$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 3 \
   block
{
   Move_W,    Stk(), LocReg(Lit(3), 0)
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShRI_W3$0")  ; Jcnd_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShRI_W3$32") ; Jcnd_Lit
   Casm,      Lit(84), Lit(64), Lit("___GDCC__ShRI_W3$64") ; Jcnd_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShRI_W3$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret1 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret2 = l2 >> r
   ShRI_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRI_W3$gt32"
   CmpI_LT_W, Stk(), LocReg(Lit(3), 0), Lit(64)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShRI_W3$gt64")

   ; r -= 32
   SubU_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(32)

   ; ret0 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret1 = l2 >> r
   ShRI_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)

   ; ret2 = l2 < 0 ? -1 : 0
   ShRI_W, Stk(), LocReg(Lit(2), 0), Lit(31)

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRI_W3$gt64"
   ; ret0 = l2 >> (r - 64)
   Move_W, Stk(), LocReg(Lit(2), 0)
   SubU_W, Stk(), LocReg(Lit(3), 0), Lit(64)
   ShRI_W, Stk(), Stk(), Stk()

   ; ret1 = l2 < 0 ? -1 : 0
   ShRI_W, Stk(), LocReg(Lit(2), 0), Lit(31)

   ; ret2 = l2 < 0 ? -1 : 0
   ShRI_W, Stk(), LocReg(Lit(2), 0), Lit(31)

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRI_W3$0"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRI_W3$32"
   Move_W, Stk(), LocReg(Lit(1), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   ShRI_W, Stk(), LocReg(Lit(2), 0), Lit(31)
   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRI_W3$64"
   Move_W, Stk(), LocReg(Lit(2), 0)
   ShRI_W, Stk(), LocReg(Lit(2), 0), Lit(31)
   ShRI_W, Stk(), LocReg(Lit(2), 0), Lit(31)
   Retn,   Stk(), Stk(), Stk()
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
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShRU_W2$0")  ; Jcnd_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShRU_W2$32") ; Jcnd_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShRU_W2$gt32")

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

;;
;; ___GDCC__ShRU_W3
;;
Function "___GDCC__ShRU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ShRU_W3$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 3 \
   block
{
   Move_W,    Stk(), LocReg(Lit(3), 0)
   Casm,      Lit(84), Lit(0),  Lit("___GDCC__ShRU_W3$0")  ; Jcnd_Lit
   Casm,      Lit(84), Lit(32), Lit("___GDCC__ShRU_W3$32") ; Jcnd_Lit
   Casm,      Lit(84), Lit(64), Lit("___GDCC__ShRU_W3$64") ; Jcnd_Lit
   CmpI_LT_W, Stk(), Stk(), Lit(32)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShRU_W3$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret1 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret2 = l2 >> r
   ShRU_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRU_W3$gt32"
   CmpI_LT_W, Stk(), LocReg(Lit(3), 0), Lit(64)
   Jcnd_Nil,  Stk(), Lit("___GDCC__ShRU_W3$gt64")

   ; r -= 32
   SubU_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(32)

   ; ret0 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   SubU_W, Stk(), Lit(32), LocReg(Lit(3), 0)
   ShLU_W, Stk(), Stk(), Stk()
   OrIU_W, Stk(), Stk(), Stk()

   ; ret1 = l2 >> r
   ShRU_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)

   ; ret2 = 0
   Move_W, Stk(), Lit(0)

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRU_W3$gt64"
   ; ret0 = l2 >> (r - 64)
   Move_W, Stk(), LocReg(Lit(2), 0)
   SubU_W, Stk(), LocReg(Lit(3), 0), Lit(64)
   ShRU_W, Stk(), Stk(), Stk()

   ; ret1 = 0
   Move_W, Stk(), Lit(0)

   ; ret2 = 0
   Move_W, Stk(), Lit(0)

   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRU_W3$0"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Move_W, Stk(), LocReg(Lit(1), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk(), Stk()

"___GDCC__ShRU_W3$32"
   Move_W, Stk(), LocReg(Lit(1), 0)
   Move_W, Stk(), LocReg(Lit(2), 0)
   Move_W, Stk(), Lit(0)
   Retn,   Stk(), Stk(), Stk()

"___GDCC__ShRU_W3$64"
   Move_W, Stk(), LocReg(Lit(2), 0)
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), Lit(0)
   Retn,   Stk(), Stk(), Stk()
}

;; EOF

