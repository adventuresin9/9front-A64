#define	PLL_CPUX_CTRL_REG		0x0000		//PLL_CPUX Control Register
#define	PLL_AUDIO_CTRL_REG		0x0008		//PLL_AUDIO Control Register
#define	PLL_VIDEO0_CTRL_REG		0x0010		//PLL_VIDEO0 Control Register
#define	PLL_VE_CTRL_REG			0x0018		//PLL_VE Control Register
#define	PLL_DDR0_CTRL_REG		0x0020		//PLL_DDR0 Control Register
#define	PLL_PERIPH0_CTRL_REG	0x0028		//PLL_PERIPH0 Control Register
#define	PLL_PERIPH1_CTRL_REG	0x002C		//PLL_PERIPH1 Control Register
#define	PLL_VIDEO1_CTRL_REG		0x0030		//PLL_VIDEO1 Control Register
#define	PLL_GPU_CTRL_REG		0x0038		//PLL_GPU Control Register
#define	PLL_MIPI_CTRL_REG		0x0040		//PLL_MIPI Control Register
#define	PLL_HSIC_CTRL_REG		0x0044		//PLL_HSIC Control Register
#define	PLL_DE_CTRL_REG			0x0048		//PLL_DE Control Register
#define	PLL_DDR1_CTRL_REG		0x004C		//PLL_DDR1 Control Register
#define	CPU_AXI_CFG_REG			0x0050		//CPUX/AXI Configuration Register
#define	AHB1_APB1_CFG_REG		0x0054		//AHB1/APB1 Configuration Register
#define	APB2 _CFG_REG			0x0058		//APB2 Configuration Register
#define	AHB2_CFG_REG			0x005C		//AHB2 Configuration Register
#define	BUS_CLK_GATING_REG0		0x0060		//Bus Clock Gating Register 0
#define	BUS_CLK_GATING_REG1		0x0064		//Bus Clock Gating Register 1
#define	BUS_CLK_GATING_REG2		0x0068		//Bus Clock Gating Register 2
#define	BUS_CLK_GATING_REG3		0x006C		//Bus Clock Gating Register 3
#define	BUS_CLK_GATING_REG4		0x0070		//Bus Clock Gating Register 4
#define	THS_CLK_REG				0x0074		//THS Clock Register
#define	NAND_CLK_REG			0x0080		//NAND Clock Register
#define	SDMMC0_CLK_REG			0x0088		//SDMMC0 Clock Register
#define	SDMMC1_CLK_REG			0x008C		//SDMMC1 Clock Register
#define	SDMMC2_CLK_REG			0x0090		//SDMMC2 Clock Register
#define	TS_CLK_REG				0x0098		//TS Clock Register
#define	CE_CLK_REG				0x009C		//CE Clock Register
#define	SPI0_CLK_REG			0x00A0		//SPI0 Clock Register
#define	SPI1_CLK_REG			0x00A4		//SPI1 Clock Register
#define	I2SPCM0_CLK_REG		0x00B0		//I2S/PCM-0 Clock Register
#define	I2SPCM1_CLK_REG		0x00B4		//I2S/PCM-1 Clock Register
#define	I2SPCM2_CLK_REG		0x00B8		//I2S/PCM-2 Clock Register
#define	SPDIF_CLK_REG			0x00C0		//SPDIF Clock Register
#define	USBPHY_CFG_REG			0x00CC		//USBPHY Configuration Register
#define	DRAM_CFG_REG			0x00F4		//DRAM Configuration Register
#define	PLL_DDR_CFG_REG			0x00F8		//PLL_DDR Configuration Register
#define	MBUS_RST_REG			0x00FC		//MBUS Reset Register
#define	DRAM_CLK_GATING_REG		0x0100		//DRAM Clock Gating Register
#define	DE_CLK_REG				0x0104		//DE Clock Register
#define	TCON0_CLK_REG			0x0118		//TCON0 Clock Register
#define	TCON1_CLK_REG			0x011C		//TCON1 Clock Register

