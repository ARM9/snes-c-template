# Makefile for wdc816cc nicked from devkitPro
#--------------------------------------------------
export 65XX	:=	$(DEVKITSNES)/65xx
export PATH	:=	$(PATH):$(65XX)/tools:$(65XX)/bin
#--------------------------------------------------
export CC	:=	wdc816cc
export AS	:=	wdc816as
export LD	:=	wdcln

TOOLS		:=	$(DEVKITSNES)/tools
gfx2snes	:=	gfx2snes

EMUS	:=	$(DEVKITPRO)/emulators/snes
snes9x	:=	$(EMUS)/snes9x/snes9x-x64
nosns	:=	$(EMUS)/nosns/nosns
higan-p	:=	$(EMUS)/higan/higan-performance
higan-b	:=	$(EMUS)/higan/higan-balance
higan-a	:=	$(EMUS)/higan/higan-accuracy

#--------------------------------------------------
%.obj : %.c
	$(CC) $(INCLUDE) $(CFLAGS) -O $@ $<

%.obj : %.s
	$(AS) $(INCLUDE) $(ASFLAGS) -O $@ $<

%.asm : %.c
	$(CC) $(INCLUDE) $(C2SFLAGS) -O $(LISTING)/$@ $<

%.sfc : 
	$(LD) $(LDFLAGS) $(OFILES) -LCL -O $@
	bin2sfc $@ $@
#	padbin 0 262144 $@
#--------------------------------------------------

TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
LISTING		:=	listing
SOURCES		:=	src
INCLUDES	:=	include
GRAPHICS	:=	gfx
AUDIO		:=	audio


CFLAGS		:=	-ML -SI -SP -WL
#-WP warn about function calls without prototype
C2SFLAGS	:=	-ML -AT -SI -SP -WL
ASFLAGS		:=	-S
LDFLAGS		:=	-B -T -HB -MN80 -P00 -zCODE=8000 -C808000,8000 -K8000 -D7E1000,18000
# section linking explanation:
#	-D7E1000,18000
#		means that the data is linked in rom at $18000 (so in bank $01),
#		while it should treat the data as if it were at $7E1000 when resolving the labels
#	This could go haywire unless declaring all variables far, since some may be read as absolute
#	even if at > $7E2000. Use far only in bigger projects where it's likely you'll exceed
#	4KiB of data.

#-zCODE=8000 -C8000 -K8000 -D0000,1F00 -U0000,1F00
#-LCL for stdlib

export WDC_LIB	:=	$(65XX)/lib
export LIBSNES	:=	$(65XX)/lib/libsnes

ifneq ($(BUILD),$(notdir $(CURDIR)))

CFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
# uses .asm extension to generate list files
LISTFILES	:= $(CFILES:.c=.asm)

export OFILES	:=	$(CFILES:.c=.obj) $(SFILES:.s=.obj)

export OUTPUT	:=	$(CURDIR)/$(TARGET).sfc

export INCLUDE	:=	-I$(65XX)/include \
					$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBSNES)/include,-I$(dir))

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir)) \
					$(foreach dir,$(AUDIO),$(CURDIR)/$(dir)) \
					$(LIBSNES)
#--------------------------------------------------
.PHONY: $(BUILD) $(LISTING) clean run run2

$(BUILD): $(LISTING) $(LISTFILES)
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(LISTING):
	@mkdir -p $@

all: $(BUILD)
	

clean:
	@echo clean
	@rm -rf $(BUILD) $(OUTPUT) $(TARGET).bnk $(TARGET).map $(LISTING)

run: $(BUILD)
	$(snes9x) $(OUTPUT)

run2: $(BUILD)
	$(higan-a) $(OUTPUT)

#--------------------------------------------------
else

$(OUTPUT) : $(OFILES)

endif