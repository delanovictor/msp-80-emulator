MVI A,AA
MVI B,0F
STC
ADC B
ADC B
CMC
MVI A,06
MVI L,02
SBB L
CMC
SBB L
SBB L
HLT