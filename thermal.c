/*
 *	Thermal Sensor Controller
 *	A64 manual, pg 264
 *	Temp = -8.56 * Data + 2170 °C
 */


#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"


#define	THS_CTRL0			0x00	/*THS Control Register0*/
#define	THS_CTRL1			0x04	/*THS Control Register1*/
#define	ADC_CDAT			0x14	/*ADC calibration data Register*/
#define	THS_CTRL2			0x40	/*THS Control Register2*/
#define	THS_INT_CTRL		0x44	/*THS Interrupt Control Register*/
#define	THS_STAT			0x48	/*THS Status Register*/
#define	THS0_ALARM_CTRL		0x50	/*Alarm threshold Control Register0*/
#define	THS1_ALARM_CTRL		0x54	/*Alarm threshold Control Register1*/
#define	THS2_ALARM_CTRL		0x58	/*Alarm threshold Control Register2*/
#define	THS0_SHUTDOWN_CTRL	0x60	/*Shutdown threshold Control Register0*/
#define	THS1_SHUTDOWN_CTRL	0x64	/*Shutdown threshold Control Register1*/
#define	THS2_SHUTDOWN_CTRL	0x68	/*Shutdown threshold Control Register2*/
#define	THS_FILTER			0x70	/*Median filter Control Register*/
#define	THS0_1_CDATA		0x74	/*Thermal Sensor 0 1 Calibration Data*/
#define	THS2_CDATA			0x78	/*Thermal Sensor2 Calibration Data*/
#define	THS0_DATA			0x80	/*THS0 Data Register*/
#define	THS1_DATA			0x84	/*THS1 Data Register*/
#define	THS2_DATA			0x88	/*THS2 Data Register*/

#define	THS_DATA_MASK		0xFFF

enum {
	/*THS_CTRL0*/
	Acq0		=	0xFFFF<<0,
	Acq0shft	=	0,
	/*THS_CTRL1*/
	OpBias		=	0x3<<20,
	OBshft		=	20,
	ACaliEn		=	1<<17,
	/*ADC_CDAT*/
	AdcCdat		=	0xFFF<<0,
	ADCshft		=	0,
	/*THS_CTRL2*/
	Acq1		=	0xFFFF<<16,
	Acq1shft	=	16,
	Sens2En		=	1<<2,
	Sens1En		=	1<<1,
	Sens0En		=	1<<0,
	/*THS_INT_CTRL & THS_STAT*/
	Idata2		=	1<<10,
	Idata1		=	1<<9,
	Idata0		=	1<<8,
	Ishut2		=	1<<6,
	Ishut1		=	1<<5,
	Ishut0		=	1<<4,
	Ialrm2		=	1<<2,
	Ialrm1		=	1<<1,
	Ialrm0		=	1<<0,
	/*THS_INT_CTRL*/
	ThemPer		=	0xFFFFF<<12,
	TPshft		=	12,
	/*THS_STAT*/
	IAoff2		=	1<<14,
	IAoff1		=	1<<13,
	IAoff0		=	1<<12,
	/*THS_FILTER*/
	FltrEn		=	1<<2,
	FltrType	=	0x3<<0,
	FTshft		=	0<<0,
	Fltr2		=	0x0,
	Fltr4		=	0x1,
	Fltr8		=	0x2,
	Fltr16		=	0x3,
};


enum {
	TPval	=	0x18,
	AcqTime	=	0x190,
	TmpBase	=	2170000,
	TempMul	=	1000,
	TempDiv	=	8560,
};


static u32int
thrmrd(int offset)
{
		return *IO(u32int, (THERMAL + offset));
}


static void
thrmwr(int offset, u32int val)
{
	*IO(u32int, (THERMAL + offset)) = val;
}


static int
tempformula(u32int val)
{
	return ((TmpBase - (val * TempMul)) / TempDiv);
}


int
gettemp0(void)
{
	u32int	val;

	val = thrmrd(THS0_DATA);
	val &= THS_DATA_MASK;

	return tempformula(val);
}


int
gettemp1(void)
{
	u32int	val;

	val = thrmrd(THS1_DATA);
	val &= THS_DATA_MASK;

	return tempformula(val);
}


int
gettemp2(void)
{
	u32int	val;

	val = thrmrd(THS2_DATA);
	val &= THS_DATA_MASK;

	return tempformula(val);
}


static void
thrminterrupt(Ureg*, void*)
{
	u32int	buf;

	buf = thrmrd(THS_STAT);
	thrmwr(THS_STAT, buf);

	if(buf & Ialrm0)
		print("CPU IS HOT\n");

	if(buf & Ishut0)
		print("CPU ON FIRE!!!\n");
}


void
thermalinit(void)
{
	u32int	buf;

	/* turn on THS clock */
	turnonths();

	/* start calibration */
	thrmwr(THS_CTRL1, ACaliEn);
	thrmwr(THS_CTRL0, AcqTime << Acq0shft);
	thrmwr(THS_CTRL2, AcqTime << Acq1shft);

	/* enable filter, average of 4 */
	thrmwr(THS_FILTER, FltrEn | Fltr4);

	/* clear interrupts */
	buf = thrmrd(THS_STAT);
	thrmwr(THS_STAT, buf);

	/* set interrupts and THERMAL_PER value */
	thrmwr(THS_INT_CTRL, Ialrm0 | Ishut0 | (TPval << TPshft));

	/* start sensors 0, 1, 2 */
	buf = thrmrd(THS_CTRL2);
	buf |= (Sens0En | Sens1En | Sens2En);;
	thrmwr(THS_CTRL2, buf);

	intrenable(IRQthermal, thrminterrupt, nil, BUSUNKNOWN, "thermal");
}