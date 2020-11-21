#include <iostream>		// Standard C++ Lib
#include <conio.h>		// Input Functions
#include <ctime>		// Time Functions
#include <windows.h>	// Windows ONLY Functions
using namespace std;

COORD screen;											// Screen Coordinates (Windows Only)
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);		// Handle For Console Window

const int Dimensions[2] = { 15, 40 };							// Board Height & Width (h, w)
int Ball_P[2] = { Dimensions[1] / 2, Dimensions[0] / 2 };		// Ball Position (x,y)
int Ball_V[2] = { 0,0 };										// Ball Direction (x,y)
int Paddle_P[2] = { Dimensions[0] / 2,  Dimensions[0] / 2 };	// Paddle Positions (l, r)
int Score[2] = { 0,0 };											// Scores (l, r)
int AI = 3;														// AI Determiner (higher => easier)


void Draw(), Input(), Logic();		// Main Routines
void ResetBall();					// Secondary Routine

int main() {		
	screen.Y = 0;
	screen.X = 0;

	ResetBall();								

	while (true) {
		Draw();
		Input();
		Logic();
		SetConsoleCursorPosition(console, screen);	// Reset Screen Drawing
		Sleep(100);									// Buffer Frames
	}
	return 0;
}

void Draw() {		// Draw characters to screen
	
	for (int i = 0; i < Dimensions[1]; i++) {		// Score

		if (i == 8)
			cout << Score[0];

		else if (i == Dimensions[1] - 9)
			cout << Score[1];

		else
			cout << ' ';
	}

	cout << endl;

	for (int y = 0; y < Dimensions[0]; y++) {

		for (int x = 0; x < Dimensions[1]; x++) {

			if (y == 0 || y == Dimensions[0] - 1)															// Top & Bottom Border
				cout << '#';

			else if (x == 0 && (y == Paddle_P[0] || y == Paddle_P[0] + 1 || y == Paddle_P[0] - 1))				// Left Paddle
				cout << ']';

			else if (x == Dimensions[1] - 1 && (y == Paddle_P[1] || y == Paddle_P[1] + 1 || y == Paddle_P[1] - 1))		// Right Paddle
				cout << '[';

			else if (y == Ball_P[1] && x == Ball_P[0])													// Ball
				cout << 'O';

			else if (x == (Dimensions[1] / 2))																// Center Line
				cout << "|";

			else
				cout << ' ';																			// Empty Space
		}
		cout << endl;
	}

}

void Input() {		// Logic For Input

	if (_kbhit())
		switch (_getch()) {

		case 'w':		// Up
			Paddle_P[0]--;		
			break;
			
		case 's':		// Down
			Paddle_P[0]++;
			break;
		}

	/* AI Controlled Paddle */
	srand(time(0));		// Random Seed

	if (Paddle_P[1] < Ball_P[1] && rand() % AI == 0)
		Paddle_P[1]++;

	if (Paddle_P[1] > Ball_P[1])
		Paddle_P[1]--;

}

void Logic() {		// Regular Logic

	/* Paddle Limits */
	if (Paddle_P[0] < 2)
		Paddle_P[0] = 2;
	if (Paddle_P[0] > Dimensions[0] - 3)
		Paddle_P[0] = Dimensions[0] - 3;
	if (Paddle_P[1] < 2)
		Paddle_P[1] = 2;
	if (Paddle_P[1] > Dimensions[0] - 3)
		Paddle_P[1] = Dimensions[0] - 3;

	/* Velocity */
	Ball_P[0] += Ball_V[0];
	Ball_P[1] += Ball_V[1];

	/* Paddle and Ceiling Collision */
	if (Ball_P[0] == 0 && (Ball_P[1] == Paddle_P[0] || Ball_P[1] == Paddle_P[0] + 1 || Ball_P[1] == Paddle_P[0] - 1))
		Ball_V[0] *= -1;

	if (Ball_P[0] == Dimensions[1] - 1 && (Ball_P[1] == Paddle_P[1] || Ball_P[1] == Paddle_P[1] + 1 || Ball_P[1] == Paddle_P[1] - 1))
		Ball_V[0] *= -1;

	if (Ball_P[1] == 0 || Ball_P[1] == Dimensions[0] - 1)
		Ball_V[1] *= -1;
		
	/* Scoring */
	if (Ball_P[0] > Dimensions[1] - 1) {
		Score[0]++;
		ResetBall();
	}
	if (Ball_P[0] < 0) {
		Score[1]++;
		ResetBall();
	}

	

}

void ResetBall() {		// Resets Ball To Default Position & Determines Velocity Direction

	srand(time(0));		// Random Seed

	/* Reset Ball Positions To Center Board */
	Ball_P[0] = Dimensions[1] / 2;		
	Ball_P[1] = Dimensions[0] / 2;

	/* Determine Balls Y Direction (Random) */
	if (rand() % 2 == 1)
		Ball_V[1] = -1;
	else
		Ball_V[1] = 1;

	/* Determine Balls X Direction (Towards Winning Player, Else Random) */
	if (Score[0] > Score[1])
		Ball_V[0] = -1;
	else if (Score[0] < Score[1])
		Ball_V[0] = 1;
	else {
		if (rand() % 2 == 1)
			Ball_V[0] = 1;
		else
			Ball_V[0] = -1;
	}

}
