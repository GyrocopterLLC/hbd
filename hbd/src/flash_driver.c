/*
 * Flash chip (SST25VF032B) driver
 * 
 * Read / write / erase functions for the flash chip
 * Includes base level SPI commands
 *
 * David Miller
 * May 6, 2013
 *
 */
 

#include "stm32f0xx.h"
#include "flash_driver.h"
#include "gpio.h"


uint8_t junk;

/** 
 * SPI Control Functions
 *
 * spi_init - sets up gpio and spi module for transfers
 *
 * spi_txrx - sends or receives specified bytes on the spi module
 *
**/

void spi_init(void)
{
	uint8_t cmd[4];
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN);
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN);

	gpio_pinmode(FLASH_SPI_PORT, SPI_MOSI_PIN, GPIO_AlternateFunction);
	gpio_pinmode(FLASH_SPI_PORT, SPI_MISO_PIN, GPIO_AlternateFunction);
	gpio_pinmode(FLASH_SPI_PORT, SPI_CLK_PIN, GPIO_AlternateFunction);
	gpio_af(FLASH_SPI_PORT, SPI_MOSI_PIN, FLASH_AF);
	gpio_af(FLASH_SPI_PORT, SPI_MISO_PIN, FLASH_AF);
	gpio_af(FLASH_SPI_PORT, SPI_CLK_PIN, FLASH_AF);

	gpio_pinmode(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_Output);

	/** HAL Version
	GPIO_InitTypeDef initStruct;
	
	initStruct.Pin = SPI_MOSI_PIN | SPI_MISO_PIN | SPI_CLK_PIN;
	initStruct.Mode = GPIO_MODE_AF_PP;
	initStruct.Speed = GPIO_SPEED_HIGH;
	initStruct.Pull = GPIO_NOPULL;
	initStruct.Alternate = GPIO_AF0_SPI1;
	
	HAL_GPIO_Init(FLASH_SPI_PORT, &initStruct);
	
	initStruct.Pin = FLASH_CS_PIN;
	initStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(FLASH_CS_PORT, &initStruct);
	*/
	
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);

	SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;
	SPI1->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_FRXTH;
	SPI1->CR1 |= SPI_CR1_SPE;

/*
	Flash_Spi_Handle.Instance = SPI1;
	Flash_Spi_Handle.Init.Direction = SPI_DIRECTION_2LINES;
	Flash_Spi_Handle.Init.Mode = SPI_MODE_MASTER;
	Flash_Spi_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
	Flash_Spi_Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	Flash_Spi_Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	Flash_Spi_Handle.Init.NSS = SPI_NSS_SOFT;
	Flash_Spi_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	Flash_Spi_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	Flash_Spi_Handle.Init.CRCPolynomial = 1; // Not used anyway
	
	HAL_SPI_Init(&Flash_Spi_Handle);
*/
	//SPI_Cmd(SPI1, ENABLE);
        
        // Let's turn off those block protection bits
        flash_cs(ENABLE);
        cmd[0] = FLASH_EN_WRITE_STATUS_REG; // Write status register
        spi_txrx(cmd, 1, SPI_TX_DIR);
        flash_cs(DISABLE);
        //delay_cycles(100);
        flash_cs(ENABLE);
        cmd[0] = FLASH_WRITE_STATUS_REG;
        cmd[1] = 02; // BP2 and BP1 and BP0 turned off, write enable on.
        spi_txrx(cmd, 2, SPI_TX_DIR);
        flash_cs(DISABLE);
}

