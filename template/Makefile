include ../base.mk

target	:= $(shell basename $(CURDIR))
output	:= $(CURDIR)/$(target).sfc
build	:= build
listing	:= listing
sources	:= src ../lib
graphics:= gfx
audio	:= audio
incdirs	:= $(sources) $(build) include ../include

cfiles	:= $(foreach dir,$(sources),$(notdir $(wildcard $(dir)/*.c)))
sfiles	:= $(foreach dir,$(sources),$(notdir $(wildcard $(dir)/*.asm)))
pngfiles:= $(foreach dir,$(graphics),$(notdir $(wildcard $(dir)/*.png)))
ofiles	:= $(cfiles:%.c=build/%.o) $(sfiles:%.asm=build/%.o) $(pngfiles:%.png=build/%.o)
listfiles:= $(cfiles:%.c=listing/%.asm)

LDFLAGS	:= -B -E -T -P00 -F $(CURDIR)/hirom.cfg
libs	:= -LCL

includes:= -I$(65xx)/include \
	$(foreach dir,$(incdirs),-I$(dir)) \
	$(foreach dir,$(graphics),-I$(dir))

export VPATH	:= $(foreach dir,$(sources),$(CURDIR)/$(dir)) \
		$(foreach dir,$(graphics),$(CURDIR)/$(dir)) \
		$(foreach dir,$(audio),$(CURDIR)/$(dir))
#--------------------------------------------------
.PHONY: all debug release $(listing) clean run run2

all: release

debug: CFLAGS	:= -ML -SI -SP -WL -WP -DDEBUG
debug: C2SFLAGS	:= -ML -AT -SI -SP -WS -DDEBUG
debug: ASFLAGS	:= -S -DDEBUG
debug: $(listing) $(listfiles) $(output)

release: CFLAGS		:= -ML -SOP -WL -WP
release: C2SFLAGS	:= -ML -AT -SOP -WS
release: ASFLAGS	:= -S
release: $(listing) $(listfiles) $(output)

$(build):
	@[ -d $@ ] || mkdir -p $@

$(output): $(build) $(ofiles)


$(listing): $(cfiles)
	@[ -d $@ ] || mkdir -p $@

clean:
	rm -rf $(build) $(output) $(target).bnk $(target).sym $(target).map $(listing)

run:
	$(snes9x) $(output)

run2:
	$(higan-a) $(output)

%.asm : %.png %.grit
	$(snesgrit) $<

