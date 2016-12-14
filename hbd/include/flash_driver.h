/* flash driver */

#ifndef _FLASH_DRIVER_H_
#define _FLASH_DRIVER_H_

#define FLASH_BYTE_WRITE	(0x02)
#define FLASH_BYTE_READ		(0x03)
#define FLASH_WRITE_EN		(0x06)
#define FLASH_WRITE_DIS		(0x04)
#define FLASH_READ_STATUS	(0x05)
#define FLASH_AAI_WRITE		(0xAD)
#define FLASH_EN_RDBY		(0x70)
#define FLASH_DIS_RDBY		(0x80)
#define FLASH_CHIP_ERASE        (0x60)
#define FLASH_4K_ERASE          (0x20)
#define FLASH_32K_ERASE         (0x52)
#define FLASH_64K_ERASE         (0xD8)
#define FLASH_WRITE_STATUS_REG		(0x01)
#define FLASH_EN_WRITE_STATUS_REG	(0x50)

#define SPI_TX_DIR		0x00
#define SPI_RX_DIR		0x01
#define ERASE_4KB               0x00
#define ERASE_32KB              0x01
#define ERASE_64KB              0x02

#define SPI_MOSI_PIN		5//GPIO_PIN_5
#define SPI_MISO_PIN		4//GPIO_PIN_4
#define SPI_CLK_PIN			3//GPIO_PIN_3
//#define SPI_MOSI_PINSRC		GPIO_PinSource5
//#define SPI_MISO_PINSRC		GPIO_PinSource4
//#define SPI_CLK_PINSRC		GPIO_PinSource3
#define FLASH_SPI_PORT		GPIOB
#define FLASH_CS_PIN		15//GPIO_PIN_15
#define FLASH_CS_PORT		GPIOA

//#define MISO_MODE_MASK		(0x03 << (SPI_MISO_PINSRC*2))
//#define MISO_MODE_GPI		(0x01 << (SPI_MISO_PINSRC*2))
//#define MISO_MODE_AF		(0x02 << (SPI_MISO_PINSRC*2))

void spi_init(void);
void spi_txrx(uint8_t* buff, uint32_t count, uint8_t dir);
void spi_ISR(void);
void flash_cs(FunctionalState newState);
uint8_t flash_pollBusy(void);
void flash_read(uint32_t addr, uint8_t* buff, uint32_t count);
void flash_writeByte(uint32_t addr, uint8_t byteToWrite);
void flash_chipErase(void);
uint8_t flash_sectorErase(uint32_t addr, uint8_t blockSize);


#define FLASH_MAX_ADDR		(0x003FFFFF) // 4M-1 = 4,194,303
#define FLASH_SECTOR_SIZE	(0x400) // 4k = 4,096 byte sectors
#define FLASH_MAX_SECTOR	(0xFFF) // 1k-1 = 1,023 (sectors go from zero to 1,023)



#endif //_FLASH_DRIVER_H_
