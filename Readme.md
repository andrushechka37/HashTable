## МОСКОВСКИЙ ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ (НАЦИОНАЛЬНЫЙ ИССЛЕДОВАТЕЛЬСКИЙ УНИВЕРСИТЕТ)

## Физтех-школа Радиотехники и Компьютерных технологий
<br/>


# ![1](/images/2.png)

##  Отчёт о выполнении лабораторной работы 3.7.8
#  Оптимизация Хеш-таблицы, основанной на двусвязном списке на массивах, с разрешенной коллизией при помощи знаний ассемблера и Intrinsic функций

<br/>
<br/>
<br/>
<br/>
<br/>
<br/>

## Автор: Житников Андрей Павлович, Б01-303
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>

## Долгопрудный
## 1 апреля, 2024
<br/>
<br/>
<br/>
<br/>
<br/>

# `1 Введение`:
# *Цель работы*:
* Написать Хеш-таблицу на С
* Сравнить дисперсию для различных хеш-функций
* Найти узкие места программы с помощью профилировщика
* Ускорить узкие места при помощи Intrinsic функций и ассемблерных оптимизаций

# `2 Ход работы`:
## `Нулевая часть работы`: написание хеш таблицы на С, которая хранит английские слова

## За основу таблицы был взят двусвязный список на массивах из репозитория [List](https://github.com/andrushechka37/List).
## Были реализованы функции:
* Вставки в таблицу
* Поиска данных в таблице
* Чтения данных из файла, заранее обработанного с помощью `get_data_storage`, разбивающей текст на слова и убирающей знаки препинания из текста.
* Текстового дампа с псевдографикой
![kl](/images/dump.png)
## `Первая часть работы`: исследование дисперсии различных хеш функций
## Были исследованы несколько хеш функций:
<br/>
<br/>
<br/>
<br/>

## 1. Функция, всегда возвращающая константу
```C
const int random_const = 50;

size_t always_const_func(char * word, int len_of_word) {
    return random_const;
}
```
![1](images/1_func.png)

### Все слова в одном списке - это наихудшее распределение

## 2. Функция, возвращающая ASCII номер первой буквы слова
```C
size_t first_letter_func(char * word, int len_of_word) {
    return word[0];
}
```
![1](images/2_func.png)
### Все еще плохое распределение, видно 2 пика, соответствующих кодам больших и маленьких буквам

## 3. Функция, возвращающая длину слова
```C
size_t word_len_func(char * word, int len_of_word) {
    return len_of_word;
}
```
![1](images/3_func.png)

### Плохое распределение, так как длина слова редко превышает 10

## 4. Функция, считающая контрольную сумму (сумма всех ASCII кодов букв слова) для нескольких размеров таблицы
```C
size_t ascii_sum_func(char * word, int len_of_word) {
    size_t sum = 0;
    for (int i = 0; i < len_of_word; i++) {
        sum += word[i];
    }
    return sum;
}
```
### для размера таблицы 2003
![1](images/4_func.png)

### для размера таблицы 101

![77_funccc](images/77_funccc.png)

### Для небольшого размера таблицы функция дает удовлетворительное распределение, но при увеличении размера таблицы явно видны пики и неравномерность распределения.

## 5. Функция, возвращающая контрольную сумму, деленную на длину слова
```C
int ascii_sum_div_len_func(char * word, int len_of_word) {
    int sum = 0;

    for (int i = 0; i < len_of_word; i++) {
        sum += word[i];
    }

    return sum / len_of_word;
}
```

### Четко виден пик в районе 100, где расположено большинство слов

![1](images/5_func.png)
## 6. Rol xor функция
```C
inline size_t my_rol(size_t x) {
    return (x << 1) | (x >> 63);
}

size_t rol_hash_func(char * word, int len_of_word) {
    size_t hash = 0;
    for (int i = 0; i < len_of_word; i++) {
        hash = my_rol(hash) ^ word[i];
    }
    return hash % hash_table_size;
}

```
## rol:
![1](images/6_func.png)

### Видно, что распределение довольно-таки равномерное, значения в пиках не сильно отличаются от среднего

## Заметим что, при компилировании  с -O1 и выше, my_ror/my_rol заменяются компиллятором на ror и rol соответственно (код взят с сайта [godbolt](https://godbolt.org))

