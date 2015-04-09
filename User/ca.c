	{

			SPI_FLASH_SectorErase(0x000000);
			SPI_FLASH_BufferWrite(&high_score, 0x000000,1);
		

			SPI_FLASH_SectorErase(0x005000);
			SPI_FLASH_BufferWrite(&high_score, 0x005000,1);
		

			SPI_FLASH_SectorErase(0x006000);
			SPI_FLASH_BufferWrite(&high_score, 0x006000,1);
		

			SPI_FLASH_SectorErase(0x007000);
			SPI_FLASH_BufferWrite(&high_score, 0x007000,1);
		

			SPI_FLASH_SectorErase(0x008000);
			SPI_FLASH_BufferWrite(&high_score, 0x008000,1);
		

			SPI_FLASH_SectorErase(0x009000);
			SPI_FLASH_BufferWrite(&high_score, 0x009000,1);
		
	}