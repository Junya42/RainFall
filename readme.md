Install the dependencies first:
```sh
sudo apt-get install qemu-system
```

Download the iso file if not present and run the VM:
```sh
sudo make
```

Connect using ssh (recommended):
```sh
ssh level0@localhost -p 4240
```
level0 password is `level0`