### Компиллятор: x86-64 gcc 13.2
# -O1 и выше:
```assembly
my_ror(unsigned long):
        mov     rax, rdi
        ror     rax
        ret
my_rol(unsigned long):
        mov     rax, rdi
        rol     rax
        ret
```
# -O0:
```assembly
my_ror(unsigned long):
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        mov     rax, QWORD PTR [rbp-8]
        ror     rax
        pop     rbp
        ret
my_rol(unsigned long):
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        mov     rax, QWORD PTR [rbp-8]
        rol     rax
        pop     rbp
        ret

```


## CRC32
![df](images/ctc.png)

### CRC32 дает лучшее распределение среди всех предыдущих функций

# Результаты измерений

|          hash-func         | Dispersion * $10^3$ |
|:--------------------------:|---------------------|
|        return const        | 7993                |
|       return word[0]       | 7                   |
|      return len(word)      | 72                  |
|      return ascii_sum      | 0.021               |
| return ascii_sum/len(word) | 14                  |
|        rol xor func        | 0.002               |
|            CRC32           | 0.001               |




















# `Вторая часть работы`: Оптимизация узких мест программы
## С помощью профилировщика perf были выявлены функции, с самым большим временем исполнения.
![2](/images/perf1.png)

## Видно, что основной вклад в нагрузку вносит хеш-функция и strcmp, следовательно это первые кандитаты на оптимизацию.


## Суммарное время поиска всех слов, находящихся в таблице, считалось при помощи __rdtsc, повторялось 100 000 раз, и считалось среднее. Для увеличения достоверности данных программа запускалась на отдельном ядре с высшим приоритетом(прерывания я не убирал, страшно было чето там напортачить, по наблюдениям разброс невелик). `nice -n -19 taskset 1`



| Version | Compilation flags | Ticks * $10^3$ | Acceleration relative to baseline |  Acceleration relative to previous optimization |
|:-------:|:-----------------:|:--------------:|:---------------------------------:|:-----------------------------------------------:|
|   base  |        -O0        |      1185      |                 1                 |                        1                        |
|   base  |        -O3        |       159      |                 1                 |                        1                        |


# Оптимизация CRC32
## Функция CRC32 была ускорена путем использования intrinsic функций, а именно `_mm_crc32_u8`
```C
size_t CRC32_modified(char * word, int len_of_word) {

	size_t hash = 0;
	uint32_t crc = 0x407EF1CA;

	for (size_t i = 0; i < len_of_word; i++) {
		hash = _mm_crc32_u8(hash, word[i]);
	}

	return hash % hash_table_size;
}
```


|    Version    | Compilation flags | Ticks * $10^3$ | Acceleration relative to baseline |  Acceleration relative to previous optimization |
|:-------------:|:-----------------:|:--------------:|:---------------------------------:|:-----------------------------------------------:|
| intrinsic opt |        -O0        |       125      |                9,41               |                       9,41                      |
| intrinsic opt |        -O3        |       22       |                7,10               |                       7,10                      |

## Результат впечатляющий, ускорение для обоих версий значительное, значит intrinsic оптимизация и переход на платформно-зависимый код оправдан.

# Оптимизация strcmp при помощи ассемблерной функции:

## Первоначально код выглядел так:
```assembly
section .text
global asm_strcmp_s

asm_strcmp_s:
        vmovdqu ymm1, yword [rdi]
        vpcmpeqb ymm0, ymm1, yword [rsi]
        vpmovmskb rax, ymm0
        ret
```
## Однако он был медленнее, чем стандартный strcmp ~ 2 раза, что было крайне странно.
## Спустя многие часы разбирательств я обратил на странное поведение функции:
![](/images/emul.png)
## Основная нагрузка приходилась на cmp, что было крайне странно. Оказалось, что данная инструкция поддерживается начиная с avx2, но на сервере был только avx. Видимо процессор эмулировал поведение инструкции, из-за чего скорость выполнения значительно проседала.

## После изменения код выглядел так:
```assembly
section .text
global asm_strcmp_s

asm_strcmp_s:
        vmovdqu ymm1, yword [rdi]
        vpcmpeqd ymm0, ymm1, yword [rsi]
        vmovmskps eax, ymm0
        xor eax, 0xFF
        ret
```


