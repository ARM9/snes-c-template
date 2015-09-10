
ifeq ($(strip $(DEVKITSNES)),)
$(error "Please set DEVKITSNES in your environment. export DEVKITSNES=<path to>devkitSNES")
endif

export 65xx	:= $(DEVKITSNES)/65xx
export PATH	:= $(PATH):$(65xx)/tools:$(65xx)/bin
export tools	:= $(DEVKITSNES)/tools

export CC	:= WDC816CC.exe
export AS	:= WDC816AS.exe
export LD	:= WDCLN.exe

export WDC_LIB	:= $(65xx)/lib

gfx2snes	:= $(tools)/gfx2snes
snesgrit	:= $(tools)/snesgrit

emus	:= $(DEVKITPRO)/emulators/snes
ifeq ($(OS),Windows_NT)
snes9x	:= $(emus)/snes9x/snes9x-x64
else
snes9x	:= $(emus)/snes9x/snes9x-gtk
endif
nosns	:= $(emus)/nosns/NO\$$SNS.exe
higan-p	:= $(emus)/../higan/higan-performance
higan-b	:= $(emus)/../higan/higan-balance
higan-a	:= $(emus)/../higan/higan-accuracy

#--------------------------------------------------
build/%.o : %.c
	$(CC) $(includes) $(CFLAGS) -O $@ $<

build/%.o : %.asm
	$(AS) $(includes) $(ASFLAGS) -O $@ $<

listing/%.asm : %.c
	$(CC) $(includes) $(C2SFLAGS) -O $@ $<

%.sfc:
	$(LD) $(LDFLAGS) $(ofiles) -O $@ $(libs)
	@sed -e "s/^\(\w\)/;\1/g" $(target).map | sed -e "s/^\t//g" > $(target).sym
#	bin2sfc $@ $@
#	padbin 0 262144 $@
#--------------------------------------------------

