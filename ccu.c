#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "io.h"
#include "ccu.h"



static u32int
ccurd(int offset)
{
	return *IO(u32int, (CCUBASE + offset));
}


static void
ccuwr(int offset, u32int val)
{
	*IO(u32int, (CCUBASE + offset)) = val;
}


char*
listgates(int i)
{
	Gate *g = gates;
	u32int v;
	char p[32];
	int l, m;

	g += i;

	if(g->name == nil)
		return nil;
	
	v = ccurd(BUS_CLK_GATING_REG0 + g->bank);
	m = (v & g->mask ? 1 : 0);

	print("%s-\n", g->name);
	print("%d-\n", m);

	snprint(p, sizeof(p), "%s %d", g->name, m);
	
	return p;
}


char*
getgatename(int i)
{
	Gate *g = gates;

	g += i;

	return g->name;
}


int
getgatestate(int i)
{
	Gate *g = gates;
	int r, s;

	g += i;

	r = ccurd(BUS_CLK_GATING_REG0 + g->bank);
	s = (r & g->mask ? 1 : 0);

	return s;
}


char*
getresetname(int i)
{
	Gate *g = gates;

	g += i;

	return g->name;
}


int
getresetstate(int i)
{
	Gate *g = gates;
	int r, s;

	g += i;

	r = ccurd(BUS_SOFT_RST_REG0 + g->bank);
	s = (r & g->mask ? 1 : 0);

	return s;
}


void
debuggates(void)
{
	print("%ulb\n", ccurd(BUS_CLK_GATING_REG0));
	print("%ulb\n", ccurd(BUS_CLK_GATING_REG1));
	print("%ulb\n", ccurd(BUS_CLK_GATING_REG2));
	print("%ulb\n", ccurd(BUS_CLK_GATING_REG3));
	print("%ulb\n", ccurd(BUS_CLK_GATING_REG4));
}

u32int
getcpuclk_n(void)
{
	u32int n;

	n = ccurd(PLL_CPUX_CTRL_REG);
	n = (n & 0x1F00) >> 8;
	return n;
}

u32int
getcpuclk_k(void)
{
	u32int k;

	k = ccurd(PLL_CPUX_CTRL_REG);
	k = (k & 0x30) >> 4;
	return k;
}

u32int
getcpuclk_m(void)
{
	u32int m;

	m = ccurd(PLL_CPUX_CTRL_REG);
	m = (m & 0x3);
	return m;
}

u32int
getcpuclk_p(void)
{
	u32int p;

	p = ccurd(PLL_CPUX_CTRL_REG);
	p = (p & 0x30000) >> 16;
	return p;
}


static Nkmp*
findcpuclk(uint findrate)
{
	Nkmp *nkmp;

	for(nkmp = cpu_nkmp; nkmp->rate != 0; nkmp++){
		if(nkmp->rate == findrate)
			return nkmp;
	}

	return nil;
}

static Nkmp*
findcpuclk_n(u32int findn)
{
	Nkmp *nkmp;

	for(nkmp = cpu_nkmp; nkmp->rate != 0; nkmp++){
		if(nkmp->n == findn)
			return nkmp;
	}

	return nil;
}



int
setcpuclk_n(u32int setn)
{
	Nkmp *nkmp;
	u32int reg;

	nkmp = findcpuclk_n(setn);

	if(nkmp == nil){
		iprint("setcpuclk_n found nil\n");
		return -1;
	}

	reg = ccurd(PLL_CPUX_CTRL_REG);
	reg &= 0xFFFC0000;
	reg |= (nkmp->n << 8) & 0x1F00;
	reg |= (nkmp->k << 4) & 0x30;
	reg |= (nkmp->m << 0) & 0x3;
	reg |= (nkmp->p << 16) & 0x30000;

	ccuwr(PLL_CPUX_CTRL_REG, reg);

	return 1;
}


int
setcpuclk(uint setrate)
{
	Nkmp *nkmp;
	u32int reg;

	nkmp = findcpuclk(setrate);

	if(nkmp == nil){
		iprint("setcpuclk found nil\n");
		return -1;
	}

	reg = ccurd(PLL_CPUX_CTRL_REG);
	reg &= 0xFFFC0000;
	reg |= (nkmp->n << 8) & 0x1F00;
	reg |= (nkmp->k << 4) & 0x30;
	reg |= (nkmp->m << 0) & 0x3;
	reg |= (nkmp->p << 16) & 0x30000;

	ccuwr(PLL_CPUX_CTRL_REG, reg);

	return 1;
}

void
turnonts(void)
{
	u32int	buf;

	ccuwr(THS_CLK_REG, 0x80000000);

	buf = ccurd(BUS_SOFT_RST_REG3);
	buf |= 1<<8;
	ccuwr(BUS_SOFT_RST_REG3, buf);

	buf = ccurd(BUS_CLK_GATING_REG2);
	buf |= 1<<8;
	ccuwr(BUS_CLK_GATING_REG2, buf);
}
