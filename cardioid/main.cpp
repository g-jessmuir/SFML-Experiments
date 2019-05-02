#define _USE_MATH_DEFINES
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <vector>


const int window_width = 600;
const int window_height = 600;

int main() {
	sf::Window window(sf::VideoMode(window_width, window_height, 32), "My window");
	window.setVerticalSyncEnabled(true); // call it once, after creating the window
	//window.setFramerateLimit(60);

	//openGL stuff
	glViewport(0, 0, window_width, window_height); // viewport definition
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1, 1);

	//initialization
	int pointNumber = 600;
	float factor = 2.1f;
	int radius = window_width / 2 - 10;
	std::vector<float> circlePoints;
	std::vector<float> linePoints;
	std::vector<byte> lineColours;

	for (int i = 0; i < pointNumber * 3 / 2; i++) {
		lineColours.push_back(255);
		for (int j = 0; j < 4; j++)
			lineColours.push_back(0);
		lineColours.push_back(255);
	}

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}
		//Processing
		if(factor <= 10)
			factor += 0.05;
		circlePoints.clear();
		linePoints.clear();
		for (int i = 0; i < pointNumber; i++) {
			float theta = 2 * M_PI / pointNumber * i;
			float xc = radius * cos(theta) + window_width / 2;
			float yc = radius * sin(theta) + window_height / 2;
			circlePoints.push_back(xc);
			circlePoints.push_back(yc);
		}
		for (int i = 0; i < pointNumber; i++) {
			linePoints.push_back(circlePoints[2 * i]);
			linePoints.push_back(circlePoints[2 * i + 1]);
			linePoints.push_back(circlePoints[fmod((factor * 2 * i), (pointNumber * 2))]);
			linePoints.push_back(circlePoints[fmod((factor * 2 * i + 1), (pointNumber * 2))]);
		}
		

		float* pointCoords = &circlePoints[0];
		float* lineCoords = &linePoints[0];
		byte* lineRGB = &lineColours[0];
		
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		//glVertexPointer(2, GL_FLOAT, 0, pointCoords);
		//glDrawArrays(GL_POINTS, 0, pointNumber);
		glVertexPointer(2, GL_FLOAT, 0, lineCoords);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, lineRGB);
		glDrawArrays(GL_LINES, 0, pointNumber * 2);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glPopMatrix();
		glFlush();

		/*drawing with openGL example
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

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
		*/

		window.display();
	}
	return 0;
}