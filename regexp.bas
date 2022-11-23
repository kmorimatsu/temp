useclass CLDHEX
static private C_CODE
field RE,STR

method INIT
  var t,u
  if not(C_CODE) then gosub INIT_C
  t=args(1)
  if args(0)=2 then
    if peek(args(2))=0x69 then t=gosub(C_CASE_INSENSITIVE,t)
  endif
  u=gosub(C_SUPPORT_CURLY,t)
  RE=gosub(C_REGCOMP,u)
  delete t
  delete u
return

method REGEXEC
  var t
  if args(0)=1 then
    STR$=args$(1)
    return gosub(C_REGEXEC,RE,STR)
  else
    return gosub(C_REGEXEC,RE,RE(10))
  endif

method MATCH
  var t,l
  t=RE(args(1))
  l=RE(10+args(1))-t
  return t$(0,l)

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
