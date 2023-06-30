

typedef struct Pmicregs Pmicregs;


struct Pmicregs{
	char	*name;
	u8int	onoffreg;
	u8int	onoffbit;
	u8int	voltreg;
	u8int	voltmask;
	int		voltbase;	/* ints in mV */
	int		voltstep1;
	int		voltsplit;
	int		voltstep2;
	int		voltmax;
};


static Pmicregs pmicregs[] = {
	{"DCDC6",	0x10,	5,	0x25,	0x7F,	600,	10,		1100,	20,		1520},
	{"DCDC5",	0x10,	4,	0x24,	0x7F,	800,	10,		1200,	20,		1840},
	{"DCDC4",	0x10,	3,	0x23,	0x7F,	500,	10,		1200,	20,		1300},
	{"DCDC3",	0x10,	2,	0x22,	0x7F,	500,	10,		1200,	20,		1300},
	{"DCDC2",	0x10,	1,	0x21,	0x7F,	500,	10,		1200,	20,		1300},
	{"DCDC1",	0x10,	0,	0x20,	0x1F,	1600,	100,	3400,	0,		3400},
	{"DC1SW",	0x12,	7,	0xFF,	0x00,	0,		0,		0,		0,		0},		// no volt register?
	{"DLDO4",	0x12,	6,	0x18,	0x1F,	700,	100,	3300,	0,		3300},
	{"DLDO3",	0x12,	5,	0x17,	0x1F,	700,	100,	3300,	0,		3300},
	{"DLDO2",	0x12,	4,	0x16,	0x1F,	700,	100,	3400,	200,	4200},
	{"DLDO1",	0x12,	3,	0x15,	0x1F,	700,	100,	3300,	0,		3300},
	{"ELDO3",	0x12,	2,	0x1B,	0x1F,	700,	50,		1900,	0,		1900},
	{"ELDO2",	0x12,	1,	0x1A,	0x1F,	700,	50,		1900,	0,		1900},
	{"ELDO1",	0x12,	0,	0x19,	0x1F,	700,	50,		1900,	0,		1900},
	{"ALDO3",	0x13,	7,	0x2A,	0x1F,	700,	100,	3300,	0,		3300},
	{"ALDO2",	0x13,	6,	0x29,	0x1F,	700,	100,	3300,	0,		3300},
	{"ALDO1",	0x13,	5,	0X28,	0x1F,	700,	100,	3300,	0,		3300},
	{"FLDO2",	0x13,	3,	0x1D,	0x07,	700,	50,		1450,	0,		1450},
	{"FLDO1",	0x13,	2,	0x1C,	0x07,	700,	50,		1450,	0,		1450},
	{ nil },
};

