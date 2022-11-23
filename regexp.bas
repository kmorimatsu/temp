useclass CLDHEX
static private C_CODE
field RE,STR

method INIT
  var t
  if not(C_CODE) then gosub INIT_C
  if args(0)<1 then return
  t$=args$(1)+""
  if args(0)=2 then
    if peek(args(2))=0x69 then t$=gosub$(C_CASE_INSENSITIVE,t$)
  endif
  t$=gosub$(C_SUPPORT_CURLY,t$)
  RE=gosub(C_REGCOMP,t$)
  delete t
return

method REGEXEC
  var t
  if args(0)=1 then
    STR$=args$(1)+""
    return gosub(C_REGEXEC,RE,STR)
  else
    return gosub(C_REGEXEC,RE,RE(10))
  endif

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
  V=(A+844-DATAADDRESS(C_MACHIKANIA_INIT)-12)>>1
  poke16 DATAADDRESS(C_MACHIKANIA_INIT)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_MACHIKANIA_INIT)+10,$f800+(V and $7ff)
  V=(A+5404-DATAADDRESS(C_REGCOMP)-12)>>1
  poke16 DATAADDRESS(C_REGCOMP)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_REGCOMP)+10,$f800+(V and $7ff)
  V=(A+5760-DATAADDRESS(C_REGEXEC)-12)>>1
  poke16 DATAADDRESS(C_REGEXEC)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_REGEXEC)+10,$f800+(V and $7ff)
  V=(A+1028-DATAADDRESS(C_REGSUB)-12)>>1
  poke16 DATAADDRESS(C_REGSUB)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_REGSUB)+10,$f800+(V and $7ff)
  V=(A+6064-DATAADDRESS(C_CASE_INSENSITIVE)-12)>>1
  poke16 DATAADDRESS(C_CASE_INSENSITIVE)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_CASE_INSENSITIVE)+10,$f800+(V and $7ff)
  V=(A+6800-DATAADDRESS(C_SUPPORT_CURLY)-12)>>1
  poke16 DATAADDRESS(C_SUPPORT_CURLY)+8,$f000+(V>>11)
  poke16 DATAADDRESS(C_SUPPORT_CURLY)+10,$f800+(V and $7ff)
  REM Initialize C global variables
  gosub C_MACHIKANIA_INIT
return

label C_MACHIKANIA_INIT
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_REGCOMP
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_REGEXEC
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_REGSUB
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_CASE_INSENSITIVE
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
label C_SUPPORT_CURLY
  exec $68f0,$6931,$6972,$69b3,$f000,$f800,$bd00
