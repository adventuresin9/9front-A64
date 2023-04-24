/*
 *	Ethernet for the Allwinner A64 (sun50i)
 */

#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "io.h"
#include "../port/netif.h"
#include "../port/etherif.h"
#include "../port/ethermii.h"




#define	ETH_BASIC_CTL_0			0x00	/*Basic Control 0 Register*/
#define	ETH_BASIC_CTL_1			0x04	/*Basic Control 1 Register*/
#define	ETH_INT_STA				0x08 	/*Interrupt Status Register*/
#define	ETH_INT_EN				0x0C	/*Interrupt Enable Register*/
#define	ETH_TX_CTL_0			0x10	/*Transmit Control 0 Register*/
#define	ETH_TX_CTL_1			0x14	/*Transmit Control 1 Register*/
#define	ETH_TX_FLOW_CTL			0x1C	/*Transmit Flow Control Register*/
#define	ETH_TX_DMA_DESC_LIST	0x20	/*Transmit Descriptor List Address Register*/
#define	ETH_RX_CTL_0			0x24	/*Receive Control 0 Register*/
#define	ETH_RX_CTL_1			0x28	/*Receive Control 1 Register*/
#define	ETH_RX_DMA_DESC_LIST	0x34	/*Receive Descriptor List Address Register*/
#define	ETH_RX_FRM_FLT			0x38	/*Receive Frame Filter Register*/
#define	ETH_RX_HASH_0			0x40	/*Hash Table 0 Register*/
#define	ETH_RX_HASH_1			0x44	/*Hash Table 1 Register*/
#define	ETH_MII_CMD				0x48	/*Management Interface Command Register*/
#define	ETH_MII_DATA			0x4C	/*Management Interface Data Register*/
#define	ETH_ADDR_HIGH_0			0x50	/*MAC Address High Register 0*/
#define	ETH_ADDR_LOW_0			0x54	/*MAC Address High Register 0*/
#define	ETH_TX_DMA_STA			0xB0	/*Transmit DMA Status Register*/
#define	ETH_TX_CUR_DESC			0xB4	/*Current Transmit Descriptor Register*/
#define	ETH_TX_CUR_BUF			0xB8	/*Current Transmit Buffer Address Register*/
#define	ETH_RX_DMA_STA			0xC0	/*Receive DMA Status Register*/
#define	ETH_RX_CUR_DESC			0xC4	/*Current Receive Descriptor Register*/
#define	ETH_RX_CUR_BUF			0xC8	/*Current Receive Buffer Addres*/
#define	ETH_RGMII_STA			0xD0	/*RGMII Status Register*/

#define	EMAC_CLK_REG			0x30	/*in System Control*/

/* Satatus Descriptor */
/* TX */
#define	TX_DESC_CTL		(1 << 31)
#define	TX_HEADER_ERR		(1 << 16)
#define	TX_LENGTH_ERR		(1 << 14)
#define	TX_PAYLOAD_ERR		(1 << 12)
#define	TX_CRS_ERR		(1 << 10)
#define	TX_COL_ERR_0		(1 << 9)
#define	TX_COL_ERR_1		(1 << 8)
#define	TX_COL_CNT		(0xf << 3)
#define	TX_COL_CNT_SHIFT	3
#define	TX_DEFER_ERR		(1 << 2)
#define	TX_UNDERFLOW_ERR	(1 << 1)
#define	TX_DEFER		(1 << 0)
/* RX */
#define	RX_DESC_CTL		(1 << 31)
#define	RX_DAF_FAIL		(1 << 30)
#define	RX_FRM_LEN		(0x3fff << 16)
#define	RX_FRM_LEN_SHIFT	16
#define	RX_NO_ENOUGH_BUF_ERR	(1 << 14)
#define	RX_SAF_FAIL		(1 << 13)
#define	RX_OVERFLOW_ERR		(1 << 11)
#define	RX_FIR_DESC		(1 << 9)
#define	RX_LAST_DESC		(1 << 8)
#define	RX_HEADER_ERR		(1 << 7)
#define	RX_COL_ERR		(1 << 6)
#define	RX_FRM_TYPE		(1 << 5)
#define	RX_LENGTH_ERR		(1 << 4)
#define	RX_PHY_ERR		(1 << 3)
#define	RX_CRC_ERR		(1 << 1)
#define	RX_PAYLOAD_ERR		(1 << 0)

