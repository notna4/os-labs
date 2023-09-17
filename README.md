# File System Utility

This is a C program that provides a command-line utility for inspecting and manipulating file system objects such as files, directories, and symbolic links. The program accepts various options and performs different actions based on the provided input. It uses system calls and C library functions to retrieve information about file system objects.

The program is designed to work with three types of file system objects:

1. **Symbolic Links**: It can display information about symbolic links, including their name, size, target size, and access rights. It can also delete symbolic links.

2. **Regular Files**: For regular files, it can display their name, size, number of hard links, last modification time, and access rights. Additionally, it can create symbolic links to these files.

3. **Directories**: When working with directories, it can display their name, size, access rights, and count the number of files with the `.c` extension within the directory. It can also create text files within directories.

The program accepts command-line arguments to specify the file system object to inspect or manipulate. Here's the basic usage:

```shell
./programus.c [options] [file_path]
```
The program supports various options, each represented by a single letter:

1. -n: Display the name of the file system object.
2. -d: Display the size of the file system object.
3. -h: Display the count of hard links (for regular files).
4. -m: Display the last time of modification (for regular files).
5. -a: Display access rights for the file system object.
6. -l: Create a symbolic link to the file system object (waits for input).
