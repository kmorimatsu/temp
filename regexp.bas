useclass CLDHEX
static private C_CODE
field RE,STR

method INIT
  var t
  if not(C_CODE) then gosub INIT_C
  if args(0)<1 then return
  if args(0)=2 then
    t=gosub(C_PRECOMP,args(1),args(2))
  else
    t=gosub(C_PRECOMP,args(1),"")
  endif
  RE=gosub(C_REGCOMP,t)
  delete t
return

method REGEXEC
  var r
  if args(0)=1 then
    STR$=args$(1)+""
    r=gosub(C_REGEXEC,RE,STR)
  else
    r=gosub(C_REGEXEC,RE,RE(10))
  endif
  if 0=r then return 0
  if RE(0)=RE(10) then return 0
  return r

method MATCH
  var t
  t=RE(args(1))
  return t$(0,RE(10+args(1))-t)

method REPLACE
  var t,i,r
  STR$=args$(1)
  r$=""
  t=STR
  i=gosub(C_REGEXEC,RE,t)
  do while i
    r$=r$+t$(0,RE(0)-t)+args$(2)
    t=RE(10)
    i=gosub(C_REGEXEC,RE,t)
  loop
  STR$=r$+t$
  return STR$

method REPLACE_CALLBACK
  var t,i,r
  STR$=args$(1)
  r$=""
  t=STR
  i=gosub(C_REGEXEC,RE,t)
  do while i
    r$=r$+t$(0,RE(0)-t)+gosub$(REPLACE_CALLBACK_SUB,RE,args(2))
    t=RE(10)
    i=gosub(C_REGEXEC,RE,t)
  loop
  STR$=r$+t$
  return STR$

label REPLACE_CALLBACK_SUB
  REM 6931    ldr r1, [r6, #16]
  REM 4708    bx r1
  exec $6931,$4708

label INIT_C
  var A,V
  REM Load the main code
  A=$20002000
  C_CODE=new(CLDHEX,"REGEXP02.HEX",A,50176)
  REM data_cpy_table
  REM Link functions
  V=(A+884-DATAADDRESS(C_MACHIKANIA_INIT)-12)>>1
  poke16 DATAADDRESS(C_MACHIKANIA_INIT)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_MACHIKANIA_INIT)+10,$f800+(V and $7ff)
  V=(A+7060-DATAADDRESS(C_PRECOMP)-12)>>1
  poke16 DATAADDRESS(C_PRECOMP)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_PRECOMP)+10,$f800+(V and $7ff)
  V=(A+5444-DATAADDRESS(C_REGCOMP)-12)>>1
  poke16 DATAADDRESS(C_REGCOMP)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_REGCOMP)+10,$f800+(V and $7ff)
  V=(A+5800-DATAADDRESS(C_REGEXEC)-12)>>1
  poke16 DATAADDRESS(C_REGEXEC)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_REGEXEC)+10,$f800+(V and $7ff)
  V=(A+1068-DATAADDRESS(C_REGSUB)-12)>>1
  poke16 DATAADDRESS(C_REGSUB)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_REGSUB)+10,$f800+(V and $7ff)
  REM Initialize C global variables
  gosub C_MACHIKANIA_INIT
return

label C_MACHIKANIA_INIT
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_PRECOMP
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_REGCOMP
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_REGEXEC
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_REGSUB
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
