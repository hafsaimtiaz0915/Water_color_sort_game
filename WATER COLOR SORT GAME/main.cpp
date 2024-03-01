#include <iostream>
#include "raylib.h"
#include "levels.h"
#include "doublelinkedlist.h"
#include <ctime>

using namespace std;

// level related variables
#define MAX_LEVELS 10
Node *currentNode;
DoublyLinkedList doublyList;
int selectedLevel;

// Variables for tubes
int tubes_no = 0;
int tubesPerRow = 0;
Stack<int> *tubes_colors;
int selectedTube = -1;
int targetTube = -1;
Rectangle tubeRectangle[14];
Color colorChoice[] = {ORANGE, MAROON, YELLOW, GRAY, DARKBLUE, DARKPURPLE, PINK, SKYBLUE, DARKGREEN, LIME, DARKBROWN, GOLD};

// variables for moves and tubes selection
int leftMoves = 0;
bool selected = false;

// Variables for click
bool canClick = true;
clock_t lastClickTime = 0;
const double clickDelay = 0.5; // Delay in seconds before allowing another click

//  variables related to screen
float spacing = 0.0;
const int screenWidth = 800;
const int screenHeight = 600;
const int buttonSize = 80;
const int padding = 20;
bool gameOver = false;

// variables for music
Music backgroundMusic;
bool isMusicPlaying = false;

bool game();
// constant variables for menu selection
enum MenuOption
{
    Choose_Level,
    EXIT,
};

MenuOption currentOption = Choose_Level;

// Function to blend two colors based on a progress value
Color gradientColorBlend(const Color &color1, const Color &color2, float progress)
{
    Color result;

    result.r = static_cast<unsigned char>(color1.r + static_cast<int>((color2.r - color1.r) * progress));
    result.g = static_cast<unsigned char>(color1.g + static_cast<int>((color2.g - color1.g) * progress));
    result.b = static_cast<unsigned char>(color1.b + static_cast<int>((color2.b - color1.b) * progress));
    result.a = static_cast<unsigned char>(color1.a + static_cast<int>((color2.a - color1.a) * progress));

    return result;
}

// Functions for music

// Function to initialize audio
void InitAudio()
{
    InitAudioDevice();
}
// Function to load and play background music
void PlayBackgroundMusic(const char *filePath)
{
    if (!isMusicPlaying)
    {
        backgroundMusic = LoadMusicStream(filePath);
        SetMusicVolume(backgroundMusic, 0.5f);
        PlayMusicStream(backgroundMusic);
        isMusicPlaying = true;
    }
}
// Function to update background music
void UpdateBackgroundMusic()
{
    UpdateMusicStream(backgroundMusic);
}
// Function to stop and unload background music
void StopAndUnloadBackgroundMusic()
{
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
    isMusicPlaying = false;
}
// Function to close audio device
void CloseAudio()
{
    CloseAudioDevice();
}

