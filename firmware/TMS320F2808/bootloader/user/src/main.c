
#include "main.h"
#include "BootLoader.h"
#include "LED.h"

#define BOOT_FLAG_OF_EEPROM 0   //启动运行标志存储在EEPROM中

#define SPI_CS(x)  {if(x==0){GpioDataRegs.GPACLEAR.all=1L<<23;}else{GpioDataRegs.GPASET.all=1L<<23;}}
#define SPI_MOSI(x) {if(x==0){GpioDataRegs.GPACLEAR.all=1L<<20;}else{GpioDataRegs.GPASET.all=1L<<20;}}
#define SPI_CLK(x) {if(x==0){GpioDataRegs.GPACLEAR.all=1L<<22;}else{GpioDataRegs.GPASET.all=1L<<22;}}
#define SPI_MISO() GpioDataRegs.GPADAT.bit.GPIO21

#if 1
void spi_init()
{
#if 0
    SpicRegs.SPICCR.all =0x0007;                 // Reset on, rising edge, 16-bit char bits
    SpicRegs.SPICTL.all =0x0006;                 // Enable master mode, normal phase,
                                                 // enable talk, and SPI int disabled.
    SpicRegs.SPIBRR =0x007F;
    SpicRegs.SPICCR.all =0x0087;                 // Relinquish SPI from Reset
    SpicRegs.SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission
#else
    EALLOW;
    //GPIO20->MOSI  GPIO21->MISO  GPIO22->CLK  GPIO23->CS
        GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;
        GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;
        GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;
        GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;
    EDIS;
    SPI_CS(1);
#endif
}

void spi_xmit(Uint16 a)
{
    SpicRegs.SPITXBUF=a;
}

void spi_fifo_init()
{
// Initialize SPI FIFO registers
    SpicRegs.SPIFFTX.all=0xE040;
    SpicRegs.SPIFFRX.all=0x204f;
    SpicRegs.SPIFFCT.all=0x0;
}
#else
void spi_fifo_init(void)
{
    SpicRegs.SPIFFTX.all=0xE040;   //使能SPIFIFO功能；TXFIFO复位；清除 TXFF INT 中断位;TXFIFO 中 断不使能；
    SpicRegs.SPIFFRX.all=0x204f;   //重新使能接收 FIFO 操作；清RXFIFO中断标志位；中断不使能。
    SpicRegs.SPIFFCT.all=0x0;      //这个寄存器是设置FIFO延时的，不需要用到。
}

void spi_init(void)
{
    SpicRegs.SPICCR.all =0x004F;       // 复位,下降沿发送，上升沿接收(即时钟极性是：空闲时为高电平),  字长16位。关闭SPI内部LOOP BACK  禁止回送
    SpicRegs.SPICTL.all =0x0006;    // 主机模式, 时钟相位为正常相位, SpiaRegs.SPICTL.bit.CLK_PHASE=0;
                               //TALK=1使能主机发送, SPI中断不使能.  时钟相位为：数据在第2个时钟边沿被选择
    SpicRegs.SPIBRR =0x007F;      //波特率=195.3KHz 。波特率=LSPCLK/(SPIBRR+1)=25MHz/128=195.3KHz
    //SpiaRegs.SPIBRR=24;          //Baud=25M/(24+1)=1M  波特率太大的话，SCK只有一个脉冲出来
    SpicRegs.SPICCR.all =0x00CF;       //下降沿发送，上升沿接收(即时钟极性是：空闲时为高电平), 字长16位。准备发送或接收  禁止回送模式SPILBK=0
    SpicRegs.SPIPRI.bit.FREE = 1;    //仿真用的
}

#endif
void spi_write_bytes(u8 *pData,u8 len)
{
    int i=0,j=0;
#if 0
    delay(5000);
    for(i=0;i<len;i++){
        SpicRegs.SPITXBUF = *pData++;
    }
    // Wait until data is received
    //while(SpiaRegs.SPIFFRX.bit.RXFFST < 2) {}
    delay(5000);
    delay(5000);
    for(i=0;i<len;i++){
        SpicRegs.SPIRXBUF;
    }
    delay(5000);
#else
    for(i=0;i<len;i++){
      j=8;
      while(j--){
        if((*pData)&(1<<j)){
            SPI_MOSI(1);
        }else{
            SPI_MOSI(0);
        }
        delay(10);
        SPI_CLK(1);
        delay(10);
        SPI_CLK(0);
      }
      pData++;
    }
#endif
}