#define	DEINTERLACE_CLK_REG		0x0124		//DEINTERLACE Clock Register
#define	CSI_MISC_CLK_REG		0x0130		//CSI_MISC Clock Register
#define	CSI_CLK_REG				0x0134		//CSI Clock Register
#define	VE_CLK_REG				0x013C		//VE Clock Register

#define	AC_DIG_CLK_REG			0x0140		//AC Digital Clock Register
#define	AVS_CLK_REG				0x0144		//AVS Clock Register
#define	HDMI_CLK_REG			0x0150		//HDMI Clock Register
#define	HDMI_SLOW_CLK_REG		0x0154		//HDMI Slow Clock Register
#define	MBUS_CLK_REG			0x015C		//MBUS Clock Register
#define	MIPI_DSI_CLK_REG		0x0168		//MIPI_DSI Clock Register
#define	GPU_CLK_REG				0x01A0		//GPU Clock Register

#define	PLL_STABLE_TIME_REG0	0x0200		//PLL Stable Time Register0
#define	PLL_STABLE_TIME_REG1	0x0204		//PLL Stable Time Register1
#define	PLL_PERIPH1_BIAS_REG	0x021C		//PLL_PERIPH1 Bias Register
#define	PLL_CPUX_BIAS_REG		0x0220		//PLL_CPUX Bias Register
#define	PLL_AUDIO_BIAS_REG		0x0224		//PLL_AUDIO Bias Register
#define	PLL_VIDEO0_BIAS_REG		0x0228		//PLL_VIDEO0 Bias Register
#define	PLL_VE_BIAS_REG			0x022C		//PLL_VE Bias Register
#define	PLL_DDR0_BIAS_REG		0x0230		//PLL_DDR0 Bias Register
#define	PLL_PERIPH0_BIAS_REG	0x0234		//PLL_PERIPH0 Bias Register
#define	PLL_VIDEO1_BIAS_REG		0x0238		//PLL_VIDEO1 Bias Register
#define	PLL_GPU_BIAS_REG		0x023C		//PLL_GPU Bias Register
#define	PLL_MIPI_BIAS_REG		0x0240		//PLL_MIPI Bias Register
#define	PLL_HSIC_BIAS_REG		0x0244		//PLL_HSIC Bias Register
#define	PLL_DE_BIAS_REG			0x0248		//PLL_DE Bias Register
#define	PLL_DDR1_BIAS_REG		0x024C		//PLL_DDR1 Bias Register
#define	PLL_CPUX_TUN_REG		0x0250		//PLL_CPUX Tuning Register
#define	PLL_DDR0_TUN_REG		0x0260		//PLL_DDR0 Tuning Register
#define	PLL_MIPI_TUN_REG		0x0270		//PLL_MIPI Tuning Register
#define	PLL_PERIPH1_PAT_CTRL_REG	0x027C	//PLL_PERIPH1 Pattern Control Register
#define	PLL_CPUX_PAT_CTRL_REG	0x0280		//PLL_CPUX Pattern Control Register
#define	PLL_AUDIO_PAT_CTRL_REG	0x0284		//PLL_AUDIO Pattern Control Register
#define	PLL_VIDEO0_PAT_CTRL_REG	0x0288		//PLL_VIDEO0 Pattern Control Register
#define	PLL_VE_PAT_CTRL_REG		0x028C		//PLL_VE Pattern Control Register
#define	PLL_DDR0_PAT_CTRL_REG	0x0290		//PLL_DDR0 Pattern Control Register
#define	PLL_VIDEO1_PAT_CTRL_REG	0x0298		//PLL_VIDEO1 Pattern Control Register
#define	PLL_GPU_PAT_CTRL_REG	0x029C		//PLL_GPU Pattern Control Register
#define	PLL_MIPI_PAT_CTRL_REG	0x02A0		//PLL_MIPI Pattern Control Register
#define	PLL_HSIC_PAT_CTRL_REG	0x02A4		//PLL_HSIC Pattern Control Register
#define	PLL_DE_PAT_CTRL_REG		0x02A8		//PLL_DE Pattern Control Register
#define	PLL_DDR1_PAT_CTRL_REG0	0x02AC		//PLL_DDR1 Pattern Control Register0
#define	PLL_DDR1_PAT_CTRL_REG1	0x02B0		//PLL_DDR1 Pattern Control Register1

