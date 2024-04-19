section .text
global asm_strcmp_s

asm_strcmp_s:
        vmovdqa ymm1, yword [rdi]  
        vpcmpeqd ymm0, ymm1, yword [rsi]    
        vmovmskps eax, ymm0
        xor eax, 0xFF
        ret