/* Size Descriptor */
/* TX */
#define	TX_INT_CTL		(1 << 31)
#define	TX_LAST_DESC		(1 << 30)
#define	TX_FIR_DESC		(1 << 29)
#define	TX_CHECKSUM_CTL		(0x3 << 27)
#define	TX_CHECKSUM_CTL_IP	1
#define	TX_CHECKSUM_CTL_NO_PSE	2
#define	TX_CHECKSUM_CTL_FULL	3
#define	TX_CHECKSUM_CTL_SHIFT	27
#define	TX_CRC_CTL		(1 << 26)
#define	TX_BUF_SIZE		(0x7ff << 0)
#define	TX_BUF_SIZE_SHIFT	0
/* RX */
#define	RX_INT_CTL		(1 << 31)
#define	RX_CHAIN_DESC		(1 << 24)
#define	RX_BUF_SIZE		(0x7ff << 0)
#define	RX_BUF_SIZE_SHIFT	0


/* ETH_BASIC_CTL_0 */
enum {
	CtlSpeedShift	=	2,
	CtlSpeed10		=	2<<2,
	CtlSpeed100		=	3<<2,
	CtlSpeed1000	=	0<<2,
	CtlLoopback		=	1<<1,
	CtlDuplex		=	1<<0,
};

/* ETH_BASIC_CTL_1 */
enum {
	CtlBurstLen		=	0x3F<<24,
	CtlBurstShift	=	24,
	CtlRxPriortiy	=	1<<1,
	CtlSoftRst		=	1<<0,
};

/* ETH_INT_STA/EN */
enum {
	RGMIILinkSta	=	1<<16,
	RXEarly			=	1<<13,
	RXOverflow		=	1<<12,
	RXTimeout		=	1<<11,
	RXDMAStopped	=	1<<10,
	RXBufUa			=	1<<9,
	RXInt			=	1<<8,
	TXEarly			=	1<<5,
	TXUnderflow		=	1<<4,
	TXTimeout		=	1<<3,
	TXBufUa			=	1<<2,
	TXDMAStopped	=	1<<1,
	TXInt			=	1<<0,
};

/* ETH_TX_CTL_0 */
enum {
	TXEn			=	1<<31,
	TXFrmLen		=	1<<30,
};

/* ETH_TX_CTL_1 */
enum {
	TXDMAStart		=	1<<31,
	TXDMAEn			=	1<<30,
	TX_TH			=	0x7<<8,
	TXNextFrm		=	1<<2,
	TX_MD			=	1<<1,
	FlushTXFIFO		=	1<<0,
};

/* ETH_TX_FLOW_CTL */
enum {
	PauseTime		=	0xFFFF<<4,
	PauseTimeShift	=	4,
	TXFlowCtlEn		=	1<<0,
};

/* ETH_RX_CTL_0 */
enum {
	RXEn			=	1<<31,
	RXFrmLen		=	1<<30,
	JumboFrmEn		=	1<<29,
	StripFCS		=	1<<28,
	CheckCRC		=	1<<27,
	RXPauseRrm		=	1<<17,
	RXFlowCtlEn		=	1<<16,
};

/* ETH_RX_CTL_1 */
enum {
	RXDMAStart		=	1<<31,
	RXDMAEn			=	1<<30,
	RXFlowCtl		=	1<<24,
	RXFlowDeact		=	0x3<<22,
	RXFlowAct		=	0x3<<20,
	RX_TH			=	0x3<<4,
	RXErrFrm		=	1<<3,
	RXRuntFrm		=	1<<2,
	RX_MD			=	1<<1,
	FlushRXFrm		=	1<<0,
};

/* ETH_RX_FRM_FLT */
enum {
	DisAddrFlt		=	1<<31,
	DisBroadcast	=	1<<17,
	RXMulticast		=	1<<16,
	CtlFrmFlt		=	0x3<<12,
	CtlFrmFltShift	=	12,
	HashMulti		=	1<<9,
	HashUni			=	1<<8,
	SAFltEn			=	1<<6,
	SAInvFlt		=	1<<5,
	DAInvFlt		=	1<<4,
	FltMD			=	1<<1,
	RX_ALL			=	1<<0,
};

/* ETH_MII_CMD */
enum {
	DivRatio		=	0x7<<20,
	DivRatio16		=	0,
	DivRatio32		=	1,
	DivRatio64		=	2,
	DivRatio128		=	3,
	DivRatioShift	=	20,
	PhyAddr			=	0x1F<<12,
	PhyAddrShift	=	12,
	RegAddr			=	0x1F<<4,
	RegAddrShift	=	4,
	MiiWr			=	1<<1,
	MiiBusy			=	1<<0,
};

