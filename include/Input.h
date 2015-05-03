#ifndef INPUT_H__
#define INPUT_H__

#include <CSCIx239.h>
#include <vector>

class KeyCode {
public:
	unsigned int value;

	KeyCode(unsigned int key);

	static KeyCode Esc;
	static KeyCode UpArrow;
	static KeyCode DownArrow;
	static KeyCode LeftArrow;
	static KeyCode RightArrow;
	static KeyCode PageUp;
	static KeyCode PageDown;
};

class Input {
private:
	static std::vector<bool> keyMap;
public:
	static void init();

	/**
	 * Returns true while the user holds down the key identified by name. Think auto fire.
	 */
	static bool getKey(KeyCode key);

private:
	static void setKey(KeyCode key);
	static void resetMaps();
	friend class Renderer;
};

#endif
