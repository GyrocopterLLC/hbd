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

#include "stm32f0xx_hal.h"
#include "flash_driver.h"
#include "sfs.h"
#include <string.h>


void _sfs_flash_write_long_int(uint32_t addr, uint32_t data)
{
	uint8_t* dataptr = (uint8_t*)(&data);
	for(uint8_t i = 0; i < 4; i++)
	{
		flash_writeByte(addr+i,dataptr[i]);
	}

}

uint32_t _sfs_flash_read_long_int(uint32_t addr)
{
	uint32_t retval;
	uint8_t* dataptr = (uint8_t*)(&retval);
	flash_read(addr,dataptr,4);
	return retval;
}

void _sfs_write_index(uint32_t ipage, uint8_t fileID, const uint8_t* fileName, uint32_t startPage, uint32_t dataLength)
{
	uint8_t i;
	// Write the index flag
	flash_writeByte(ipage*SFS_IPAGE_SIZE + SFS_IPAGE_FLAG_OFFSET, SFS_FLAG_IPAGE);
	// Write the file ID
	flash_writeByte(ipage*SFS_IPAGE_SIZE + SFS_IPAGE_FILEID_OFFSET, fileID);
	// Write the file name
	for(i = 0; i < SFS_MAX_FN; i++)
	{
		flash_writeByte(ipage*SFS_IPAGE_SIZE+i+SFS_IPAGE_FILENAME_OFFSET,fileName[i]);
		if(fileName[i] == 0) break;
	}
	// Write the starting data page
	_sfs_flash_write_long_int(ipage*SFS_IPAGE_SIZE+SFS_IPAGE_STARTPAGE_OFFSET,startPage);
	// Write the data length
	_sfs_flash_write_long_int(ipage*SFS_IPAGE_SIZE+SFS_IPAGE_DATALENGTH_OFFSET,dataLength);

	//flash_writeByte(ipage*SFS_IPAGE_SIZE+18,startPage&0xFF);
	//flash_writeByte(ipage*SFS_IPAGE_SIZE+19,(startPage>>8)&0xFF);
	//flash_writeByte(ipage*SFS_IPAGE_SIZE+20,(startPage>>16)&0xFF);
	//flash_writeByte(ipage*SFS_IPAGE_SIZE+21,(startPage>>24)&0xFF);

	//flash_writeByte(ipage*SFS_IPAGE_SIZE+22,dataLength&0xFF);
	//flash_writeByte(ipage*SFS_IPAGE_SIZE+23,(dataLength>>8)&0xFF);
	//flash_writeByte(ipage*SFS_IPAGE_SIZE+24,(dataLength>>16)&0xFF);
	//flash_writeByte(ipage*SFS_IPAGE_SIZE+25,(dataLength>>24)&0xFF);
}

uint8_t _sfs_find_file(const uint8_t* fileName)
{
	uint8_t read_buf[SFS_MAX_FN];
	// Search all the filenames until we find the one we're looking for
	for(uint32_t i = 0; i < SFS_NUM_IPAGES; i++)
	{
		flash_read(i*SFS_IPAGE_SIZE + SFS_IPAGE_FLAG_OFFSET,read_buf,1);
		if(read_buf[0] == SFS_FLAG_IPAGE)
		{
			// Now check the filename
			flash_read(i*SFS_IPAGE_SIZE + SFS_IPAGE_FILENAME_OFFSET,read_buf,SFS_MAX_FN);
			// Compare strings
			if(strncmp((const char*)fileName,(const char*)read_buf,SFS_MAX_FN) == 0)
			{
				// This is a match!
				flash_read(i*SFS_IPAGE_SIZE + SFS_IPAGE_FILEID_OFFSET, read_buf, 1);
				return read_buf[0];
			}
		}
		else if(read_buf[0] == SFS_FLAG_UNPROGRAMMED)
		{
			break;
		}
	}
	return 0; // Zero = not found, no file!
}

void sfs_format(void)
{
	flash_chipErase();
}

