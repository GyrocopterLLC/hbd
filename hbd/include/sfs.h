/*
 * Shitty File System
 *
 * This "file system" has very limited functionality. It only allows for files to be
 * written once, and never deleted. This is useful for a storing large data outside of
 * a microcontroller's flash memory (for example, in an external SPI flash chip).
 *
 * There are no directories. Files are located by a short filename only.
 *
 * SFS allows for files to be written once, and never modified. The size and number of
 * data pages occupied by a file cannot be changed after the first write. This means the file
 * must be written all at once.
 *
 * When reading, SFS searches for a file by name. There is no read pointer, you must
 * supply the offset (in bytes) into the file for each read. You can read as many bytes
 * as you'd like for each call of the sfs_read function.
 *
 *  Created on: Apr 27, 2016
 *      Author: David Miller
 */

#ifndef _SFS_H
#define _SFS_H

#define SFS_IPAGE_SIZE		(64)
#define SFS_NUM_IPAGES		(64) // The first block (4kB) is all index pages
#define SFS_FIRST_DPAGE		(4096) // Start of block 2
#define SFS_DPAGE_SIZE		(256)
#define SFS_FLASH_SIZE		(1024*1024*4)
#define SFS_NUM_DPAGES		(1023*16) // (4MB - 4kB) / 256 = 16k - 16

#define SFS_MAX_FN			16

#define SFS_FLAG_IPAGE			1
#define SFS_FLAG_DPAGE			2
#define SFS_FLAG_ERASED			0
#define SFS_FLAG_UNPROGRAMMED	0xFF

typedef struct FileIndex_S
{
	uint8_t flag;
	uint8_t fileID;
	uint8_t fileName[SFS_MAX_FN];
	uint32_t startPage;
	uint32_t dataLength;
} FileIndex_Type;

#define SFS_IPAGE_FLAG_OFFSET			0
#define SFS_IPAGE_FILEID_OFFSET			1
#define SFS_IPAGE_FILENAME_OFFSET		2
#define SFS_IPAGE_STARTPAGE_OFFSET		18
#define SFS_IPAGE_DATALENGTH_OFFSET		22

#define SFS_DPAGE_FLAG_OFFSET			0
#define SFS_DPAGE_FILEID_OFFSET			1
#define SFS_DPAGE_DATA_OFFSET			2

void sfs_format(void);
void sfs_write(const uint8_t* fileName, const uint8_t* fileData, uint32_t size);
void sfs_read(const uint8_t* fileName, uint8_t* fileData, uint32_t startOffset, uint32_t size);
uint32_t sfs_filesize(const uint8_t* fileName);
uint8_t sfs_filename(uint8_t fileID, uint8_t* fileName);

#endif //_SFS_H