// Function for Loading Screen
void LoadingScreen()
{

    // Set the progress bar colors
    Color progressBarColor = GRAY;
    Color progressBarBackColor = DARKGRAY;

    // Loading variables
    float progress = 0.0f;
    int screenWidthHalf = screenWidth / 2;
    int progressBarWidth = 400;
    int progressBarHeight = 30;
    int progressBarCornerRadius = 15;

    // narrowing conversion warnings by casting to float
    Rectangle progressBar = {
        static_cast<float>(screenWidthHalf - progressBarWidth / 2),
        static_cast<float>(screenHeight / 2 - progressBarHeight / 2),
        static_cast<float>(progressBarWidth),
        static_cast<float>(progressBarHeight)};

    // Slower amount of progress per frame
    float progressPerFrame = 0.005f;

    // Initializing audio if it is not already initialized
    if (!IsAudioDeviceReady())
        InitAudio();

    // Play background music for loading screen
    PlayBackgroundMusic("loadingsound.mp3");

    // Main game loop
    while ((!WindowShouldClose()) && (progress < 1.0f))
    {
        UpdateBackgroundMusic();

        // Updating the progress
        progress += progressPerFrame;
        if (progress > 1.0f)
            progress = 1.0f;

        // starting Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Drawing background gradient
        DrawRectangleGradientEx(
            {0, 0, screenWidth, screenHeight},
            gradientColorBlend(gradientColorBlend(gradientColorBlend(RAYWHITE, GRAY, progress), DARKGRAY, progress), {69, 44, 99, 255}, progress), // Dark Purple (#452C63)
            gradientColorBlend({0, 71, 146, 255}, {0, 34, 68, 255}, progress),                                                                     // Blue to Dark Blue
            gradientColorBlend({0, 71, 146, 255}, {0, 34, 68, 255}, progress),                                                                     // Blue to Dark Blue
            gradientColorBlend({0, 71, 146, 255}, {0, 34, 68, 255}, progress)                                                                      // Blue to Dark Blue
        );

        // Drawing progress bar background with curved borders
        DrawRectangleRounded(progressBar, static_cast<float>(progressBarCornerRadius), 0, progressBarBackColor);

        // Drawing progress bar with curved borders
        DrawRectangleRounded({progressBar.x, progressBar.y, progressBar.width * progress, progressBar.height}, static_cast<float>(progressBarCornerRadius), 0, progressBarColor);

        // Drawing title with increased size and higher position
        DrawText("Water Color Sort", screenWidthHalf - MeasureText("Water Color Sort", 50) / 2, screenHeight / 2 - 120, 50, RAYWHITE);

        // Drawing progress text using snprintf
        char progressText[32];
        snprintf(progressText, sizeof(progressText), "Loading... %.0f%%", progress * 100);
        DrawText(progressText, screenWidthHalf - MeasureText("Loading... 100%", 20) / 2, screenHeight / 2 + 50, 20, LIGHTGRAY);

        EndDrawing();
    }
    StopAndUnloadBackgroundMusic();
}

// Drawing Menu
void DrawMenu()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleGradientEx(
        {0, 0, screenWidth, screenHeight},
        gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5f),
        gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5f),
        gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5f),
        gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5f));

    // Drawing Title
    DrawText("Water Color Sort", screenWidth / 2 - MeasureText("Water Color Sort", 40) / 2, 80, 40, WHITE);

    // Draw menu options with highlighting
    for (int i = Choose_Level; i <= EXIT; ++i)
    {
        const char *optionText = "";
        int fontSize = 20;
        Color textColor = BLUE;

        switch (static_cast<MenuOption>(i))
        {
        case Choose_Level:
            optionText = "Choose Level";
            break;
        case EXIT:
            optionText = "Exit";
            break;
        default:
            break;
        }

        if (currentOption == static_cast<MenuOption>(i))
        {
            // Drawing Rectangle Around Highlighted option and changing the fontsize and color
            DrawRectangleLines(screenWidth / 2 - 150, 200 + i * 50, 300, 40, PURPLE);
            fontSize = 40;
            textColor = DARKBLUE;
        }

        DrawText(optionText, screenWidth / 2 - MeasureText(optionText, fontSize) / 2, 200 + i * 50, fontSize, textColor);
    }
    EndDrawing();
}

// Generating Game Functions
// These functions handle the generation and initialization of game levels.

// Function to generate all game levels
void generateGame()
{
    Level level;
    for (int i = 1; i <= MAX_LEVELS; i++)
    {
        int **value = level.set_level(i);
        int tubesNo = level.get_noTubes();
        int leftMove = level.get_leftMoves();

        Stack<int> *tubes = new Stack<int>[tubesNo];

        for (int j = 0; j < tubesNo; j++)
        {
            tubes[j].set_size(4);
        }

        for (int j = 0; j < tubesNo - 2; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                tubes[j].push(value[j][k]);
            }
        }
        doublyList.appendNode(tubes, tubesNo, leftMove);
    }
}

// Function to generate a specific level
void generateLevel(int l)
{
    Node *temp = doublyList.head;
    for (int i = 1; i < l; i++)
    {
        if (temp == nullptr)
        {
            cout << "Level " << l << " doesn't exist." << endl;
            return;
        }
        temp = temp->next;
    }
    currentNode = temp;

    if (temp != nullptr)
    {
        tubes_no = temp->tubes_no;
        leftMoves = temp->leftMove;
        tubes_colors = new Stack<int>[tubes_no];
        for (int j = 0; j < tubes_no; j++)
        {
            tubes_colors[j].set_size(4);
        }
        for (int i = 0; i < tubes_no - 2; i++)
        {
            tubes_colors[i] = temp->stacks[i];
        }
    }
    else
    {
        cout << "Level " << l << " doesn't exist." << endl;
    }
}

