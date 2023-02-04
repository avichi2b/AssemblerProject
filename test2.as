;test
MAIN: mov Dfa.1, r1
Lab: mov r1, TEST.1
macro m121
inc W
prn #-5
endmacro
.entry TEST
.extern W
TEST: prn Lab
m121
.entry MAIN
.extern Dfa
