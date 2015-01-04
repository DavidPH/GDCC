;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2015 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Bitwise count leading zeros/ones routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Objects                                                                    |
;;

;;
;; ___GDCC__Bclz_Table
;;
Object "___GDCC__Bclz_Table" \
   alias = 1 \
   alloc = 1 \
   defin = 1 \
   initi = Value Array(Fixed(32, 0, 0, 0), 256) \
   ( \
       8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, \
       3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
   ) \
   linka = ExtC \
   space = Sta "" \
   words = 256


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__Bclo_W1
;;
Function "___GDCC__Bclo_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 1 \
   param    = 1 \
   retrn    = 1 \
   block
{
   InvU_W 1, LocReg(Lit(0)), LocReg(Lit(0))

   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0xFFFF0000)
   Jcnd_Tru 1, Stk(), Lit(:"$FFFF0000")

;"$0000FFFF"
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x0000FF00)
   Jcnd_Tru 1, Stk(), Lit(:"$0000FF00")

;"$000000FF"
   Move_W   1, Stk(), LocReg(Lit(0))
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   AddU_W   1, Stk(), Stk(), Lit(24)
   Retn     1, Stk()

:"$0000FF00"
   ShRI_W   1, Stk(), LocReg(Lit(0)), Lit(8)
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   AddU_W   1, Stk(), Stk(), Lit(16)
   Retn     1, Stk()

:"$FFFF0000"
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0xFF000000)
   Jcnd_Tru 1, Stk(), Lit(:"$FF000000")

;"$00FF0000"
   ShRI_W   1, Stk(), LocReg(Lit(0)), Lit(16)
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   AddU_W   1, Stk(), Stk(), Lit(8)
   Retn     1, Stk()

:"$FF000000"
   ShRU_W   1, Stk(), LocReg(Lit(0)), Lit(24)
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   Retn     1, Stk()
}

;;
;; ___GDCC__Bclz_W1
;;
Function "___GDCC__Bclz_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 1 \
   param    = 1 \
   retrn    = 1 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0xFFFF0000)
   Jcnd_Tru 1, Stk(), Lit(:"$FFFF0000")

;"$0000FFFF"
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x0000FF00)
   Jcnd_Tru 1, Stk(), Lit(:"$0000FF00")

;"$000000FF"
   Move_W   1, Stk(), LocReg(Lit(0))
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   AddU_W   1, Stk(), Stk(), Lit(24)
   Retn     1, Stk()

:"$0000FF00"
   ShRI_W   1, Stk(), LocReg(Lit(0)), Lit(8)
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   AddU_W   1, Stk(), Stk(), Lit(16)
   Retn     1, Stk()

:"$FFFF0000"
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0xFF000000)
   Jcnd_Tru 1, Stk(), Lit(:"$FF000000")

;"$00FF0000"
   ShRI_W   1, Stk(), LocReg(Lit(0)), Lit(16)
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   AddU_W   1, Stk(), Stk(), Lit(8)
   Retn     1, Stk()

:"$FF000000"
   ShRU_W   1, Stk(), LocReg(Lit(0)), Lit(24)
   AddU_W   1, Stk(), Stk(), Lit("___GDCC__Bclz_Table")
   Move_W   1, Stk(), Sta(Stk())
   Retn     1, Stk()
}

;; EOF

