/*
 *	Allwinner A64 specific stuff looking for a home
 */

#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"


/* R_PIO regs */
#define	RPIO_CFG00	0x00	/* Port L Configure Register 0 */
	#define	S_RSB_SCK	0x2
	#define	S_RSB_SDA	0x20
#define	RPIO_CFG10	0x04	/* Port L Configure Register 1 */
#define	RPIO_CFG20	0x08	/* Port L Configure Register 2 */
#define	RPIO_CFG30	0x0C	/* Port L Configure Register 3 */
#define	RPIO_DAT0	0x10	/* Port L Data Register */
#define	RPIO_DRV00	0x14	/* Port L Multi-Driving Register 0 */

#define	RPIO_DRV10	0x18	/* Port L Multi-Driving Register 1 */
#define	RPIO_PUL00	0x1C	/* Port L Pull Register 0 */
#define	RPIO_PUL10	0x20	/* Port L Pull Register 1 */


/*
 *	Information for the PRMC regs is 
 *	absent on the A64 datasheet.
 *	This is taken from the A80 datasheet
 */

/* R_PRMC regs */
#define	CPUS_RST_REG		0x0000		/* CPUS Reset Register */
#define	C0_CPUX_RST_CTRL	0x0004		/* Cluster0 CPUX Reset Control Register */
#define	CPUS_CLK_CFG_REG	0x0010		/* CPUS Clock Configuration Register */
#define	APBS_CLK_DIV_REG	0x001C		/* APBS Clock Divide Register */
#define	APBS_CLK_GATING_REG	0x0028		/* APBS Clock Gating Register */
	#define	R_PIO_GATING	1<<0
	#define	R_RSB_GATING	1<<3
#define	PLL_CTRL_REG1		0x0044		/* PLL Control Register 1 */
#define	APBS_SOFT_RST_REG	0x00B0		/* APBS Software Reset Register */
	#define	R_RSB_RESET		1<<3



static void
prcmwr(int offset, u32int val)
{
	*IO(u32int, (PRCM + offset)) = val;
}


static u32int
prcmrd(int offset)
{
	return *IO(u32int, (PRCM + offset));
}


static void
piowr(int offset, u32int val)
{
	*IO(u32int, (PIO + offset)) = val;
}


static u32int
piord(int offset)
{
	return *IO(u32int, (PIO + offset));
}


/*
 *	This is needed to make sure RSB and access to the PMIC
 *	works on the Pine64 Pinephone
 */



void
arch_rsbsetup(void)
{
	/* gate pass for R_PIO */
	prcmwr(APBS_CLK_GATING_REG, prcmrd(APBS_CLK_GATING_REG) | R_PIO_GATING);

	/* sets PL0 and PL1 to use RSB */
	piowr(RPIO_CFG00, piord(RPIO_CFG00) & ~0xff | (S_RSB_SCK | S_RSB_SDA));
	/* sets PL0 and PL1 to "Level 2" */
	piowr(RPIO_DRV00, piord(RPIO_DRV00) & ~0x0f | 0xa);
	/* sets PL0 and PL1 to pull-up */
	piowr(RPIO_PUL00, piord(RPIO_PUL00) & ~0x0f | 0x5);

	/* asser then de-assert reset on RSB */
	prcmwr(APBS_SOFT_RST_REG, prcmrd(APBS_SOFT_RST_REG) & ~R_RSB_RESET);
	prcmwr(APBS_SOFT_RST_REG, prcmrd(APBS_SOFT_RST_REG) | R_RSB_RESET);

	/* gate pass for R_RSB */
	prcmwr(0x28, prcmrd(0x28) | R_RSB_GATING);
}


