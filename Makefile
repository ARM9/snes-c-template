
ifeq ($(strip $(DEVKITSNES)),)
$(error "Please set DEVKITSNES in your environment. export DEVKITSNES=<path to>devkitSNES")
endif

include $(DEVKITSNES)/wdc816_rules.mk

DEBUG := 1

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
C2SFLAGS	:= -ML -AT -SOP -WS
ASFLAGS		:= -S
endif

CFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
PNGFILES	:= $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
LISTFILES	:= $(CFILES:.c=.asm)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export LDFLAGS	:= -B -E -T -P00 -F $(CURDIR)/hirom.cfg

export OFILES	:= $(CFILES:.c=.obj) $(SFILES:.asm=.obj) $(PNGFILES:.png=.obj)

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

all: $(LISTING) $(LISTFILES) $(BUILD)
	$(symfix) $(TARGET).map > $(TARGET).sym

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(LISTING): $(CFILES)
	@[ -d $@ ] || mkdir -p $@

clean:
	rm -rf $(BUILD) $(OUTPUT) $(TARGET).bnk $(TARGET).sym $(TARGET).map $(LISTING)

run: all
	$(snes9x) $(OUTPUT)

run2: all
	$(higan-a) $(OUTPUT)

#--------------------------------------------------
else

$(OUTPUT) : $(OFILES)

# Convert PNG graphics
%.asm : %.png %.grit
	$(SNESGRIT) $<

endif
