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

const string red("\033[0;31m");
const string green("\033[1;32m");
const string yellow("\033[1;33m");
const string cyan("\033[0;36m");
const string magenta("\033[0;35m");

const string reset("\033[0m");

const string red_BG("\033[0;41m");
const string green_BG("\033[1;42m");
const string yellow_BG("\033[1;43m");
const string cyan_BG("\033[0;46m");
const string magenta_BG("\033[0;45m");

// Screens
void titleScreen();
void instructionScreen();
void credits();

void printChoices(int screen, int (&choices)[4]);
void handleInput(int screen, char input, int (&choices)[4]);

// Game Story + Choices
void wakeUp();
void leaveShelter();

void soloZone();
void soloZone_car();
void soloZone_house();

void abandonedFactory();
void abandonedFactory_bandits();
void abandonedFactory_stalkers();

void flickeringLight();
void flickeringLight_1();
void flickeringLight_2();
void flickeringLight_healWoundedStalker();
void factoryRaid_Stalkers();

void roadToPripyat();

void resetInput();
void getInt(int &input, int program, int min, int max, string cmessage, bool clearOnFail);
void getChar(char &input, int screen, string cmessage, bool allowInt, bool clearOnFail);
bool quickTimeEvent(char expected, int time);
void PressEnterToContinue();
bool shootQTE(char qte);

// Unfortunatly messy and super short as I'm restricted to a single file and only a couple days to complete this project
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
	while (true)
	{
		ch = _getch();
		if (ch == 13)
			break;
	}
	cout << endl;
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

char qteChar()
{
	int rnd = random(1, 4);
	char qte;

	switch (rnd)
	{
	case 1:
		qte = 'a';
		break;
	case 2:
		qte = 's';
		break;
	case 3:
		qte = 'd';
		break;
	case 4:
		qte = 'f';
		break;
	}

	return qte;
}

void changeScene(int scene)
{
	clearScreen();
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
	cout << "a/s/d/f: Quick Time Events\n";
	cout << "==============================\n";
	cout << "Press ENTER to return to the main menu...";
	cin.get();
	getScreen(1);
}

