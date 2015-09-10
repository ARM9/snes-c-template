; Sample assets bundler

    .kdata
    .xdef ~~text
    .xdef ~~text_size
~~text:
    .ifdef DEBUG
    .string "white",$0a,"ducks",0
    .date
    .endif
    .insert text.bin
~~text_size:
    .ends

sample_section .section offset $28000

    .xdef ~~ocean_tiles
    .xdef ~~ocean_tiles_size
~~ocean_tiles:
    .insert ocean.chr
~~ocean_tiles_size:

    .xdef ~~ocean_map
    .xdef ~~ocean_map_size
~~ocean_map:
    .insert ocean.map
~~ocean_map_size: ; equ *-~~ocean_map

    .xdef ~~ocean_pal
~~ocean_pal:
    .insert ocean.pal

    .ends

sample_section2 .section offset $38000
    .insert text.bin
    .ends

