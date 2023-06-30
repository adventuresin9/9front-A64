#include "u.h"
#include "../port/lib.h"
#include "io.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"

#define RTC_YYMMDD  (0x10)
#define RTC_HHMMSS  (0x14)

#define LOSC_CTRL_REG        (0x00)
#define LOSC_OUT_GATING_REG  (0x60)

#define INTOSC_CLK_PRESCAL_REG (0x8)

#define BITS(n) ((1<<n)-1)

#define SEC2MIN  (60L)
#define SEC2HOUR (60L*SEC2MIN)
#define SEC2DAY  (24L*SEC2HOUR)

typedef struct Rtc Rtc;
struct Rtc
{
	int sec;
	int min;
	int hour;
	int mday;
	int wday;
	int mon;
	int year;
};

static ulong rtc2sec(Rtc*);
static void sec2rtc(ulong, Rtc*);

// days per month plus days/year
static int dmsize[] = 
{
	365, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
static int ldmsize[] =
{
	366, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int*
yrsize(int y)
{
	if ((y%4) == 0 && ((y%100) != 0 || (y%400) == 0))
		return ldmsize;
	else
		return dmsize;
}

long
readtime(void)
{
	Rtc rtc;
	
	u32int dreg = *IO(u32int, (RTC + RTC_YYMMDD));
	u32int treg = *IO(u32int, (RTC + RTC_HHMMSS));
	
	rtc.mday   = (dreg >>  0) & BITS(5);
	rtc.mon = (dreg >>  8) & BITS(4);
	rtc.year  = (dreg >> 16) & BITS(6);
	
	rtc.sec  = (treg >>  0) & BITS(6);
	rtc.min  = (treg >>  8) & BITS(6);
	rtc.hour = (treg >> 16) & BITS(5);
	
	rtc.year += 2010;
	
	return rtc2sec(&rtc);
}

enum {
	Qdir = 0,
	Qrtc,
};

Dirtab rtcdir[] = {
	".",   {Qdir, 0, QTDIR}, 0, 0555,
	"rtc", {Qrtc, 0},        0, 0664,
};

long
rtctime(void)
{
	int i;
	long t, ot;
	
	t = readtime();
	for (i = 0; i < 100; i++) {
		ot = t;
		t = readtime();
		if (ot == t)
			break;
	}
	if (i == 100)
		print("we are boofheads\n");
	
	return t;
}

static long
rtcread(Chan *c, void *buf, long n, vlong off)
{
	ulong offset = off;
	
	if (c->qid.type & QTDIR)
		return devdirread(c, buf, n, rtcdir, nelem(rtcdir), devgen);
	
	switch ((ulong)c->qid.path) {
	case Qrtc:
		return readnum(offset, buf, n, rtctime(), 12);
	}
	error(Ebadarg);
	return 0;
}

static long
rtcwrite(Chan *c, void *buf, long n, vlong off)
{
	Rtc rtc;
	ulong secs;
	ulong num;
	uint leap = 0;
	char *cp, sbuf[32];
	ulong offset = off;
	
	if (offset != 0)
		error(Ebadarg);
	
	switch((ulong)c->qid.path) {
	case Qrtc:
		if (n >= sizeof(sbuf))
			error(Ebadarg);
		strncpy(sbuf, buf, n);
		sbuf[n] = '\0';
		for (cp = sbuf; *cp != '\0'; cp++)
			if (*cp >= '0' && *cp <= '9')
				break;
		secs = strtoul(cp, 0, 0);
		
		sec2rtc(secs, &rtc);
		
		/* leap year */
		if (yrsize(rtc.year) == ldmsize)
			leap = 1;
		
		rtc.year -= 2010;
		
		num = (leap << 22)
			| (((rtc.mday) & BITS(5)) << 0)
			| (((rtc.mon)  & BITS(4)) << 8)
			| (((rtc.year) & BITS(6)) << 16);
		*IO(ulong, (RTC + RTC_YYMMDD)) = num;
		
		while (*IO(u32int, (RTC + LOSC_CTRL_REG)) & (1 << 7))
			;
		
		num = 0
			| ((rtc.sec  & BITS(6)) << 0)
			| ((rtc.min  & BITS(6)) << 8)
			| ((rtc.hour & BITS(5)) << 16)
			| ((rtc.wday & BITS(3)) << 29);
		*IO(u32int, (RTC + RTC_HHMMSS)) = num;
		
		while (*IO(u32int, (RTC + LOSC_CTRL_REG)) & (1 << 8))
			;
		
		return n;
	}
	
	error(Ebadarg);
	return 0;
}

static Chan*
rtcattach(char *spec)
{
	u32int ctrl = 0x16aa << 16; // magic KEY_FIELD
	ctrl |= 1 << 14; // LOSC_AUTO_SWT_EN
	*IO(u32int, (RTC + LOSC_CTRL_REG)) = ctrl;
	
	*IO(u32int, (RTC + LOSC_OUT_GATING_REG)) = 0x0;
	*IO(u32int, (RTC + INTOSC_CLK_PRESCAL_REG)) = 0xf;

	return devattach('r', spec);
}

static Walkqid*
rtcwalk(Chan *c, Chan *nc, char **name, int nname)
{
	return devwalk(c, nc, name, nname, rtcdir, nelem(rtcdir), devgen);
}

static int
rtcstat(Chan *c, uchar *dp, int n)
{
	return devstat(c, dp, n, rtcdir, nelem(rtcdir), devgen);
}

static Chan*
rtcopen(Chan *c, int omode)
{
	omode = openmode(omode);
	switch ((ulong)c->qid.path) {
	case Qrtc:
		if (strcmp(up->user, eve) != 0 && omode != OREAD)
			error(Eperm);
		break;
	}
	return devopen(c, omode, rtcdir, nelem(rtcdir), devgen);
}

static void
rtcclose(Chan*)
{
}

Dev rtcdevtab = {
	'r',
	"rtc",
	
	devreset,
	devinit,
	devshutdown,
	rtcattach,
	rtcwalk,
	rtcstat,
	rtcopen,
	devcreate,
	rtcclose,
	rtcread,
	devbread,
	rtcwrite,
	devbwrite,
	devremove,
	devwstat,
};

static ulong
rtc2sec(Rtc *rtc)
{
	ulong secs;
	int i;
	int *d2m;

	secs = 0;

	/*
	 *  seconds per year
	 */
	for(i = 1970; i < rtc->year; i++){
		d2m = yrsize(i);
		secs += d2m[0] * SEC2DAY;
	}

	/*
	 *  seconds per month
	 */
	d2m = yrsize(rtc->year);
	for(i = 1; i < rtc->mon; i++)
		secs += d2m[i] * SEC2DAY;

	secs += (rtc->mday-1) * SEC2DAY;
	secs += rtc->hour * SEC2HOUR;
	secs += rtc->min * SEC2MIN;
	secs += rtc->sec;

	return secs;
}

static void
sec2rtc(ulong secs, Rtc *rtc)
{
	int d;
	long hms, day;
	int *d2m;

	/*
	 * break initial number into days
	 */
	hms = secs % SEC2DAY;
	day = secs / SEC2DAY;
	if(hms < 0) {
		hms += SEC2DAY;
		day -= 1;
	}

	/*
	 * 19700101 was thursday
	 */
	rtc->wday = (day + 7340036L) % 7;

	/*
	 * generate hours:minutes:seconds
	 */
	rtc->sec = hms % 60;
	d = hms / 60;
	rtc->min = d % 60;
	d /= 60;
	rtc->hour = d;

	/*
	 * year number
	 */
	if(day >= 0)
		for(d = 1970; day >= *yrsize(d); d++)
			day -= *yrsize(d);
	else
		for (d = 1970; day < 0; d--)
			day += *yrsize(d-1);
	rtc->year = d;

	/*
	 * generate month
	 */
	d2m = yrsize(rtc->year);
	for(d = 1; day >= d2m[d]; d++)
		day -= d2m[d];
	rtc->mday = day + 1;
	rtc->mon = d;

	return;
}

