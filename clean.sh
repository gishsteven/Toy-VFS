#!/bin/bash
FS_TYPE="toyfs"
MODULE_FILE="toyfs_mod.ko"
MODULE_NAME="toyfs_mod"
DISK_FILEPATH="/tmp/toyfs_disk.img"
MOUNT_DIR="/mnt/$FS_TYPE"

# Initialization must be run as root user
if [[ $EUID -ne 0 ]]; then
   echo "Usage: sudo ./clean.sh" 1>&2
   exit 1
fi

# Unmount the disk
umount $DISK_FILEPATH

# Unload kernel module
rmmod $MODULE_NAME

# Clean directory
make clean

# Remove Disk
rm $DISK_FILEPATH

# Display dmesg logs

dmesg | tail -50