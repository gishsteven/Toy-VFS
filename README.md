# Toy-VFS
A Linux Toy File System involving the Linux Kernel Operating System.


## Overview
1. A kernel module interfaces with the VFS or Linux and redirects all calls to your toy file system which must be implemented in User space.
2. Your toy file system implements a simple, flat file system that can create files and keep its attributes in a root directory.
3. Your file system can be mounted like any other file system in Linux.
4. You must be able to read/write to your files. And files must reflect such operations.
5. You must maintain basic metadata for your files.
6. A linux kernel module that captures file system requests addressed to your file system and forwards them to user-space program.
7. A communication mechanism to pass data from kernel to user and viceversa (i.e. proc file system, device under dev/ directory, netlink sockets--your choice of mechanism)
8. A process in user-space that stores your file and process all file system requests forwarded by the kernel module.

##Resources
1. [Writing a Simple File System](http://www2.comp.ufscar.br/~helio/fs/rkfs.html)
2. [Sample file system implementation] (https://github.com/psankar/simplefs)
3. [Another sample file system implementation] (https://github.com/souravzzz/lab5fs)

##Instructions (so far)
1. Run make from working directory
2. run 'insmod toyfs_mod.ko' as root user
3. run 'touch image' to create empty disk image
4. run 'mkdir mount' to create mount folder in directory
5. run 'mount -o loop -t toyfs ./image ./mount' to assemble file system
6. run 'umount ./mount' to disassemble file system
7. run 'rmmod toyfs_mod' to unload module 

