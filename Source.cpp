#include <iostream>		// Standard C++ Lib
#include <conio.h>		// Input Functions
#include <ctime>		// Time Functions
#include <windows.h>	// Windows ONLY Functions
using namespace std;

COORD screen;											// Screen Coordinates (Windows Only)
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);		// Handle For Console Window

const int HEIGHT = 10, WIDTH = 60;								// Board Height & Width (h, w)
int Ball_P[2] = { WIDTH / 2, HEIGHT / 2 };						// Ball Position (x,y)
int Ball_V[2] = { 0,0 };										// Ball Direction (x,y)
int Paddle_P[2] = { HEIGHT / 2,  HEIGHT / 2 };					// Paddle Positions (l, r)
int Score[2] = { 0,0 };											// Scores (l, r)
int AI = 2;														// AI Determiner (higher => easier)


void Draw(), Input(), Logic();		// Main Routines
void ResetBall();					// Secondary Routine

int main() {		
	srand(time(0));		// Random Seed
	screen.Y = 0; screen.X = 0;

	ResetBall();								

	while (true) {
		Draw(); Input(); Logic();					// Logic Loop (Frame)
		SetConsoleCursorPosition(console, screen);	// Reset Screen Drawing
		Sleep(100);									// Buffer Frames
	}
	return 0;
}

void Draw() {		// Draw characters to screen
	
	for (int i = 0; i < WIDTH; i++) {		// Score

		if (i == 8)
			cout << Score[0];

		else if (i == WIDTH - 9)
			cout << Score[1];

		else
			cout << ' ';
	}

	cout << endl;

	for (int y = 0; y < HEIGHT; y++) {

		for (int x = 0; x < WIDTH; x++) {

			if (y == 0 || y == HEIGHT - 1)																	// Top & Bottom Border
				cout << '=';

			else if (x == 0 && (y == Paddle_P[0] || y == Paddle_P[0] + 1 || y == Paddle_P[0] - 1))					// Left Paddle
				cout << ']';

			else if (x == WIDTH - 1 && (y == Paddle_P[1] || y == Paddle_P[1] + 1 || y == Paddle_P[1] - 1))	// Right Paddle
				cout << '[';

			else if (y == Ball_P[1] && x == Ball_P[0])																// Ball
				cout << 'O';

			else if (x == (WIDTH / 2))																		// Center Line
				cout << "|";

			else
				cout << ' ';																						// Empty Space
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
	if (Paddle_P[1] < Ball_P[1] && rand() % AI == 0)
		Paddle_P[1]++;

	if (Paddle_P[1] > Ball_P[1] && rand() % AI == 0)
		Paddle_P[1]--;

}

void Logic() {		// Regular Logic

	/* Paddle Limits */
	if (Paddle_P[0] < 2)
		Paddle_P[0] = 2;
	if (Paddle_P[0] > HEIGHT - 3)
		Paddle_P[0] = HEIGHT - 3;
	if (Paddle_P[1] < 2)
		Paddle_P[1] = 2;
	if (Paddle_P[1] > HEIGHT - 3)
		Paddle_P[1] = HEIGHT - 3;

	/* Velocity */
	Ball_P[0] += Ball_V[0];
	Ball_P[1] += Ball_V[1];

	/* Paddle and Ceiling Collision */
	if (Ball_P[0] == 0 && (Ball_P[1] == Paddle_P[0] || Ball_P[1] == Paddle_P[0] + 1 || Ball_P[1] == Paddle_P[0] - 1))
		Ball_V[0] *= -1;

	if (Ball_P[0] == WIDTH - 1 && (Ball_P[1] == Paddle_P[1] || Ball_P[1] == Paddle_P[1] + 1 || Ball_P[1] == Paddle_P[1] - 1))
		Ball_V[0] *= -1;

	if (Ball_P[1] == 0 || Ball_P[1] == HEIGHT - 1)
		Ball_V[1] *= -1;
		
	/* Scoring */
	if (Ball_P[0] > WIDTH - 1) {
		Score[0]++;
		ResetBall();
	}
	if (Ball_P[0] < 0) {
		Score[1]++;
		ResetBall();
	}

	

}

void ResetBall() {		// Resets Ball To Default Position & Determines Velocity Direction


	/* Reset Ball Positions To Center Board */
	Ball_P[0] = WIDTH / 2;		
	Ball_P[1] = HEIGHT / 2;

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
