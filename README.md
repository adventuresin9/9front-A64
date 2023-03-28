# 9front-A64
experimental 9 Front kernel for the Allwinner A64

currently runs all 4 cores and the UART.
reported to run on the PinePhone

for a kernel that runs on the PinePhone, 
modfiy the mkfile like so;

9:V:	$p$CONF #$p$CONF.u

#$p$CONF.u:D:	$p$CONF
#	aux/aout2uimage -Z$kzero $p$CONF

#$p$CONF:D:	$OBJ $CONF.$O $LIB
#	$LD -a -o $target -T$loadaddr -l $prereq >DEBUG
#	size $target

$p$CONF:D:	$OBJ $CONF.$O $LIB
	$LD -a -o $target -H6 -R0x10000 -T$loadaddr -l $prereq >DEBUG
#	size $target

comment out the regular kernel and the uimage stuff.
build the kernel with the -H6 "headerless" option.

