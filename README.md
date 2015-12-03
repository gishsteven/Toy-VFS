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
8. :white_check_mark: A process in user-space that stores your file and process all file system requests forwarded by the kernel module.

##Resources
#### Documents
- [Linux Cross Reference](http://lxr.free-electrons.com/source/?v=3.19)
- [Linux Kernel Module Programming Guide](http://linux.die.net/lkmpg/x769.html)
- [Brouwer, The Linux Kernel](https://www.win.tue.nl/~aeb/linux/lk/lk-8.html)

#### Readings/Tutorials
- [Writing A File System in Linux Kernel](http://kukuruku.co/hub/nix/writing-a-file-system-in-linux-kernel)
- [rkfs, Writing a Simple Filesystem](http://www2.comp.ufscar.br/~helio/fs/rkfs.html)
- [Kernel 3.19 Filesystem Documentation](http://lxr.free-electrons.com/source/Documentation/filesystems/vfs.txt?v=3.19)

#### Examples/References
- [simplefs](https://github.com/psankar/simplefs)
- [lab5fs](https://github.com/souravzzz/lab5fs)


##Compiling Instructions
 |Commands | Notes
 ---|---|---
1.  |$ make                                       |[Run from local file]
2.  |$ sudo insmod toyfs_mod.ko                   |[Insert Module]   
3.  |$ dmesg \| tail -5                            |[Check for printk commands 'Successful']
4.  |$ cat /proc/filesystems \| grep toyfs         |[Check if 'toyfs' is registered        ]
5.  |$ touch image                                |[Create an empty 'Disk Image']
6.  |$ mkdir dir                                  |[Create the Mount Folder in Directory]        
7.  |$ sudo mount -o loop -t toyfs ./image ./dir  |[Assemble File System]
8.  |$ dmesg \| tail -10                           |[Change tail as necessary]
9.  |$ modinfo toyfs_mod.ko                       |[View module information]
10. |$ sudo umount ./dir                          |[Unmount File System]
11. |$ sudo rmmod toyfs_mod                       |[Remove Module]
12. |$ dmesg                                      |[Check if Unregistered and Unounted]