/* EMAC_CLK_REG */
enum {
	ClkRmiiEn		=	1<<13,
	ClkETXDC		=	0x7<<10,
	ClkETXDCShift	=	10,
	ClkERXDC		=	0x1F<<5,
	ClkERXDCShift	=	5,
	ClkPIT			=	0x1<<2,
	ClkPITMII		=	0<<2,
	ClkPITRGMII		=	1<<2,
	ClkSrc			=	0x3<<0,
	ClkSrcMII		=	0<<0,
	ClkSrcExtRGMII	=	1<<0,
	ClkSrcRGMII		=	2<<0,
};

/* TX desc status */





/* Debugging options */
enum{
	Miidebug	=	1,
	Ethdebug	=	1,
	Attchbug	=	1,
};


enum{
	Nrd		= 256,	/* Number rx descriptors */
	Ntd		= 256,	/* Number tx descriptors */
	Rbsz	= 2048,	/* block size */
	RxBuf	= 2044, /* rumors of problems at 2048 */
};


/* fine tuning for gigabit speed */
enum{
	TxDelay	=	0x3,
	RxDelay	=	0x0,
};


typedef struct Desc Desc;
typedef	struct Ctlr Ctlr;


struct Desc
{
	u32int	status;
	u32int	size;
	u32int	addr;
	u32int	next;
};


struct Ctlr
{
	int		attached;
	QLock;
	Ether	*edev;		/* point back */

	struct {
		Block	*b[Nrd];
		Desc	*d;
		Rendez;
		Lock;
	}	rx[1];

	struct {
		Block	*b[Ntd];
		Desc	*d;
		Rendez;
	}	tx[1];

//	Mii		*mii;
	struct {
		Mii *mii;
		int		done;
		Rendez;
	}	mdio[1];

	uint	divratio;

	QLock	statlock;
	int		rxstat;
	int		rxintr;
	int		rxdmaerr;
	int		txstat;
	int		txintr;
	int		txdmaerr;
	int		nointr;
	int		badrx;
};


static u32int
sysconrd(int offset)
{
	return *IO(u32int, (SYSCTL + offset));
}


static void
sysconwr(int offset, u32int val)
{
	*IO(u32int, (SYSCTL + offset)) = val;
}


static u32int
ethrd(int offset)
{
	return *IO(u32int, (EMAC + offset));
}


static void
ethwr(int offset, u32int val)
{
	*IO(u32int, (EMAC + offset)) = val;
}


static int
miird(Mii *mii, int pa, int ra)
{
	//int val = 0;
	int	timeout;
	u32int buf;

	if(Miidebug)
		iprint("miird, phy_addr; %d phy_reg: %d", pa, ra);

	ethwr(ETH_MII_CMD, 
		(DivRatio128 << DivRatioShift) | 
		(pa << PhyAddrShift) | (ra << RegAddrShift) | MiiBusy);

	for(timeout = 0; timeout < 2000; timeout++){
		if((ethrd(ETH_MII_CMD) & MiiBusy) == 0){
			buf = ethrd(ETH_MII_DATA) & 0xFFFF;
			if(Miidebug)
				iprint(" - %ulX\n", buf);
			//return (ethrd(ETH_MII_DATA) & 0xFFFF);
			return buf;
		}
		microdelay(100);
	}

	/* read failed */
	if(Miidebug)
		iprint("miird FAIL\n");
	return -1;
}


static int
miiwr(Mii *mii, int pa, int ra, int val)
{
	int timeout;

	if(Miidebug)
		iprint("miiwr, phy_addr; %d phy_reg: %d val: 0x%04X\n", pa, ra, val);

	ethwr(ETH_MII_DATA, val);
	ethwr(ETH_MII_CMD, 
		(DivRatio128 << DivRatioShift) | 
		(pa << PhyAddrShift) | (ra << RegAddrShift) | 
		MiiWr | MiiBusy);

	for(timeout = 0; timeout < 2000; timeout++){
		if((ethrd(ETH_MII_CMD) & MiiBusy) == 0)
			return 0;
		microdelay(100);
	}

	/* write failed */
	if(Miidebug)
		iprint("miiwr FAIL\n");
	return -1;

}


