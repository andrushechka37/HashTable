section .text
global asm_strlen

asm_strlen:
    xor     rax, rax        ; Обнуление счетчика длины строки
    vxorps  ymm2, ymm2, ymm2 

    vmovdqu ymm0, [rdi]       ; Загрузка 32 байтов в YMM0
    vpcmpeqb ymm1, ymm2, ymm0 ; Сравнение самих себя, чтобы найти нулевые байты
    vpmovmskb edx, ymm1       ; Получение маски нулевых байтов
    bsf     rcx, rdx          ; Находим позицию первого нулевого бита
    add     rax, rcx          ; Добавляем найденную позицию к счетчику
    ret                       ; Возвращаем длину строки