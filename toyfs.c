#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/sched.h>	//Task_struct
#include <asm/uaccess.h>
#include <asm/current.h>	//Process Info

#include "toyfs.h"

#define TOYFS_MAGIC	0x10040203

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kevin Gadek");
MODULE_AUTHOR("Steven Nam D. Le");
MODULE_AUTHOR("David Warren");
MODULE_AUTHOR("Nathan Sanders");
MODULE_DESCRIPTION("Toy File System");


/*=======================================================
To-Do List
=========================================================
- Implement Functionality
	- Read | Write | Create
=========================================================*/

//file struct defined line 912 of fs.h
//*********************************FILE AND INODE OPERATIONS*************************
//function to implement ls command and list contents of directory

/*
Doesn't work. 

static int toyfs_read(struct file *file, char *buf, int len, int *offset)
{
    char *buffer[100];
    int length = 3;
    buffer[0] = (char)file->private_data;
    buffer[1] = '\n';
    buffer[2] = '\0';
    if(*offset > length)
        return 0;
        if(len > length - *offset)
            len = length - *offset;
    //int copy_to_user(void *dst, const void *src, unsigned int size);
    if(copy_to_user(buf, buffer + *offset, len))
            return -1;
    *offset += len;
    return len;

}
*/

//data structure for file_operations
/*
const struct file_operations toyfs_directory_operations = {
	.owner = THIS_MODULE,
	//.readdir = toyfs_read_directory,
};
*/
/*
const struct file_operations toyfs_file_operations = {
    .read = toyfs_read,
};
*/

struct dentry *toyfs_lookup(struct inode *parent, struct dentry *child, unsigned int flags)
{
	return NULL;
}
//inode_operations data struct defined in fs.h
static struct inode_operations toyfs_inode_ops = {
	.lookup = toyfs_lookup,
};

//*********************************END FILE/INODE OPERATIONS*************************


//a function that returns an inode
// inode struct defined in linux/fs.h
//umode_t and dev_t as defined in sys/types.h; typedef unsigned short umode_t and typedef __kernel_dev_t
struct inode *toyfs_get_inode(struct super_block *sb, const struct inode *directory, umode_t mode, dev_t dev)
{
	struct inode *inode = new_inode(sb);

	if(inode){
		//stores next available inode # into inode->i_ino
		inode->i_ino = get_next_ino();
		//sets permissions etc
		inode_init_owner(inode, directory, mode);
		inode->i_blocks = 10;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
		//bitwise AND between mode and S_IFMT can extract the file type code from a mode value. That new value should then be compared to the following cases.
   		switch(mode & S_IFMT) {
			//S_IFDIR = file type const of a directory file
			case S_IFDIR:
				//inc_nlink() defined in linux/fs/inode.c; checks if # of links that the inode contains is 0. If not, # of links is incremented by 1.
                    inode->i_op = &toyfs_inode_ops; //use inode operations
                    //inode->i_fop = &toyfs_directory_operations; //simple directory operations
			     	inc_nlink(inode);
			     	break;
			//S_IFREG = non-zero if file is a regular file
			case S_IFREG:
                    inode->i_op = &toyfs_inode_ops;
                    //inode->i_fop = &toyfs_file_operations;
			//S_ISLNK = non-zero if file is symbolic link
			default: printk(KERN_ERR "Not dir or reg file.\n");
				return 0;
				break;
		} //end switch

	} // end if

	//returns inode object
	return inode;
}




/*=======================================================
Super Block
=========================================================
	- toy_fs_fill_superblock
	- Edit: Add Inode Allocation
=========================================================*/

static int toyfs_fill_superblock(struct super_block *sb, void *data, int silent)
{
	struct inode *inode = NULL;
	printk(KERN_INFO "Initializing: toyfs_fill_superblock\n");
	

	//identifer for the filesystem type
	sb->s_magic = TOYFS_MAGIC;
	inode = toyfs_get_inode(sb, NULL, S_IFDIR, 0);
	//inode operations table
	inode->i_op = &toyfs_inode_ops;
	//file operations table
	//inode->i_fop = &toyfs_directory_operations;
	//creates root inode for superblock ; root directory
	sb->s_root = d_make_root(inode);
	//error checking
	if(!sb->s_root)
	{	
		printk(KERN_ERR "Root Creation has Failed\n");
		return -ENOMEM; //not enough memory
	}
	return 0;
}

/*=====================================================
File System Types
=======================================================
- dentry *toyfs_mount
	- Defined in linux/fs/super.c
	- mount_bdev() uses disk instead of prog memory
- toyfs_kill_superblock
- file_system_type
	- Definitions
======================================================*/
//this implementation similar to ramfs with mount_nodev() design
static struct dentry *toyfs_mount(struct file_system_type *filesystem_type, int flags, const char *device_name, void *data)
{
	struct dentry *det = mount_bdev(filesystem_type, flags, device_name, data, 				toyfs_fill_superblock);	
	printk(KERN_INFO "Initializing: toyfs_mount \n");

	
	if(IS_ERR(det))
		printk(KERN_ERR "Error Mounting Toy File System\n");
	else
		printk(KERN_INFO "File System Sucessfully Mounted!");

	return det;
}

static void toyfs_kill_superblock(struct super_block *sb)
{
	printk(KERN_INFO "Intializing: toyfs_kill_superblock\n");
	kill_block_super(sb);
	return;

}

struct file_system_type toyfs = {
	.owner = THIS_MODULE,
	.name = "toyfs",
	.mount = toyfs_mount,
	.kill_sb = toyfs_kill_superblock,
};

/*=======================================================
Register / Unregister
=========================================================
- toyfs_init
- toyfs_cleanup
========================================================*/

static int toyfs_init(void) {
	printk(KERN_INFO "Registering File System | Loading Module\n"); 
	printk(KERN_INFO "Userspace Process:'%s'\n", current->comm); 
	printk(KERN_INFO "Userspace PID:'%d'\n", current->pid); 
	return register_filesystem(&toyfs);
}

static void toyfs_cleanup(void) {
  printk(KERN_INFO "Unregistering File System | Unloading Module\n");
  unregister_filesystem(&toyfs);
}

module_init(toyfs_init);
module_exit(toyfs_cleanup);