static int
rdfull(void *arg)
{
	Desc *d = arg;
	return (d->status & RX_DESC_CTL) == 0;
}


static void
rxproc(void *arg)
{
	Ether	*edev = arg;
	Ctlr	*ctlr = edev->ctlr;
	Block	*b;
	Desc	*d;
	int		len, i;



	i = 0;

	while(waserror())
		;

	for(;;){
		ctlr->rxstat++;

		d = &ctlr->rx->d[i];

		if((d->status & RX_DESC_CTL) != 0)
			sleep(ctlr->rx, rdfull, d);

		len = (d->status & RX_FRM_LEN) >> RX_FRM_LEN_SHIFT;	/* get length of packet */
		b = ctlr->rx->b[i];

		if(len > 0){
			b->wp = b->rp + len;
			dmaflush(0, b->rp, BLEN(b));	/* move block to ram */
			etheriq(edev, b);			/* move block to ether input queue */

		if(Ethdebug)
			iprint("rxproc: (%d) len=%d | ", i, len);

		} else {
			ctlr->badrx++;
			freeb(b);
		}

			/* replenish */
			b = allocb(Rbsz);
			if(b == nil)
				panic("NO RX BLOCKS");
			ctlr->rx->b[i] = b;
			dmaflush(1, b->rp, Rbsz);
			d->addr = PADDR(b->rp);	/* point to fresh block */
			d->status = RX_DESC_CTL;
			d->size = RxBuf;

		i = (i + 1) % Nrd;
	}
}


static int
tdfree(void *arg)
{
	Desc *d = arg;
	return (d->status & TX_DESC_CTL) == 0;
}


static void
txproc(void *arg)
{
	Ether *edev = arg;
	Ctlr *ctlr = edev->ctlr;
	Block *b;
	Desc *d;
	int i, len, Δlen;

	i = 0;

	while(waserror())
		;

	for(;;){
		ctlr->txstat++;
		if((b = qbread(edev->oq, 100000)) == nil)	/* fetch packet from queue */
			break;
		

		d = &ctlr->tx->d[i];
		while(!tdfree(d))
			sleep(ctlr->tx, tdfree, d);

		ilock(ctlr->tx);	/* helps with packet loss */
		if(ctlr->tx->b[i] != nil)
			freeb(ctlr->tx->b[i]);

		ctlr->tx->b[i] = b;
		len = BLEN(b);

		if(Ethdebug)
			iprint("txproc: (%d) len=%d | ", i, len);

		dmaflush(1, b->rp, Rbsz);	/* move packet to ram */
		d->addr = PADDR(b->rp);
		d->size = (len & TX_BUF_SIZE) | TX_FIR_DESC | TX_LAST_DESC | TX_INT_CTL;
		d->status = TX_DESC_CTL;
		coherence();

		i = (i + 1) % Ntd;;
		iunlock(ctlr->tx);
	}
}


static void
linkproc(void *arg)
{
	Ether *edev = arg;
	Ctlr *ctlr = edev->ctlr;
	MiiPhy *phy;
	int link = 0;
	u32int buf;

	while(waserror())
		;
	miiane(ctlr->mdio->mii, ~0, ~0, ~0);
	for(;;){
		miistatus(ctlr->mdio->mii);
		phy = ctlr->mdio->mii->curphy;
		if(phy->link == link){
			tsleep(ctlr->mdio, return0, nil, 5000);
			continue;
		}
		link = phy->link;
		if(link){
			/* Start Interface */
			ethwr(ETH_BASIC_CTL_0, CtlSpeed100 | CtlDuplex);

			buf = ethrd(ETH_RX_CTL_0);
			buf |= RXFlowCtlEn;
			ethwr(ETH_RX_CTL_0, buf);

			buf = ethrd(ETH_TX_FLOW_CTL);
			buf &= ~(PauseTime | TXFlowCtlEn);
			buf |= TXFlowCtlEn;
				ethwr(ETH_TX_FLOW_CTL, buf);
			coherence();

			edev->mbps = phy->speed;
		} 
		edev->link = link;
		print("#l%d: link %d speed %d\n", edev->ctlrno, edev->link, edev->mbps);
	}
}