// Level Navigation Functions
// These functions handle moving between levels in the game sequence.

// Function to move to next level also updating selected level
void moveToNextLevel()
{
    currentNode = currentNode->next;
    tubes_no = currentNode->tubes_no;
    leftMoves = currentNode->leftMove;
    selectedLevel++;
    tubes_colors = new Stack<int>[tubes_no];
    for (int j = 0; j < tubes_no; j++)
    {
        tubes_colors[j].set_size(4);
    }
    for (int i = 0; i < tubes_no - 2; i++)
    {
        tubes_colors[i] = currentNode->stacks[i];
    }
}

// Function to move to previous level also updating selected level
void moveToPrevLevel()
{
    currentNode = currentNode->prev;
    tubes_no = currentNode->tubes_no;
    leftMoves = currentNode->leftMove;
    selectedLevel--;
    tubes_colors = new Stack<int>[tubes_no];
    for (int j = 0; j < tubes_no; j++)
    {
        tubes_colors[j].set_size(4);
    }
    for (int i = 0; i < tubes_no - 2; i++)
    {
        tubes_colors[i] = currentNode->stacks[i];
    }
}

// Function to draw gameplay tubes
void drawTubes()
{
    int upper, lower;
    if (tubes_no == 7)
    {
        upper = 70;
        lower = 50;
    }
    else if (tubes_no == 9)
    {
        upper = 50;
        lower = 35;
    }
    else if (tubes_no == 11)
    {
        upper = 40;
        lower = 25;
    }
    else if (tubes_no == 14)
    {
        upper = 25;
        lower = 25;
    }
    bool offset = true;
    if (tubes_no % 2 == 0)
    {
        offset = true;
        tubesPerRow = tubes_no / 2;
    }
    else
    {
        offset = false;
        tubesPerRow = (tubes_no / 2) + 1;
    }

    if (tubesPerRow == 0)
    {
        cout << "error" << endl;
    }
    else
    {
        spacing = screenWidth / tubesPerRow;
    }
    for (int i = 0; i < tubesPerRow; i++)
    {
        for (int j = 0; j < tubes_colors[i].get_capacity(); j++)
        {
            DrawRectangle(upper + spacing * i, 200 - (50 * j), 65, 50, colorChoice[tubes_colors[i].get_index(j)]);
        }
    }
    for (int i = 0; i < tubesPerRow; i++)
    {
        Rectangle outer = {upper + spacing * i, 50, 65, 200};
        DrawRectangleLinesEx(outer, 2, WHITE);
        tubeRectangle[i] = outer;
    }
    if (offset)
    {

        for (int i = 0; i < tubesPerRow; i++)
        {

            for (int j = 0; j < tubes_colors[i + tubesPerRow].get_capacity(); j++)
            {
                DrawRectangle(lower + spacing * i, 450 - (50 * j), 65, 50, colorChoice[tubes_colors[tubesPerRow + i].get_index(j)]);
            }
        }

        for (int i = 0; i < tubesPerRow; i++)
        {
            Rectangle outer = {lower + (spacing * i), 300, 65, 200};
            DrawRectangleLinesEx(outer, 2, WHITE);
            tubeRectangle[i + tubesPerRow] = outer;
        }
    }
    else
    {

        for (int i = 0; i < (tubes_no - tubesPerRow); i++)
        {

            if (tubes_colors[i + tubesPerRow].get_capacity() > 0)
            {
                for (int j = 0; j < tubes_colors[i + tubesPerRow].get_capacity(); j++)
                {
                    DrawRectangle(lower + (spacing * 0.6) + spacing * i, 450 - (50 * j), 65, 50, colorChoice[tubes_colors[tubesPerRow + i].get_index(j)]);
                }
            }
        }

        for (int i = 0; i < (tubes_no - tubesPerRow); i++)
        {
            Rectangle outer = {lower + static_cast<float>((spacing * 0.6) + (spacing * i)), 300, 65, 200};
            DrawRectangleLinesEx(outer, 2, WHITE);
            tubeRectangle[i + tubesPerRow] = outer;
        }
    }
    if (selectedTube != -1)
    {
        if (tubes_no % 2 != 0)
        {
            if (selectedTube <= tubes_no / 2)
            {
                Rectangle selectedRect = {upper + spacing * selectedTube, 50, 65, 200};
                DrawRectangleLinesEx(selectedRect, 4, YELLOW);
            }
            else
            {
                // Second row tube
                int adjustedIndex = selectedTube - (tubes_no + 1) / 2;
                Rectangle selectedRect = {lower + static_cast<float>((spacing * 0.6) + spacing * adjustedIndex), 300, 65, 200};
                DrawRectangleLinesEx(selectedRect, 4, YELLOW);
            }
        }
        else
        {
            if (selectedTube < tubes_no / 2)
            {
                // First row tube
                Rectangle selectedRect = {upper + spacing * selectedTube, 50, 65, 200};
                DrawRectangleLinesEx(selectedRect, 4, YELLOW);
            }
            else
            {
                // Second row tube
                int adjustedIndex = selectedTube - (tubes_no + 1) / 2;
                Rectangle selectedRect = {lower + spacing * adjustedIndex, 300, 65, 200};
                DrawRectangleLinesEx(selectedRect, 4, YELLOW);
            }
        }
    }
}
// Functions for Checking Game Conditions
// Function to check victory condition
bool checkVictory()
{
    bool won = true;
    for (int i = 0; i < tubes_no; i++)
    {
        if (tubes_colors[i].get_capacity() > 0)
        {
            if (tubes_colors[i].get_capacity() != 4)
            {
                won = false;
            }
            else
            {
                int top_colour = tubes_colors[i].get_top();
                for (int j = 0; j < 3; j++)
                {
                    if (tubes_colors[i].get_index(2 - j) != top_colour)
                    {
                        won = false;
                    }
                }
            }
        }
    }

    return won;
}
// Function to check Valid move
int checkvalidMove()
{
    int length = 0;
    for (int i = 0; i < tubes_no; i++)
    {
        int fromTube = i;
        for (int j = 0; j < tubes_no; j++)
        {
            if (i != j)
            {
                int toTube = j;
                if (tubes_colors[toTube].get_capacity() == 0)
                    return true;
                int colorFrom = tubes_colors[fromTube].get_top();
                int colorTo = tubes_colors[toTube].get_top();
                if (tubes_colors[toTube].get_capacity() < 4 && colorFrom == colorTo)
                {
                    length = 0;
                    for (int k = 0; k < tubes_colors[fromTube].get_capacity(); k++)
                    {

                        if (tubes_colors[fromTube].get_index(tubes_colors[fromTube].get_capacity() - k - 1) == colorFrom)
                        {
                            length += 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (4 - tubes_colors[toTube].get_capacity() - length >= 0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Function to perform a move
bool Move(int selectedTube, int targetTube)
{
    int length = 0;
    int colorontop = -1;
    int colortomove = -1;
    bool flag = false;
    if (selectedTube == targetTube)
    {
        selectedTube = -1;
        return false;
    }
    if (tubes_colors[selectedTube].get_capacity() > 0)
    {
        colortomove = tubes_colors[selectedTube].get_top();
        for (int i = 0; i < tubes_colors[selectedTube].get_capacity(); i++)
        {

            if (tubes_colors[selectedTube].get_index(tubes_colors[selectedTube].get_capacity() - i - 1) == colortomove)
            {
                length += 1;
            }
            else
            {
                break;
            }
        }
    }
    if (tubes_colors[targetTube].get_capacity() < 4)
    {
        if (tubes_colors[targetTube].get_capacity() == 0)
        {
            colorontop = colortomove;
        }
        else
        {
            colorontop = tubes_colors[targetTube].get_top();
        }
        if (colorontop == colortomove)
        {

            for (int i = 0; i < length; i++)
            {
                if (tubes_colors[targetTube].get_capacity() < 4)
                {
                    if (tubes_colors[selectedTube].get_capacity() > 0)
                    {
                        tubes_colors[targetTube].push(colorontop);
                        tubes_colors[selectedTube].pop();
                        flag = true;
                    }
                }
            }
            if (flag)
                leftMoves--;
        }
        else
        {
            return false;
        }
    }
    return true;
}



void UpdateMouseClick()
{
    if (!canClick)
    {
        // Check if the delay time has passed since the last click
        clock_t currentTime = clock();
        double elapsed = (double)(currentTime - lastClickTime) / CLOCKS_PER_SEC;
        if (elapsed >= clickDelay)
        {
            canClick = true; // Enable clicking after the delay
        }
    }
}

// Function Related to Level Selection Screen
int GetLevelAtMousePosition(int mouseX, int mouseY)
{
    for (int i = 1; i <= MAX_LEVELS; i++)
    {
        int row = (i - 1) / 5;
        int col = (i - 1) % 5;

        int gridWidth = 5 * (buttonSize + padding) - padding;
        int gridHeight = 2 * (buttonSize + padding) - padding;

        int buttonX = screenWidth / 2 - gridWidth / 2 + (buttonSize + padding) * col;
        int buttonY = screenHeight / 2 - gridHeight / 2 + (buttonSize + padding) * row;

        float radius = buttonSize / 2.0f;

        // Check if the mouse is inside the circle
        if (CheckCollisionPointCircle({(float)mouseX, (float)mouseY}, {buttonX + radius, buttonY + radius}, radius))
        {
            return i;
        }
    }

    return -1; // No level found at the mouse position
}

void DrawLevelButton(int level, bool selected, bool highlighted)
{
    int row = (level - 1) / 5;
    int col = (level - 1) % 5;

    int gridWidth = 5 * (buttonSize + padding) - padding;
    int gridHeight = 2 * (buttonSize + padding) - padding;

    int buttonX = screenWidth / 2 - gridWidth / 2 + (buttonSize + padding) * col;
    int buttonY = screenHeight / 2 - gridHeight / 2 + (buttonSize + padding) * row;

    float radius = buttonSize / 2.0f;

    Color buttonColor = selected ? DARKBLUE : (highlighted ? BLUE : SKYBLUE);

    DrawCircle(buttonX + radius, buttonY + radius, radius, buttonColor);
    DrawCircleLines(buttonX + radius, buttonY + radius, radius, RAYWHITE);

    // Drawing level number with dark blue color
    DrawText(TextFormat("%d", level), buttonX + radius - MeasureText(TextFormat("%d", level), 24) / 2, buttonY + radius - 12, 24, selected ? SKYBLUE : DARKBLUE);
}
int DrawLevelSelectionMenu()
{
    int selectedLevel = 0;

    while (!WindowShouldClose())
    {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        UpdateMouseClick();

        int highlightedLevel = GetLevelAtMousePosition(mouseX, mouseY);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && highlightedLevel != -1 && canClick)
        {
            canClick = false;
            lastClickTime = clock();
            selectedLevel = highlightedLevel;
            printf("Selected Level: %d\n", selectedLevel);
            break; // Exit the level selection menu when a level is selected
        }

        BeginDrawing();

        // Draw level selection menu
        ClearBackground(BLACK);

        DrawRectangleGradientEx(
            {0, 0, screenWidth, screenHeight},
            gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5),
            gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5),
            gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5),
            gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5));

        DrawText("WATER COLOR SORT", screenWidth / 2 - MeasureText("WATER COLOR SORT", 40) / 2, padding + 60, 40, SKYBLUE);
        DrawText("LEVEL SELECTION", screenWidth / 2 - MeasureText("LEVEL SELECTION", 20) / 2, padding + 100, 20, SKYBLUE);

        for (int i = 1; i <= MAX_LEVELS; i++)
        {
            bool isHighlighted = (i == highlightedLevel);
            DrawLevelButton(i, i == selectedLevel, isHighlighted);
        }

        EndDrawing();
    }

    return selectedLevel;
}
// Functions Related to hint
bool CanHint(int fromTube, int toTube)
{
    bool flag = false;
    int length = 0;
    if (fromTube < 0 || fromTube >= tubes_no || toTube < 0 || toTube >= tubes_no || fromTube == toTube)
        return false;

    int topIndexFrom = tubes_colors[fromTube].get_capacity() - 1;
    int topIndexTo = tubes_colors[toTube].get_capacity() - 1;

    if (topIndexTo < 0)
        return true;

    if (topIndexFrom < 0)
        return false;

    int colorFrom = tubes_colors[fromTube].get_top();
    int colorTo = tubes_colors[toTube].get_top();
    if (tubes_colors[toTube].get_capacity() < 4 && colorFrom == colorTo)
    {
        for (int i = 0; i < tubes_colors[fromTube].get_capacity(); i++)
        {

            if (tubes_colors[fromTube].get_index(tubes_colors[fromTube].get_capacity() - i - 1) == colorFrom)
            {
                length += 1;
            }
            else
            {
                break;
            }
        }
        if (4 - tubes_colors[toTube].get_capacity() - length >= 0)
        {
            flag = true;
        }
        else
        {
            flag = false;
        }
        if (flag)
            return true;
    }

    return false;
}
// function to Draw a rectangle around the top element of the tube
void DrawHintRectangles(int i, int j)
{
    DrawRectangleLinesEx(tubeRectangle[i], 10, GREEN);
    DrawRectangleLinesEx(tubeRectangle[j], 10, GREEN);
}

void Hint()
{
    // Forward iteration
    for (int i = 0; i < tubes_no; ++i)
    {
        for (int j = 0; j < tubes_no; ++j)
        {
            if (CanHint(i, j))
            {
                DrawHintRectangles(i, j);
                return; // Stop hinting after finding the first valid pair
            }
        }
    }

    // Backward iteration
    for (int i = tubes_no - 1; i >= 0; --i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (CanHint(i, j))
            {
                DrawHintRectangles(i, j);
                return;
            }
        }
    }
}
// Functions for screens
// Function to handle winning screen
void winning()
{
    // Button dimensions
    const int buttonWidth = 120;
    const int buttonHeight = 45;
    const int buttonGap = 20;
    const int cornerRadius = 8;
    Color buttonColor = SKYBLUE;
    Color buttonHoverColor = DARKBLUE;
Vector2 nextButtonPos,prevButtonPos;
    // Calculate the total width of the buttons with the gap
    int totalButtonWidth = 2 * buttonWidth + buttonGap;

    // Center position for the buttons
    float centerX = screenWidth / 2 - totalButtonWidth / 2 + buttonWidth;

    // Button positions
    if ((selectedLevel > 1)&&(selectedLevel != 10)){
     nextButtonPos = {(float)(centerX + buttonWidth / 2 + buttonGap / 2), (float)(screenHeight / 2 + 60)};
    }
    else{
             nextButtonPos = {(float)(screenWidth / 2 - buttonWidth/2), (float)(screenHeight / 2 + 60)};
    }
    if ((selectedLevel > 1)&&(selectedLevel != 10))
     prevButtonPos = {(float)(centerX - buttonWidth / 2 - buttonGap / 2 - buttonWidth), (float)(screenHeight / 2 + 60)};
    else{
             prevButtonPos = {(float)(screenWidth / 2 - buttonWidth/2), (float)(screenHeight / 2 + 60)};

    }

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawRectangleGradientEx(
            {0, 0, screenWidth, screenHeight},
            gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5f),
            gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5f),
            gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5f),
            gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5f));

        DrawText("WATER COLOR SORT", screenWidth / 2 - MeasureText("WATER COLOR SORT", 40) / 2, screenHeight / 2 - 200, 40, SKYBLUE);
        DrawText(TextFormat("Level: %d", selectedLevel), screenWidth / 2 - MeasureText("Level 1", 30) / 2, screenHeight / 2 - 100, 30, WHITE);

        // Drawing "You Win!" text
        DrawText("YOU WIN!", screenWidth / 2 - MeasureText("You Win!", 40) / 2, screenHeight / 2 - 20, 40, GREEN);
        if (selectedLevel != 10)
        {
            // Drawing "Next" button with rounded corners
            DrawRectangleRounded((Rectangle){nextButtonPos.x, nextButtonPos.y, buttonWidth, buttonHeight}, (float)cornerRadius, 0, SKYBLUE);
            DrawText("Next", nextButtonPos.x + 20, nextButtonPos.y + 10, 20, WHITE);
        }
        // Drawing "Previous" button
        if (selectedLevel != 1)
{
    // Drawing "Previous" button with rounded corners
    DrawRectangleRounded((Rectangle){prevButtonPos.x, prevButtonPos.y, buttonWidth, buttonHeight}, (float)cornerRadius, 0, SKYBLUE);
    DrawText("Previous", prevButtonPos.x + 10, prevButtonPos.y + 10, 20, WHITE);
}
        if (selectedLevel != 10)
        {
            // Drawing "Next" button with rounded corners and handle hover effect
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){nextButtonPos.x, nextButtonPos.y, buttonWidth, buttonHeight}))
            {
                DrawRectangleRounded((Rectangle){nextButtonPos.x, nextButtonPos.y, buttonWidth, buttonHeight}, (float)cornerRadius, 0, buttonHoverColor);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    // Handle button click
                    moveToNextLevel();
                    bool restartGame;
                    do
                    {
                        restartGame = game();
                        if (gameOver)
                        {
                            gameOver = false;
                            generateLevel(selectedLevel);
                            game();
                        }

                        if (restartGame)
                        {
                            ClearBackground(BLACK);
                            generateLevel(selectedLevel);
                        }
                    } while (restartGame);
                    break;
                }
            }
            else
            {
                DrawRectangleRounded((Rectangle){nextButtonPos.x, nextButtonPos.y, buttonWidth, buttonHeight}, (float)cornerRadius, 0, buttonColor);
            }
            DrawText("Next", nextButtonPos.x + 20, nextButtonPos.y + 10, 20, WHITE);
        }
        if (selectedLevel != 1)
        {
            // Drawing "Previous" button with rounded corners and handle hover effect
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){prevButtonPos.x, prevButtonPos.y, buttonWidth, buttonHeight}))
            {
                DrawRectangleRounded((Rectangle){prevButtonPos.x, prevButtonPos.y, buttonWidth, buttonHeight}, (float)cornerRadius, 0, buttonHoverColor);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    // Handle button click
                    moveToPrevLevel();
                    bool restartGame;
                    do
                    {
                        restartGame = game();
                        if (gameOver)
                        {
                            gameOver = false;
                            generateLevel(selectedLevel);
                            game();
                        }

                        if (restartGame)
                        {
                            ClearBackground(BLACK);
                            generateLevel(selectedLevel);
                        }
                    } while (restartGame);
                    break;
                }
            }
            else
            {
                DrawRectangleRounded((Rectangle){prevButtonPos.x, prevButtonPos.y, buttonWidth, buttonHeight}, (float)cornerRadius, 0, buttonColor);
            }
            DrawText("Previous", prevButtonPos.x + 10, prevButtonPos.y + 10, 20, WHITE);
        }
        EndDrawing();
    }
}