void spi_txrx(uint8_t* buff, uint32_t count, uint8_t dir)
{
  //SPI_Cmd(SPI1, ENABLE);
	uint8_t junk;
  // Use an 8-bit pointer for these data register accesses
	__IO uint8_t* spi_dr = (__IO uint8_t*)&SPI1->DR;
  if(dir == SPI_TX_DIR)
  {
	  //HAL_SPI_Transmit(&Flash_Spi_Handle, buff, count, count);

	  while(count > 0)
	  {
		  // *((__IO uint8_t*)&hspi->Instance->DR) = (*hspi->pTxBuffPtr++);
		  *spi_dr = *buff;
		  //SPI1->DR = *buff;
		  while(SPI1->SR & SPI_SR_BSY)
		  {// Sit and spin until done
		  }
		  // Toss out the received byte
		  junk = *spi_dr;
		  count--;
		  buff++;
	  }
    //while(count > 0)
    //{

      //SPI_SendData8(SPI1, *buff);
      // sit n spin
      //while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
      //count--;
      //buff++;
    //}
  }
  else
  {
	//  HAL_SPI_Receive(&Flash_Spi_Handle, buff, count, count);
    while(count > 0)
    {
    	*spi_dr = (uint8_t)0;
		while(SPI1->SR & SPI_SR_BSY)
		{// Sit and spin until done
		}
		*buff = *spi_dr;
		buff++;
		count--;
      //SPI_SendData8(SPI1, 0);
      //while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
      //*buff = SPI_ReceiveData8(SPI1);
      //count--;
      //buff++;
    }
  }
  //SPI_Cmd(SPI1, DISABLE);
  
  // Clear the buffer
  //SPI_ReceiveData8(SPI1);
  //SPI_ReceiveData8(SPI1);
  //SPI_ReceiveData8(SPI1);
  //SPI_ReceiveData8(SPI1);
} 

/* Controls the chip select pin */
void flash_cs(FunctionalState newState)
{
	if(newState == ENABLE)
	{
		FLASH_CS_PORT->BRR |= 1<<FLASH_CS_PIN;
	}
	else
	{
		FLASH_CS_PORT->BSRR |= 1<<FLASH_CS_PIN;
	}
}

/* Shortcut for write enable/disable 
 * (and maybe save codespace too? yay modularity!)
 */
