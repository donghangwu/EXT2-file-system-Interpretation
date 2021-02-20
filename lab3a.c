// Name: Donghang Wu Tristan Que
// ID: 605346965
// Email: dwu20@g.ucla.edu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "ext2_fs.h"

#define SUPPER_BLOCK_OFFSET 1024

//file descriptor
int image_fd;

//used in superblock()
//from ext_fs.h, 
//super block struct
struct ext2_super_block super;
unsigned int inodes_count = 0, blocks_count = 0, log_block_size = 0;
unsigned int blocks_per_group=0,inodes_per_group=0,inode_size=0, first_ino=0;
//block size
unsigned int block_size;


//used in group_info()
// group descriptor
struct ext2_group_desc group;
unsigned int block_bitmap=0,inode_bitmap=0,inode_table=0;


//used in directory_entries()
//directory entries
struct ext2_dir_entry dir;




//print super block info
void superblock()
{
    pread(image_fd,&super,sizeof(super),SUPPER_BLOCK_OFFSET);
    block_size=1024 << super.s_log_block_size;
    inodes_count = super.s_inodes_count;
    blocks_count = super.s_blocks_count;
    blocks_per_group=super.s_blocks_per_group;
    inodes_per_group=super.s_inodes_per_group;
    inode_size=super.s_inode_size;
    first_ino=super.s_first_ino;
    printf("SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n",
            blocks_count, inodes_count, block_size, inode_size,
            blocks_per_group, inodes_per_group, first_ino);

}

//print group summary
void group_info()
{
    int start_block=0;
    //1024 
    if(block_size==1024)
        start_block=2;
    else//or >2048
        start_block=1;

    //assume only 1 group
    pread(image_fd,&group,sizeof(group),(start_block*block_size));

    block_bitmap=group.bg_block_bitmap;//starting block# for free block entries
    inode_bitmap=group.bg_inode_bitmap;//starting block# for free node entries
    inode_table=group.bg_inode_table;//starting block# for first block of i-nodes
    int group_num=0;//always 0 since we only have 1 single group
    printf("GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n",
            group_num, blocks_count, inodes_per_group,
            group.bg_free_blocks_count, group.bg_free_inodes_count,
            block_bitmap, inode_bitmap, inode_table);
    
}

//print free block entries
void free_block_entries()
{
    char *blocks=malloc(block_size);

    pread(image_fd, blocks, block_size, block_size*block_bitmap);

    for(unsigned int i=0;i<block_size/8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if((blocks[i] & (1 << j))==0)
            {
                printf("BFREE,%d\n",i*8+j+1);//from TA's slides
            }
        }
    }

    free(blocks);

}

void free_Inode_entry()
{
    char *inodes=malloc(block_size);

    pread(image_fd, inodes, block_size, block_size*inode_bitmap);

    for(unsigned int i=0;i<block_size/8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if((inodes[i] & (1 << j))==0)
            {
                printf("IFREE,%d\n",i*8+j+1);//from TA's slides
            }
        }
    }

    free(blocks);
}

void Inode_summary()
{

}

void directory_entries()
{
    

}

void indirect_block()
{

}




int main(int argc, char** argv)
{
    if(argc!=2)
    {
        fprintf(stderr,"Wrong arugments\n");
        exit(1);
    }

    char *image=argv[1];
    image_fd=open(image,O_RDONLY);
    if(image_fd<0)
    {
        fprintf(stderr,"cannot open the given image %s",image);
        exit(1);
    }

    //read superblock
    superblock();

    //group descriptor block summary, 
    //assign block bitmap, inode bitmap, inode table addresses
    group_info();


    free_block_entries();

    free_Inode_entry();

    exit(0);
}