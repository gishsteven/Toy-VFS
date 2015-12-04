#include <linux/init.h>								// Standard kernel headers
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/sched.h>							//Task_struct
#include <asm/uaccess.h>
#include <asm/current.h>							//Process Info
#include <linux/proc_fs.h>							// For procfs communication Kernel-To-User
#include <linux/seq_file.h>
#include "toyfs.h"								// User defined includes

#define TOYFS_MAGIC	0x10040203

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kevin Gadek");
MODULE_AUTHOR("Steven Nam D. Le");
MODULE_AUTHOR("David Warren");
MODULE_AUTHOR("Nathan Sanders");
MODULE_DESCRIPTION("Toy File System");

/*=======================================================
File Operations (Non-Functional)
=========================================================
- file_operations (Data structure for file operations)
	- .readdr
- toyfs_read
	- 
=========================================================*/

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


/*=======================================================
Inode Operations
=========================================================
- toyfs_get_inode
	- Function should return an inode.
- inode_operations
	- .lookup
	- .create
=========================================================*/
struct dentry *toyfs_lookup(struct inode *parent, struct dentry *child, unsigned int flags)
{
	//In Progress
	return NULL;
}

static int toyfs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool boo)
{	
	//In Progress
	return 1;
}

static struct inode_operations toyfs_inode_ops = {				//Defined in fs.h:1541
	.lookup = toyfs_lookup,
	.create = toyfs_create,
};

struct inode *toyfs_get_inode(struct super_block *sb, const struct inode *directory, umode_t mode, dev_t dev)
{
	struct inode *inode = new_inode(sb);
	printk(KERN_INFO "Initializing: toyfs_get_inode\n");
	if(inode){
		inode->i_ino = get_next_ino();					//stores next available inode # into inode->i_ino
		inode_init_owner(inode, directory, mode);			//sets permissions etc
		inode->i_blocks = 10;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
   		switch(mode & S_IFMT) {						//bitwise AND between mode and S_IFMT can extract the file type code from a mode value. That new value should then be compared to the following cases.										
			case S_IFDIR:						//S_IFDIR = file type const of a directory file
                    		inode->i_op = &toyfs_inode_ops; 		//Inode operations
                    		//inode->i_fop = &toyfs_directory_operations; 	//simple directory operations
			     	inc_nlink(inode);
			     	break;
			case S_IFREG:						//S_IFREG = non-zero if file is a regular file
                   		 inode->i_op = &toyfs_inode_ops;
                   		 //inode->i_fop = &toyfs_file_operations;
										//S_ISLNK = non-zero if file is symbolic link (Non-Fuctional)
			default: printk(KERN_ERR "Not DIR or REG file.\n");
				return 0;
				break;
		} 
	} 
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
	
	sb->s_magic = TOYFS_MAGIC;						//identifer for the filesystem type
	inode = toyfs_get_inode(sb, NULL, S_IFDIR, 0);
	inode->i_op = &toyfs_inode_ops;						//inode operations table
	//inode->i_fop = &toyfs_directory_operations;				//file operations table
	sb->s_root = d_make_root(inode);					//creates root inode for superblock ; root directory

	if(!sb->s_root)
	{	
		printk(KERN_ERR "Root Creation has Failed\n");
		return -ENOMEM; 
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
- toyfs_proc_fops
	- toyfs_proc_show | toyfs_proc_open
		- /proc filesystem
======================================================*/

static int toyfs_proc_show(struct seq_file *m, void *v) {
  	seq_printf(m, "Hello from toyfs+proc!\n");
  	return 0;
}

static int toyfs_proc_open(struct inode *inode, struct  file *file) {
	printk(KERN_INFO "Initializing: toyfs_proc_open\n");
	printk(KERN_INFO "Test        :$cat /proc/toyfs_proc\n");
  	return single_open(file, toyfs_proc_show, NULL);
}

static struct dentry *toyfs_mount(struct file_system_type *filesystem_type, int flags, const char *device_name, void *data)
{
	struct dentry *det = mount_bdev(filesystem_type, flags, device_name, data, toyfs_fill_superblock);	

	if(IS_ERR(det))
		printk(KERN_ERR "Error Mounting Toy File System\n");
	else
		printk(KERN_INFO "Initializing: toyfs_mount \n");
	return det;
}

static void toyfs_kill_superblock(struct super_block *sb)
{
	printk(KERN_INFO "Initializing: toyfs_kill_superblock\n");
	kill_block_super(sb);
	return;
}

struct file_system_type toyfs = {
	.owner = THIS_MODULE,
	.name = "toyfs",
	.mount = toyfs_mount,
	.kill_sb = toyfs_kill_superblock,
};

static const struct file_operations toyfs_proc_fops = {
  .owner = THIS_MODULE,
  .open = toyfs_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

/*=======================================================
Register / Unregister
=========================================================
- toyfs_init
- toyfs_cleanup
- remove_proc_entry
	- Testing the procfs communication. 
========================================================*/

static int toyfs_init(void) {
	printk(KERN_INFO "||Registering File System  ||\n\n"); 
	printk(KERN_INFO "Userspace Process:'%s'\n", current->comm); 
	printk(KERN_INFO "Userspace PID:'%d'\n", current->pid); 

	proc_create("toyfs_proc", 0666, NULL, &toyfs_proc_fops);		// Create a procfs file for communication
	return register_filesystem(&toyfs);
}

static void toyfs_cleanup(void) {
 	printk(KERN_INFO "\n||Unregistering File System||\n");
	
 	remove_proc_entry("toyfs_proc", NULL);					// Remove the procfs file
 	unregister_filesystem(&toyfs);
}

module_init(toyfs_init);
module_exit(toyfs_cleanup);
