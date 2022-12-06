	 
	EXPORT SystemInit
    EXPORT __main


	AREA MYPROG,CODE,READONLY 

SystemInit FUNCTION
	; initialization code
 ENDFUNC


__main FUNCTION

start
		MOV R0,#7
		MOV R1,R0
		MOV R2,#0;
		
digit_counter		
		LSR R1,R1,#1
		ADD R2,R2,#1
		CMP R1,#0
		BNE digit_counter
		
set_counter_zero		
		MOV R3,#0
		MOV R4,#0

zero_one_counter 
		LSRS R0,#1
		BCC zero_count
one_count		
		ADD R4,#1
		B 	end_count
zero_count		
		ADD R3,#1	
		B 	end_count
end_count
		SUB R2,#1
		CMP R2,#0
		BEQ end_func
		B zero_one_counter

end_func
	b end_func

ENDFUNC	
END


