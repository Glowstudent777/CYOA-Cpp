/*****
 * Author: Glowstudent
 * CYOA Game
 *****/

#include <iostream>
#include <string>
#include <limits>

// https://stackoverflow.com/questions/70879879/how-can-i-make-a-rectangle-with-text-in-the-center-in-c
// https://stackoverflow.com/questions/31396911/c-print-a-box-application
// https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
// https://cplusplus.com/forum/general/175000/
// https://www.codeproject.com/Questions/543606/Howplustoplusdrawplusatplusborderplusofplusscreenp
// https://cplusplus.com/forum/beginner/28341/
// https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
// https://stackoverflow.com/questions/1761626/weighted-random-numbers
// https://cplusplus.com/forum/general/272102/
// https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Something

using namespace std;

// Screens
void titleScreen();
void instructionScreen();
void gameLoop();
void credits();

void resetInput();
void getInt(int &input, int program, int min, int max, string cmessage, bool clearOnFail);
void getChar(char &input, int screen, string cmessage, bool allowInt, bool clearOnFail);

void clearScreen()
{
	// Clear the screen
#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#elif defined(__linux__)
	system("clear");
#elif defined(__APPLE__)
	system("clear");
#endif
}

void resetInput()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void getScreen(int screen)
{
	switch (screen)
	{
	case 1:
		titleScreen();
		break;
	case 2:
		instructionScreen();
		break;
	case 3:
		credits();
		break;
	}
}

void getInt(int &input, int screen, int min, int max, string cmessage = "", bool clearOnFail = false)
{
	if (cmessage != "")
	{
		cout << cmessage;
	}
	else
	{
		cout << "Enter your choice: ";
	}
	cin >> input;

	while (cin.fail() || input < min || input > max)
	{
		if (clearOnFail == true)
		{
			clearScreen();
			getScreen(screen);
		}
		resetInput();
		cout << "Invalid Input. Please enter a valid input: ";
		cin >> input;
	}
}

void getChar(char &input, int screen, string cmessage = "", bool allowInt = false, bool clearOnFail = false)
{
	if (cmessage != "")
	{
		cout << cmessage;
	}
	else
	{
		cout << "Enter your choice: ";
	}
	cin >> input;

	while (cin.fail() || (!allowInt && !isalpha(input)))
	{
		if (clearOnFail == true)
		{
			clearScreen();
			getScreen(screen);
		}
		resetInput();
		cout << "Invalid Input. Please enter a valid input: ";
		cin >> input;
	}
}

void titleScreen()
{
	int choice;

	/*****
	 * ASCII Art or something on title
	 * 1) Play or Start Game
	 * 2) Controls or Help
	 * 3) Credits
	 *****/

	clearScreen();

	cout << "===============================\n";
	cout << "   CHOOSE YOUR OWN ADVENTURE   \n";
	cout << "===============================\n";
	cout << "1. Start Adventure\n";
	cout << "2. Instructions\n";
	cout << "3. Credits\n";
	cout << "4. Exit\n";
	cout << "===============================\n";

	getInt(choice, 1, 1, 4, "Enter your choice (1-4): ");

	switch (choice)
	{
	case 1:
		cout << "Starting Adventure...\n";
		gameLoop();
		break;
	case 2:
		getScreen(2);
		break;
	case 3:
		getScreen(3);
		break;
	case 4:
		cout << "Exiting...\n";
		break;
	}
}

void credits()
{
	clearScreen();
	resetInput();

	cout << "===============================\n";
	cout << "            CREDITS            \n";
	cout << "===============================\n";
	cout << "Author: Glowstudent\n";
	cout << "===============================\n";
	cout << "Press ENTER to return to the main menu...";
	cin.get();
	getScreen(1);
}

void instructionScreen()
{
	clearScreen();
	resetInput();

	cout << "==============================\n";
	cout << "         INSTRUCTIONS         \n";
	cout << "==============================\n";
	cout << "q: Exit\n";
	cout << "1: Choice 1\n";
	cout << "2: Choice 2\n";
	cout << "==============================\n";
	cout << "Press ENTER to return to the main menu...";
	cin.get();
	getScreen(1);
}

void gameLoop()
{
	char input;

	clearScreen();

	cout << "Do something" << endl;
	cout << "q to exit" << endl;

	getChar(input, 1, "Enter your choice: ", true);

	switch (input)
	{
	case ('1'):
		cout << "Choice 1";
		break;
	case ('2'):
		cout << "Choice 2";
		break;
	}
}

int main()
{
	clearScreen();
	getScreen(1);

	return 0;
}