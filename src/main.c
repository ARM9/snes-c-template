/*
*	wdc816cc template
*	By ARM9 2014
*/
//#pragma section CODE=BANK2,offset $2:8000
#include "main.h"

extern u8 far text;
char *foo;

u8 *test_heap;

void main(void){
	SetupVideo();
	
	test_heap = (u8*)farmalloc((u32)1024);
	
	test_heap[5] = 0x42;
	
	test_heap[6] = 0;
	test_heap[7] = 1;
	test_heap[9] = 3;
	test_heap[8] = 2;
	
	while(1){
		if(test_heap[6] < 15)
			REG_INIDISP = ++test_heap[6]; // Heap test, looks awfully slow in listing
		
		foo = &text;
		WaitForInterrupt;
		++frame_count;
	}
	farfree(test_heap);
}