| Version | Compilation flags | Ticks * $10^3$ | Acceleration relative to baseline |  Acceleration relative to previous optimization |
|:-------:|:-----------------:|:--------------:|:---------------------------------:|:-----------------------------------------------:|
| asm opt |        -O0        |       114      |               10,37               |                       1,1                       |
| asm opt |        -O3        |       28       |                5,55               |                       0,78                      |


## Результат -O0 предсказуем, но результат для -O3 непонятен. Почему так произошло?

## Рассмотрим первую оптимизацию
![](/images/int%201.png)
![](/images/int%202.png)

TODO: add more context about what's on the screenshot

## Видим, что компилятор заинлайнил и функцию поиска, и strcmp

![](/images/asm%201.png)
![](/images/asm%202.png)

## Во втором случае компиллятор не заинлайнил мой strcmp, в результате чего производительность могла ухудшится.

# Оптимизация strcmp с помощью встроенного ассемблера
## Так как я был не удовлетворен оптимизацией strcmp я решил использовать встроенный ассемблер, чтобы иметь возможность заинлайнить свою функцию.
```C
static inline int inline_asm_strcmp(const char * str1, const char * str2) {
    int res = 0;

    asm (".intel_syntax noprefix\n"
         "vmovdqu ymm1, YMMWORD PTR [rdi] \n"
         "vpcmpeqd ymm0, ymm1, YMMWORD PTR [rsi]\n"
         "vmovmskps eax, ymm0\n"
         "xor eax, 0xFF\n"
         ".att_syntax prefix\n"
         : "=r" (res) :: "ymm0", "ymm1", "cc");

    return res;
}
```



| Version | Compilation flags | Ticks * $10^3$ | Acceleration relative to baseline |  Acceleration relative to previous optimization |
|:-------:|:-----------------:|:--------------:|:---------------------------------:|:-----------------------------------------------:|
| asm opt |        -O0        |       105      |               11,25               |                       1,08                      |
| asm opt |        -O3        |       19       |                8,36               |                       1,5                       |

## Результат в принципе ожидаем, asm_strcmp ускорилась относительно библиотечного при -O3 в 1,17 раза, что является приемлемым результатом.


# `Обсуждение результатов`
## Ну нам удалось неплохо оптимизировать, DED_coeff >> 1, что очень хорошо

TODO: make all of this into formula
DED_coeff = $\frac{\text{ускорение программы}}{\text{количество ассемблерных строк} + строк с intrinsicами} * 1000 = \frac{8360}{1 + 5*2} = 760$


# Additional task from [alexpaniman](https://github.com/alexpaniman)


## Как зависит производительность от размера таблицы?
![](/images/tablesize.png)

Сначала у нас уменьшается коллизия с ростом размера(линейый поиск не такой долгий)
а рост наверно потому что долго обращаться в память по столь отдаленным друг от друга мест в памяти
ну и где-то получается inf


TODO: Run existing hash tables to see performance difference dense_hash_map by google, sparse_hash_map by google, std::unordered_map, std::map (although this is a map but not a hash map)

TODO: Improve your measuring metadology (use a separate thread for running your program that is not suseptible to random interruptions by task scheduler): https://stackoverflow.com/questions/13583146/whole-one-core-dedicated-to-single-process http://www.hydrogen18.com/blog/howto-give-a-single-process-its-own-cpu-core-in-linux.html https://askubuntu.com/questions/483824/how-to-run-a-program-with-only-one-cpu-core

TODO: Graph of hash map performance depending on size (explain what happens)

TODO: Don't store array of lists, it's too wasteful in terms of memory and allocation count and also cache-locality

NOTE: You can read about open addressing, different probing methods and also Robin Good method

TODO: lib for logging














baseline O0  --  1185525

baseline O3  --  159119 

1opt O0  --  125921

1opt O3  --  22401


2opt O0  --  114249    (дала незначительный прирост нормас)

2opt O3  --  28633     (потому что при первой оптимизации inline search и inline cmp, 2 фотки) (a во втором случае не инлайнит 2 фотки)



105303    O0 ускорило но не сильно 

19014    O3   почти также
