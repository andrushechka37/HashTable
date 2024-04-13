section .text
global asm_strlen

asm_strlen:
    xor     rax, rax          ; Обнуление счетчика длины строки
    vxorps  ymm2, ymm2, ymm2 

    vmovdqu ymm0, [rdi]       ; Загрузка 32 байтов в YMM0
    vpcmpeqb ymm1, ymm2, ymm0 ; Сравнение самих себя, чтобы найти нулевые байты
    vpmovmskb eax, ymm1       ; Получение маски нулевых байтов
    bsf     rax, rax          ; Находим позицию первого нулевого бита
    ret                       ; Возвращаем длину строки