#define	BUS_SOFT_RST_REG0		0x02C0		//Bus Software Reset Register 0
#define	BUS_SOFT_RST_REG1		0x02C4		//Bus Software Reset Register 1
#define	BUS_SOFT_RST_REG2		0x02C8		//Bus Software Reset Register 2
#define	BUS_SOFT_RST_REG3		0x02D0		//Bus Software Reset Register 3
#define	BUS_SOFT_RST_REG4		0x02D8		//Bus Software Reset Register 4

#define	CCM_SEC_SWITCH_REG		0x02F0		//CCM Security Switch Register
#define	PS_CTRL_REG				0x0300		//PS Control Register
#define	PS_CNT_REG				0x0304		//PS Counter Register
#define	PLL_LOCK_CTRL_REG		0x0320		//PLL Lock Control Register



typedef struct Gate Gate;
typedef struct Reset Reset;
typedef	struct Nkmp	Nkmp;


struct Gate{
	char	*name;
	uint	bank;
	uint	mask;
};


struct Reset{
	char	*name;
	uint	bank;
	uint	mask;
};


static Gate gates[] = {
	{"USBOHCI0",		0x0,	1<<29},
	{"USB-OTG-OHCI",	0x0,	1<<28},
	{"USBEHCI0",		0x0,	1<<25},
	{"USB-OTG-EHCI",	0x0,	1<<24},
	{"USB-OTG-Device",	0x0,	1<<23},
	{"SPI1",			0x0,	1<<21},
	{"SPI0",			0x0,	1<<20},
	{"HSTMR",			0x0,	1<<19},
	{"TS",				0x0,	1<<18},
	{"EMAC",			0x0,	1<<17},
	{"DRAM",			0x0,	1<<14},
	{"NAND",			0x0,	1<<13},
	{"SMHC2",			0x0,	1<<10},
	{"SMHC1",			0x0,	1<<9},
	{"SMHC0",			0x0,	1<<8},
	{"DMA",				0x0,	1<<6},
	{"CE",				0x0,	1<<5},
	{"MIPIDSI",			0x0,	1<<1},
	{"SPINLOCK",		0x4,	1<<22},
	{"MSGBOX",			0x4,	1<<21},
	{"GPU",				0x4,	1<<20},
	{"DE",				0x4,	1<<12},
	{"HDMI",			0x4,	1<<11},
	{"CSI",				0x4,	1<<8},
	{"DEINTERLACE",		0x4,	1<<5},
	{"TCON1",			0x4,	1<<4},
	{"TCON0",			0x4,	1<<3},
	{"VE",				0x4,	1<<0},
	{"I2SPCM2",			0x8,	1<<14},
	{"I2SPCM1",			0x8,	1<<13},
	{"I2SPCM0",			0x8,	1<<12},
	{"THS",				0x8,	1<<8},
	{"PIO",				0x8,	1<<5},
	{"SPDIF",			0x8,	1<<1},
	{"AC_DIG",			0x8,	1<<0},
	{"UART4",			0xC,	1<<20},
	{"UART3",			0xC,	1<<19},
	{"UART2",			0xC,	1<<18},
	{"UART1",			0xC,	1<<17},
	{"UART0",			0xC,	1<<16},
	{"SCR",				0xC,	1<<5},
	{"TWI2",			0xC,	1<<2},
	{"TWI1",			0xC,	1<<1},
	{"TWI0",			0xC,	1<<0},
	{"DBGSYS",			0x10,	1<<7},
	{ nil },
};


