#include <linux/types.h>
const int SUPERBLOCK_DEFAULT_SIZE = 4096;

struct toyfs_inode {
    mode_t mode;
    uint32_t inode_num, block_num;
    uint64_t child_count, file_size;
};

//uint32_t better data type to use than unsigned int at this point
struct toyfs_super_block {
	int id;
	int block_size;
	int num_free_blocks;
	//default superblock size on disk = 4096 bytes
	/*char padding[(SUPERBLOCK_DEFAULT_SIZE) - (4 * sizeof(unsigned int))];
*/
};
