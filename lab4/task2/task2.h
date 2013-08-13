#define O_DIRECTORY 00200000

struct linux_dirent {
	unsigned long  d_ino;                 /* inode number */
	unsigned long  d_off;                /* offset to next dirent */
	unsigned short d_reclen;    /* length of this dirent */
	char d_name[];   /* filename (null-terminated) */
};


struct stat {
	unsigned long  st_dev;
	unsigned long  st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned long  st_rdev;
	unsigned long  st_size;
	unsigned long  st_blksize;
	unsigned long  st_blocks;
	unsigned long  st_atime;
	unsigned long  st_atime_nsec;
	unsigned long  st_mtime;
	unsigned long  st_mtime_nsec;
	unsigned long  st_ctime;
	unsigned long  st_ctime_nsec;
	unsigned long  __unused4;
	unsigned long  __unused5;
};
