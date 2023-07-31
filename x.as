       ;dxcgf ml,                                
                       MAIN: mov @r3 ,LENGTH
	LOOP: jmp L1
mcro m1
	sub @r1, @r4
bne END
endmcro
prn -5
	bne LOOP
m1
	                                                                               L1: inc K 

mcro m2
	jmp 
endmcro

bne LOOP

mcro m3

endmcro

END: stop f
;zxdfcgvnjk
m3
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
m2
