/*****
 * Author: Glowstudent
 * CYOA Game
 *****/

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <random>

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

const string title = "STALKER: Zone of Shadows";
int FLAGS[] = {0, 0, 0, 0};
// Inventory Items
// 0: Empty
// 1: Medkit
// 2: Food
// 3: Ammo
// 4: Pistol
int inventory[] = {0, 0, 0, 0};

// Screens
void titleScreen();
void instructionScreen();
void gameLoop();
void credits();

// Game Story + Choices
void printChoices(int screen, int (&choices)[4]);
void handleInput(int screen, char input, int (&choices)[4]);
void wakeUp();
void leaveShelter();
void flickeringLight();

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

void resetChoices(int (&choices)[4])
{
	for (int i = 0; i < size(choices); i++)
	{
		choices[i] = 0;
	}
}

void exitGame(int code = 0)
{
	cout << "Exiting " << title << "..." << endl;
	exit(code);
}

int random(int min, int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(min, max);

	return dis(gen);
}

int flipCoin()
{
	int weight = random(1, 10);

	if (weight <= 5)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int flipWeighted(int min, int max, int weight)
{
	int initial = random(1, 10);

	if (initial <= weight)
	{
		return 1;
	}
	else
	{
		return 0;
	}
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

void changeScene(int scene)
{
	switch (scene)
	{
	case 1:
		wakeUp();
		break;
	case 2:
		leaveShelter();
		break;
	}
}

void addFlag(int flag)
{
	FLAGS[flag] = 1;
}

void addInventory(int item)
{
	// Find the first empty slot
	for (int i = 0; i < size(inventory); i++)
	{
		if (inventory[i] == 0)
		{
			inventory[i] = item;
			break;
		}
	}
}

void removeInventory(int item)
{
	for (int i = 0; i < size(inventory); i++)
	{
		if (inventory[i] == item)
		{
			inventory[i] = 0;
			break;
		}
	}
}

bool hasItem(int item)
{
	for (int i = 0; i < size(inventory); i++)
	{
		if (inventory[i] == item)
		{
			return true;
		}
	}
	return false;
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

	cout << "==============================\n";
	cout << setw(title.length() + 3) << title << "\n";
	cout << "==============================\n";
	cout << "1. Start Adventure\n";
	cout << "2. Instructions\n";
	cout << "3. Credits\n";
	cout << "4. Exit\n";
	cout << "==============================\n";

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
	cout << "1-4: Choices 1-4\n";
	cout << "==============================\n";
	cout << "Press ENTER to return to the main menu...";
	cin.get();
	getScreen(1);
}

void gameLoop()
{
	char input;

	clearScreen();

	// The story begins here
	wakeUp();
}

void handleInput(int screen, char input, int (&choices)[4])
{
	resetInput();

	// DEBUG
	cout << choices[0] << choices[1] << choices[2] << choices[3] << endl;
	cout << !isalpha(input) << endl;
	if (!isalpha(input))
		cout << choices[(input - '0') - 1] << endl;

	while (!isalpha(input) && choices[(input - '0') - 1] == 1)
	{
		clearScreen();
		cout << "You have already chosen this option.\n";
		printChoices(screen, choices);
		getChar(input, screen, "Enter your choice: ", true);
	}

	if (input == 'q')
		exitGame();

	switch (screen)
	{
	case 1:
		clearScreen();
		switch (input)
		{
		case ('1'):
			resetChoices(choices);
			changeScene(2);
			break;
		case ('2'):
			if (flipWeighted(1, 10, 8))
			{
				cout << "You check your gear for supplies and weapons.\n";
				cout << "You find a medkit.\n";
				addInventory(1);
			}
			else
			{
				cout << "You check your gear for supplies and weapons.\n";
				cout << "You find nothing useful.\n";
			}
			choices[1] = 1;
			break;
		case ('3'):
			cout << "You listen to the radio for any updates from fellow stalkers.\n";
			choices[2] = 1;
			break;
		case ('4'):
			cout << "You draw a map of the nearby locations you remember.\n";
			choices[3] = 1;
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;
	case 2:
		clearScreen();
		switch (input)
		{
		case ('1'):
			cout << "You investigate the flickering light.\n";
			break;
		case ('2'):
			cout << "You head towards the abandoned factory.\n";
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;
	// You investigate the flickering light.
	case 3:
		clearScreen();
		switch (input)
		{
		case ('1'):
			cout << "You approach the stalkers and introduce yourself.\n";
			break;
		case ('2'):
			cout << "You sneak around to search their supplies.\n";
			break;
		case ('3'):
			cout << "You offer to help the wounded stalker.\n";
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;
	}

	cout << "\nWhat will you do next?\n";
	printChoices(screen, choices);
	getChar(input, screen, "Enter your choice: ", true);
	handleInput(screen, input, choices);
}

void printChoices(int screen, int (&choices)[4])
{
	int count = 0;
	int choicesCount = sizeof(choices) / sizeof(int);
	cout << "==============================\n";
	switch (screen)
	{
	case 1:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1;

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << ". Leave the shelter and scout the area." << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << ". Check your gear for supplies and weapons." << endl;
				}
				break;
			case 2:
				if (choices[i] == 0)
				{
					cout << ". Listen to the radio for any updates from fellow stalkers." << endl;
				}
				break;
			case 3:
				if (choices[i] == 0)
				{
					cout << ". Draw a map of the nearby locations you remember." << endl;
				}
				break;
			}
			count++;
		}
		break;
	case 2:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1;

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << ". Investigate the flickering light." << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << ". Head towards the abandoned factory." << endl;
				}
				break;
			}
			count++;
		}
		break;
	case 3:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1;

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << ". Approach the stalkers and introduce yourself." << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << ". Sneak around to search their supplies." << endl;
				}
				break;
			case 2:
				if (choices[i] == 0)
				{
					cout << ". Offer to help the wounded stalker." << endl;
				}
				break;
			}
			count++;
		}
		break;
	}

	cout << "==============================\n";
}

void wakeUp()
{
	char input;
	int choices[] = {0, 0, 0, 0};

	cout << "You wake up in a dimly lit shelter deep within the Zone, the air thick with tension. The faint sounds of distant gunfire echo, reminding you that danger is ever-present. You need to decide your next move to survive and find valuable artifacts.\n"
		 << endl;

	printChoices(1, choices);
	getChar(input, 1, "Enter your choice: ", true);
	handleInput(1, input, choices);
}

void leaveShelter()
{
	char input;
	int choices[] = {0, 0, -1, -1};

	cout << "You leave the shelter and scout the area.\n"
		 << "You see a flickering light in the distance and an abandoned factory nearby.\n"
		 << endl;

	printChoices(2, choices);
	getChar(input, 2, "Enter your choice: ", true);
	handleInput(2, input, choices);
}

void flickeringLight()
{
	char input;
	int choices[] = {0, 0, -1, -1};

	cout << "You approach the light and discover it's coming from a small campfire. A group of stalkers sits around it, looking wary but not hostile.\n"
		 << "As you get closer you see a wounded stalker who needs medical attention.\n"
		 << endl;

	printChoices(2, choices);
	getChar(input, 2, "Enter your choice: ", true);
	handleInput(2, input, choices);
}

int main()
{
	cout << "\033]0;" << title << "\007";

	clearScreen();
	getScreen(1);

	return 0;
}