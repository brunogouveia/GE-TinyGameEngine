/*
 * Inpuc.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: Bruno Gouveia
 */

#include "Input.h"

KeyCode KeyCode::Esc(27);
KeyCode KeyCode::UpArrow(256);
KeyCode KeyCode::DownArrow(257);
KeyCode KeyCode::LeftArrow(258);
KeyCode KeyCode::RightArrow(259);
KeyCode KeyCode::PageUp(260);
KeyCode KeyCode::PageDown(261);

/**
 * KeyCode constructor
 */
KeyCode::KeyCode(unsigned int v) :
		value(v) {
}

std::vector<bool> Input::keyMap;

void Input::init() {
	keyMap.resize(262);

	for (int i = 0; i < 256; i++) {
		keyMap[i] = false;
	}
}


/**
 * Returns true while the user holds down the key identified by name. Think auto fire.
 */
bool Input::getKey(KeyCode key) {
	return keyMap[key.value];
}

void Input::setKey(KeyCode key) {
	keyMap[key.value] = true;
}

void Input::resetMaps() {
	for (unsigned int i = 0; i < keyMap.size(); ++i) {
		keyMap[i] = false;
	}
}

