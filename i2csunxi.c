#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"
#include "../port/i2c.h"


enum (
	TWI_ADDR	=	0x0000,	/*TWI Slave address*/
	TWI_XADDR	=	0x0004,	/*TWI Extended slave address*/
	TWI_DATA	=	0x0008,	/*TWI Data byte*/
	TWI_CNTR	=	0x000C,	/*TWI Control register*/
	TWI_STAT	=	0x0010,	/*TWI Status register*/
	TWI_CCR		=	0x0014,	/*TWI Clock control register*/
	TWI_SRST	=	0x0018,	/*TWI Software reset*/
	TWI_EFR		=	0x001C,	/*TWI Enhance Feature register*/
	TWI_LCR		=	0x0020,	/*TWI Line Control register*/
};


typedef struct Ctlr Ctlr;
struct Ctlr
{
	void	*regs;
	int	irq;

	Rendez;
};





static Ctlr ctlr0 = {
	.regs = (void*)(VIRTIO + TWI0),
	.irq = IRQtwi0,
};
static Ctlr ctlr1 = {
	.regs = (void*)(VIRTIO + TWI1),
	.irq = IRQtwi1,
};
static Ctlr ctlr2 = {
	.regs = (void*)(VIRTIO + TWI2),
	.irq = IRQtwi2,
};

void
i2csunxilink(void)
{
	static I2Cbus i2c0 = { "i2c0", 400000, &ctlr0, init, io };
	static I2Cbus i2c1 = { "i2c1", 400000, &ctlr1, init, io };
	static I2Cbus i2c2 = { "i2c2", 400000, &ctlr2, init, io };

	addi2cbus(&i2c0);
	addi2cbus(&i2c1);
	addi2cbus(&i2c2);
}