#include <GameWindow.h>
#include <Cube.h>
#include <light.h>
#include <Script.h>
#include <xml/XmlParser.h>
#include <iostream>

int main(int argc, char ** argv) {

	// Read;
	if (argc < 2)
		XmlParser::loadFromXml("scene.xml", &argc, argv);
	else {
			XmlParser::loadFromXml(argv[1], &argc, argv);
	}

	// Call main loop
	GameWindow::mainLoop();
	return 0;
}
