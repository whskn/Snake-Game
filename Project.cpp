#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <conio.h>
#include <random>
#include <ctime>

int double_buffering();
int move();
int check();

bool game = 1;
int length = 1;
int direct = 4;

int* Y = new int[length]{ 7 };
int* X = new int[length]{ 10 };

int X_gum;
int Y_gum;

int check(int x, int y) { // check in coords
	for (int i = 0; i != length; i++) {
		if (X[i] == x && Y[i] == y) return 1;
	}
	
	return 0;
}

int main() {
	srand(time(0));

	std::thread drow(double_buffering);

	X_gum = rand() % 19;
	Y_gum = rand() % 15;

	while (game) {
		if (_kbhit()) { // check for keypress
			switch (_getch()) {
			case 'W':
			case 'w':
				if(direct != 2)
					direct = 0;
				break;
			case 'A':
			case 'a':
				if (direct != 3)
					direct = 1;
				break;
			case 'S':
			case 's':
				if (direct != 0)
					direct = 2;
				break;
			case 'D':
			case 'd':
				if (direct != 1)
					direct = 3;
				break;
			}
		}

		move();
		check(); // check for trouble

		std::this_thread::sleep_for(std::chrono::milliseconds(150)); // 150 millisec. delay
	}

	drow.join();
	return 0;
}

int move() {
	switch (direct) {
	case 0:
		for (int i = length - 1; i > 0; i--) {
			Y[i] = Y[i - 1];
			X[i] = X[i - 1];
		}
		Y[0]--;
		break;
	case 1:
		for (int i = length - 1; i > 0; i--) {
			Y[i] = Y[i - 1];
			X[i] = X[i - 1];
		}
		X[0]--;
		break;
	case 2:
		for (int i = length - 1; i > 0; i--) {
			Y[i] = Y[i - 1];
			X[i] = X[i - 1];
		}
		Y[0]++;
		break;
	case 3:
		for (int i = length - 1; i > 0; i--) {
			Y[i] = Y[i - 1];
			X[i] = X[i - 1];
		}
		X[0]++;
		break;
	}

	return 0;
}

int check() {
	if (X[0] > 19 || X[0] < 0 || Y[0] > 14 || Y[0] < 0) {
		game = false;
		std::cout << "You\'re lost! Gotta try again!";
	}
	else {
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				if (i == j) continue;
				if (Y[i] == Y[j]) {
					if (X[i] == X[j]) {
						game = false;
						std::cout << "You\'re lost! Gotta try again!";
						return 0;
					}
				}
			}
		}
	}

	if (X[0] == X_gum && Y[0] == Y_gum) {
		X_gum = rand() % 19;
		Y_gum = rand() % 15;

		length++;

		int* X_new = new int[length];
		int* Y_new = new int[length];

		for (int i = 0; i < length - 1; i++) {
			X_new[i] = X[i];
			Y_new[i] = Y[i];
		}

		X_new[length - 1] = 100;
		Y_new[length - 1] = 100;

		delete[] X;
		delete[] Y;

		X = X_new;
		Y = Y_new;

		X_new = nullptr;
		Y_new = nullptr;
	}

	if (length > 389) {
		game = false;

		system("cls");

		std::cout << "OMG! How did you reach this?" << std::endl;
		
		Beep(200, 300);
	}

	return 0;
}

int double_buffering() { // double buffering
	bool count = 0;

	char* buffer_0 = new char[390]; // first buffer
	char* buffer_1 = new char[390]; // second buffer

	char* buffer = buffer_0;

	while (game) { // endless loop
		int i = 0;

		for (; i < 22; i++)
			buffer[i] = '#';
		buffer[i++] = '\n';

		for (int line = 0; line < 15; line++) {
			buffer[i++] = '#';

			for (int let = 0; let < 20; let++) {
				if (check(let, line))
					buffer[i++] = 'o';
				else if(let == X_gum && line == Y_gum)
					buffer[i++] = '&';
				else
					buffer[i++] = ' ';
			}

			buffer[i++] = '#';
			buffer[i++] = '\n';
		}

		for (; i < 390; i++)
			buffer[i] = '#';

		buffer[390] = '\0'; // last 0

		COORD coord = { 0, 0 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // set cursor 0.0 position

		std::cout << buffer << std::endl; // printing

		if (count == 0) { // changing buffer
			count = 1;
			buffer = buffer_1;
		}
		else {
			count = 0;
			buffer = buffer_0;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(150)); // 150 ms delay
	}

	return 0;
}