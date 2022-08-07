09 - Porting Newlib: `malloc` & `printf`
===

In this lecture, we will retarget the `printf` function to USART1 and make the `malloc` function work.

We will also introduce Newlib, the standard C library that defaultly used by the arm-none-eabi-gcc compiler.

**<p style="color:crimson"><i class="fa fa-github"></i> Please fork the [LAB09](https://github.com/ybcsie/ESEmbedded_LAB09) repo, and then clone the repo you just forked.</p>**

**<p style="color:crimson">　 You will have to push the modified version to GitHub at the end of this lecture.</p>**

---

## Calling `printf`

You must be familiar with the following code:

```clike=
#include <stdio.h>

int main()
{
 printf("Hello, World!");
 return 0;
}
```

This is probably the "simplest" C program you have written since you first started to learn C.

:::warning

The concepts behind the `printf` function are actually not simple.

:::

* Let's see what will happen if we directly call `printf` in our STM32F4 program.

```clike=
int main(void)
{
	printf("Hello World");

	while(1);
}
```

* The error messages from make:

```shell=
arm-none-eabi-gcc -std=c11 -Wall -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c startup.c vector_table.s -o printf.elf
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-sbrkr.o): In function `_sbrk_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/sbrkr.c:58: undefined reference to `_sbrk'
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-writer.o): In function `_write_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/writer.c:58:undefined reference to `_write'
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-closer.o): In function `_close_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/closer.c:53:undefined reference to `_close'
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-lseekr.o): In function `_lseek_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/lseekr.c:58:undefined reference to `_lseek'
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-readr.o): In function `_read_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/readr.c:58: undefined reference to `_read'
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-fstatr.o): In function `_fstat_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/fstatr.c:62:undefined reference to `_fstat'
/usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/lib/armv7e-m/libc.a(lib_a-isattyr.o): In function `_isatty_r':
/build/newlib-5zwpxE/newlib-2.2.0+git20150830.5a3d536/build/arm-none-eabi/armv7e-m/newlib/libc/reent/../../../../../../newlib/libc/reent/isattyr.c:58: undefined reference to `_isatty'
collect2: error: ld returned 1 exit status
makefile:6: recipe for target 'printf.bin' failed
make: *** [printf.bin] Error 1
```

:::success

What is Newlib?

:::

:::success

What are the undefined symbols `_sbrk`, `_write` etc?

:::


------------------------------------------------------------------------------------------


## About C Standard Library

* The C language provides no built-in facilities for performing such common operations as **input/output**, **memory management**, **string manipulation**, and the like (such as fopen, malloc, printf, exit etc).

* **These facilities are defined in a standard library**, which you compile and link with your programs.

* The term "libc" is commonly used as a shorthand for the "standard C library"

* The Implementations of the standard library are platform dependent.
    * Linux: GNU C Library (glibc)
    * Windows: Microsoft C run-time library (msvcrt)

* Under an OS, many of the basic facilities require **system calls** to be implemented.
    * The `printf` in Linux, for example, uses `write` system call to send out a string.
    * In Windows, it calls `WriteConsole` API.

* In MMU-less embedded systems, there are several implementations of the standard library, such as Newlib, μClibc etc.

* By default, the arm-none-eabi-gcc is configured to link against Newlib.

* refs:
    * [*The GNU C Library Reference Manual*](https://www.gnu.org/software/libc/manual/)
    * [*glibc wiki*](https://sourceware.org/glibc/wiki/HomePage)
    * [*Linux Programmer's Manual*](http://man7.org/linux/man-pages/man7/libc.7.html)




------------------------------------------------------------------------------------------



## Newlib Overview

* Newlib is an implementation of the standard C library that is specifically thought to run somewhere with **low resources and undefined hardware**.

* Some of the library functions depend on support from the underlying operating system and may not be available on every platform.

* For embedded systems in particular, many of these underlying operating system services may not be available or may not be fully functional.

* The C subroutine library depends on a handful of subroutine calls for operating system services. If you use the C library on a system that complies with the POSIX.1 standard (also known as IEEE 1003.1), most of these subroutines are supplied with your operating system.

* If some of these subroutines are not provided with your system or if you are developing software without an OS, you will at least need to provide do-nothing stubs (or subroutines with minimal functionality) to allow your programs to link with the subroutines in libc.a.

* We can find the complete set of system definitions (primarily subroutines) required in [*Red Hat newlib C Library Documentation*]. The examples shown implement the minimal functionality required to allow libc to link, and fail gracefully where OS services are not available.

* According to [*Red Hat newlib C Library Documentation*], the `_sbrk` is used to increase program data space. As malloc and related functions depend on this, it is useful to have a working implementation. It means that the `malloc` function is called in the `printf` implementation. So before making the `printf` work, we will first port `malloc`.




------------------------------------------------------------------------------------------







## LAB09-1 Porting Newlib: `malloc`

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB09/1_malloc`</p>**

:::warning

Please copy your completed `main.c` in the previous lab to this lab directory, and then follow the steps below to modify it.

:::

### `_sbrk`

From [*Red Hat newlib C Library Documentation*]:
>Increase program data space. As malloc and related functions depend on this, it is useful to have a working implementation. The following suffices for a standalone system; it exploits the symbol _end automatically defined by the GNU linker.
>```clike=
>caddr_t sbrk(int incr) {
>  extern char _end;		/* Defined by the linker */
>  static char *heap_end;
>  char *prev_heap_end;
> 
>  if (heap_end == 0) {
>    heap_end = &_end;
>  }
>  prev_heap_end = heap_end;
>  if (heap_end + incr > stack_ptr) {
>    write (1, "Heap and stack collision\n", 25);
>    abort ();
>  }
>
>  heap_end += incr;
>  return (caddr_t) prev_heap_end;
>}
>```

The modified version for our STM32F4 program:

```clike=
#define HEAP_MAX (64 * 1024) //64 KB
void *_sbrk(int incr)
{
	extern uint8_t _mybss_vma_end; //Defined by the linker script
	static uint8_t *heap_end = NULL;
	uint8_t *prev_heap_end;

	if (heap_end == NULL)
		heap_end = &_mybss_vma_end;

	prev_heap_end = heap_end;
	if (heap_end + incr > &_mybss_vma_end + HEAP_MAX)
		return (void *)-1;

	heap_end += incr;
	return (void *)prev_heap_end;
}
```

1. Add the code above into your `man.c`.
2. To use the `malloc` function, you will have to include the `<stdlib.h>` header.
3. Modify the `main` function as below.

    ```clike=
    int main(void)
    {
        init_usart1();

        char *ch = malloc(3 * sizeof(char));
        if (ch != NULL)
        {
            ch[0] = 'A';
            ch[1] = 'B';
            ch[2] = 'C';

            usart1_send_char(ch[0]);
            usart1_send_char(ch[1]);
            usart1_send_char(ch[2]);

            free(ch);
        }

        blink(LED_BLUE);
    }
    ```

4. Try to build with:

    ```shell
    make
    ```

5. Try to flash with:

    ```shell
    make flash
    ```

    You will get error messages like this:

    ```shell
    st-flash --reset write malloc.bin 0x8000000
    st-flash 1.5.0
    2019-01-29T00:44:03 INFO common.c: Loading device parameters....
    2019-01-29T00:44:03 INFO common.c: Device connected is: F4 device, id 0x10076413
    2019-01-29T00:44:03 INFO common.c: SRAM size: 0x30000 bytes (192 KiB), Flash: 0x100000 bytes (1024 KiB) in pages of 16384 bytes
    2019-01-29T00:44:03 INFO common.c: Attempting to write 536873020 (0x2000083c) bytes to stm32 address: 134217728 (0x8000000)
    2019-01-29T00:44:03 ERROR common.c: addr too high
    stlink_fwrite_flash() == -1
    makefile:10: recipe for target 'flash' failed
    make: *** [flash] Error 255
    ```

7. The error might be caused by the sections that were undefined in our linker script. Use objdump to check:

    ```shell
    arm-none-eabi-objdump -h malloc.elf
    ```

    You will get:

    ```shell
    malloc.elf:     file format elf32-littlearm

    Sections:
    Idx Name          Size      VMA       LMA       File off  Algn
      0 .mytext       00000fe0  00000000  00000000  00010000  2**3
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      1 .text.malloc  00000010  00000fe0  00000fe0  00010fe0  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      2 .text.free    00000010  00000ff0  00000ff0  00010ff0  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      3 .text._malloc_r 00000524  00001000  00001000  00011000  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      4 .text.__malloc_lock 00000004  00001524  00001524  00011524  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      5 .text.__malloc_unlock 00000004  00001528  00001528  00011528  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      6 .text._sbrk_r 00000024  0000152c  0000152c  0001152c  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      7 .text._malloc_trim_r 0000009c  00001550  00001550  00011550  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      8 .text._free_r 000001b0  000015ec  000015ec  000115ec  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
      9 .text.cleanup_glue 0000001c  0000179c  0000179c  0001179c  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
     10 .text._reclaim_reent 00000088  000017b8  000017b8  000117b8  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
     11 .rodata.str1.4 00000004  00001840  00001840  00011840  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, DATA
     12 .rodata._global_impure_ptr 00000004  00001844  00001844  00011844  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, DATA
     13 .data.__malloc_av_ 00000408  20000000  20000000  00020000  2**2
                      CONTENTS, ALLOC, LOAD, DATA
     14 .data.__malloc_trim_threshold 00000004  20000408  20000408  00020408  2**2
                      CONTENTS, ALLOC, LOAD, DATA
     15 .data.__malloc_sbrk_base 00000004  2000040c  2000040c  0002040c  2**2
                      CONTENTS, ALLOC, LOAD, DATA
     16 .data.impure_data 00000428  20000410  20000410  00020410  2**3
                      CONTENTS, ALLOC, LOAD, DATA
     17 .data._impure_ptr 00000004  20000838  20000838  00020838  2**2
                      CONTENTS, ALLOC, LOAD, DATA
     18 .mybss        00000008  2000083c  00000fe0  0002083c  2**2
                      ALLOC
     19 .bss.__malloc_max_total_mem 00000004  20000844  00000fe8  0002083c  2**2
                      ALLOC
     20 .bss.__malloc_max_sbrked_mem 00000004  20000848  00000fec  0002083c  2**2
                      ALLOC
     21 .bss.__malloc_top_pad 00000004  2000084c  00000ff0  0002084c  2**2
                      ALLOC
     22 .bss.__malloc_current_mallinfo 00000028  20000850  00000ff4  0002084c  2**2
                      ALLOC
     23 .comment      00000070  00000000  00000000  0002083c  2**0
                      CONTENTS, READONLY
     24 .ARM.attributes 00000031  00000000  00000000  000208ac  2**0
                      CONTENTS, READONLY
     25 .debug_line   00000fe5  00000000  00000000  000208dd  2**0
                      CONTENTS, READONLY, DEBUGGING
     26 .debug_info   00004924  00000000  00000000  000218c2  2**0
                      CONTENTS, READONLY, DEBUGGING
     27 .debug_abbrev 00000ed1  00000000  00000000  000261e6  2**0
                      CONTENTS, READONLY, DEBUGGING
     28 .debug_aranges 00000158  00000000  00000000  000270b8  2**3
                      CONTENTS, READONLY, DEBUGGING
     29 .debug_loc    00001483  00000000  00000000  00027210  2**0
                      CONTENTS, READONLY, DEBUGGING
     30 .debug_ranges 000000c0  00000000  00000000  00028693  2**0
                      CONTENTS, READONLY, DEBUGGING
     31 .debug_str    000008fc  00000000  00000000  00028753  2**0
                      CONTENTS, READONLY, DEBUGGING
     32 .debug_frame  000001b0  00000000  00000000  00029050  2**2
                      CONTENTS, READONLY, DEBUGGING
    ```

8. There are additional sections that need to be added into the linker script.

    The modified linker script:


    ```
    MEMORY
    {
        FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
        SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    }

    SECTIONS
    {
        .mytext :
        {
            KEEP(*(.vector_table))
            *(.text)
            *(.text.*)
            *(.rodata)
            *(.rodata.*)
            _mydata_lma_start = .; /* LMA is equivalent to VMA in this section */
        } > FLASH

        .mydata : AT(_mydata_lma_start) /* tell linker whrere (LMA) this section should be put at */
        {
            _mydata_vma_start = .;
            *(.data)
            *(.data.*)
            _mydata_vma_end = .;
        } > SRAM

        .mybss :
        {
            _mybss_vma_start = .;
            *(.bss)
            *(.bss.*)
            *(COMMON)
            _mybss_vma_end = .;
        } > SRAM

        _msp_init = ORIGIN(SRAM) + LENGTH(SRAM);
    }
    ```

9. Build the program again and flash.

10. The USB to TTL module should be connected (see the guides in lecture 08 - [USART Basic]).

11. Use screen to see the result: (**replace ttyUSBx with yours**)

    ```shell
    sudo screen /dev/ttyUSBx 115200
    ```


------------------------------------------------------------------------------------------


## LAB09-2 Porting Newlib: `printf`

The lab files are in `09labs/printf` and are are duplicated from the previous lab (malloc)

:::warning

Please copy your completed `main.c` and `stm32f4.ld` in the previous lab (malloc) to this lab directory, and then follow the steps below to modify it.

:::

### `_write`

From [*Red Hat newlib C Library Documentation*]:

>Write to a file. libc subroutines will use this system routine for output to all files, including stdout—so if you need to generate any output, for example to a serial port for debugging, you should make your minimal write capable of doing this. The following minimal implementation is an incomplete example; it relies on a outbyte subroutine (not shown; typically, you must write this in assembler from examples provided by your hardware manufacturer) to actually perform the output.
>
>```clike=
>int write(int file, char *ptr, int len) {
>  int todo;
>
>  for (todo = 0; todo < len; todo++) {
>    outbyte (*ptr++);
>  }
>  return len;
>}

### `_close`

From [*Red Hat newlib C Library Documentation*]:

>Close a file. Minimal implementation:
>
>```clike=
>int close(int file) {
>  return -1;
>}

### `_lseek`

From [*Red Hat newlib C Library Documentation*]:

>Set position in a file. Minimal implementation:
>
>```clike=
>int lseek(int file, int ptr, int dir) {
>  return 0;
>}
>```

### `_read`

From [*Red Hat newlib C Library Documentation*]:

>Read from a file. Minimal implementation:
>
>```clike=
>int read(int file, char *ptr, int len) {
>  return 0;
>}
>```

### `_fstat`

From [*Red Hat newlib C Library Documentation*]:

>Status of an open file. For consistency with other minimal implementations in these examples, all files are regarded as character special devices. The sys/stat.h header file required is distributed in the include subdirectory for this C library.
>
>```clike=
>#include <sys/stat.h>
>int fstat(int file, struct stat *st) {
>  st->st_mode = S_IFCHR;
>  return 0;
>}
>```

### `_isatty`

From [*Red Hat newlib C Library Documentation*]:

>Query whether output stream is a terminal. For consistency with the other minimal implementations, which only support output to stdout, this minimal implementation is suggested:
>
>```clike=
>int isatty(int file) {
>  return 1;
>}
>```


The modified version for our STM32F4 program:

```clike=
int _write(int file, char *ptr, int len)
{
	for (unsigned int i = 0; i < len; i++)
		usart1_send_char(*ptr++);

	return len;
}

int _close(int file)
{
	return -1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}
```

1. Add the code above into your `man.c` (remember to include the `<sys/stat.h>` header).
2. To use the `printf` function, you will have to include the `<stdio.h>` header.
3. Modify the `main` function as below.

    ```clike=
    int main(void)
    {
        init_usart1();

        printf("Hello World\r\n");

        int i = 75;

        printf("Decimal: %d  Hexadecimal: 0x%x \r\n", i, i);

        printf("Character: %c\r\n", i);

        blink(LED_BLUE);
    }
    ```

4. Try to build with:

    ```shell
    make
    ```

    You will get an error message like this:

    ```shell
    arm-none-eabi-gcc -std=c11 -Wall -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c startup.c vector_table.s -o printf.elf
    /usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/bin/ld: section .eh_frame loaded at [0000000000007648,00000000000076b7] overlaps section .mydata loaded at [0000000000007648,0000000000007f03]
    collect2: error: ld returned 1 exit status
    makefile:6: recipe for target 'printf.bin' failed
    make: *** [printf.bin] Error 1
    ```

5. The error indicates the `.eh_frame` section is not loaded at a proper address. Add it into our linker script.

    The modified linker script:

    ```
    MEMORY
    {
        FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
        SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    }

    SECTIONS
    {
        .mytext :
        {
            KEEP(*(.vector_table))
            *(.text)
            *(.text.*)
            *(.rodata)
            *(.rodata.*)
        } > FLASH

        .eh_frame :
        {
            *(.eh_frame)
            _mydata_lma_start = .; /* LMA is equivalent to VMA in this section */
        } > FLASH

        .mydata : AT(_mydata_lma_start) /* tell linker whrere (LMA) this section should be put at */
        {
            _mydata_vma_start = .;
            *(.data)
            *(.data.*)
            _mydata_vma_end = .;
        } > SRAM

        .mybss :
        {
            _mybss_vma_start = .;
            *(.bss)
            *(.bss.*)
            *(COMMON)
            _mybss_vma_end = .;
        } > SRAM

        _msp_init = ORIGIN(SRAM) + LENGTH(SRAM);
    }
    ```

6.  Try to build again. You will get another error message like this:

    ```shell
    arm-none-eabi-gcc -std=c11 -Wall -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c startup.c vector_table.s -o printf.elf
    /usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/bin/ld: section .ARM.exidx loaded at [00000000000076b8,00000000000076bf] overlaps section .mydata loaded at [00000000000076b8,0000000000007f73]
    collect2: error: ld returned 1 exit status
    makefile:6: recipe for target 'printf.bin' failed
    make: *** [printf.bin] Error 1
    ```

7. Again, add the udefined section into our linker script and rebuild the program.

    The modified linker script:


    ```
    MEMORY
    {
        FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
        SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    }

    SECTIONS
    {
        .mytext :
        {
            KEEP(*(.vector_table))
            *(.text)
            *(.text.*)
            *(.rodata)
            *(.rodata.*)
        } > FLASH

        .eh_frame :
        {
            *(.eh_frame)
        } > FLASH

        .ARM.exidx :
        {
            *(.ARM.exidx)
            _mydata_lma_start = .; /* LMA is equivalent to VMA in this section */
        } > FLASH

        .mydata : AT(_mydata_lma_start) /* tell linker whrere (LMA) this section should be put at */
        {
            _mydata_vma_start = .;
            *(.data)
            *(.data.*)
            _mydata_vma_end = .;
        } > SRAM

        .mybss :
        {
            _mybss_vma_start = .;
            *(.bss)
            *(.bss.*)
            *(COMMON)
            _mybss_vma_end = .;
        } > SRAM

        _msp_init = ORIGIN(SRAM) + LENGTH(SRAM);
    }
    ```


8. After success, flash with:

    ```shell
    make flash
    ```

9. The USB to TTL module should be connected (see the guides in lecture 08 - [USART Basic]).

10. Use screen to see the result: (**replace ttyUSBx with yours**)

    ```shell
    sudo screen /dev/ttyUSBx 115200
    ```



[USART Interrupt]: http://www.nc.es.ncku.edu.tw/course/embedded/08/#LAB-%E2%94%80-USART-Interrupt
[*Red Hat newlib C Library Documentation*]: https://sourceware.org/newlib/libc.html#Stubs
[USART Basic]: http://www.nc.es.ncku.edu.tw/course/embedded/08/#Circuit-Schematic



-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`