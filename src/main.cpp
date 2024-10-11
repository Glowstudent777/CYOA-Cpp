/*****
 * Author: Glowstudent
 * CYOA Game
 *****/

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <random>

#include <conio.h>

#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

const string title = "STALKER: Zone of Shadows";
int FLAGS[] = {0, 0, 0, 0};
int inventory[] = {0, 0, 0, 0};

// Screens
void titleScreen();
void instructionScreen();
void credits();

void printChoices(int screen, int (&choices)[4]);
void handleInput(int screen, char input, int (&choices)[4]);

// Game Story + Choices
void wakeUp();
void leaveShelter();

void abandonedFactory();

void flickeringLight();
void flickeringLight_1();
void flickeringLight_2();

void resetInput();
void getInt(int &input, int program, int min, int max, string cmessage, bool clearOnFail);
void getChar(char &input, int screen, string cmessage, bool allowInt, bool clearOnFail);
bool quickTimeEvent(char expected, int time);
void PressEnterToContinue();

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

void PressEnterToContinue()
{
	char ch;

	cout << "Press Enter to continue...";
	ch = getch();
	// cin.get();
	// cin.ignore(numeric_limits<streamsize>::max(), '\n');
	// cin.ignore();
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
	case 3:
		flickeringLight();
		break;
	case 4:
		abandonedFactory();
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
		clearScreen();
		wakeUp();
		break;
	case 2:
		getScreen(2);
		break;
	case 3:
		getScreen(3);
		break;
	case 4:
		exitGame();
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

bool quickTimeEvent(char expected, int time)
{
	char ch;
	auto startTime = chrono::high_resolution_clock::now();

	bool playerResponded = false;
	std::thread inputThread([&playerResponded, &ch]()
							{
		ch = getch();
        playerResponded = true; });

	while (!playerResponded &&
		   chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime).count() < time * 1000)
	{
		this_thread::sleep_for(chrono::milliseconds(50));
	}

	inputThread.detach();
	return playerResponded && ch == expected;
}

void handleInput(int screen, char input, int (&choices)[4])
{
	resetInput();

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
			cout << "You listen to the radio for any updates from fellow stalkers.\n"
				 << "There isn't much chatter but there is a guitar playing.\n";
			addFlag(1);
			choices[2] = 1;
			break;
		case ('4'):
			cout << "You draw a map of the nearby locations you remember.\n"
				 << "You remember a place called Pripyat.\n";
			// Remember Pripyat
			addFlag(0);
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
			// Stalkers
			changeScene(3);
			break;
		case ('2'):
			// Bandits
			changeScene(4);
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
			flickeringLight_1();
			break;
		case ('2'):
			flickeringLight_2();
			break;
		case ('3'):
			// Basically a skip to Pripyat
			cout << "You offer to help the wounded stalker.\n";
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;
	case 4:
		clearScreen();
		switch (input)
		{
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
	case 4:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1;

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					// Stalkers around the campfire
					// Some quest to help them
					cout << ". Ask what they're doing." << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << ". SOMETHING HERE." << endl;
				}
				break;
			case 2:
				if (choices[i] == 0)
				{
					cout << ". Ask about Pripyat." << endl;
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

void abandonedFactory()
{
	char input;
	int choices[] = {0, 0, -1, -1};
	// Enter cautiously and explore the ground floor. - Sewers/Tunnel, door to the basement, and a staircase to the upper floors.
	// UPSTAIRS: Climb to the roof for a better vantage point. - Probably see another location and/or the Stalker group. Pripayt?
	// Search the surrounding area for supplies first. - Find something, maybe an artifact or a weapon.

	// Alternative story: The factory is under bandit control and will be attacked by the Stalker group.
	// Join Bandits or Stalkers? - This depends on whether the player went to the flickering light or not.
	// Somehow get Pripyat location from the bandits or the building.
	cout << "You head towards the abandoned factory.\n"
		 << endl;

	// This is just for testing purposes
	cout << "You are shot at by bandits as you approach the factory.\n"
		 << "Get ready to dodge the bullets!\n"
		 << endl;

	PressEnterToContinue();

	// Quick time event to dodge the bullets
	cout << "Press 'd' to dodge the bullets: ";
	bool dodged = quickTimeEvent('d', 2);

	clearScreen();

	if (dodged)
	{
		cout << "You dodge the bullets and take cover behind a nearby car.\n"
			 << "You see a group of bandits guarding the entrance to the factory.\n"
			 << "You need to decide your next move.\n"
			 << endl;

		PressEnterToContinue();
		changeScene(3);
	}
	else
	{
		cout << "You are hit by a bullet and fall to the ground.\nThe bandits approach you and take your gear.\nYou are left to die in the Zone."
			 << endl;

		PressEnterToContinue();
		getScreen(1);
	}
}

void flickeringLight()
{
	char input;
	int choices[] = {0, 0, -1, -1};
	if (hasItem(1))
		choices[2] = 0;

	cout << "You approach the light and discover it's coming from a small campfire. A group of stalkers sits around it, looking wary but not hostile.\n"
		 << "As you get closer you see a wounded stalker who needs medical attention.\n"
		 << endl;

	printChoices(3, choices);
	getChar(input, 3, "Enter your choice: ", true);
	handleInput(3, input, choices);
}

void flickeringLight_1()
{
	char input;
	int choices[] = {0, 0, -1, -1};
	if (FLAGS[0] == 1)
		choices[2] = 0;

	clearScreen();
	cout << "You approach the stalkers and introduce yourself.\n"
		 << "They welcome you and offer you a seat by the fire.\n";

	// FLAG for guitar playing
	if (FLAGS[1] == 1)
		cout << "One of the stalkers is playing the guitar you heard on the radio.\n";

	// Awarded Pripyat location from the stalkers for being friendly.
	// Quick time event to dodge a bullet from a bandit?

	cout << "You learn that they are a group of stalkers who are also looking for artifacts.\n"
		 << "They offer you a chance to join them in a raid on the nearby factory.\n"
		 << endl;

	printChoices(4, choices);
	getChar(input, 4, "Enter your choice: ", true);
	handleInput(3, input, choices);
}

void flickeringLight_healWoundedStalker()
{
	clearScreen();
	cout << "You offer to help the wounded stalker.\n"
		 << "You use your medkit to heal his wounds.\n"
		 << "The stalker thanks you and offers you a reward.\n"
		 << "He gives you a map of the area and marks a location called Pripyat.\n"
		 << "He tells you that there are valuable artifacts there.\n"
		 << "You decide to head to Pripyat to find the artifacts.\n"
		 << endl;

	// Start Pripyat story line
}

void flickeringLight_2()
{
	clearScreen();
	cout << "GAME OVER\n"
		 << "As you start sneaking around, you hear a voice behind you.\n"
		 << "You turn around and see a stalker aiming a rifle at you.\n"
		 << "Your gear is taken from you and you are left to die in the Zone.\n"
		 << endl;

	cout << "Press ENTER to return to the main menu...";
	cin.get();
	getScreen(1);
}

int main()
{
	cout << "\033]0;" << title << "\007";

	clearScreen();
	getScreen(1);

	return 0;
}