//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
//------------------------------------------------------------------------

using namespace std;

//Current position and what is in that position. The string is mostly used for making the map and checking if the player is colliding with something
struct Position {
	int x;
	int y;
	string currentObj;
};

//Struct with all the important info of the player
struct PlayerInfo {
	int x;
	int y;
	int playerScore;
	string lastMovement;
};

//Current level the player is in
int currentLevel = 1;

//Variables for the timer to place pressure on the player to not spend to much time in each level
float timePerLevel = 75000; //150 seconds / 2.5 minutes
float timeLeftLevel = timePerLevel;

//Boolean for when the game ends
bool gameEnded;

//Sets up the map size.
int mapSizeX = 15;
int mapSizeY = 15;
//Size of the boxes that will be drawn for the walls and finish post
int boxSize = 0;

//Sets up the 2d matrix map for each level using the position struct
Position gameMap[15][15] = {};

//Sets up the player struct
PlayerInfo player;

//Methods I use
void GuIDisplay();
void MakeLevel(int mapSizeX, int mapSizeY);
void ShowMap(int mapSizeX, int mapSizeY);
void Box(float startLine1xVal, float startLine1yVal, string currentObj);
void IncreaseScore();
void NewLevel();
void EndOfGame();

CSimpleSprite *testSprite;
enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	testSprite = App::CreateSprite(".\\TestData\\UbisoftNextCharacterArt.bmp", 4, 7);

	//Makes the initial map
	MakeLevel(mapSizeX, mapSizeY);

	//Sets the animation for the sprites
	float speed = 2.0f / 10.0f;
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 0,4,8,12,16,20,24 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 1,5,9,13,17,21,25 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 2,6,10,14,18,22,26 });
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 3,7,11,15,19,23,27 });
	testSprite->SetScale(0.5f);

	gameEnded = false;
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	testSprite->Update(deltaTime);
	
	//A timer to add some challenge and so the player dosent stay for to long attempting to get each block in each level
	timeLeftLevel -= deltaTime;
	if (timeLeftLevel <= 0) {
		gameEnded = true;
	}

	//Checks the strings in the 4 adjacent blocks to the character and saves them to a string variable
	string rightPosObj = gameMap[player.x + 1][player.y].currentObj;
	string leftPosObj = gameMap[player.x - 1][player.y].currentObj;
	string upPosObj = gameMap[player.x][player.y + 1].currentObj;
	string downPosObj = gameMap[player.x][player.y - 1].currentObj;

	//If the D key was pressed on a windows divice and if the spot to the right isnt a wall then it will move the character one spot in the grid to the right
	if (App::GetController().GetLeftThumbStickX() > 0.5f && rightPosObj != "Wall" && rightPosObj != "Breakable Wall")
	{
		//timeButtonPressed += deltaTime;
		player.x = player.x + 1;
		player.lastMovement = "Right";
		int x = player.x;
		int y = player.y;
		//Grabs the map cordinates to place the player sprite
		int gameMapX = gameMap[x][y].x;
		int gameMapY = gameMap[x][y].y;
		//Updates the players sprite and animation
		testSprite->SetPosition(gameMapX + 25, gameMapY + 25);
		testSprite->SetAnimation(ANIM_RIGHT);
	}
	//If the A key was pressed on a windows divice and if the spot to the right isnt a wall then it will move the character one spot in the grid to the right
	if (App::GetController().GetLeftThumbStickX() < -0.5f && leftPosObj != "Wall" && leftPosObj != "Breakable Wall")
	{
		//Updates the players struct info
		player.x = player.x - 1;
		player.lastMovement = "Left";
		int x = player.x;
		int y = player.y;
		//Grabs the map cordinates to place the player sprite
		int gameMapX = gameMap[x][y].x;
		int gameMapY = gameMap[x][y].y;
		//Updates the players sprite and animation
		testSprite->SetPosition(gameMapX + 25, gameMapY + 25);
		testSprite->SetAnimation(ANIM_LEFT);
	}
	//If the W key was pressed on a windows divice and if the spot to the right isnt a wall then it will move the character one spot in the grid to the right
	//Will then update the players position in the struct and the sprites position and animation.
	if (App::GetController().GetLeftThumbStickY() < -0.5f && upPosObj != "Wall" && upPosObj != "Breakable Wall")
	{
		//Updates the players struct info
		player.y = player.y + 1;
		player.lastMovement = "Up";
		int x = player.x;
		int y = player.y;
		//Grabs the map cordinates to place the player sprite
		int gameMapX = gameMap[x][y].x;
		int gameMapY = gameMap[x][y].y;
		//Updates the players sprite and animation
		testSprite->SetPosition(gameMapX + 25, gameMapY + 25);
		testSprite->SetAnimation(ANIM_FORWARDS);
	}
	//If the S key was pressed on a windows divice and if the spot to the right isnt a wall then it will move the character one spot in the grid to the right
	if (App::GetController().GetLeftThumbStickY() > 0.5f && downPosObj != "Wall" && downPosObj != "Breakable Wall")
	{
		//Updates the players struct info
		player.y = player.y - 1;
		player.lastMovement = "Down";
		int x = player.x;
		int y = player.y;
		//Grabs the map cordinates to place the player sprite
		int gameMapX = gameMap[x][y].x;
		int gameMapY = gameMap[x][y].y;
		//Updates the players sprite and animation
		testSprite->SetPosition(gameMapX + 25, gameMapY + 25);
		testSprite->SetAnimation(ANIM_BACKWARDS);
	}
	if (GetAsyncKeyState(0x45)) {
		if (player.lastMovement == "Right" && rightPosObj == "Breakable Wall") {
			gameMap[player.x + 1][player.y].currentObj = "Empty";
			IncreaseScore();
			testSprite->SetAnimation(ANIM_RIGHT);
		}
		else if (player.lastMovement == "Left" && leftPosObj == "Breakable Wall") {
			gameMap[player.x - 1][player.y].currentObj = "Empty";
			IncreaseScore();
			testSprite->SetAnimation(ANIM_LEFT);
		}
		else if (player.lastMovement == "Up" && upPosObj == "Breakable Wall") {
			gameMap[player.x][player.y + 1].currentObj = "Empty";
			IncreaseScore();
			testSprite->SetAnimation(ANIM_FORWARDS);
		}
		else if (player.lastMovement == "Down" && downPosObj == "Breakable Wall") {
			gameMap[player.x][player.y - 1].currentObj = "Empty";
			IncreaseScore();
			testSprite->SetAnimation(ANIM_BACKWARDS);
		}
		else if (gameMap[player.x][player.y].currentObj == "End Point") {
			//End of level game
			NewLevel();
		}
	}

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		testSprite->SetScale(testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		testSprite->SetScale(testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		testSprite->SetAnimation(-1);
	}
}

