/*
 *	Reduced Serial Bus(RSB™)
 *	some special I²C bus from Allwinner
 *	used to talk to the power controller
 *
 *	cobbled together from documentation 
 *	for the Allwinner A80 and NetBSD
 */

#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"


#define RSB_CTRL_REG			0x0000
#define  RSB_CTRL_START_TRANS		1<<7
#define  RSB_CTRL_ABORT_TRANS		1<<6
#define  RSB_CTRL_GLOBAL_INT_ENB	1<<1
#define  RSB_CTRL_SOFT_RESET		1<<0
#define RSB_CCR_REG					0x0004
#define  RSB_CCR_SDA_ODLY			1<<8		//__BITS(10,8)
#define  RSB_CCR_CLK_DIV			1<<0		//__BITS(7,0)
#define RSB_INTE_REG				0x0008
#define  RSB_INTE_LOAD_BSY_ENB		1<<2
#define  RSB_INTE_TRANS_ERR_ENB		1<<1
#define  RSB_INTE_TRANS_OVER_ENB	1<<0
#define RSB_STAT_REG				0x000c
#define  RSB_STAT_TRANS_ERR_ID		1<<8		//__BITS(15,8)
#define  RSB_STAT_LOAD_BSY			1<<2
#define  RSB_STAT_TRANS_ERR			1<<1
#define  RSB_STAT_TRANS_OVER		1<<0
#define  RSB_STAT_MASK		\
	(RSB_STAT_LOAD_BSY |	\
	 RSB_STAT_TRANS_ERR |	\
	 RSB_STAT_TRANS_OVER)
#define RSB_DADDR0_REG				0x0010
#define RSB_DADDR1_REG				0x0014
#define RSB_DLEN_REG				0x0018
#define  RSB_DLEN_READ_WRITE_FLAG	1<<4
#define  RSB_DLEN_ACCESS_LENGTH(v)	(v)<<0		//__BITS(2,0)
#define RSB_DATA0_REG				0x001c
#define RSB_DATA1_REG				0x0020
#define RSB_LCR_REG					0x0024
#define  RSB_LCR_SCL_STATE			1<<5
#define  RSB_LCR_SDA_STATE			1<<4
#define  RSB_LCR_SCL_CTL			1<<3
#define  RSB_LCR_SCL_CTL_EN			1<<2
#define  RSB_LCR_SDA_CTL			1<<1
#define  RSB_LCR_SDA_CTL_EN			1<<0
#define RSB_PMCR_REG				0x0028
#define  RSB_PMCR_PMU_INIT_SEND		1<<31
#define  RSB_PMCR_PMU_INIT_DATA		1<<16		//__BITS(23,16)
#define  RSB_PMCR_PMU_MODE_CTRL_REG_ADDR 1<<8	//__BITS(15,8)
#define  RSB_PMCR_PMU_DEVICE_ADDR	1<<0		//__BITS(7,0)
#define RSB_CMD_REG					0x002c
#define  RSB_CMD_IDX				1<<0		//__BITS(7,0)
#define   RSB_CMD_IDX_SRTA			0xe8
#define   RSB_CMD_IDX_RD8			0x8b
#define   RSB_CMD_IDX_RD16			0x9c
#define   RSB_CMD_IDX_RD32			0xa6
#define   RSB_CMD_IDX_WR8			0x4e
#define   RSB_CMD_IDX_WR16			0x59
#define   RSB_CMD_IDX_WR32			0x63
#define RSB_DAR_REG					0x0030
#define  RSB_DAR_RTA(x)				(x)<<16		//__BITS(23,16)
#define  RSB_DAR_DA(x)				(x)<<0		//__BITS(15,0)




static u32int
rsbrd(u32int offset)
{
	return *IO(u32int, (R_RSB + offset));
}


static void
rsbwr(int offset, u32int val)
{
	*IO(u32int, (R_RSB + offset)) = val;
}


static int
rsbreset(void)
{
	int timeout = 1000;

	rsbwr(RSB_CTRL_REG, RSB_CTRL_SOFT_RESET);

	while(timeout > 0){
		if((rsbrd(RSB_CTRL_REG) & RSB_CTRL_SOFT_RESET) == 0)
			break;
		timeout--;
		delay(10);
	}

	if(timeout == 0){
		return -1;
	}

	return 1;
}


