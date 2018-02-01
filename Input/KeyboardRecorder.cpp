#include "KeyboardRecorder.h"

void KeyboardRecorder::fillInputs()
{
	for (int key : keysToListen) {
		if (keyboard->keyTriggered(static_cast<KeyboardKeys>(key))) {

		}
		else if (keyboard->keyHeld(static_cast<KeyboardKeys>(key))) {

		}
	}
}