void flash_writeEnDis(FunctionalState newState)
{
  uint8_t cmd;
  flash_cs(ENABLE);
  if(newState == ENABLE)
  {
    cmd = FLASH_WRITE_EN;
  }
  else
  {
    cmd = FLASH_WRITE_DIS;
  }
  spi_txrx(&cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
}
		
uint8_t flash_pollBusy(void)
{
	uint8_t cmdStatus = FLASH_READ_STATUS;
	flash_cs(ENABLE);
	spi_txrx(&cmdStatus, 1, SPI_TX_DIR);
	spi_txrx(&junk, 1, SPI_RX_DIR);
	flash_cs(DISABLE);
	
	if((junk & 0x01) != 0)
	{
		return 1;
	}
	return 0;
}
 
/* Read a specified number ("count") of bytes from the SPI Flash chip, starting
 * at address "addr"
 */
void flash_read(uint32_t addr, uint8_t* buff, uint32_t count)
{
	uint8_t cmd[4];
	// Pull chip select low
	flash_cs(ENABLE);
	// Transfer in the command and address
	cmd[0] = FLASH_BYTE_READ;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;
	spi_txrx(cmd, 4, SPI_TX_DIR);
	spi_txrx(buff, count, SPI_RX_DIR);
	flash_cs(DISABLE);
}

/* Write a single byte n to the SPI Flash chip, starting
 * at address "addr". 
 */
void flash_writeByte(uint32_t addr, uint8_t byteToWryte)
{
  uint8_t cmd[4];
  cmd[1] = (addr >> 16) & 0xFF;
  cmd[2] = (addr >> 8) & 0xFF;
  cmd[3] = addr & 0xFF;
  
  /*
  flash_cs(ENABLE);
  cmd[0] = FLASH_WRITE_EN;
  spi_txrx(cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  */
  flash_writeEnDis(ENABLE);
  cmd[0] = FLASH_BYTE_WRITE;
  flash_cs(ENABLE);
  spi_txrx(cmd, 4, SPI_TX_DIR);
  spi_txrx(&byteToWryte, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  
  // Poll the busy bit
  while(flash_pollBusy() != 0);
  
  // Disable writes
  /*
  cmd[0] = FLASH_WRITE_DIS;
  flash_cs(ENABLE);
  spi_txrx(cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  */
  flash_writeEnDis(DISABLE);
}

#ifdef ENABLE_FLASH_AAI
void flash_writeArray(uint32_t addr, uint8_t* buff, uint32_t count)
{
	if(count == 1)
	{
		flash_writeByte(addr, buff[0]);
		return;
	}
	uint8_t cmd[4];
	uint32_t bytes_transmitted = 0;
	cmd[1] = (addr & 0x00FF0000) >> 16;
	cmd[2] = (addr & 0x0000FF00) >> 8;
	cmd[3] = (addr & 0x000000FF);

	flash_writeEnDis(ENABLE);
	cmd[0] = FLASH_AAI_WRITE;
	flash_cs(ENABLE);
	spi_txrx(cmd,4,SPI_TX_DIR);
	spi_txrx(buff,2,SPI_TX_DIR);
	bytes_transmitted += 2;
	flash_cs(DISABLE);

	while(flash_pollBusy() != 0);

	while((count - bytes_transmitted) > 1)
	{
		flash_cs(ENABLE);
		spi_txrx(cmd,1,SPI_TX_DIR);
		spi_txrx(&(buff[bytes_transmitted]), 2, SPI_TX_DIR);
		bytes_transmitted += 2;
		flash_cs(DISABLE);
		while(flash_pollBusy() != 0);
	}

	flash_writeEnDis(DISABLE);

	if(count == 1)
	{
		flash_writeByte(addr + count - 1, buff[count-1]);
	}
}
#endif

/* Erases the flash chip.
 * Goes without saying: use with caution!
 * But I said it anyway.
 */

void flash_chipErase(void)
{
  uint8_t cmd;
  /*
  flash_cs(ENABLE);
  cmd = FLASH_WRITE_EN;
  spi_txrx(&cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  */
  flash_writeEnDis(ENABLE);
  flash_cs(ENABLE);
  cmd = FLASH_CHIP_ERASE;
  spi_txrx(&cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  
  while(flash_pollBusy() != 0); // wait for it to finish erasing ~50ms
  /*
  cmd = FLASH_WRITE_DIS;
  flash_cs(ENABLE);
  spi_txrx(&cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  */
  flash_writeEnDis(DISABLE);
}

/* Erases a sector starting at addr
 * Sector size can be:
 * ERASE_4KB
 * ERASE_32KB
 * ERASE_64KB
 * The function checks to make sure the address
 * is on a sector boundary.
 * For a 4k erase, the low 12 bits must be zero.
 * For 32k, the low 15, and for 64k the low 16 bits.
 * 
 * The function returns 0 if successful, or 1 if not (bad address)
 */

uint8_t flash_sectorErase(uint32_t addr, uint8_t blockSize)
{
  uint8_t cmd[4];

  
  switch(blockSize)
  {
  case ERASE_4KB:
    if((addr & 0x000FFF) != 0)
    {
      return 1;
    }
    cmd[0] = FLASH_4K_ERASE;
    break;
  case ERASE_32KB:
    if((addr & 0x007FFF) != 0)
    {
      return 1;
    }
    cmd[0] = FLASH_32K_ERASE;
    break;
  case ERASE_64KB:
    if((addr & 0x00FFFF) != 0)
    {
      return 1;
    }
    cmd[0] = FLASH_64K_ERASE;
    break;
  }
  
  /*
  flash_cs(ENABLE);
  cmd[0] = FLASH_WRITE_EN;
  spi_txrx(cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  */
  flash_writeEnDis(ENABLE);
  //cmd[0] = FLASH_SECT_ERASE;
  cmd[1] = (addr >> 16) & 0xFF;
  cmd[2] = (addr >> 8)  & 0xFF;
  cmd[3] = addr & 0xFF;
  flash_cs(ENABLE);
  spi_txrx(cmd, 4, SPI_TX_DIR);
  flash_cs(DISABLE);
  
  while(flash_pollBusy() != 0);
  /*
  cmd = FLASH_WRITE_DIS;
  flash_cs(ENABLE);
  spi_txrx(&cmd, 1, SPI_TX_DIR);
  flash_cs(DISABLE);
  */
  flash_writeEnDis(DISABLE);
  
  return 0;
}
