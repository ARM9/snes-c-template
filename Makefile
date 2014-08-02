#--------------------------------------------------
export 65XX	:= $(DEVKITSNES)/65xx
export PATH	:= $(PATH):$(65XX)/tools:$(65XX)/bin
#--------------------------------------------------
export CC	:= WDC816CC.exe
export AS	:= WDC816AS.exe
export LD	:= WDCLN.exe

DEBUG = 1

TOOLS		:= $(DEVKITSNES)/tools
gfx2snes	:= $(TOOLS)/gfx2snes

EMUS	:= $(DEVKITPRO)/emulators/snes
ifeq ($(OS),Windows_NT)
snes9x	:= $(EMUS)/snes9x/snes9x-x64
else
snes9x	:= $(EMUS)/snes9x/snes9x-gtk
endif
nosns	:= $(EMUS)/nosns/nosns
higan-p	:= $(EMUS)/../higan/higan-performance
higan-b	:= $(EMUS)/../higan/higan-balance
higan-a	:= $(EMUS)/../higan/higan-accuracy

#--------------------------------------------------
%.obj : %.c
	$(CC) $(INCLUDE) $(CFLAGS) -O $@ $<

%.obj : %.asm
	$(AS) $(INCLUDE) $(ASFLAGS) -O $@ $<

%.asm : %.c
	$(CC) $(INCLUDE) $(C2SFLAGS) -O $(LISTING)/$@ $<

%.sfc :
	$(LD) $(LDFLAGS) $(OFILES) -LCL -O $@
#	bin2sfc $@ $@
#	padbin 0 262144 $@
#--------------------------------------------------

TARGET		:= $(shell basename $(CURDIR))
BUILD		:= build
LISTING		:= listing
SOURCES		:= src
INCLUDES	:= include
GRAPHICS	:= gfx
AUDIO		:= audio

ifneq ($(DEBUG),)
#Debug build
CFLAGS		:= -ML -SI -SP -WL -DDEBUG
#-WP warn about function calls without prototype
C2SFLAGS	:= -ML -AT -SI -SP -WS -DDEBUG
ASFLAGS		:= -S -DDEBUG
else
#Release build
CFLAGS		:= -ML -SF -SI -SM -SS -SP -WL 
#-WP warn about function calls without prototype
C2SFLAGS	:= -ML -AT -SOP -WS
ASFLAGS		:= -S
endif

LDFLAGS		:= -B -E -T -P00 -F ../hirom.cfg

export WDC_LIB	:= $(65XX)/lib
export LIBSNES	:= $(65XX)/lib/libsnes

ifneq ($(BUILD),$(notdir $(CURDIR)))

CFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
LISTFILES	:= $(CFILES:.c=.asm)

export OFILES	:= $(CFILES:.c=.obj) $(SFILES:.asm=.obj)

export OUTPUT	:= $(CURDIR)/$(TARGET).sfc

export INCLUDE	:= -I$(65XX)/include \
					$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBSNES)/include,-I$(dir))

export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir)) \
					$(foreach dir,$(AUDIO),$(CURDIR)/$(dir)) \
					$(LIBSNES)
#--------------------------------------------------
.PHONY: $(BUILD) $(LISTING) clean run run2

$(BUILD): $(LISTING) $(LISTFILES)
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(LISTING):
	@[ -d $@ ] || mkdir -p $@
#	@mkdir -p $@

all: $(BUILD)
	

clean:
	@echo clean
	@rm -rf $(BUILD) $(OUTPUT) $(TARGET).bnk $(TARGET).map $(LISTING)

run: all
	$(snes9x) $(OUTPUT)

run2: all
	$(higan-a) $(OUTPUT)

#--------------------------------------------------
else

$(OUTPUT) : $(OFILES)

endif
