#include <iostream>
#include <conio.h>
#include <time.h>
#include <chrono>
using namespace std;
using namespace chrono;

// Function Templates

	/* Frame Functions */
void Draw();							// Draw current frame
void Input();							// Check for input on current frame
void Logic();							// Apply logic for frame

	/* Utility Functions */
double Wait(double waitTime);			// Pause execution for given time [ms]
double GetTimeSince(double startTime);	// Get time since given time  [double]
double GetTime();						// Get current time

// Game Variables

const int X = 0, L = 0;
const int Y = 1, R = 1;

int screenHeight = 15;
int screenWidth = 60;

int ballPos[] = { screenWidth / 2, screenHeight / 2 };
int ballDir[] = { 1, -1 };
int paddle[] = { screenHeight / 2, screenHeight / 2 };

int score[] = { 0, 0 };
int AI_Level = 3;
double lastFrameTime;
double FPS = 0.05;

// Main Function

int main() {

	bool GAME_RUNNING = true;
	lastFrameTime = GetTime();

	do {
		Draw();				// Update screen
		Input();			// Check for input
		Logic();			// Handle logic per frame
	} while (GAME_RUNNING);


	return EXIT_SUCCESS;
}

// Function Definitions

void Draw() { 

	cout << "Score: " << score[0];
	for (int space = 0; space < screenWidth - 16; space++) cout << ' ';
	cout << "Score: " << score[1] << endl;

	for (int cursorY = 0; cursorY < screenHeight; cursorY++) {
		for (int cursorX = 0; cursorX < screenWidth; cursorX++) {

			if (cursorX == ballPos[0] && cursorY == ballPos[1]) cout << 'o';
			else if (cursorY == 0 || cursorY == screenHeight - 1)  cout << '-';
			else if (cursorX == screenWidth / 2 && cursorY != 0 && cursorY != screenHeight - 1) cout << '|';
			else if (cursorX == 0 && (cursorY == paddle[L] || cursorY == paddle[L] + 1 || cursorY == paddle[L] - 1)) cout << ']';
			else if (cursorX == screenWidth - 1 && (cursorY == paddle[R] || cursorY == paddle[R] + 1 || cursorY == paddle[R] - 1)) cout << '[';
			else cout << ' ';

		}
		cout << endl;
	}
	printf("\033[%d;%dH", 1, 1);

}

void Input() {

	if (_kbhit())
		switch (_getch()) {
			case 'w':
			case 38:
				if (!(paddle[L] - 2 <= 0)) paddle[L]--;
				break;
			case 's':
			case '40':
				if (!(paddle[L] + 2 >= screenHeight - 1)) paddle[L]++;
				break;
		}
}

void Logic() {

	// Right-side AI
	if (ballPos[Y] > paddle[R]) paddle[R]++;
	if (ballPos[Y] < paddle[R]) paddle[R]--;
	paddle[R] += 1 - ((time(nullptr) % AI_Level == 0) * 2);
	if (paddle[R] - 2 <= 0) paddle[R] = 1;
	if (paddle[R] + 2 >= screenHeight - 1) paddle[R] = screenHeight - 2;

	// Movement
	ballPos[X] += ballDir[X];
	ballPos[Y] += ballDir[Y];

	// Collision
	if (ballPos[Y] <= 0 || ballPos[Y] >= screenHeight - 1) ballDir[Y] *= -1;
	if (ballPos[X] == 1 && (ballPos[Y] == paddle[L] || ballPos[Y] == paddle[L] + 1 || ballPos[Y] == paddle[L] - 1)) ballDir[X] *= -1;
	if (ballPos[X] == screenWidth - 2 && (ballPos[Y] == paddle[R] || ballPos[Y] == paddle[R] + 1 || ballPos[Y] == paddle[R] - 1)) ballDir[X] *= -1;

	// Scoring
	if (ballPos[X] < 0 || ballPos[X] > screenWidth - 1) {
		ballDir[X] = (ballPos[X] < 0);
		ballDir[Y] = time(nullptr) % 2;
		for (int dir = 0; dir < 2; dir++) if (ballDir[dir] == 0) ballDir[dir] = -1;

		score[1] += (ballPos[X] < 0);
		score[0] += (ballPos[X] > screenWidth - 1);

		ballPos[X] = screenWidth / 2;
		ballPos[Y] = screenHeight / 2;
	}
	
	while (GetTimeSince(lastFrameTime) < FPS) {}
	lastFrameTime = GetTime();
}

// Functions reused for delta time

double GetTime() {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9;
}

double GetTimeSince(double startTime) {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9 - startTime;
}
