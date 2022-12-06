	 
	EXPORT SystemInit
    EXPORT __main


	AREA MYPROG,CODE,READONLY 

SystemInit FUNCTION
	; initialization code
 ENDFUNC


__main FUNCTION
	
	
	
start
		MOV R0,#1
		MOV R1,#9
factorial
		CMP R1, #1
		BLE end_func
		MUL R0, R1, R0
		SUB R1, R1, #2
		B factorial
		
end_func
	b end_func
 
 
 
 
 ENDFUNC	
 END





