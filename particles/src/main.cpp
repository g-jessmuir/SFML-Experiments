#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <ctime>
#include <cmath>

const int particle_number = 25000;
const int window_width = 1200;
const int window_height = 600;

class Particle {
public:
	float position[2] = { 0.f, 0.f };
	float velocity[2] = { 0.f, 0.f };
	Particle() {}
	Particle(float posx, float posy) {
		position[0] = posx;
		position[1] = posy;
	}
};

void update_physics(Particle& particle, sf::Vector2i mc);

int main() {
	sf::Window window(sf::VideoMode(window_width, window_height, 32), "My window");
	window.setVerticalSyncEnabled(true); // call it once, after creating the window
	window.setFramerateLimit(30);

	//openGL stuff
	glViewport(0, 0, window_width, window_height); // viewport definition
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1, 1);

	//initialization
	std::srand(time(0));
	Particle particles[particle_number];
	float particle_coords[particle_number * 3];
	int particle_colours[particle_number * 3];
	for (int i = 0; i < particle_number; i++) {
		int posx = rand() % window_width + 1;
		int posy = rand() % window_height + 1;
		particles[i] = Particle(posx, posy);
	}
	for (int i(0); i < particle_number; i += 2) {
		particle_coords[i] = particles[i].position[0];
		particle_coords[i + 1] = particles[i].position[1];
	}
	for (int i = 0; i < particle_number; i++) {
		//particle_colours[3 * i] = rand() % 127 + 128;
		//particle_colours[3 * i + 1] = rand() % 127 + 128;
		//particle_colours[3 * i + 2] = rand() % 127 + 128;
		particle_colours[3 * i] = 255;
			particle_colours[3 * i + 1] = 255;
			particle_colours[3 * i + 2] = 255;
	}

	while (window.isOpen())
	{

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}
		glClearColor(0, 0, 0, 0); // we clear the screen with black (else, frames would overlay...)
		glClear(GL_COLOR_BUFFER_BIT); // clear the buffer

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i mc = sf::Mouse::getPosition(window);
			for (int index = 0; index < particle_number; index++) {
				update_physics(particles[index], mc);
				particle_coords[2 * index] = particles[index].position[0];
				particle_coords[2 * index + 1] = particles[index].position[1];
				//std::cout << particle_coords[2 * index] << ',' << particle_coords[2 * index + 1] << std::endl;
			}
		}
		else {
			for (int index = 0; index < particle_number; index++) {
				particles[index].velocity[0] = particles[index].velocity[0] * 0.5;
				particles[index].velocity[1] = particles[index].velocity[1] * 0.5;
				particle_coords[2 * index] = particles[index].position[0];
				particle_coords[2 * index + 1] = particles[index].position[1];
			}
		}

		//drawing with openGL
		glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, 0, particle_coords);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, particle_colours);
		glDrawArrays(GL_POINTS, 0, particle_number);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glPopMatrix();
		glFlush();

		//end the current frame
		window.display();
	}
	return 0;
}

void update_physics(Particle& particle, sf::Vector2i mc) {
	float force[2];
	force[0] = mc.x - particle.position[0];
	force[1] = mc.y - particle.position[1];
	float magnitude = std::sqrt(force[0] * force[0] + force[1] * force[1]);
	float unit_force[2];
	unit_force[0] = force[0] / (magnitude);
	unit_force[1] = force[1] / (magnitude);
	particle.velocity[0] += unit_force[0];
	particle.velocity[1] += unit_force[1];
	particle.position[0] += particle.velocity[0];
	particle.position[1] += particle.velocity[1];
}