static Reset resets[] = {
	{"USBOHCI0",		0x0,	1<<29},
	{"USB-OTG-OHCI",	0x0,	1<<28},
	{"USBEHCI0",		0x0,	1<<25},
	{"USB-OTG-EHCI",	0x0,	1<<24},
	{"USB-OTG-Device",	0x0,	1<<23},
	{"SPI1",			0x0,	1<<21},
	{"SPI0",			0x0,	1<<20},
	{"HSTMR",			0x0,	1<<19},
	{"TS",				0x0,	1<<18},
	{"EMAC",			0x0,	1<<17},
	{"DRAM",			0x0,	1<<14},
	{"NAND",			0x0,	1<<13},
	{"SMHC2",			0x0,	1<<10},
	{"SMHC1",			0x0,	1<<9},
	{"SMHC0",			0x0,	1<<8},
	{"DMA",				0x0,	1<<6},
	{"CE",				0x0,	1<<5},
	{"MIPIDSI",			0x0,	1<<1},
	{"DBGSYS",			0x4,	1<<31},
	{"SPINLOCK",		0x4,	1<<22},
	{"MSGBOX",			0x4,	1<<21},
	{"GPU",				0x4,	1<<20},
	{"DE",				0x4,	1<<12},
	{"HDMI",			0x4,	1<<11},		// listed as HDMI1 in datasheet
	{"HDMI0",			0x4,	1<<10},
	{"CSI",				0x4,	1<<8},
	{"DEINTERLACE",		0x4,	1<<5},
	{"TCON1",			0x4,	1<<4},
	{"TCON0",			0x4,	1<<3},
	{"VE",				0x4,	1<<0},
	{"LVDS",			0x8,	1<<0},
	{"I2SPCM2",			0x10,	1<<14},
	{"I2SPCM1",			0x10,	1<<13},
	{"I2SPCM0",			0x10,	1<<12},
	{"THS",				0x10,	1<<8},
	{"OWA",				0x10,	1<<1},
	{"AC_DIG",			0x10,	1<<0},
	{"UART4",			0x18,	1<<20},
	{"UART3",			0x18,	1<<19},
	{"UART2",			0x18,	1<<18},
	{"UART1",			0x18,	1<<17},
	{"UART0",			0x18,	1<<16},
	{"SCR",				0x18,	1<<5},
	{"TWI2",			0x18,	1<<2},
	{"TWI1",			0x18,	1<<1},
	{"TWI0",			0x18,	1<<0},
	{ nil },
};


struct Nkmp {
	uint	rate;
	u32int	n;
	u32int	k;
	u32int	m;
	u32int	p;
};


