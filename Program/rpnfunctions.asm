section .data
leftborder dq 0.0000000000
rightborder dq 4.0000000000
const0 dq 2.0
const1 dq 4.0
const2 dq 0.2
section .text
global fh1
global fh2
global fh3
global leftborder
global rightborder
fh1:
  push ebp
  mov ebp, esp
  finit
  ; 2
  fld qword [const0]
  ; x
  fld qword[ebp + 8]
  ; 4
  fld qword [const1]
  ; /
  fdivp
  ; tan
  fptan
  fstp st0
  ; -
  fsubp
  leave
  ret
fh2:
  push ebp
  mov ebp, esp
  finit
  ; x
  fld qword[ebp + 8]
  leave
  ret
fh3:
  push ebp
  mov ebp, esp
  finit
  ; 0.2
  fld qword [const2]
  ; pi
  fldpi
  ; *
  fmulp
  leave
  ret
