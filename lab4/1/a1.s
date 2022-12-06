	 
	EXPORT SystemInit
    EXPORT __main


	AREA MYPROG,CODE,READONLY 

SystemInit FUNCTION
	; initialization code
 ENDFUNC

;int lucas(n){
;    if(n=0)
;       return 2
;    if(n=1)
;       return 1
;    return lucas(n-1)+lucas(n-2)
;}
;int main{
;    lucas(n)
;}


; main logic of code
__main FUNCTION
     
start
	MOV R0,#3
	BL lucas
	B end_func
lucas
	PUSH {LR}
	CMP R0, #0
	BNE if_2 
	MOV R0,#2
	POP {LR}
	MOV PC, LR
if_2
	CMP R0, #1
	BNE exit_if
	MOV R0,#1
	POP {LR}
	MOV PC, LR
exit_if
	MOV R1, R0
	ADD R0, #-1
	BL 	lucas
	MOV R2, R0
	MOV R0, R1
	ADD R0, #-2
	BL 	lucas
	MOV R3, R0
	ADD R0, R2, R3
	POP {LR}
	MOV PC, LR
	


end_func
	b end_func
 ENDFUNC	
 END




