       ;dxcgf ml,                                
                       MAIN: mov LENGTH, @r3
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
	jmp @r1
endmcro

bne LOOP

mcro m3
.extern b
endmcro

END: stop f
;zxdfcgvnjk
m3
.extern 
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data     22
m2
