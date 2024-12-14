# Additional lab work: xv6 Symbolic links
Authors (team): [Ksenia Kretsula](https://github.com/kretsulaksusha)

**Task**: Add support of symbolic links to xv6.

**Hardware**: CPU: Apple M1 Max; CPU cores (P+E): 10 (8+2).

## Installation on Mac OSX

1. Agree to the Xcode license:

   ```shell
   sudo xcodebuild -license accept
   ```
2. Installing QEMU for macOS:

   ```shell
   brew install qemu x86_64-elf-gcc
   ```
3. Building xv6:

   ```shell
   export TOOLPREFIX=x86_64-elf-
   export QEMU=qemu-system-x86_64
   make
   ```
4. Using QEMU emulator run xv6 (`-nox` is added to run with only the serial console),

   ```shell
   make qemu-nox
   ```
5. Exit xv6

   `Ctrl + a x`

### Adding `symlink` to xv6

Described in [Homework: Naming](https://pdos.csail.mit.edu/6.828/2010/homework/xv6-names.html).

A symbolic link is simply a file with a special type (e.g., `T_SYMLINK` instead of `T_FILE` or `T_DIR`) whose contents contain the path being linked to.

**Steps:**

1. Read: `namei` in `fs.c`, `file.c`, `sysfile.c`.
2. 

#### Implementation of symlink

## Testing & Results

All the commands executed in the video:
```bash
make clean
make
make qemu-nox CPUS=1
```

Link to the video on Google Drive: [xv6 symlink example]().

## Resources

- [Homework: Naming](https://pdos.csail.mit.edu/6.828/2010/homework/xv6-names.html)
