	 
	EXPORT SystemInit
    EXPORT __main


	AREA MYPROG,CODE,READONLY 

SystemInit FUNCTION
	; initialization code
 ENDFUNC


__main FUNCTION

start
	MOV R0,#3
	MOV R1,#1
	MOV R3,#3
	
	
sum
	ADDS R2,R0,R1
	ADD R3,#1
	BCS result
	MOV R1, R0
	MOV R0, R2
	B sum

result 
	PUSH {R0}  
	SUB R3,#1
	PUSH {R3}
	
end_func
	b end_func

ENDFUNC	
END


