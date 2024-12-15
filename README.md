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

## Adding `symlink` to xv6

A symbolic link is a special type of file (`T_SYMLINK`) whose contents store the path to which it links. To implement symbolic links in xv6, the first step is to define the new file type `T_SYMLINK`.

File types like `T_FILE` and `T_DIR` are defined in `stat.h`. To add support for symbolic links, include the following definition in `stat.h`:

```c
#define T_SYMLINK  4   // Symbolic link
```

Next, we need to examine the `namei` function in `fs.c`, which uses the `namex` function to resolve a file path to its corresponding inode. The function signature is as follows:

```c
static struct inode* namex(char *path, int nameiparent, char *name);
```

It is responsible for traversing the filesystem and locating the inode corresponding to the given path.

**Parameters:**
- `char *path`: The file path to resolve (can be absolute or relative).
- `int nameiparent`:
  - If `0`, return the inode corresponding to the full path.
  - If non-zero, return the **parent inode** of the path (used for operations like creating or deleting files).
- `char *name`: A buffer to store the path component.

#### Function flow

1. Determine the starting inode

   ```c
   if(*path == '/')
    ip = iget(ROOTDEV, ROOTINO);
   else
    ip = idup(myproc()->cwd);
   ```
   
   - If the path starts with `'/'` (absolute path), the traversal begins at the **root inode** (`ROOTINO` on device `ROOTDEV`).
   - If the path is relative, the traversal starts from the current working directory (`myproc()->cwd`).
   - `iget` fetches the root inode, and `idup` duplicates the current working directory inode.

2. Iteratively traverse the path

   ```c
   while((path = skipelem(path, name)) != 0){
   // Process each component of the path
   }
   ```
   
   - The function processes the path **one element at a time** using `skipelem`.

   In the loop it:
   - checks if current inode is a directory
      - After locking the current inode (`ip`), the function checks if it's a directory (`T_DIR`). 
   - handles `nameiparent` for parent directory lookups
     - If `nameiparent` is set, the function stops one level **before** the final element of the path.
     - This is used when performing operations like creating or removing files, where the parent directory is needed.
   - looks up the next path component
     - `dirlookup` searches the current directory (`ip`) for the next path component (`name`) and returns its corresponding inode.
     - If the component is not found, the function returns `0` (file or directory not found).
   - Prepare for the next iteration
     - The current inode (`ip`) is unlocked.
     - The resolved inode (`next`) becomes the current inode for the next iteration.

3. Handle final return

   - If `nameiparent` is set, return `0` because the caller is looking for the **parent directory**, not the final inode.
   - Otherwise, return the resolved inode corresponding to the full path.


Thus, we will modify this function to handle symbolic links during path resolution.

#### Key modifications for symbolic link support

```c
namex(struct inode *root, char *path, int nameiparent, char *name, int depth);
```

- **New parameters**:
   - `root`: allows `namex` to resolve paths relative to a specific root inode (useful for symlink resolution).
   - `depth`: prevents infinite recursion by limiting the number of symbolic links that can be followed.
- **Handling symbolic links**:
   - When a symbolic link (`T_SYMLINK`) is encountered, the function reads the path stored in the symlink and recursively resolves it by calling `namex` again, following the linked path.
   - If the symlink path is invalid or too long, it returns `0`.
- **Preventing infinite recursion**:
   - The `depth` parameter ensures that symbolic links do not cause an infinite loop by limiting the depth of recursion.

#### Add `symlink` syscall

1. Add syscall in `syscall.h`.

   ```c
   #define SYS_symlink 27
   ```
2. Add syscall function prototype in `syscall.c`.

   ```c
   extern int sys_symlink(void);
   ```
3. Add pointer to syscall in `syscall.c`.

   ```c
   [SYS_symlink] sys_symlink,
   ```
   When system call occurred with number 27, function pointed by function pointer `sys_symlink` will be called.
4. Implementing `sys_symlink` function in `sysfile.c`.

   ```c
   int
   sys_symlink(void)
   {
     char *old, *new;
     struct inode *ip;
   
     if(argstr(0, &old) < 0 || argstr(1, &new) < 0)
       return -1;
   
     begin_op();
     if((ip = create(new, T_SYMLINK, 0, 0)) == 0){
       end_op();
       return -1;
     }
   
     if(writei(ip, old, 0, strlen(old)) != strlen(old)){
       iunlockput(ip);
       end_op();
       return -1;
     }
   
     iunlockput(ip);
     end_op();
     return 0;
   }
   ```

   - **Create symlink** retrieves paths for the target and symlink, creates a `T_SYMLINK` inode, and writes the target path to it.
5. Add the interface for the system call in `usys.S`.

   ```c
   SYSCALL(symlink)
   ```
   - In order for a user program to call the system call, an interface needs to be added.
6. Add function in `user.h` that user program will be calling.

   ```c
   int symlink(const char*, const char*);
   ```

#### Modify `ln` to support symbolic links

- **Handling of symbolic links**: the program now checks if the `-s` flag is provided, indicating the creation of a symbolic link. If `-s` is passed, the `symlink` function is used instead of `link` to create the symlink.
- **Function pointer**: Introduced a function pointer `ln` that initially points to the `link` function. If the `-s` flag is detected, it points to the `symlink` function, allowing the program to dynamically choose which function to call based on the command-line argument.
- **Usage message**: The usage message was updated to reflect the new optional `-s` flag, which allows users to create symbolic links.
- **Error handling**: The program prints a failure message depending on whether the `link` or `symlink` operation failed, based on which function was invoked.

## Testing & Results

All the commands executed in the video:
```bash
make clean
make
make qemu-nox

ls
ln -h
ln README test.txt
ls
cat test.txt
```

Link to the video on Google Drive: [xv6 symlink example](https://drive.google.com/file/d/1dO-MnEpqJKvKdmdaSr_vyAdd6N9EpEHe/view?usp=share_link).

## Resources

- [Homework: Naming](https://pdos.csail.mit.edu/6.828/2010/homework/xv6-names.html)