static Nkmp bad_nkmp[] = {
	{ 60000000, 9, 0, 0, 2 },
	{ 66000000, 10, 0, 0, 2 },
	{ 72000000, 11, 0, 0, 2 },
	{ 78000000, 12, 0, 0, 2 },
	{ 84000000, 13, 0, 0, 2 },
	{ 90000000, 14, 0, 0, 2 },
	{ 96000000, 15, 0, 0, 2 },
	{ 102000000, 16, 0, 0, 2 },
	{ 108000000, 17, 0, 0, 2 },
	{ 114000000, 18, 0, 0, 2 },
	{ 120000000, 9, 0, 0, 1 },
	{ 132000000, 10, 0, 0, 1 },
	{ 144000000, 11, 0, 0, 1 },
	{ 156000000, 12, 0, 0, 1 },
	{ 168000000, 13, 0, 0, 1 },
	{ 180000000, 14, 0, 0, 1 },
	{ 192000000, 15, 0, 0, 1 },
	{ 204000000, 16, 0, 0, 1 },
	{ 216000000, 17, 0, 0, 1 },
	{ 228000000, 18, 0, 0, 1 },
	{ 240000000, 9, 0, 0, 0 },
	{ 264000000, 10, 0, 0, 0 },
	{ 288000000, 11, 0, 0, 0 },
	{ 312000000, 12, 0, 0, 0 },
	{ 336000000, 13, 0, 0, 0 },
	{ 360000000, 14, 0, 0, 0 },
	{ 384000000, 15, 0, 0, 0 },
	{ 408000000, 16, 0, 0, 0 },
	{ 432000000, 17, 0, 0, 0 },
	{ 456000000, 18, 0, 0, 0 },
	{ 480000000, 19, 0, 0, 0 },
	{ 504000000, 20, 0, 0, 0 },
	{ 528000000, 21, 0, 0, 0 },
	{ 552000000, 22, 0, 0, 0 },
	{ 576000000, 23, 0, 0, 0 },
	{ 600000000, 24, 0, 0, 0 },
	{ 624000000, 25, 0, 0, 0 },
	{ 648000000, 26, 0, 0, 0 },
	{ 672000000, 27, 0, 0, 0 },
	{ 696000000, 28, 0, 0, 0 },
	{ 720000000, 29, 0, 0, 0 },
	{ 768000000, 15, 1, 0, 0 },
	{ 792000000, 10, 2, 0, 0 },		/* seem to be set by u-boot */
	{ 816000000, 16, 1, 0, 0 },
	{ 864000000, 17, 1, 0, 0 },
	{ 912000000, 18, 1, 0, 0 },
	{ 936000000, 12, 2, 0, 0 },
	{ 960000000, 19, 1, 0, 0 },
	{ 1008000000, 20, 1, 0, 0 },
	{ 1056000000, 21, 1, 0, 0 },
	{ 1080000000, 14, 2, 0, 0 },
	{ 1104000000, 22, 1, 0, 0 },
	{ 1152000000, 23, 1, 0, 0 },	/* seems to be the prefered speed */
//	{ 1152000000, 31, 2, 1, 0 },
	{ 1200000000, 24, 1, 0, 0 },
	{ 1224000000, 16, 2, 0, 0 },
	{ 1248000000, 25, 1, 0, 0 },
	{ 1296000000, 26, 1, 0, 0 },
	{ 1344000000, 27, 1, 0, 0 },
	{ 1368000000, 18, 2, 0, 0 },
	{ 1440000000, 19, 2, 0, 0 },
	{ 1512000000, 20, 2, 0, 0 },
	{ 1536000000, 15, 3, 0, 0 },
	{ 1584000000, 21, 2, 0, 0 },
	{ 1632000000, 16, 3, 0, 0 },
	{ 1656000000, 22, 2, 0, 0 },
	{ 1728000000, 23, 2, 0, 0 },
	{ 1800000000, 24, 2, 0, 0 },
	{ 1872000000, 25, 2, 0, 0 },
	{ 0 }
};

static Nkmp cpu_nkmp[] = {
	{ 36000000, 0, 2, 1, 0 },
	{ 72000000, 1, 2, 1, 0 },
	{ 108000000, 2, 2, 1, 0 },
	{ 144000000, 3, 2, 1, 0 },
	{ 180000000, 4, 2, 1, 0 },
	{ 216000000, 5, 2, 1, 0 },
	{ 252000000, 6, 2, 1, 0 },
	{ 288000000, 7, 2, 1, 0 },
	{ 324000000, 8, 2, 1, 0 },
	{ 360000000, 9, 2, 1, 0 },
	{ 396000000, 10, 2, 1, 0 },
	{ 432000000, 11, 2, 1, 0 },
	{ 468000000, 12, 2, 1, 0 },
	{ 504000000, 13, 2, 1, 0 },
	{ 540000000, 14, 2, 1, 0 },
	{ 576000000, 15, 2, 1, 0 },
	{ 612000000, 16, 2, 1, 0 },
	{ 648000000, 17, 2, 1, 0 },
	{ 684000000, 18, 2, 1, 0 },
	{ 720000000, 19, 2, 1, 0 },
	{ 756000000, 20, 2, 1, 0 },
	{ 792000000, 21, 2, 1, 0 },
	{ 828000000, 22, 2, 1, 0 },
	{ 864000000, 23, 2, 1, 0 },
	{ 900000000, 24, 2, 1, 0 },
	{ 936000000, 25, 2, 1, 0 },
	{ 972000000, 26, 2, 1, 0 },
	{ 1008000000, 27, 2, 1, 0 },
	{ 1044000000, 28, 2, 1, 0 },
	{ 1080000000, 29, 2, 1, 0 },
	{ 1116000000, 30, 2, 1, 0 },
	{ 1152000000, 31, 2, 1, 0 },
	{ 0 }
};