//------------------------------------------------------------------------
// Renders items to the screen
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	testSprite->Draw();

	GuIDisplay();

	if (gameEnded) {
		EndOfGame();
	}
	else {
		ShowMap(mapSizeX, mapSizeY);
	}
}

//Basic GUI that draws the current level, players score, the time thats left and some basic instructions so that newer users can understand what the goal is
void GuIDisplay() {
	App::Print(765, 745, "Current Level:");
	App::Print(885, 745, std::to_string(currentLevel).c_str());
	App::Print(765, 720, "Current Score:");
	App::Print(890, 720, std::to_string(player.playerScore).c_str());

	float timeInFloat = timeLeftLevel / 60000;
	string timerInString = std::to_string(timeInFloat);
	App::Print(765, 695, "Time left:");
	App::Print(845, 695, timerInString.c_str());

	App::Print(765, 650, "Goal of game:");
	App::Print(765, 625, "Destroy as green blocks!");
	App::Print(765, 600, "Each green block destroyed");
	App::Print(765, 580, "is 50 points!");
	App::Print(765, 555, "Finish the level before timer");
	App::Print(765, 535, "runs out.");
	App::Print(765, 500, "Press W to move Up");
	App::Print(765, 475, "Press S to move Down");
	App::Print(765, 450, "Press A to move Left");
	App::Print(765, 425, "Press D to move Right");
	App::Print(765, 400, "Press E to activate action.");
}

void MakeLevel(int mapSizeX, int mapSizeY) {
	//Sets up the players position
	player = { 1 , 1, player.playerScore, "None" };
	testSprite->SetPosition(gameMap[1][1].x * 50 + 25, gameMap[1][1].y * 50 + 25);
	gameMap[1][1].currentObj = "Player";

	//The two open positions next to the player are set as player to avoid a breakable wall being set up next to them and blocking them in and creating a soft lock.
	gameMap[2][1].currentObj = "Player";
	gameMap[1][2].currentObj = "Player";
	
	//Gives the random method the current time as a seed to allow for a more sudo random number
	srand((unsigned)time(NULL));
	//Goes through each spot in the map and decides what type of block will exist there there
	for (int i = 0; i < mapSizeX; i++) {
		for (int j = 0; j < mapSizeY; j++) {
			int randomInt = rand(); //Randomly gives a number between 0 and 32767 while using the current time as a seed to calculate the value
			string currentPosObj = gameMap[i][j].currentObj;
			if (i == 0 || j == 0 || i == 14 || j == 14 || ((i%2 == 0) && (j%2 == 0))) {
				gameMap[i][j] = { i * 50 + 5, j * 50 + 5, "Wall" }; // initialize position with x and y coordinates and set up as wall
			}
			else if (i == 13 && j == 13) {
				gameMap[i][j] = { i * 50 + 7, j * 50 + 7, "End Point" }; // initialize position with x and y coordinates and add end point
			}
			else if (randomInt < 10922 && currentPosObj != "Player" && currentPosObj != "End Point") { //Checks if random value is less then a fourth of rand() and what is currently there
				gameMap[i][j] = { i * 50 + 7, j * 50 + 7, "Breakable Wall" }; // initialize position with x and y coordinates and set up as a breakable wall
			}
			else {
				gameMap[i][j] = { i * 50 + 7, j * 50 + 7, "Empty" }; // initialize position with x and y coordinates and sets it as an empty point to draw nothing
			}
		}
	}
}