void sfs_write(const uint8_t* fileName, const uint8_t* fileData, uint32_t size)
{
	uint8_t read_buf[4];
	uint16_t ipage = SFS_NUM_IPAGES;
	uint32_t dpage = 0;
	uint32_t temp;
	uint32_t num_dpages = (size / (SFS_DPAGE_SIZE-SFS_DPAGE_DATA_OFFSET)) + 1;
	// Fill an index page
	// Get the first free index page
	for(uint16_t i = 0; i < SFS_NUM_IPAGES; i++)
	{
		flash_read(i*SFS_IPAGE_SIZE,read_buf,1);
		if(read_buf[0] == SFS_FLAG_UNPROGRAMMED)
		{
			ipage = i;
			break;
		}
	}
	if(ipage >= SFS_NUM_IPAGES) return;

	// Find the first free data page
	if(ipage == 0) dpage = 0;
	else
	{
		dpage = _sfs_flash_read_long_int((ipage-1)*SFS_IPAGE_SIZE+SFS_IPAGE_STARTPAGE_OFFSET);
		temp = _sfs_flash_read_long_int((ipage-1)*SFS_IPAGE_SIZE+SFS_IPAGE_DATALENGTH_OFFSET);
		dpage += (temp / (SFS_DPAGE_SIZE - SFS_DPAGE_DATA_OFFSET)) + 1;

//		flash_read((ipage-1)*SFS_IPAGE_SIZE+SFS_IPAGE_STARTPAGE_OFFSET,read_buf,4);
//		dpage = read_buf[0]
//				+ (((uint32_t) read_buf[1]) << 8)
//				+ (((uint32_t) read_buf[2]) << 16)
//				+ (((uint32_t) read_buf[3]) << 32);
//		flash_read((ipage-1)*SFS_IPAGE_SIZE+SFS_IPAGE_DATALENGTH_OFFSET,read_buf,4);
//		temp = read_buf[0]
//				+ (((uint32_t) read_buf[1]) << 8)
//				+ (((uint32_t) read_buf[2]) << 16)
//				+ (((uint32_t) read_buf[3]) << 32);
		// How many pages to offset?
//		dpage += (temp / (SFS_DPAGE_SIZE-2)) + 1;
	}
	// Too many data pages?
	if(dpage + num_dpages > SFS_NUM_DPAGES) return;

	// Ready to write the index entry
	_sfs_write_index(ipage,ipage+1,fileName,dpage,size);

	// Start writing data

	uint32_t dpage_offset = 0;
	for(uint32_t i = 0; i < size; i++)
	{
		if(dpage_offset == 0)
		{
			flash_writeByte(SFS_FIRST_DPAGE + (dpage*SFS_DPAGE_SIZE) + SFS_DPAGE_FLAG_OFFSET, SFS_FLAG_DPAGE); // datapage
			flash_writeByte(SFS_FIRST_DPAGE + (dpage*SFS_DPAGE_SIZE) + SFS_DPAGE_FILEID_OFFSET, ipage+1); // fileid
			dpage_offset = SFS_DPAGE_DATA_OFFSET;
		}
		flash_writeByte(SFS_FIRST_DPAGE + dpage*SFS_DPAGE_SIZE + dpage_offset,
				fileData[i]);
		dpage_offset++;
		if(dpage_offset >= SFS_DPAGE_SIZE)
		{
			dpage++;
			dpage_offset = 0;
		}
	}
}

void sfs_read(const uint8_t* fileName, uint8_t* fileData, uint32_t startOffset, uint32_t size)
{
	// Find the file
	uint8_t fileid = _sfs_find_file(fileName);
	if(fileid == 0) return;

	// Check the size
	uint32_t dataSize, startPage, dpage_offset, current_byte;
	startPage = _sfs_flash_read_long_int(SFS_IPAGE_SIZE*(fileid-1) + SFS_IPAGE_STARTPAGE_OFFSET);
	dataSize = _sfs_flash_read_long_int(SFS_IPAGE_SIZE*(fileid-1) + SFS_IPAGE_DATALENGTH_OFFSET);

	// No reading out of bounds
	if(startOffset >= dataSize) return;
	// Also, clip the data to the maximum available
	if((startOffset + size) > dataSize)
	{
		size = dataSize - startOffset;
	}

	// Start grabbing data
	// Calculate the location in the page
	dpage_offset = startOffset;
	while(dpage_offset >= (SFS_DPAGE_SIZE - SFS_DPAGE_DATA_OFFSET))
	{
		dpage_offset -= (SFS_DPAGE_SIZE - SFS_DPAGE_DATA_OFFSET);
		startPage++;
	}
	current_byte = 0;
	for(;;)
	{
		// If more than a page remaining
		if((size - current_byte) > (SFS_DPAGE_SIZE - SFS_DPAGE_DATA_OFFSET - dpage_offset))
		{
			// Read until the end of the page
			flash_read((startPage*SFS_DPAGE_SIZE) + SFS_FIRST_DPAGE + SFS_DPAGE_DATA_OFFSET + dpage_offset,
					&(fileData[current_byte]),
					(SFS_DPAGE_SIZE - SFS_DPAGE_DATA_OFFSET - dpage_offset));
			current_byte += (SFS_DPAGE_SIZE - SFS_DPAGE_DATA_OFFSET - dpage_offset);
			startPage++;
			dpage_offset = 0;
		}
		else
		{
			// Read the rest of the data
			flash_read((startPage*SFS_DPAGE_SIZE) + SFS_FIRST_DPAGE + SFS_DPAGE_DATA_OFFSET + dpage_offset,
					&(fileData[current_byte]),
					(size - current_byte));
			current_byte += (size - current_byte);
			// And now we're finished reading
			return;
		}
	}
}


uint32_t sfs_filesize(const uint8_t* fileName)
{
	// Find the file
	uint8_t fileid = _sfs_find_file(fileName);
	if(fileid == 0) return 0;

	return _sfs_flash_read_long_int(SFS_IPAGE_SIZE*(fileid-1) + SFS_IPAGE_DATALENGTH_OFFSET);
}

uint8_t sfs_filename(uint8_t fileID, uint8_t* fileName)
{
	uint8_t tempID;
	// See if a file exists with this ID
	for(uint32_t ipage = 0; ipage < SFS_NUM_IPAGES; ipage++)
	{
		flash_read(ipage*SFS_IPAGE_SIZE + SFS_IPAGE_FILEID_OFFSET, &tempID, 1);
		if(tempID == fileID)
		{
			// Match! Get the file name.
			flash_read(ipage*SFS_IPAGE_SIZE + SFS_IPAGE_FILENAME_OFFSET,fileName,SFS_MAX_FN);
			return 1;
		}
	}
	return 0;
}
