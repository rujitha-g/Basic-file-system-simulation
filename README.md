# Basic File System Operations Simulation

A simple Operating System project that simulates basic file system operations using C.

## Features

- Create Directories
- Create Files
- Write Data to Files
- Read File Contents
- List Directory Contents
- Delete Files
- Delete Directories
- Rename / Move Files and Directories
- Hierarchical Directory Structure

## Technologies Used

- C Programming
- GCC Compiler
- Linked Lists
- Tree Data Structure

## Project Structure

```
basic-file-system-simulation/
│
├── src/
│   └── filesystem.c
│
├── README.md
└── LICENSE
```

## Compilation

```bash
gcc src/filesystem.c -o filesystem
```

## Run

```bash
./filesystem
```

## Sample Commands

### Create Directory

```bash
mkdir /docs
```

### Create File

```bash
touch /docs/file1.txt
```

### Write Content

```bash
write /docs/file1.txt Hello World
```

### Read Content

```bash
read /docs/file1.txt
```

### List Directory

```bash
ls /docs
```

### Rename

```bash
rename /docs/file1.txt /docs/file2.txt
```

### Delete File

```bash
rm /docs/file2.txt
```

### Delete Directory

```bash
rmdir /docs
```

### Exit

```bash
exit
```

## Future Enhancements

- File Permissions
- Timestamps
- Persistent Storage
- User Authentication
- Disk Block Allocation

## Author

Rujitha G
