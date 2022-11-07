    .MODEL SMALL
    .STACK 64
    .DATA
    
    .CODE
MAIN:    
    PROC FAR
    MOV AX,@DATA
    MOV DS,AX
    
    MOV DX,9
    MOV [1000H],DX
    
    MOV DX,7
    MOV [1002H],DX
    
    MOV DX,8
    MOV [1004H],DX
    
    MOV DX,2
    MOV [1006H],DX
    
    ;counter for outer loop
    MOV BX,1000H   
 label1:     
    
    ;make counter zero for inner loop    
    XOR CX,CX
 LABEL2:
    ;increase counter
    ADD CX,0002H
    
    ; get num1 in AX and num2 in DX
    MOV AX, [BX]
    ADD BX,CX
    MOV DX, [BX]
    SUB BX,CX
    
    ;compare to DO swap or not          
    CMP AX, DX 
    JLE AFTERSWAP
    
    ;DO swap
    MOV [BX], DX
    ADD BX,CX
    MOV [BX], AX
    SUB BX,CX
AFTERSWAP:    
    
    ;check if BX+CX is achieve to end or not
    XOR AX,AX          
    ADD AX,BX
    ADD AX,CX
    CMP AX,1006H
    JL LABEL2       
       
    ;increase BX and check if BX is achieve to end or not
    ADD BX,0002H          
    CMP BX,1006H
    JL LABEL1      
    
    
HERE:
    JMP HERE