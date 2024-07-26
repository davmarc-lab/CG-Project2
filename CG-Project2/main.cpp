#include "Application/Application.hpp"

#define APP_NAME "Lost In Space"

int main(void) {

	Application app = Application(APP_NAME);
	app.launch();

	return 0;
}
