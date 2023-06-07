#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 6
#define TILE_SIZE 128
#define TILE_SPACING 5
#define BOARD_SIZE (GRID_SIZE * (TILE_SIZE + TILE_SPACING))

int mod = 0;

typedef struct {
	int x, y;
} Position;

typedef struct {
	Texture2D texture;
	bool revealed;
	bool matched;
} Card;

void shuffleCards(Card cards[], int size);

void menuLoop(bool * isGameRunning, int * opzione_selezionata) {
	// Menu Loop
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
		(*opzione_selezionata)--;
		if (*opzione_selezionata < 0) {
			*opzione_selezionata = 2;
		}
		printf("Tasto su op:%d!\n", *opzione_selezionata);
		
	} else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
		(*opzione_selezionata)++;
		if (*opzione_selezionata > 2) {
			*opzione_selezionata = 0;
		}
		printf("Tasto giu op:%d!\n", *opzione_selezionata);
	}
	
	int textY = GetScreenHeight() / 2 - 20;
	Vector2 mousePos = GetMousePosition();
	
	if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth() / 2 - MeasureText("GIOCO", 30) / 2, textY - 30, MeasureText("GIOCO", 30), 30})) {
		printf("Uno!\n");
		*opzione_selezionata = 0;		
	} else if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth() / 2 - MeasureText("IMPOSTAZIONI", 30) / 2, textY, MeasureText("IMPOSTAZIONI", 30), 30})) {
		printf("Due!\n");
		*opzione_selezionata = 1;		
	} else if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth() / 2 - MeasureText("USCITA", 30) / 2, textY + 30, MeasureText("USCITA", 30), 30})) {
		printf("Tre!\n");
		*opzione_selezionata = 2;		
	}

	//printf("Bottone premuto: %d %d %d\n" , IsMouseButtonPressed(MOUSE_LEFT_BUTTON), *opzione_selezionata, *isGameRunning);

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)) {
	
		if (*opzione_selezionata==0)	{
			mod = 1;
			*isGameRunning = true;  // Start the game
		}
		else if (*opzione_selezionata==2) {
			CloseWindow();
		}
		else if (*opzione_selezionata==1) {
			mod = 2;
		}
	}
	
	BeginDrawing();
	ClearBackground(BLACK);
	// debug
	printf("(%d", *opzione_selezionata);
	
	DrawText("MEMORY", GetScreenWidth() / 2 - MeasureText("MEMORY", 50) / 2, GetScreenHeight() / 2 - 300, 50, WHITE);
	DrawText("GAME", GetScreenWidth() / 2 - MeasureText("GAME", 30) / 2, textY - 30, 30, (*opzione_selezionata == 0) ? RED : WHITE);
	DrawText("SETTINGS", GetScreenWidth() / 2 - MeasureText("SETTINGS", 30) / 2, textY, 30, (*opzione_selezionata == 1) ? RED : WHITE);
	DrawText("EXIT", GetScreenWidth() / 2 - MeasureText("EXIT", 30) / 2, textY + 30, 30, (*opzione_selezionata == 2) ? RED : WHITE);
	printf(",%d)", *opzione_selezionata);
	EndDrawing();
}

void settingsLoop(int * gamemode) {
	int textY = GetScreenHeight() / 2 - 20;
	Vector2 mousePos = GetMousePosition();

	if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth() / 2 - MeasureText("COLORS", 30) / 2, textY - 30, MeasureText("COLORS", 30), 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		printf("Uno!\n");
		*gamemode = 1;
	} else if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth() / 2 - MeasureText("IMAGES", 30) / 2, textY, MeasureText("IMAGES", 30), 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		printf("Due!\n");
		*gamemode = 2;
	} else if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth() / 2 - MeasureText("BACK", 30) / 2, textY + 70, MeasureText("BACK", 30), 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		printf("Indietro!\n");
		mod = 0;
	}

	BeginDrawing();
	ClearBackground(BLACK);

	DrawText("SETTINGS", GetScreenWidth() / 2 - MeasureText("SETTINGS", 50) / 2, GetScreenHeight() / 2 - 300, 50, WHITE);
	DrawText("COLORS", GetScreenWidth() / 2 - MeasureText("COLORS", 30) / 2, textY - 30, 30, (*gamemode == 1) ? RED : WHITE);
	DrawText("IMAGES", GetScreenWidth() / 2 - MeasureText("IMAGES", 30) / 2, textY, 30, (*gamemode == 2) ? RED : WHITE);
	DrawText("BACK", GetScreenWidth() / 2 - MeasureText("BACK", 30) / 2, textY + 70, 30, WHITE);

	EndDrawing();
}