void spi_read_bytes(u8 *pData,u8 len)
{
    int i=0,j=0;
#if 0
    delay(5000);
    for(i=0;i<len;i++){
        SpicRegs.SPITXBUF = 0xFF;
    }
    delay(5000);
    delay(5000);
    // Wait until data is received
    //while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) {}
    for(i=0;i<len;i++){
        *pData = SpicRegs.SPIRXBUF;
        pData++;
    }
    delay(5000);
#else
    for(i=0;i<len;i++){
      j=8;
      while(j--){
        delay(10);
        SPI_CLK(1);
        delay(5);
        if(SPI_MISO()){
            *pData |= 1<<j;
        }else{
            *pData &= ~(1<<j);
        }
        delay(5);
        SPI_CLK(0);
      }
      pData++;
    }
#endif
}

u8 check_boot_flag(void)
{
    //标志存储地址0x3E8FA
    u8 write_buffer[4]={0x03,0x03,0xE8,0xFA};
    u8 boot_flag;
    SPI_CS(0);
    delay(20);
    spi_write_bytes(write_buffer,4);
    spi_read_bytes(&boot_flag,1);
    delay(20);
    SPI_CS(1);
    return boot_flag;
}

void write_boot_flag(u8 flag)
{
    //标志存储地址0x3E8FA
    u8 write_buffer[5]={0x02,0x03,0xE8,0xFA,flag};
    u8 write_en = 0x06;
    u8 read_status = 0x05;
    u8 status = 0;
    SPI_CS(0);
    delay(20);
    spi_write_bytes(&write_en,1);
    delay(20);
    SPI_CS(1);
    do{
        SPI_CS(0);
        delay(20);
        spi_write_bytes(&read_status,1);
        spi_read_bytes(&status,1);
        delay(20);
        SPI_CS(1);
        delay(200);
    }while(!(status&0x02));
    SPI_CS(0);
    delay(20);
    spi_write_bytes(write_buffer,5);
    delay(20);
    SPI_CS(1);
}

int main(void)
{
	DINT;
	DRTM;
	InitSysCtrl();
#if BOOT_FLAG_OF_EEPROM
    spi_init();        // init SPI
#endif
	CAN_GPIO_Config(CANA);
	//LED_GPIO_Config();
	CsmUnlock();
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	EALLOW;
#if !BOOT_FLAG_OF_EEPROM
	PieVectTable.TINT0 = &cpu_timer0_isr;
#endif
	PieVectTable.ECAN1INTA = &Ecana_isr1;
	EDIS;
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd,&Flash28_API_RunStart);
	InitFlash();
	FlashAPI_Init();
#if BOOT_FLAG_OF_EEPROM
    if(!check_boot_flag()){
        CAN_BOOT_JumpToApplication(0x3E8010);
    }
#endif
	CAN_Config(CANA,250);
	CAN_Rx_Config();
	CAN_Rx_IT_Concig();
#if !BOOT_FLAG_OF_EEPROM
	LED_Timer_Config();
#endif
	//------------------------------------
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
	IER |= M_INT9;
	IER |= M_INT1;
	__enable_irq();
	//GpioDataRegs.GPADAT.bit.GPIO1 = 1;
	//GpioDataRegs.GPADAT.bit.GPIO1 = 0;
	while (1)
	{
#if !BOOT_FLAG_OF_EEPROM
		if(updata_info.time_out_flag == 0)
		{
			if(can_rx_msg.rx_update == UPDATE)
			{
			    //GpioDataRegs.GPADAT.bit.GPIO1 = 1;
				if(CpuTimer0Regs.TCR.bit.TSS == 0)
				{
					CpuTimer0Regs.TCR.bit.TSS = 1;
				}
				updata_info.time_out_flag = 0;
				can_rx_msg.rx_update = NON_CHANGE;
				CAN_BOOT_ExecutiveCommand(&can_rx_msg);
			}
		}
		else
		{
		    //GpioDataRegs.GPADAT.bit.GPIO1 = 0;
		    //GpioDataRegs.GPADAT.bit.GPIO0 = 0;
		    if((*(uint32_t*)0x3E8010) != 0xFFFFFFFF){
		        CAN_BOOT_JumpToApplication(0x3E8010);
		    }else{
                if(CpuTimer0Regs.TCR.bit.TSS == 0)
                {
                    CpuTimer0Regs.TCR.bit.TSS = 1;
                }
                updata_info.time_out_flag = 0;
		    }
		}
#else
        if(can_rx_msg.rx_update == UPDATE)
        {
            can_rx_msg.rx_update = NON_CHANGE;
            CAN_BOOT_ExecutiveCommand(&can_rx_msg);
        }
#endif
	}
}