static int
rsbidle(void)
{
	u32int buf;
	int timeout = 1000;

	while(timeout > 0){
		buf = rsbrd(RSB_CTRL_REG);
		if((buf & RSB_CTRL_START_TRANS) == 0){
			return 1;}
		timeout--;
		delay(10);
	}

	/* not idle */
	return -1;
}


static int
rsb_xmit(void)
{
	u32int buf;

	/* set interrupts */
	rsbwr(RSB_INTE_REG, RSB_INTE_LOAD_BSY_ENB | RSB_INTE_TRANS_ERR_ENB | RSB_INTE_TRANS_OVER_ENB);

	/* start transfer with interrupts enabled */
	rsbwr(RSB_CTRL_REG, RSB_CTRL_START_TRANS | RSB_CTRL_GLOBAL_INT_ENB);

	/* wait for the transmission to finish */
	if(!rsbidle()){
		iprint("RSB: read timeout\n");
		return 0;
	}

	/* check for errors */
	buf = rsbrd(RSB_STAT_REG);
	if(buf & RSB_STAT_TRANS_ERR){
		iprint("RSB: read transmit error\n");
		return 0;
	}
	if(buf & RSB_STAT_LOAD_BSY){
		iprint("RSB: read busy error\n");
		return 0;
	}

	return 1;
}



u32int
rsb_read(u8int rtaddr, u16int devaddr, u8int reg, uint bytes)
{
	u32int buf, cmd;

	if(!rsbreset()){
		print("RSB: read reset FAIL\n");
		return 0;
	}

	/* set command for READ and byte size */
	switch(bytes){
	case(1):
		cmd = RSB_CMD_IDX_RD8;
		break;
	case(2):
		cmd = RSB_CMD_IDX_RD16;
		break;
	case(4):
		cmd = RSB_CMD_IDX_RD32;
		break;
	default:
		print("RSB: invalid read size\n");
		return 0;
	}

	/* Make sure the controller is idle */
	if(!rsbidle()){
		print("RSB: read not idle\n");
		return 0;
	}

	/* set rtaddr and devaddr for the device to talk to */
	rsbwr(RSB_DAR_REG, (rtaddr << 16) | devaddr);
	rsbwr(RSB_CMD_REG, RSB_CMD_IDX_SRTA);

	/* set the register in the device to be read */
	rsbwr(RSB_DADDR0_REG, reg);

	/* set the command for the action to perform */
	rsbwr(RSB_CMD_REG, cmd);

	/* run transmit and error check */
	if(rsb_xmit())
		return rsbrd(RSB_DATA0_REG);

	return 0;
}


u32int
rsb_write(u8int rtaddr, u16int devaddr, u8int reg, u32int val, uint bytes)
{
	u32int buf, cmd;

	if(!rsbreset()){
		iprint("RSB: write reset FAIL\n");
		return 0;
	}

	/* set command for WRITE and byte size */
	switch(bytes){
	case(1):
		cmd = RSB_CMD_IDX_WR8;
		break;
	case(2):
		cmd = RSB_CMD_IDX_WR16;
		break;
	case(4):
		cmd = RSB_CMD_IDX_WR32;
		break;
	default:
		print("RSB: invalid write size\n");
		return 0;
	}

	/* Make sure the controller is idle */
	if(!rsbidle()){
		iprint("RSB: write not idle\n");
		return 0;
	}

	/* set rtaddr and devaddr for the device to talk to */
	rsbwr(RSB_DAR_REG, (rtaddr << 16) | devaddr);
	rsbwr(RSB_CMD_REG, RSB_CMD_IDX_SRTA);

	/* set the register in the device to be writen to */
	rsbwr(RSB_DADDR0_REG, reg);

	/* set data to write to device register */
	rsbwr(RSB_DATA0_REG, val);

	/* set the command for the action to perform */
	rsbwr(RSB_CMD_REG, cmd);

	/* run transmit and error check */
	if(rsb_xmit())
		return 1;

	iprint("rsb_write: failed\n");
	return 0;
}
