# Rainfall

## Level0

```sh
level0@RainFall:~$ ls -l
total 732
-rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0
level0@RainFall:~$ ./level0 
Segmentation fault (core dumped)
level0@RainFall:~$ ./level0 test
No !
level0@RainFall:~$ 
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   #Function prologue

   0x08048ec0 <+0>:     push   %ebp
   # Saves the old base pointer %ebp onto the stack
   0x08048ec1 <+1>:     mov    %esp,%ebp
   # Set the base pointer %ebp to the current stack pointer %esp
   0x08048ec3 <+3>:     and    $0xfffffff0,%esp
   # Align the stack pointer %esp to a 16-byte boundary (ensure proper stack alignment and performance optimization)
   0x08048ec6 <+6>:     sub    $0x20,%esp
```
```sh
   0x08048ec9 <+9>:     mov    0xc(%ebp),%eax
   # This moves the value at memory location [ebp + 0xC] into the eax register
```
```sh
   0x08048ecc <+12>:    add    $0x4,%eax
   # This adds 4 to eax, effectively moving from argv[0] to argv[1]
```
```sh
   0x08048ecf <+15>:    mov    (%eax),%eax
   # Deferencing the pointer stored in eax and storing it back into eax
   # Converts eax from a char ** (pointer to argv[1]) into a char * (pointer to the string stored in argv[1])
```
```sh
   0x08048ed1 <+17>:    mov    %eax,(%esp)
   # Push argv[1] onto the stack, preparing it as a paramater for the upcoming atoi call
```
```sh
   0x08048ed4 <+20>:    call   0x8049710 <atoi>
   # atoi(argv[1])
```
```sh
   0x08048ed9 <+25>:    cmp    $0x1a7,%eax
   # Compare the hexadecimal value 0x1a7 (423 in decimal) with the return value of atoi

   0x08048ede <+30>:    jne    0x8048f58 <main+152>
   # If the comparaison is not success successful (if atoi(argv[1]) != 423)
   # Jump to main + 152
```
```sh
   0x08048ee0 <+32>:    movl   $0x80c5348,(%esp)
   0x08048ee7 <+39>:    call   0x8050bf0 <strdup>
   0x08048eec <+44>:    mov    %eax,0x10(%esp)
   0x08048ef0 <+48>:    movl   $0x0,0x14(%esp)
   0x08048ef8 <+56>:    call   0x8054680 <getegid>
   0x08048efd <+61>:    mov    %eax,0x1c(%esp)
   0x08048f01 <+65>:    call   0x8054670 <geteuid>
   0x08048f06 <+70>:    mov    %eax,0x18(%esp)
   0x08048f0a <+74>:    mov    0x1c(%esp),%eax
   0x08048f0e <+78>:    mov    %eax,0x8(%esp)
   0x08048f12 <+82>:    mov    0x1c(%esp),%eax
   0x08048f16 <+86>:    mov    %eax,0x4(%esp)
   0x08048f1a <+90>:    mov    0x1c(%esp),%eax
   0x08048f1e <+94>:    mov    %eax,(%esp)
   0x08048f21 <+97>:    call   0x8054700 <setresgid>
   0x08048f26 <+102>:   mov    0x18(%esp),%eax
   0x08048f2a <+106>:   mov    %eax,0x8(%esp)
   0x08048f2e <+110>:   mov    0x18(%esp),%eax
   0x08048f32 <+114>:   mov    %eax,0x4(%esp)
   0x08048f36 <+118>:   mov    0x18(%esp),%eax
   0x08048f3a <+122>:   mov    %eax,(%esp)
   0x08048f3d <+125>:   call   0x8054690 <setresuid>
   0x08048f42 <+130>:   lea    0x10(%esp),%eax
   0x08048f46 <+134>:   mov    %eax,0x4(%esp)
   0x08048f4a <+138>:   movl   $0x80c5348,(%esp)
   0x08048f51 <+145>:   call   0x8054640 <execv>
   0x08048f56 <+150>:   jmp    0x8048f80 <main+192>
   # Create a string on the heap (strdup)
   # Gets the effective user and group IDs of the process (getedgid, geteuid)
   # Sets all three real, effective, and saved UIDs/GIDs to these values (setresgid, setresuid)
   # Calls execv() to execute the command stored on the heap (execv)
   # Return 0
```
```sh
   0x08048f58 <+152>:   mov    0x80ee170,%eax
   0x08048f5d <+157>:   mov    %eax,%edx
   0x08048f5f <+159>:   mov    $0x80c5350,%eax
   0x08048f64 <+164>:   mov    %edx,0xc(%esp)
   0x08048f68 <+168>:   movl   $0x5,0x8(%esp)
   0x08048f70 <+176>:   movl   $0x1,0x4(%esp)
   0x08048f78 <+184>:   mov    %eax,(%esp)
   0x08048f7b <+187>:   call   0x804a230 <fwrite>
   # Print "No !\n"
   0x08048f80 <+192>:   mov    $0x0,%eax
   0x08048f85 <+197>:   leave  
   0x08048f86 <+198>:   ret
   # Return 0
End of assembler dump.
```

We just need to call the program using 423 as an argument in order to bypass the comparison happening at 0x08048ed9 <+25>

```sh
level0@RainFall:~$ ./level0 423
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```