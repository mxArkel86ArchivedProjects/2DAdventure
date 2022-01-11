#include "Peripherals.h"

std::unordered_map<WPARAM, bool> Peripherals::key_reg = std::unordered_map<WPARAM, bool>();

void Peripherals::KeyPressIn(WPARAM p, bool type) {
	key_reg[p] = type;
}

bool Peripherals::keyPressed(WPARAM p) {
	return key_reg[p]==NULL?0:key_reg[p];
	return false;
}
