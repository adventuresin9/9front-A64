
#define	IO(t,x)		((t*)(VIRTIO+((ulong)x)))

#define	SYSCLOCK	24000000

#define	CONSOLE		0

#define	PHYSDEV		0x01C00000

#define	SYSCTL		0x1C00000

#define	TCON0		0x0C000
#define	TCON1		0x0D000

/* sd-mmc */
#define	SMHC0		0x0F000
#define	SMHC1		0x10000
#define	SMHC2		0x11000

#define	MBOX		0x17000
#define	SPINLOCK	0x18000

#define	USBOTG		0x19000
#define	USB0		0x1A000
#define	USB1		0x1B000

#define	CCUBASE		0x01C20000

#define TIMER		0x20C00
#define KEYADC		0x1C21800


#define	I2S0		0x22000
#define	I2S1		0x22400
#define	I2S2		0x22800

#define	THERMAL		0x01C25000

#define	UART0		0x1C28000
#define	UART1		0x28400
#define	UART2		0x28800
#define	UART3		0x28C00
#define	UART4		0x29000

#define	GPU			0x40000

#define	SPI0		0x68000
#define	SPI1		0x69000

#define	TWI0		0x01C2AC00
#define	TWI1		0x01C2B000
#define	TWI2		0x01C2B400

/* irq controller */
#define	GIC			0x1C80000

#define	EMAC		0x1C30000

/* camera input */
#define	CSI0		0xB0000

#define	RTC			0x01F00000
#define PRCM        0x01F01400
#define PIO         0x01F02C00
#define	R_RSB		0x01F03400

/* addresses for axp803 pmic */
#define	PMICADDR	0x3a3
#define	PMICRTA		0x2d


/* IRQs */
enum {
	IRQfiq		=	-1,

	PPI		= 16,
	SPI		= 32,

	IRQcntps	=	PPI+13,
	IRQcntpns	=	PPI+14,

	IRQuart0	=	SPI+0,
	IRQuart1	=	SPI+1,
	IRQuart2	=	SPI+2,
	IRQuart3	=	SPI+3,
	IRQuart4	=	SPI+4,

	IRQtwi0		=	SPI+6,
	IRQtwi1		=	SPI+7,
	IRQtwi2		=	SPI+8,

	IRQi2s0		=	SPI+13,
	IRQi2s1		=	SPI+14,
	IRQi2s2		=	SPI+15,

	IRQtimer0	=	SPI+18,
	IRQtimer1	=	SPI+19,

	IRQwdog		=	SPI+25,

	IRQkeyadc	=	SPI+30,
	IRQthermal	=	SPI+31,
	IRQpmic		=	SPI+32,		//? schematic says axp803 irq wired to NMI

	IRQrsb		=	71,

	IRQmbox		=	SPI+49,
	IRQdma		=	SPI+50,
	IRQtimerhs	=	SPI+51,

	IRQsdmmc0	=	SPI+60,
	IRQsdmmc1	=	SPI+61,
	IRQsdmmc2	=	SPI+62,

	IRQspi0		=	SPI+65,
	IRQspi1		=	SPI+66,

	IRQotg		=	SPI+71,
	IRQotgehci	=	SPI+72,
	IRQotgohci	=	SPI+73,
	IRQusbehci	=	SPI+74,
	IRQusbohci	=	SPI+75,

	IRQemac		=	SPI+82,

	IRQtcon0	=	SPI+86,
	IRQtcon1	=	SPI+87,
	IRQhdmi		=	SPI+88,
	IRQmipi		=	SPI+89,

	/* gpu irqs */
	IRQgp		=	SPI+97,
	IRQgpmmu	=	SPI+98,
	IRQpp0		=	SPI+99,
	IRQpp0mmu	=	SPI+100,
	IRQpmu		=	SPI+101,
	IRQpp1		=	SPI+102,
	IRQppmmu1	=	SPI+103,
};

#define BUSUNKNOWN (-1)
