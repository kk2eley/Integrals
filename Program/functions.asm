section .data
    const1 dq 3.0
    const2 dq 0.5
    const3 dq -1.5
    const4 dq 2.5
    const5 dq 9.5
    const6 dq 5.0
    const7 dq -5.0

section .text
    global f1
    global df1
    global f2
    global df2
    global f3
    global df3

f1:
    ;3 * (0.5 / (x + 1) + 1)
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword [const1]
    fld1
    fld qword[const2]
    fld qword[ebp+8]
    fld1
    faddp
    fdivp
    faddp
    fmulp
    
    leave
    ret
    
df1:
    ;-1.5 / ((x + 1) * (x + 1))
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[const3]
    fld qword[ebp + 8]
    fld1
    faddp
    fld qword[ebp + 8]
    fld1
    faddp
    fmulp
    fdivp
    
    leave
    ret

f2:
    ;2.5 * x - 9.5
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[const4]
    fld qword[ebp + 8]
    fmulp
    fld qword[const5]
    fsubp
    
    leave
    ret
    
df2:
    ;2.5
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[const4]
    
    leave
    ret
    
f3:
    ;5 / x
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[const6]
    fld qword[ebp + 8]
    fdivp
    
    leave
    ret

df3:
    ;-5 / (x * x)
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[const7]
    fld qword[ebp + 8]
    fld st0
    fmulp
    fdivp
    
    leave
    ret
