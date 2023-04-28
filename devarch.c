#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"
#include "io.h"

enum {
	Qdir = 0,
	Qbase,

	Qmax = 16,
};

typedef long Rdwrfn(Chan*, void*, long, vlong);

static Rdwrfn *readfn[Qmax];
static Rdwrfn *writefn[Qmax];

static Dirtab archdir[Qmax] = {
	".",		{ Qdir, 0, QTDIR },	0,	0555,
};

Lock archwlock;	/* the lock is only for changing archdir */
QLock plock;
int narchdir = Qbase;

/*
 * Add a file to the #P listing.  Once added, you can't delete it.
 * You can't add a file with the same name as one already there,
 * and you get a pointer to the Dirtab entry so you can do things
 * like change the Qid version.  Changing the Qid path is disallowed.
 */
Dirtab*
addarchfile(char *name, int perm, Rdwrfn *rdfn, Rdwrfn *wrfn)
{
	int i;
	Dirtab d;
	Dirtab *dp;

	memset(&d, 0, sizeof d);
	strcpy(d.name, name);
	d.perm = perm;

	lock(&archwlock);
	if(narchdir >= Qmax){
		unlock(&archwlock);
		return nil;
	}

	for(i=0; i<narchdir; i++)
		if(strcmp(archdir[i].name, name) == 0){
			unlock(&archwlock);
			return nil;
		}

	d.qid.path = narchdir;
	archdir[narchdir] = d;
	readfn[narchdir] = rdfn;
	writefn[narchdir] = wrfn;
	dp = &archdir[narchdir++];
	unlock(&archwlock);

	return dp;
}

static Chan*
archattach(char* spec)
{
	return devattach('P', spec);
}

Walkqid*
archwalk(Chan* c, Chan *nc, char** name, int nname)
{
	return devwalk(c, nc, name, nname, archdir, narchdir, devgen);
}

static int
archstat(Chan* c, uchar* dp, int n)
{
	return devstat(c, dp, n, archdir, narchdir, devgen);
}

static Chan*
archopen(Chan* c, int omode)
{
	return devopen(c, omode, archdir, narchdir, devgen);
}

static void
archclose(Chan*)
{
}

static long
archread(Chan *c, void *a, long n, vlong offset)
{
	Rdwrfn *fn;

	switch((ulong)c->qid.path){
	case Qdir:
		return devdirread(c, a, n, archdir, narchdir, devgen);

	default:
		if(c->qid.path < narchdir && (fn = readfn[c->qid.path]))
			return fn(c, a, n, offset);
		error(Eperm);
		break;
	}

	return 0;
}

static long
archwrite(Chan *c, void *a, long n, vlong offset)
{
	Rdwrfn *fn;

	if(c->qid.path < narchdir && (fn = writefn[c->qid.path]))
		return fn(c, a, n, offset);
	error(Eperm);

	return 0;
}

void archinit(void);

Dev archdevtab = {
	'P',
	"arch",

	devreset,
	archinit,
	devshutdown,
	archattach,
	archwalk,
	archstat,
	archopen,
	devcreate,
	archclose,
	archread,
	devbread,
	archwrite,
	devbwrite,
	devremove,
	devwstat,
};

static long
keyadcread(Chan*, void *a, long n, vlong offset)
{
	char str[32];
	uint v = getkeyadc();
	snprint(str, sizeof str, "%ud\n", v);
	return readstr(offset, a, n, str);
}

static long
keyadceventread(Chan*, void *a, long n, vlong offset)
{
	char str[32];
	int l;
	char *s = getkeyadc_event();
	uint v = getkeyadc();
	snprint(str, sizeof str, "%s %ud\n", s, v);
	l = readstr(offset, a, n, str);
	return n;
}

static long
pllgatesread(Chan*, void *a, long n, vlong offset)
{
	char *p, *g;
	
	int i, l, s;

	p = smalloc(READSTR);

	l = 0;
	qlock(&plock);
	for(i = 0; i < 45; i++){
		g = getgatename(i);
		if(g == nil)
			continue;
		s = getgatestate(i);
		l += snprint(p+l, READSTR-l, "%s = %d\n", g, s);
	}

	n = readstr(offset, a, n, p);
	free(p);
	qunlock(&plock);
	return n;
}

static long
pllresetread(Chan*, void *a, long n, vlong offset)
{
	char *p, *g;
	
	int i, l, s;

	p = smalloc(READSTR);

	l = 0;
	qlock(&plock);
	for(i = 0; i < 45; i++){
		g = getresetname(i);
		if(g == nil)
			continue;
		s = getresetstate(i);
		l += snprint(p+l, READSTR-l, "%s = %d\n", g, s);
	}

	n = readstr(offset, a, n, p);
	free(p);
	qunlock(&plock);
	return n;
}


static long
cpuclkread(Chan*, void *a, long z, vlong offset)
{
	char str[128];
	
	u32int n, k, m, p, mhz;

	n = getcpuclk_n();
	k = getcpuclk_k();
	m = getcpuclk_m();
	p = getcpuclk_p();
	mhz = (24*(n+1)*(k+1))/((m+1)*(p+1));

	snprint(str, sizeof str, "n=%d k=%d m=%d p=%d %dMHz\n", n, k, m, p, mhz);
	z = readstr(offset, a, n, str);
	return z;
}

static long
cpuclkwrite(Chan*, void *a, long z, vlong offset)
{
	Cmdbuf *cb;
	u32int v;

	cb = parsecmd(a, z);
	if(waserror()){
		free(cb);
		nexterror();
	}

	v = atoi(cb->f[0]);

	if(v < 0 || v > 31)
		error("must be an integer from 0 to 31");

	if(setcpuclk_n((u32int)v) != 1)
		error("writing to clock failed");

	free(cb);
	USED(offset);
	poperror();
	return z;
}

static long
cputempread(Chan*, void *a, long n, vlong offset)
{
	char str[128];
	int α, β, γ;
	α = gettemp0();
	β = gettemp1();
	γ = gettemp2();

	snprint(str, sizeof(str), "%d\n%d\n%d\n",α ,β ,γ);
	return readstr(offset, a, n, str);
}

static long
irqonread(Chan*, void *a, long n, vlong offset)
{
	char *p;
	
	int i, l, s;

	p = smalloc(READSTR);

	l = 0;
	qlock(&plock);
	for(i = 0; i <= 160; i++){
		if(willitintr(i))
			l += snprint(p+l, READSTR-l, "%d\n", i);
	}

	n = readstr(offset, a, n, p);
	free(p);
	qunlock(&plock);
	return n;
}

void
archinit(void)
{
	keyadcinit();
	thermalinit();
	
	addarchfile("keyadc", 0444, keyadcread, nil);
	addarchfile("keyadc_event", 0444, keyadceventread, nil);
	addarchfile("pllgates", 0444, pllgatesread, nil);
	addarchfile("pllreset", 0444, pllresetread, nil);
	addarchfile("cpuclk", 0664, cpuclkread, cpuclkwrite);
	addarchfile("cputemp", 0444, cputempread, nil);
	addarchfile("irqon", 0444, irqonread, nil);
}

