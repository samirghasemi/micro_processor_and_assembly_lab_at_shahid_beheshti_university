    .MODEL SMALL
    .STACK 64
    .DATA
    ; ORG 200H
    DATA1 DB 4
    DATA2 DB 5
    RES DW 0
    
    .CODE
main:    
    PROC FAR
    MOV AX,@DATA
    MOV DS,AX
    

    MOV AL,DATA1
    MUL AL
    MOV RES,AX
    MOV AL,DATA2
    MUL AL
    ADD AX,RES
    MOV RES,AX
    MOV AH,4CH
    INT 21H
l2:
    jmp l2
                       
                       
                       