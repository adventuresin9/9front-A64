/*
 *	The AXP803 is the PMIC (power manager)
 *	used for Allwinner A64 SoC platforms
 */


#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"

#include "axp803.h"


#define PWR_REG_MASK	0xFF

#define	PWR_SOURCE	0x00
#define	PWR_MODE	0x01
#define	PWR_REASON	0x02
#define	PWR_ID_NUM	0x03


static u8int
pwrrd(u8int reg)
{
	u32int buf;

	buf = rsb_read(PMICRTA, PMICADDR, reg, 1);
	return (u8int)(buf & PWR_REG_MASK);
}


static int
pwrwr(u8int reg, u8int val)
{
	return rsb_write(PMICRTA, PMICADDR, reg, val, 1);
}


u8int
pmic_id(void)
{
	u8int buf, msb, lsb;

/*
 *	Reg 03, IC type no.
 *	bits 4 & 5 are uncertain
 *	AXP803 should then return b01xx0001
 *	shift bits 6 & 7 over to get b010001
 */

	buf = pwrrd(PWR_ID_NUM);

	msb = (0xC0 & buf) >> 2;
	lsb = 0x0F & buf;

	return (msb | lsb);
}


int
pmic_acin(void)
{
	u8int buf;

	buf = pwrrd(PWR_SOURCE);

	return (buf & 0x80);
}


int
pmic_vbat(void)
{
	u8int buf;

	buf = pwrrd(PWR_SOURCE);

	return (buf & 0x08);
}


static Pmicregs*
findpmicrail(char *name)
{
	Pmicregs *pr;

	for(pr = pmicregs; pr->name != nil; pr++){
		if(cistrcmp(name, pr->name) == 0)
			return pr;
	}

	return nil;
}


int
getpmicstate(int rail)
{
	u8int buf;
	Pmicregs *pr = pmicregs;

	pr += rail;

	buf = pwrrd(pr->onoffreg);

	buf &= 1<<pr->onoffbit;

	return (int)(buf? 1 : 0);
}


int
setpmicstate(char *name, int state)
{
	u8int buf;
	Pmicregs *pr = pmicregs;

	pr = findpmicrail(name);

	if(pr == nil)
		return -1;

	buf = pwrrd(pr->onoffreg);

//	iprint("setstate: %s = %d _ %08ub_", name, state, buf);

	if((buf & 1<<pr->onoffbit) == (state<<pr->onoffbit))
		return 1;

	if(state)
		buf |= 1<<pr->onoffbit;
	else
		buf ^= 1<<pr->onoffbit;

//	iprint("%08ub\n", buf);

	if(pwrwr(pr->onoffreg, buf))
		return 1;


	iprint("setpmicstate: pwrwr failed\n");
	return 0;
}


/*
 *	Some of the rails have split voltage steps.
 *	Above a certain value, each step will be 
 *	twice as many mV.
 *	ex. 10mV steps till 1200mV, and 20mv steps above
 */

int
getpmicvolt(int rail)
{
	u8int buf;
	int steps, mv;
	Pmicregs *pr = pmicregs;

	pr += rail;

	buf = pwrrd(pr->voltreg);
	buf &= pr->voltmask;

	steps = (int)buf;

	mv = (steps * pr->voltstep1) + pr->voltbase;

	if(mv > pr->voltsplit){
		int mvl, mvh;
		mvl = (pr->voltsplit - pr->voltbase) / pr->voltstep1;
		mvh = steps - mvl;
		
		mv = (mvl * pr->voltstep1) + (mvh * pr->voltstep2);
	}

	return mv;
}


int
setpmicvolt(char *name, int val)
{
	u8int buf;
	Pmicregs *pr = pmicregs;

	pr = findpmicrail(name);

	if(pr == nil)
		return -1;

	if(val > pr->voltmax)
		val = pr->voltmax;

	if(val < pr->voltbase)
		val = pr->voltbase;

	if(val <= pr->voltsplit)
		buf = (u8int)((val - pr->voltbase) / pr->voltstep1);
		buf &= pr->voltmask;

	if(val > pr->voltsplit){
		int mvl, mvh;
		mvl = (pr->voltsplit - pr->voltbase) / pr->voltstep1;
		mvh = (val - pr->voltsplit) / pr->voltstep2;
		buf = (u8int)(mvl + mvh);
		buf &= pr->voltmask;
	}

//	iprint("setvolt: %ud\n", buf);

	if(pwrwr(pr->voltreg, buf))
		return 1;


	iprint("setpmicvolt: pwrwr failed\n");
	return 0;
}


char*
getpmicname(int rail)
{
	Pmicregs *pr = pmicregs;

	pr += rail;

	return pr->name;
}