// Function to Game Over screen
bool GameOverScreen()
{
    const int buttonWidth = 120;
    const int buttonHeight = 45;
    const int cornerRadius = 8;
    Color buttonColor = SKYBLUE;
    Color buttonHoverColor = DARKBLUE;

    if (!IsAudioDeviceReady())
        InitAudio();
    PlayBackgroundMusic("mixkit-slow-sad-trombone-fail-472.wav");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        UpdateBackgroundMusic();

        // Draw background gradient
        DrawRectangleGradientEx(
            {0, 0, screenWidth, screenHeight},
            gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5),
            gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5),
            gradientColorBlend(BLACK, {0, 71, 146, 255}, 0.5),
            gradientColorBlend(BLACK, {69, 44, 99, 255}, 0.5));

        // Draw level text
        DrawText(TextFormat("Level: %d", selectedLevel), screenWidth / 2 - MeasureText("Level 1", 30) / 2, screenHeight / 2 - 180, 30, WHITE);

        if (!checkvalidMove())
            // Draw "No valid Move!" and "Game Over!" text
            DrawText("No valid Move!", screenWidth / 2 - MeasureText("No valid Move!", 40) / 2, screenHeight / 2 - 60, 40, GRAY);
        else
        {
            DrawText("No More Move!", screenWidth / 2 - MeasureText("No more Moves!", 40) / 2, screenHeight / 2 - 120, 40, GRAY);
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 60, 40, RED);
        }

        // Check if the mouse is over the "Restart" button
        if (CheckCollisionPointRec(GetMousePosition(), {screenWidth / 2 - buttonWidth / 2, screenHeight / 2 + 60, buttonWidth, buttonHeight}))
        {
            DrawRectangleRounded((Rectangle){screenWidth / 2 - buttonWidth / 2, screenHeight / 2 + 60, buttonWidth, buttonHeight}, (float)cornerRadius, 0, buttonHoverColor);
            DrawText("Restart", screenWidth / 2 - MeasureText("Restart", 20) / 2, screenHeight / 2 + 75, 20, WHITE);

            // Check for mouse click and restart the game
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                gameOver = true;
                return true;
            }
        }
        else
        {
            DrawRectangleRounded((Rectangle){screenWidth / 2 - buttonWidth / 2, screenHeight / 2 + 60, buttonWidth, buttonHeight}, (float)cornerRadius, 0, buttonColor);
            DrawText("Restart", screenWidth / 2 - MeasureText("Restart", 20) / 2, screenHeight / 2 + 75, 20, WHITE);
        }

        EndDrawing();
    }
    StopAndUnloadBackgroundMusic();
    return false;
}
bool game()
{
    bool win = false;
    int duplicate;

    while (!WindowShouldClose())
    {

        UpdateMouseClick();
        BeginDrawing();
        ClearBackground(BLACK);
        if (leftMoves != 0 && checkvalidMove())
        {
            if (!win)
            {
                drawTubes();
                DrawText("Hint", 20, 20, 20, WHITE);
                DrawText("Menu", screenWidth - MeasureText("Menu", 20) - 20, 20, 20, WHITE);
                if (leftMoves > 0)
                    DrawText(TextFormat("Moves: %d", leftMoves), screenWidth / 2 - MeasureText("Moves: 0", 20) / 2, screenHeight - 40, 20, WHITE);
                DrawText("Restart", 20, screenHeight - 40, 20, WHITE);
                DrawText(TextFormat("Level: %d", selectedLevel), screenWidth / 2 - MeasureText("Level 1", 20) / 2, 20, 20, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canClick)
                {
                    canClick = false;
                    if (!selected)
                    {
                        for (int i = 0; i < tubes_no; i++)
                        {
                            if (CheckCollisionPointRec(GetMousePosition(), tubeRectangle[i]))
                            {
                                selectedTube = i;
                                duplicate = i;
                                selected = true;

                                break;
                            }
                            else
                            {
                                selectedTube = -1;
                                selected = false;
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < tubes_no; i++)
                        {
                            if (CheckCollisionPointRec(GetMousePosition(), tubeRectangle[i]))
                            {

                                targetTube = i;
                                selected = false;
                                if (Move(duplicate, targetTube))
                                {
                                    selectedTube = i;
                                }
                                else
                                {
                                    selectedTube = -1;
                                }
                                break;
                            }
                            else
                            {
                                selectedTube = -1;
                                selected = false;
                            }
                        }
                    }

                    if (CheckCollisionPointRec(GetMousePosition(), {20, screenHeight - 40, static_cast<float>(MeasureText("Restart", 20)), 20}))
                    {

                        EndDrawing();

                        return true;
                    }
                    else if (CheckCollisionPointRec(GetMousePosition(), {20, 20, static_cast<float>(MeasureText("Hint", 20)), 20}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        Hint();
                    }
                    else if (CheckCollisionPointRec(GetMousePosition(), { screenWidth - static_cast<float>(MeasureText("Menu", 20) ), 20, static_cast<float>(MeasureText("Menu", 20)), 20}))
                    {
                        break;
                    }
                }

                win = checkVictory();

                if (win)
                {
                    winning();
                    win = false;
                    break;
                }
            }
        }
        else
        {
            if (GameOverScreen())
            {
                generateLevel(selectedLevel);
                // Continue the game loop
            }
            else
            {
                return false; // Exit the game function when the game is over
            }
        }
        EndDrawing();
    }

    return false;
}

int main()
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Water Color Sort - Loading");
    SetTargetFPS(60);
    LoadingScreen();
    generateGame();
    bool flag = true;

    while (!WindowShouldClose())
    {
            Vector2 mousePos = GetMousePosition();
            UpdateMouseClick();

            for (int i = Choose_Level; i <= EXIT; ++i)
            {
                Rectangle optionRect = {screenWidth / 2 - 150,static_cast<float>( 200 + i * 50), 300, 40};
                if (CheckCollisionPointRec(mousePos, optionRect))
                {
                    currentOption = static_cast<MenuOption>(i);
                    break;
                }
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canClick)
            {
                switch (currentOption)
                {
                case Choose_Level:
                {
                    canClick = false;
                    lastClickTime = clock();
                    selectedLevel = DrawLevelSelectionMenu();
                    generateLevel(selectedLevel);
                    bool restartGame;
                    do
                    {
                        restartGame = game();
                        if (gameOver)
                        {
                            gameOver = false;
                            generateLevel(selectedLevel);
                            game();
                        }

                        if (restartGame)
                        {
                            ClearBackground(BLACK);
                            generateLevel(selectedLevel);
                        }
                    } while (restartGame);

                    break;
                }

                case EXIT:
                    flag = false;
                    break;
                }
            }

            DrawMenu();
            if (!flag)
            {
                break;
            }
    }

    CloseWindow();

    return 0;
}