static void
etherinterrupt(Ureg*, void *arg)
{
	Ether *edev = arg;
	Ctlr *ctlr = edev->ctlr;
	u32int irq;
	int rxintΔ, txintΔ;


	rxintΔ = ctlr->rxintr;
	txintΔ = ctlr->txintr;

	irq = ethrd(ETH_INT_STA);	/* get interrupt requests */
	ethwr(ETH_INT_STA, irq);	/* writing back 1's clears irqs */

	if(Ethdebug){
		iprint("ether interrupt: %08uX |", irq);
		delay(10);
	}


	if(irq & (RXInt)){
		ctlr->rxintr++;
		wakeup(ctlr->rx);
	}

	if(irq & (TXInt | TXBufUa)){
		ctlr->txintr++;
		wakeup(ctlr->tx);
	}

	if((rxintΔ == ctlr->rxintr) && (txintΔ == ctlr->txintr)){
		ctlr->nointr++;
		iprint("etherinterrupt: spurious %X\n", irq);
	}
}



static void
getmacaddr(Ether *edev)
{
	u32int msb, lsb;

	lsb = ethrd(ETH_ADDR_HIGH_0);
	msb = ethrd(ETH_ADDR_LOW_0);

	edev->ea[0] = msb>>8;
	edev->ea[1] = msb>>0;
	edev->ea[2]	= lsb>>24;
	edev->ea[3] = lsb>>16;
	edev->ea[4] = lsb>>8;
	edev->ea[5] = lsb>>0;

	if(Attchbug){
		iprint("ether getmac: %04lX %08lX\n", (msb & 0xFFFF), lsb);
		delay(10);
	}
}


static void
setmacaddr(Ether *edev)
{
	u32int msb, lsb;

	msb = edev->ea[1] | (edev->ea[0] << 8);
	lsb = edev->ea[5] | (edev->ea[4] << 8) 
		| (edev->ea[3] << 16) | (edev->ea[2] << 24);

	ethwr(ETH_ADDR_LOW_0, msb);
	ethwr(ETH_ADDR_HIGH_0, lsb);
}


static int
initmii(Ctlr *ctlr)
{
	MiiPhy *phy;
	Ether	*edev = ctlr->edev;
	int		i, buf;

	if((ctlr->mdio->mii = malloc(sizeof(Mii))) == nil)
		return -1;

	ctlr->mdio->mii->ctlr	= ctlr;
	ctlr->mdio->mii->mir	= miird;
	ctlr->mdio->mii->miw	= miiwr;

	if(mii(ctlr->mdio->mii, ~0) == 0 || (phy = ctlr->mdio->mii->curphy) == nil){
		iprint("#l%d: init mii failure\n", edev->ctlrno);
		free(ctlr->mdio->mii);
		ctlr->mdio->mii = nil;
		return -1;
	}
	
	iprint("#l%d: phy%d id %.8ux oui %x\n", 
		edev->ctlrno, ctlr->mdio->mii->curphy->phyno, 
		ctlr->mdio->mii->curphy->id, ctlr->mdio->mii->curphy->oui);

//	miireset(ctlr->mdio->mii);

	return 0;
}


static void
setupclocks(Ctlr *ctlr)
{
	u32int reg;

/*
 *	MDIO needs to talk at 2.5MHz, but
 *	the EMAC runs off the AHB which
 *	should default to 300MHz, so I'm
 *	hard coding the 128 ratio for mii
 */
	ctlr->divratio = DivRatio128;

	reg = sysconrd(EMAC_CLK_REG);

// just 100Mb for now

	print("syscon %ulX\n", reg);

	reg &= ~(ClkPIT | ClkSrc | ClkRmiiEn);
//	reg |= ClkPITRGMII | ClkSrcRGMII;
	reg |= ClkSrcMII | ClkPITMII;
//	reg |= ClkRmiiEn | ClkSrcExtRGMII;

	sysconwr(EMAC_CLK_REG, reg);
}


