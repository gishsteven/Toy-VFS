# Toy-VFS
A Linux Toy File System involving the Linux Kernel Operating System.


## Overview
1. :white_check_mark: A kernel module interfaces with the VFS or Linux and redirects all calls to your toy file system which must be implemented in User space.
2. Your toy file system implements a simple, flat file system that can create files and keep its attributes in a root directory.
3. :white_check_mark: Your file system can be mounted like any other file system in Linux.
4. You must be able to read/write to your files. And files must reflect such operations.
5. You must maintain basic metadata for your files.
6. A linux kernel module that captures file system requests addressed to your file system and forwards them to user-space program.
7. A communication mechanism to pass data from kernel to user and viceversa (i.e. proc file system, device under dev/ directory, netlink sockets--your choice of mechanism)
8. A process in user-space that stores your file and process all file system requests forwarded by the kernel module.

##Resources
1. [Writing a Simple File System](http://www2.comp.ufscar.br/~helio/fs/rkfs.html)
2. [Sample file system implementation] (https://github.com/psankar/simplefs)
3. [Another sample file system implementation] (https://github.com/souravzzz/lab5fs)
4. [Instructions to mount] (http://kukuruku.co/hub/nix/writing-a-file-system-in-linux-kernel)

##Instructions (so far)
1. Run make from working directory
2. run 'insmod toyfs_mod.ko' as root user
3. run 'cat /proc/filesystems | tail -1' to see if filesystem is successfully registered 
4. run 'touch image' to create empty disk image
5. run 'mkdir dir' to create mount folder in directory
6. run 'mount -o loop -t toyfs ./image ./dir' to assemble file system
7. run 'dmesg | tail -10' to view printk messages. Change tail as necessary
7. run 'umount ./dir' to disassemble file system
8. run 'rmmod toyfs_mod' to unload module 
9. run 'modinfo toyfs_mod.ko' to view module info