bool quickTimeEvent(char expected, int time)
{
	char ch;
	clock_t start = clock();
	bool outOfTime = false;
	while ((clock() - start) / CLOCKS_PER_SEC < time)
	{
		if (_kbhit())
		{
			cin.get(ch);
			break;
		}
	}

	if ((clock() - start) / CLOCKS_PER_SEC >= time)
	{
		outOfTime = true;
	}

	if (!ch)
		cout << endl;

	return (ch == expected && !outOfTime);
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
			cout << "You check your gear for supplies and weapons.\n";
			if (flipWeighted(1, 10, 8))
			{
				cout << "You find a medkit.\n";
				addInventory(1);
			}
			else
			{
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
			if (hasItem(1))
				flickeringLight_healWoundedStalker();
			else
				cout << "Invalid choice\n";
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
		case ('1'):
			factoryRaid_Stalkers();
			break;
		case ('2'):
			cout << "You politely decline their offer to join the raid, explaining that you're more interested in heading to Pripyat.\n"
				 << "The stalkers nod understandingly, though they warn you about the dangers that lie ahead.\n"
				 << "\"Pripyat's a tough place,\" one of the stalkers says, poking the fire. \"But if you know what you're looking for, you might just make it out alive.\"\n"
				 << "They hand you a map with Pripyat marked and share some crucial advice: \"Beware the radiation pockets and watch out for bandits.\"\n"
				 << "With the map in hand, you prepare to set off towards Pripyat on your own, determined to find the" << magenta << " artifacts" << reset << ".\n"
				 << "But now, you'll face the dangers of the Zone alone, without backup.\n"
				 << endl;
			PressEnterToContinue();
			clearScreen();
			soloZone();
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;

	case 5:
		clearScreen();
		switch (input)
		{
		case ('1'):
			soloZone_car();
			break;
		case ('2'):
			soloZone_house();
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;

	case 6:
		clearScreen();
		switch (input)
		{
		case ('1'):
			abandonedFactory_stalkers();
			break;
		case ('2'):
			abandonedFactory_bandits();
			break;
		default:
			cout << "Invalid choice\n";
			break;
		}
		break;

	case 7:
		clearScreen();
		switch (input)
		{
		case ('1'):
			cout << "As it turns out you're pretty silver tongued and manage to talk your way out of the situation.\n"
				 << "The bandits are impressed and let you go with a warning to stay out of their territory.\n"
				 << "You continue on your way, feeling pretty good about yourself.\n";

			PressEnterToContinue();
			clearScreen();

			cout << "You start your journey towards Pripyat.\n"
				 << "The road is long and dangerous, but you're determined to make it.\n"
				 << "You've heard stories of the artifacts that lie within the city, and you're eager to find them.\n"
				 << "As you walk, you can't help but feel a sense of unease.\n"
				 << "You're not sure what lies ahead, but you know that you're ready for whatever comes your way.\n";

			PressEnterToContinue();
			clearScreen();

			roadToPripyat();

			break;
		case ('2'):
			cout << "You try and surprise the bandits by kicking the leaders leg out from under him.\n"
				 << "This surprises the bandits enough to let you escape and hide behind a nearby structure.\n"
				 << "You manage to escape the bandits by keeping behind structures so they aren't able to shoot you.\n"
				 << "You eventually lose them and are able to continue on your way.\n";

			PressEnterToContinue();
			clearScreen();

			cout << "You start your journey towards Pripyat.\n"
				 << "The road is long and dangerous, but you're determined to make it.\n"
				 << "You've heard stories of the artifacts that lie within the city, and you're eager to find them.\n"
				 << "As you walk, you can't help but feel a sense of unease.\n"
				 << "You're not sure what lies ahead, but you know that you're ready for whatever comes your way.\n";

			PressEnterToContinue();
			clearScreen();

			roadToPripyat();
			break;
		case ('3'):
			cout << "You give up and the bandits take you captive.\n"
				 << "You are put into a makeshift cell and left alone for days at a time.\n"
				 << "One night, you are mysteriously taken out of your cell with a bag over your head.\n"
				 << "You are walked for what feels like hours before the bag is removed.\n"
				 << "As you look around you notice that you are in the middle of a farm.\n"
				 << "You are shoved into a barn and the door is locked behind you.\n"
				 << "You hear something slowly walking around in the darkness.\n"
				 << "As you try and back away, you trip over something and fall to the ground.\n"
				 << "You feel something grab your leg and pull you towards it.\n"
				 << "The last thing you see is a Snork looming over you.\n"
				 << "You have died.\n";

			PressEnterToContinue();
			getScreen(1);
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
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Leave the shelter and scout the area" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Check your gear for supplies and weapons" << endl;
				}
				break;
			case 2:
				if (choices[i] == 0)
				{
					cout << "Listen to the radio for any updates from fellow stalkers" << endl;
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
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Investigate the flickering light" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Head towards the abandoned factory" << endl;
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
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Approach the stalkers and introduce yourself" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Sneak around to search their supplies" << endl;
				}
				break;
			case 2:
				if (choices[i] == 0 && hasItem(1))
				{
					cout << "Offer to help the wounded stalker" << endl;
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
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Join the raid" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Politely decline and ask for information on Pripyat instead" << endl;
				}
				break;
			}
			count++;
		}
		break;

	case 5:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Search the nearby overturned truck" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Search the crumbling house" << endl;
				}
				break;
			}
			count++;
		}
		break;
	case 6:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Join in attacking the bandits" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Help the bandits defend the factory" << endl;
				}
				break;
			}
			count++;
		}
		break;
	case 7:
		for (int i = 0; i < choicesCount; i++)
		{
			if (choices[i] == 0)
				cout << i + 1 << ". ";

			switch (count)
			{
			case 0:
				if (choices[i] == 0)
				{
					cout << "Try and talk your way out" << endl;
				}
				break;
			case 1:
				if (choices[i] == 0)
				{
					cout << "Try to surprise them and run" << endl;
				}
				break;
			case 2:
				if (choices[i] == 0)
				{
					cout << "Give up" << endl;
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
	int choices[] = {0, 0, 0, -1};

	cout << "You wake up in a dimly lit shelter deep within the Zone, the air thick with tension.\n"
		 << "The faint sounds of distant gunfire echo, reminding you that danger is ever-present.\n"
		 << "You need to decide your next move to survive, find valuable" << magenta << " artifacts" << reset << ", and make it to Pripyat.\n"
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

void soloZone()
{
	char input;
	int choices[] = {0, 0, -1, -1};

	cout << "You are alone in the Zone.\n"
		 << "You need to find the way to Pripyat.\n"
		 << "All around you are the remnants of the past, the ruins of a world long gone.\n"
		 << "As you walk, you hear the distant howl of a mutant and the crackle of anomalies.\n"
		 << endl;

	printChoices(5, choices);
	getChar(input, 5, "Enter your choice: ", true);
	handleInput(5, input, choices);
}

void soloZone_car()
{
	char input;
	int choices[] = {0, 0, 0, -1};

	cout << "You approach the overturned truck and search for supplies.\n"
		 << "You find a valuable" << magenta << " artifact" << reset << " hidden in the back.\n"
		 << "You pocket the " << magenta << "artifact" << reset << " and continue on your way.\n"
		 << endl;

	addInventory(2);

	PressEnterToContinue();
	clearScreen();

	cout << "As you leave the truck, you hear the sound of footsteps approaching.\n"
		 << "You turn around and see a group of bandits emerging from the shadows.\n"
		 << "They demand to know what you're doing here and threaten you with their guns.\n"
		 << "You need to decide your next move quickly.\n"
		 << endl;

	printChoices(7, choices);
	getChar(input, 7, "Enter your choice: ", true);
	handleInput(7, input, choices);
}

void soloZone_house()
{
	char input;
	int choices[] = {0, 0, 0, -1};

	cout << "You approach the crumbling house and see that the door is ajar.\n"
		 << "You cautiously push it open and step inside.\n"
		 << "The interior is dark and musty, the air thick with dust.\n"
		 << "Just as you're about to leave, you notice a glint of light coming from the corner.\n"
		 << "You investigate and find a valuable" << magenta << " artifact" << reset << " hidden among the debris.\n"
		 << "You pocket the " << magenta << "artifact" << reset << " and head back outside, ready to continue your journey.\n"
		 << endl;

	PressEnterToContinue();
	clearScreen();

	cout << "As you leave the house, you hear the sound of footsteps approaching.\n"
		 << "You turn around and see a group of bandits emerging from the shadows.\n"
		 << "They demand to know what you're doing here and threaten you with their guns.\n"
		 << "You need to decide your next move quickly.\n"
		 << endl;

	printChoices(7, choices);
	getChar(input, 7, "Enter your choice: ", true);
	handleInput(7, input, choices);
}

void abandonedFactory()
{
	char input;
	int choices[] = {0, 0, -1, -1};

	cout << "As you head towards the seemingly abandoned factory, you are filled with a sense of dread.\n"
		 << "The factory looms before you, its walls crumbling and windows shattered.\n"
		 << "You can hear the distant sound of gunfire and the occasional scream.\n"
		 << "You know that danger lurks within, but you also know that valuable" << magenta << " artifacts" << reset << " can be found inside.\n"
		 << endl;

	PressEnterToContinue();
	clearScreen();

	cout << "As you enter the factory, you see signs of recent activity.\n"
		 << "There are overturned tables, broken chairs, and spent shell casings littering the floor.\n"
		 << "You hear the sound of footsteps echoing through the halls.\n"
		 << "Before you can react, a group of bandits emerges from the shadows, their guns trained on you.\n"
		 << "One of them steps forward and demands to know what you're doing here.\n"
		 << "You reply that you're just passing through the area.\n"
		 << "Before you can say anything else, you hear the sound of gunfire.\n"
		 << "A voice from upstairs yells, \"Intruders! Get them!\"\n"
		 << "You realize that you're caught in the middle of a firefight between the bandits and another group of stalkers.\n"
		 << "You need to decide your next move quickly.\n"
		 << endl;

	printChoices(6, choices);
	getChar(input, 6, "Enter your choice: ", true);
	handleInput(6, input, choices);
}

void abandonedFactory_stalkers()
{
	char qte;
	int rnd = random(1, 4);

	cout << "You decide to join the stalkers in laying siege to the bandit's hideout.\n"
		 << "You grab a nearby weapon and take cover behind a stack of crates.\n"
		 << "You manage to shoot two of the bandits before they can react and run outside to join the stalkers.\n"
		 << "You find cover behind a nearby car.\n"
		 << "From behind cover you catch your breath and prepare to shoot the bandits.\n"
		 << endl;

	PressEnterToContinue();
	clearScreen();

	cout << "From behind cover you see a nearby stalkers get hit and fall to the ground.\n"
		 << "You decide to grab his grenade, pull the pin, and throw it.\n"
		 << "After you hear the explosion you get ready to open fire.\n"
		 << "Get ready to shoot the bandits!\n"
		 << endl;

	qte = qteChar();
	PressEnterToContinue();
	cout << endl;

	cout << "Press '" << qte << "' to shoot the bandits: ";
	bool success = shootQTE(qte);
	clearScreen();

	if (success)
	{
		cout << "You take out the bandits and secure the factory.\n"
			 << "The stalkers cheer and loot the bandits' gear.\n"
			 << "You find a valuable" << magenta << " artifact" << reset << " and some supplies.\n"
			 << "You are hailed as a hero and are given a share of the loot.\n"
			 << "You are now a respected member of the group.\n"
			 << endl;

		if (FLAGS[0] == 0)
		{
			cout << "As you are about to leave the factory you notice a bench with a map on it.\n"
				 << "You pick up the map and see that it marks a location called Pripyat.\n"
				 << "You decide to keep the map and head towards Pripyat.\n"
				 << endl;

			addFlag(0);
		}

		PressEnterToContinue();
		clearScreen();

		cout << "You start your journey towards Pripyat.\n"
			 << "The road is long and dangerous, but you're determined to make it.\n"
			 << "You've heard stories of the artifacts that lie within the city, and you're eager to find them.\n"
			 << "As you walk, you can't help but feel a sense of unease.\n"
			 << "You're not sure what lies ahead, but you know that you're ready for whatever comes your way.\n"
			 << endl;

		PressEnterToContinue();
		clearScreen();

		roadToPripyat();
	}
	else
	{
		cout << "Game Over\n"
			 << "You somehow miss all your shots and are hit by the return fire.\n"
			 << "In your weakened state, you are unable to help your fellow stalkers.\n"
			 << "The bandits easily overwhelm your group.\n"
			 << "You and your fellow stalkers are looted and left defensless against the creatures of the zone.\n"
			 << "When night falls, you are taken by a pack of mutated dogs.\n"
			 << endl;

		PressEnterToContinue();
		getScreen(1);
	}
}

void abandonedFactory_bandits()
{
	char qte;
	int rnd = random(1, 4);

	cout << "You decide to help the bandits defend the factory.\n"
		 << "You grab a nearby weapon and take cover behind a stack of crates.\n"
		 << "You and the bandits exchange fire with the stalkers, the sound of gunshots echoing through the factory.\n"
		 << "Get ready to shoot the stalkers!\n"
		 << endl;

	qte = qteChar();

	PressEnterToContinue();
	cout << "Press '" << qte << "' to shoot the bandits: ";
	bool success = shootQTE(qte);

	clearScreen();

	if (success)
	{
		cout << "You manage take out several of the stalkers.\n"
			 << "The remaining stalkers retreat, leaving you and the bandits victorious.\n"
			 << "Before you can catch your breath, the bandit leader approaches you.\n"
			 << "\"You fought well,\" he says, offering you a share of the" << magenta << " artifacts" << reset << " they've collected.\n"
			 << "You accept the offer. The bandit leader starts to leave, but then turns back to you.\n"
			 << "He hands you a map with a location marked on it.\n"
			 << "\"You should join us in Pripyat to collect other valuable" << magenta << " artifacts" << reset << ".\"\n"
			 << "You nod, grateful for the opportunity.\n"
			 << "You and the bandits head out of the factory, ready to face the dangers of the Zone together.\n"
			 << endl;

		// TODO: Add Pripyat location to map
		// Give end story before road to Pripyat

		PressEnterToContinue();
		clearScreen();

		cout << "You start your journey towards Pripyat.\n"
			 << "The road is long and dangerous, but you're determined to make it.\n"
			 << "You've heard stories of the artifacts that lie within the city, and you're eager to find them.\n"
			 << "As you walk, you can't help but feel a sense of unease.\n"
			 << "You're not sure what lies ahead, but you know that you're ready for whatever comes your way.\n"
			 << endl;

		PressEnterToContinue();
		clearScreen();

		roadToPripyat();
	}
	else
	{
		cout << "You try to shoot the stalkers, but your aim is off.\n"
			 << "The stalkers return fire, hitting you in the shoulder.\n"
			 << "You fall to the ground, wounded and helpless.\n"
			 << "The bandits are quickly overwhelmed and you are left at the mercy of the stalkers.\n"
			 << endl;

		PressEnterToContinue();
		clearScreen();

		cout << "Game Over\n"
			 << "You and the bandits are taken captive by the stalkers.\n"
			 << "You and your fellow bandits are locked in a makeshift cell and left to rot.\n"
			 << "You eventually succumb to your injuries and die.\n"
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

	cout << "You approach the light and discover it's coming from a small campfire.\n"
		 << "A group of stalkers sits around it, looking wary but not hostile.\n"
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

	cout << "You learn that they are a group of stalkers who are also looking for" << magenta << " artifacts" << reset << ".\n"
		 << "They offer you a chance to join them in a raid on the nearby factory.\n"
		 << endl;

	printChoices(4, choices);
	getChar(input, 4, "Enter your choice: ", true);
	handleInput(4, input, choices);
}

void flickeringLight_healWoundedStalker()
{
	char input;
	int choices[] = {0, 0, -1, -1};

	clearScreen();
	cout << "You offer medical assistance to the wounded stalker using your medkit.\n"
		 << "Grateful, the stalker gives you a map that marks a location called Pripyat,\n"
		 << "rumored to contain valuable" << magenta << " artifacts" << reset << ".\n"
		 << "The stalkers are impressed by your kindness and offer you a chance to join them\n"
		 << "in a raid on the nearby factory that is sure to yield valuable" << magenta << " artifacts" << reset << ".\n"
		 << endl;

	// Add Pripyat location
	addFlag(0);
	removeInventory(1);

	PressEnterToContinue();
	clearScreen();

	// Duplicate, if I had more time I would do a printStory() function
	// Would also probably do a choices input for these because I could reuse the previous function by just cutting out the third option
	cout << "You offer medical assistance to the wounded stalker using your medkit.\n"
		 << "Grateful, the stalker gives you a map that marks a location called Pripyat,\n"
		 << "rumored to contain valuable" << magenta << " artifacts" << reset << ".\n"
		 << "The stalkers are impressed by your kindness and offer you a chance to join them\n"
		 << "in a raid on the nearby factory that is sure to yield valuable" << magenta << " artifacts" << reset << ".\n"
		 << endl;

	cout << "What will you do next?\n";
	printChoices(4, choices);
	getChar(input, 4, "Enter your choice: ", true);
	handleInput(4, input, choices);
}

bool shootQTE(char qte)
{
	bool success;

	success = quickTimeEvent(qte, 3);
	PressEnterToContinue();

	return success;
}

void factoryRaid_Stalkers()
{
	char qte;
	int rnd = random(1, 4);

	clearScreen();
	cout << "You join the raid on the factory.\n"
		 << "The stalkers are well-prepared and have a plan to take out the bandits.\n"
		 << "You are given an AK-47 and are told to cover the group as they advance.\n"
		 << endl;

	PressEnterToContinue();
	clearScreen();

	qte = qteChar();

	cout << "As you head towards the abandoned factory.\n"
		 << "You notice it's under control of bandits.\n"
		 << "As you creep closer you are noticed and shot at by bandits.\n"
		 << "Get ready to dodge the bullets!\n"
		 << endl;

	PressEnterToContinue();

	// Quick time event to dodge the bullets
	cout << "\nPress '" << qte << "' to dodge the bullets: ";

	bool dodged = quickTimeEvent(qte, 2);

	clearScreen();

	if (dodged)
	{
		resetInput();
		cout << "You dodge the bullets and take cover behind a nearby car.\n"
			 << "One of your fellow stalkers isn't so lucky and is hit.\n"
			 << "You decide to grab his grenade, pull the pin, and throw it.\n"
			 << "After you hear the explosion you get ready to open fire.\n"
			 << "Get ready to shoot the bandits!\n"
			 << endl;

		qte = qteChar();
		PressEnterToContinue();
		cout << endl;

		cout << "Press '" << qte << "' to shoot the bandits: ";
		bool success = shootQTE(qte);
		clearScreen();

		if (success)
		{
			cout << "You take out the bandits and secure the factory.\n"
				 << "The stalkers cheer and loot the bandits' gear.\n"
				 << "You find a valuable" << magenta << " artifact" << reset << " and some supplies.\n"
				 << "You are hailed as a hero and are given a share of the loot.\n"
				 << "You are now a respected member of the group.\n"
				 << endl;

			if (FLAGS[0] == 0)
			{
				cout << "As you are about to leave the factory you notice a bench with a map on it.\n"
					 << "You pick up the map and see that it marks a location called Pripyat.\n"
					 << "You decide to keep the map and head towards Pripyat.\n"
					 << endl;

				addFlag(0);
			}

			PressEnterToContinue();
			clearScreen();

			cout << "You start your journey towards Pripyat.\n"
				 << "The road is long and dangerous, but you're determined to make it.\n"
				 << "You've heard stories of the artifacts that lie within the city, and you're eager to find them.\n"
				 << "As you walk, you can't help but feel a sense of unease.\n"
				 << "You're not sure what lies ahead, but you know that you're ready for whatever comes your way.\n"
				 << endl;

			PressEnterToContinue();
			clearScreen();

			roadToPripyat();
		}
		else
		{
			cout << "You somehow miss all your shots and are hit by the return fire.\n"
				 << "In your weakened state, you are unable to help your fellow stalkers.\n"
				 << "The bandits easily overwhelm your group and leave you to be eaten by the creatures of the zone.\n"
				 << endl;

			PressEnterToContinue();
			getScreen(1);
		}
	}
	else
	{
		cout << "You are hit by a bullet and fall to the ground.\n"
			 << "The bandits approach you and take your gear.\n"
			 << "You are left to die in the Zone.\n"
			 << endl;

		PressEnterToContinue();
		getScreen(1);
	}
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

void roadToPripyat()
{
	cout << "After a long trechious journey you finally arrive at the outskirts of Pripyat.\n"
		 << "You have survived the dangers of the Zone and found valuable" << magenta << " artifacts" << reset << ".\n"
		 << "You have achieved your goal and can now rest easy knowing that you have made it to Pripyat.\n"
		 << "Congratulations, you have completed your adventure in the Zone!\n"
		 << endl;

	PressEnterToContinue();
	getScreen(1);
}

int main()
{
	cout << "\033]0;" << title << "\007";

	clearScreen();
	getScreen(1);

	return 0;
}