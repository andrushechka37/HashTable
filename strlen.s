section .text
global asm_strcmp_s

asm_strcmp_s:
        vmovdqa ymm1, yword [rdi]  
        vpcmpeqb ymm0, ymm1, yword [rsi]    
        vpmovmskb rax, ymm0  
        ret