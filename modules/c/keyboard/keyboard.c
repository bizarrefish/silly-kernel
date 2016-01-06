#include <keyboard/keyboard.h>

char getKeyboardASCII() {		/*returns null char if no new char typed*/
	static char lastCode = '\0';

	static char shift = 0;
	static char escaped = 0;

	char newCode = getKeyboardScanCode();
	if(newCode != lastCode) {		//keypress occurs
		lastCode = newCode;
		if(escaped == 1) {		//if escape sequence
			escaped = 0;
			return 0;
		}

		if(newCode & 0x80) {		//if key is made
			if(newCode & 0x7f == 0x2a) {	//if code is shift key
				shift = 1;
				return 0;
			}

			if(shift) {
				return uTable[newCode&0x7f];
			} else {
				return lTable[newCode&0x7f];
			}


		} else {			//if key is broken
			if(newCode & 0x7f == 0x2a) { //if key is shift key
				shift = 0;		//unshift
				return 0;
			}
		}
	}
	
}
