## C project template for wdc816cc ##

Using this for wip stuff at the moment, sorry for the mess!

Convention:
    TODO double check: Compiler sets accumulator to known width (16 bit) prior to function calls, it would be wise to reset it accordingly after you're done.
        Index registers stay 16-bit at all times, make sure to change back if you use 8-bit index registers.

        Direct page = frame pointer

Things to watch out for:
        const in C is placed in DATA section, need a constify for wdc816cc it seems.
        static variables do not show up in .map
        something wrong with adding a negative ternary evaluation or subtract by a positive ternary evaluation.

