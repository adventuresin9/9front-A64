#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"

/* keyadc in a64 manual: page 273 */

#define KEYREG       (VIRTIO + KEYADC)
#define KEYADC_CTRL  (KEYREG + 0x00)
#define KEYADC_INTC  (KEYREG + 0x04)
#define KEYADC_INTS  (KEYREG + 0x08)
#define KEYADC_DATA  (KEYREG + 0x0c)

#define KEYADC_MAX   (0x3f)

/* KEYADC_CTRL */
#define FIRST_CONVERT_DLY(x)      ((x) << 24)    /* 8 bits */
#define CONTINUE_TIME_SELECT(x)   ((x) << 16)    /* 4 bits */
#define KEY_MODE_SELECT(x)        ((x) << 12)    /* 2 bits */
#define LEVELA_B_CNT(x)           ((x) << 8)     /* 4 bits */
#define KEYADC_HOLD_KEY_EN(x)     ((x) << 7)     /* 1 bit  */
#define KEYADC_HOLD_EN(x)         ((x) << 6)     /* 1 bit  */
#define LEVELB_VOL(x)             ((x) << 4)     /* 2 bits */
#define KEYADC_SAMPLE_RATE(x)     ((x) << 2)     /* 2 bits */
#define KEYADC_EN(x)              ((x) << 0)     /* 1 bit  */

/* KEYADC_INTC */
#define ADC_KEYUP_IRQ_EN(x)       ((x) << 4)
#define ADC_ALRDY_HOLD_IRQ_EN(x)  ((x) << 3)
#define ADC_HOLD_IRQ_EN(x)        ((x) << 2)
#define ADC_KEYDOWN_IRQ_EN(x)         ((x) << 1)
#define ADC_DATA_IRQ_EN(x)        ((x) << 0)

/* KEYADC_INTS */
#define ADC_KEYUP_PENDING(x)      ((x) << 4)
#define ADC_ALRDY_HOLD_PENDING(x) ((x) << 3)
#define ADC_HOLDKEY_PENDING(x)    ((x) << 2)
#define ADC_KEYDOWN_PENDING(x)    ((x) << 1)
#define ADC_DATA_PENDING(x)       ((x) << 0)

enum {
	Eunknown,
	Ekeyup,
	Ealreadyhold,
	Eholdkey,
	Ekeydown,
	Edata,
};

char *enames[] = {
	"nil",
	"keyup",
	"alreadyhold",
	"holdkey",
	"keydown",
	"data",
};


/* to get voltage: keyadc/KEYADC_MAX * vref */
uint keyadc;
uint keyadc_event;
int interrupt;
uint vref;

uint
getkeyadc()
{
	u32int *data = (u32int*)KEYADC_DATA;
	keyadc = *data & KEYADC_MAX;
	
	keyadc = keyadc / KEYADC_MAX * vref;
	return keyadc;
}

char*
getkeyadc_event()
{
	/* wait for event */
	while (!interrupt)
		;
	char *s = enames[keyadc_event];
	interrupt = 0;
	return s;
}

static uint
ints2event(u32int *ints)
{
	if (*ints & ADC_KEYUP_PENDING(1))
		return Ekeyup;
	if (*ints & ADC_ALRDY_HOLD_PENDING(1))
		return Ealreadyhold;
	if (*ints & ADC_HOLDKEY_PENDING(1))
		return Eholdkey;
	if (*ints & ADC_KEYDOWN_PENDING(1))
		return Ekeydown;
	if (*ints & ADC_DATA_PENDING(1))
		return Edata;
	return Eunknown;
}


static void
localkeyadcintr(Ureg *ureg, void*)
{
	u32int *ints = (u32int*)KEYADC_INTS;
	u32int *data = (u32int*)KEYADC_DATA;
	keyadc = *data & KEYADC_MAX;
	
	keyadc_event = ints2event(ints);
	
	print("keyadc interrupt: %ud\n", keyadc);
	
	*ints = *ints;
	interrupt = 1;
}

void
keyadcinit()
{
	vref = 3000 * 2 / 3; // assume 3V
	
	u32int *reg = (u32int*)KEYADC_CTRL;
	*reg = FIRST_CONVERT_DLY(2)
	     | LEVELA_B_CNT(1)
	     | KEYADC_HOLD_EN(1)
	     | KEYADC_SAMPLE_RATE(0)
	     | KEYADC_EN(1);
	
	reg = (u32int*)KEYADC_INTC;
	*reg = ADC_KEYUP_IRQ_EN(1) | ADC_KEYDOWN_IRQ_EN(1);
	
	localkeyadcintr(nil, nil);

	intrenable(IRQkeyadc, localkeyadcintr, nil, BUSUNKNOWN, "keyadc");
}