int main() {
	InitWindow(1920, 1080, "MEMORY");

	ToggleFullscreen();
	
	InitAudioDevice();

	Music musica = LoadMusicStream("music/freecp.mp3");
	printf("Musica caricata.");

	PlayMusicStream(musica);
	printf("Musica avviata.");

	SetTargetFPS(60);
	
	int score = 0;
	int gamemode = 1;
	int tileSize = TILE_SIZE + TILE_SPACING;
	int windowWidth = GRID_SIZE * tileSize;
	int windowHeight = GRID_SIZE * tileSize;
	int offsetX = (GetScreenWidth() - windowWidth) / 2;
	int offsetY = (GetScreenHeight() - windowHeight) / 2;
	printf("\nOffset: %d, Grid_size:%d, tileSize:%d, screenHeight:%d \n", offsetY, GRID_SIZE, tileSize, GetScreenHeight());

	int opzione_selezionata = 0;
	bool isGameRunning = false;


	while (!WindowShouldClose()) {

		UpdateMusicStream(musica);

		if (!isGameRunning && mod == 0) {
			
			menuLoop(&isGameRunning, &opzione_selezionata);
			
		}
		else if(mod==2){
			settingsLoop(&gamemode);
		}
		else if (mod==1) {

			Texture2D cardTextures[18];
			for (int i = 0; i < 18; i++) {
				char fileName[64];
				if (gamemode==1){
					snprintf(fileName, sizeof(fileName), "mod1/card_%d.png", i + 1);
				}
				else if (gamemode==2){
					snprintf(fileName, sizeof(fileName), "mod2/card_%d.png", i + 1);
				}
				printf("Caricata texture carta %d.\n", i);
				cardTextures[i] = LoadTexture(fileName);
			}

			// Game Loop
			Card cards[GRID_SIZE * GRID_SIZE];
			for (int i = 0; i < GRID_SIZE * GRID_SIZE; i += 2) {
				cards[i].texture = cardTextures[i / 2];
				cards[i].revealed = false;
				cards[i].matched = false;
		
				cards[i + 1].texture = cardTextures[i / 2];
				cards[i + 1].revealed = false;
				cards[i + 1].matched = false;
			}

			shuffleCards(cards, GRID_SIZE * GRID_SIZE);
	
			int cardSelected = -1;
			double cardRevealTimer = 0;
	
			while (isGameRunning) {

				if (score>=18) {
					score = 0;
					isGameRunning = false;
					mod = 0;
				};

				UpdateMusicStream(musica);

				BeginDrawing();
				ClearBackground(RAYWHITE);
	
				for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)	{
					Position pos = {i % GRID_SIZE, i / GRID_SIZE};
					int x = offsetX + pos.x * (TILE_SIZE + TILE_SPACING);
					int y = offsetY + pos.y * (TILE_SIZE + TILE_SPACING);
	
					if (cards[i].revealed || cards[i].matched) {
						DrawTexture(cards[i].texture, x, y, WHITE);
					}else{
						DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, DARKGRAY);
					}
				}
	
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					int x = (GetMouseX() - offsetX) / (TILE_SIZE + TILE_SPACING);
					int y = (GetMouseY() - offsetY) / (TILE_SIZE + TILE_SPACING);
					int index = y * GRID_SIZE + x;

					if (!cards[index].revealed && !cards[index].matched) {
						cards[index].revealed = true;
	
						if (cardSelected == -1)	{
							cardSelected = index;
						}else{
							if (cards[cardSelected].texture.id != cards[index].texture.id)	{
								cardRevealTimer = 0.5;
							}else{
								cards[cardSelected].matched = true;
								cards[index].matched = true;
								score++;
							}
							cardSelected = -1;
						}
					}
				}
	
				if (cardRevealTimer > 0) {
					cardRevealTimer -= GetFrameTime();
					if (cardRevealTimer <= 0) {
						for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
							if (!cards[i].matched) {
								cards[i].revealed = false;
							}
						}
					}
				}

				char scoreText[50];
				sprintf(scoreText, "Score: %d", score);
				DrawText(scoreText, GetScreenWidth() / 2 - MeasureText(scoreText, 40) / 2, GetScreenHeight() - 50, 40, BLACK);
				
	
				EndDrawing();
				
				if(IsKeyDown(KEY_ESCAPE)){
					isGameRunning = false;
					score = 0;
					mod = 0;
					WindowShouldClose();
				}
			}
			
		}
	}
	
	StopMusicStream(musica);

	UnloadMusicStream(musica);
	
	CloseAudioDevice();
	
	CloseWindow();
	return 0;
}

void shuffleCards(Card cards[], int size) {
	for (int i = 0; i < size - 1; i++) {
		int j = i + rand() / (RAND_MAX / (size - i) + 1);
		Card temp = cards[j];
		cards[j] = cards[i];
		cards[i] = temp;
	}
}
