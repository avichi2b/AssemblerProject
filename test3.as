;test
MAIN: mov Dfa.1, r3
Lab: mov r1, TEST.1
macro m121
inc W
prn TEST, MAIN
mov #-5, TESt
endmacro
.entry TEST
.extern W
TEST: prn Lab, r1
as: hlt r2
TEST: mov Lab, r1
m121
.entry MAIN
.extern Dfa