static void
attach(Ether *edev)
{
	int i;
	u32int	buf;
	Ctlr *ctlr;
	Desc *d;

	ctlr = edev->ctlr;

	if(Attchbug){
		iprint("ether attach called\n");
		delay(10);
	}

	qlock(ctlr);
	if(ctlr->attached){
		qunlock(ctlr);

		if(Attchbug){
			iprint("ether attach already?\n");
			delay(10);
		}

		return;
	}

	if(waserror()){
		qunlock(ctlr);

		if(Attchbug){
			iprint("ether attach waserror?\n");
			delay(10);
		}

		//free(ctlr->rx->d);
		//free(ctlr->tx->d);
		nexterror();
	}

	print("clocks-");
	setupclocks(ctlr);

	/* Allocate Rx/Tx ring with uncached memmory */
	ctlr->tx->d = ucalloc(sizeof(Desc) * Ntd);
	ctlr->rx->d = ucalloc(sizeof(Desc) * Nrd);

	print("rxblks-");
	/* Allocate Rx blocks, initialize Rx ring. */
	for(i = 0; i < Nrd; i++){
		Block *b = allocb(Rbsz);
		if(b == nil)
			error("rxblock");
		ctlr->rx->b[i] = b;
		dmaflush(1, b->rp, Rbsz);
		d = &ctlr->rx->d[i];
		d->addr = PADDR(b->rp);
		d->next = (uintptr)&ctlr->rx->d[i++];
		d->status = RX_DESC_CTL;
		d->size = RxBuf;
	}

	/* RX, connect last desc to the first */
	d->next = (uintptr)&ctlr->rx->d[0];

	d = nil;

	/* Initialize Tx ring */
	for(i = 0; i < Ntd; i++){
		ctlr->tx->b[i] = nil;
		d = &ctlr->tx->d[i];
		d->next = (uintptr)&ctlr->tx->d[i++];
		d->status = 0;
		d->size = 0;
	}

	/* TX, connect last desc to the first */
	d->next = (uintptr)&ctlr->tx->d[0];

	print("reset-");
	/* do a soft reset on the emac */
	ethwr(ETH_BASIC_CTL_1, CtlSoftRst);

	/* wait for reset bit to clear */
	for(i = 0; i < 1000; i++){
		if((ethrd(ETH_BASIC_CTL_1) & CtlSoftRst) == 0)
			break;
		delay(10);
	}

	setmacaddr(edev);

	/* DMA Burst, 8 is default */
	ethwr(ETH_BASIC_CTL_1, 8 << CtlBurstShift);

	/* feed in the descriptor rings */
	ethwr(ETH_TX_DMA_DESC_LIST, PADDR(ctlr->tx->d));
	ethwr(ETH_RX_DMA_DESC_LIST, PADDR(ctlr->rx->d));
	coherence();

	print("mii-");
	if(initmii(ctlr) < 0)
		error("mii failed");

	/* Enable Interrupts */
	ethwr(ETH_INT_EN, RXInt | TXInt | TXBufUa);

	/* Start DMA */
	ethwr(ETH_RX_CTL_1, RXDMAEn | RX_MD);
	ethwr(ETH_TX_CTL_1, TXDMAEn | TXNextFrm | TX_MD);

	/* Enable RX/TX */
	ethwr(ETH_RX_CTL_0, RXEn);
	ethwr(ETH_TX_CTL_0, TXEn);
	coherence();

	ctlr->attached = 1;

	print("kprocs-");
	kproc("ether-rx", rxproc, edev);
	kproc("ether-tx", txproc, edev);
//	kproc("ether-fr", frproc, edev);

	kproc("ether-link", linkproc, edev);

	qunlock(ctlr);
	poperror();
}


static void
shutdown(Ether *edev)
{
	USED(edev);
}


/* promiscuous stub */
static void
prom(void*, int)
{
}


/* multicast stub */
static void
multi(void*, uchar*, int)
{
}


static int
pnp(Ether *edev)
{
	static Ctlr ctlr[1];

	if(Attchbug)
		iprint("ether pnp called\n");

	if(edev->ctlr != nil)
		return -1;

	/* only one controller */
	if(edev->ctlrno != 0)
		return -1;

	ctlr->edev	=	edev;

//	ctlr->mdio->mii->ctlr	= ctlr;
//	ctlr->mdio->mii->mir	= miird;
//	ctlr->mdio->mii->miw	= miiwr;

	edev->port	= (uintptr)(VIRTIO|EMAC);
	edev->ctlr	= ctlr;
	edev->irq	= IRQemac;
	edev->mbps	= 1000;
//	edev->maxmtu = 1536;
	edev->arg	= edev;

	edev->attach = attach;
	edev->shutdown = shutdown;
//	edev->ifstat = ifstat;
//	edev->ctl = ctl;
	edev->promiscuous = prom;
	edev->multicast = multi;

	getmacaddr(edev);

	intrenable(edev->irq, etherinterrupt, edev, 0, edev->name);

	if(Attchbug)
		iprint("ether pnp done\n");

	return 0;
}



void
ethersunxilink(void)
{
	addethercard("ethersunxi", pnp);
}