//Will draw the map in the render method
void ShowMap(int mapSizeX, int mapSizeY) {
	int posX = 50;
	int posY = 50;
	//Loop through each position in the 2d matrix of gameMap and as long as its not an empty box then it will print it.
	for (int i = 0; i < mapSizeX; i++) {
		for (int j = 0; j < mapSizeY; j++) {
			//Gets the current object in the position
			string currentObj = gameMap[i][j].currentObj;
			if (currentObj != "Empty") {
				Box(gameMap[i][j].x, gameMap[i][j].y, currentObj);
			}
		}
	}
}

//Will draw a box to symbolize if the position on the map is either a normal wall, breakable wall or the end poing of the player. Each with their own color to better destinguish
void Box(float startLinexVal, float startLineyVal, string currentObj) {
	float r;
	float g;
	float b;

	if (currentObj == "Wall") { //If it is an unpassible wall then it will paint it red and make the sides with length 50
		r = 0.85546f;
		g = 0.09375f;
		b = 0.09375f;
		boxSize = 50;
	}
	else if (currentObj == "Breakable Wall") { //If the wall is part of the level and can be broken then paint it green and make the length of a side 46 (So that it dosent collide with the wall
		r = 0.0f;
		g = 0.8f;
		b = 0.0f;
		boxSize = 46;
	}
	else if (currentObj == "End Point") { //If the point is the end then color it blue and make the side lengths of 46
		r = 0.0f;
		g = 0.0f;
		b = 0.8f;
		boxSize = 46;
	}
	else { //If in the futrue an extra type of box is created and there isnt a specific color given then make it white
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		boxSize = 46;
	}

	//Side 1
	float startLine1x = startLinexVal;
	float startLine1y = startLineyVal;
	float endLine1x = startLine1x;
	float endLine1y = startLine1y + boxSize;

	//Side 2
	float startLine2x = endLine1x;
	float startLine2y = endLine1y;
	float endLine2x = startLine2x + boxSize;
	float endLine2y = startLine2y;

	//Side 3
	float startLine3x = endLine2x;
	float startLine3y = endLine2y;
	float endLine3x = startLine3x;
	float endLine3y = startLine3y - boxSize;

	//Side 4
	float startLine4x = endLine3x;
	float startLine4y = endLine3y;
	float endLine4x = startLine4x - boxSize;
	float endLine4y = startLine4y;

	//Draw the box
	App::DrawLine(startLine1x, startLine1y, endLine1x, endLine1y, r, g, b);
	App::DrawLine(startLine2x, startLine2y, endLine2x, endLine2y, r, g, b);
	App::DrawLine(startLine3x, startLine3y, endLine3x, endLine3y, r, g, b);
	App::DrawLine(startLine4x, startLine4y, endLine4x, endLine4y, r, g, b);
}

//Increase the score of the player
void IncreaseScore() {
	player.playerScore += 50;
}

//Once the player finishes a level create a new one. 
//Every 3 levels reduce the max time by 15 seconds so that it progressivly becomes harder to get all the boxes before the time runs out
void NewLevel() {
	MakeLevel(mapSizeX, mapSizeY);
	currentLevel++;
	if (currentLevel%3 == 0 && timePerLevel > 15000) {
		timePerLevel -= 15000;
	}
	timeLeftLevel = timePerLevel;
}

//When the game has ended display a quick message congratulating them on playing the game and showing their final score
void EndOfGame() {
	App::Print(300, 400, "Game has ended! Great Job!!");
	App::Print(335, 370, "Your final score was:");
	App::Print(400, 340, std::to_string(player.playerScore).c_str());
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	//------------------------------------------------------------------------
}