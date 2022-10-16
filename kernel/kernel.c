#include "../drivers/screen.h"
#include "string.h"
int main() {
	int row = 0;
	int col = 0;
		
	char result[5];
	int_to_str(1234, result);
	kprint(result, 0x0f, 5, 5);
//	for (; row < MAX_HEIGHT; row++)
//		for (; col < MAX_WIDTH; col++)
//			kprint_char('z', 0x0f, row, col);
	//clear_screen();
}
		
