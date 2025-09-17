#include <SFML/Graphics.hpp>

void init() {
	// initialise all the objects needed for the game. 
}

void update(float dt) {
	// Update Everything
}

void render(sf::RenderWindow& window) {
	// Draw Everything
}

void clean() {
	//free up the memory if necessary.
}

int main() {
	//create the window
	sf::RenderWindow window(sf::VideoMode({ 600, 300 }), "PONG");
	//initialise and load
	init();
	while (!window.isOpen()) {
		//Calculate dt
		static sf::Clock clock;
		const float dt = clock.restart().asSeconds();

		window.clear();
		update(dt);
		render(window);
		//wait for the time_step to finish before displaying the next frame.
		sf::sleep(clock.getElapsedTime());
		//Wait for Vsync
		window.display();
	}
	//Unload and shutdown
	clean();
}
