#include "Game_Base.h"
#include "Game_Utils.h"
#include "LTexture.h"

#include "Character.h"
#include "Enemy.h"

 std::string LAYER[BACKGROUND_LAYER] = {
	"imgs/background/9 background.png",
};
 std::string LAYER2[BACKGROUND_LAYER] = {
	"imgs/background/m_desert-background.png",
 };
 Character character;
 const int fps = 80;
 const int frameDelay = 1000 / fps;
 Uint32 frameStart;
 int frameTime;

 static SDL_Event g_event;
SDL_Window* gWindow = nullptr;
static SDL_Renderer* gRenderer = nullptr;
SDL_Color textColor = { 0, 0, 0 };
TTF_Font* gFont = nullptr;
Mix_Music* gMusic = nullptr;


Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;


SDL_Rect gCharacterClips[RUNNING_FRAMES];
SDL_Rect gEnemyClips[FLYING_FRAMES];

LTexture gMenuTexture;
LTexture gInstructionTexture;
LTexture gBackgroundTexture[BACKGROUND_LAYER];
LTexture gBackgroundTexture2[BACKGROUND_LAYER];
LTexture gCharacterTexture;
LTexture gGroundTexture;


LTexture gLoseTexture;
LTexture gText1Texture;
LTexture gScoreTexture;
LTexture gText2Texture;
LTexture gHighScoreTexture;

int main(int argc, char* argv[])
{


	if (!Init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!LoadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{

			bool Play_Again = false;
            bool Menu = true;
    while(Menu)
            {
                while(SDL_PollEvent(&g_event)!= 0)
                {
                    if(g_event.type == SDL_QUIT)
                        {
                            Menu = false;
                        }
                        switch (g_event.key.keysym.sym)
                        {
                            case SDLK_SPACE:
                            {
                                Menu = false;
                               Play_Again = true;
                            }
                            break;
                            case SDLK_ESCAPE:
                            {
                                 Play_Again = false;
                                 Menu = false;
                            }
                }
                gMenuTexture.Render(0, 0, gRenderer);
                SDL_RenderPresent(gRenderer );
                }
            }
			while (Play_Again)
			{
				srand(time(NULL));
				int time = 0;
				int score = 0;
				int acceleration = 0;
				int frame_Character = 0;
				int frame_Enemy = 0;
				std::string highscore = GetHighScoreFromFile("high_score.txt");

				SDL_Event e;
				Enemy enemy1(ON_GROUND_ENEMY);
				Enemy enemy2(ON_GROUND_ENEMY);
				Enemy enemy3(IN_AIR_ENEMY);

				Mix_PlayMusic(gMusic, IS_REPEATITIVE);
				GenerateEnemy(enemy1, enemy2, enemy3, gEnemyClips, gRenderer);

				int OffsetSpeed_Ground = BASE_OFFSET_SPEED;
				std::vector <double> OffsetSpeed_Bkgr(BACKGROUND_LAYER, BASE_OFFSET_SPEED);
				bool Quit = false;
				bool Game_State = true;
				while (!Quit)
				{
					frameStart = SDL_GetTicks();
					if (Game_State)
					{
						UpdateGameTimeAndScore(time, acceleration, score);
						while (SDL_PollEvent(&e) != 0)
						{
							if (e.type == SDL_QUIT)
							{
								Quit = true;
								Play_Again = false;
							}

							character.HandleEvent(e, gJump);
						}
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gRenderer);

						RenderScrollingBackground(OffsetSpeed_Bkgr, gBackgroundTexture2, gRenderer);
						if((score/50)%2==1){ RenderScrollingBackground(OffsetSpeed_Bkgr, gBackgroundTexture, gRenderer); }
						RenderScrollingGround(OffsetSpeed_Ground, acceleration, gGroundTexture, gRenderer);

						character.Move();
						SDL_Rect* currentClip_Character = nullptr;
						if (character.OnGround())
						{
							currentClip_Character = &gCharacterClips[frame_Character / SLOW_FRAME_CHAR];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}
						else
						{
							currentClip_Character = &gCharacterClips[0];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}
						enemy1.Move(acceleration);
						enemy1.Render(gRenderer);

						enemy2.Move(acceleration);
						enemy2.Render(gRenderer);

						SDL_Rect* currentClip_Enemy = &gEnemyClips[frame_Enemy / SLOW_FRAME_ENEMY];
						enemy3.Move(acceleration);
						enemy3.Render(gRenderer, currentClip_Enemy);

						DrawPlayerScore(gText1Texture, gScoreTexture, textColor, gRenderer, gFont, score);
						if (CheckEnemyColission(character,
							enemy1, enemy2, enemy3,
							currentClip_Character, currentClip_Enemy))
						{
							Mix_PauseMusic();
							Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
							UpdateHighScore("high_score.txt", score, highscore);
							Quit = true;
						}
						SDL_RenderPresent(gRenderer);
						ControlCharFrame(frame_Character);
						ControlEnemyFrame(frame_Enemy);
					}
					frameTime = SDL_GetTicks() - frameStart;
					if (frameDelay > frameTime)
					{
						SDL_Delay(frameDelay - frameTime);
					}DrawPlayerHighScore(gText2Texture, gHighScoreTexture, textColor, gRenderer, gFont, highscore);
				}
				DrawEndGameSelection(gLoseTexture, &e, gRenderer, Play_Again);
				if (!Play_Again)
				{
					enemy1.~Enemy();
					enemy2.~Enemy();
					enemy3.~Enemy();
				}

			}

		}
	}
	Close();

	return 0;
}



bool Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO) < 0)
	{

		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					success = false;
				}
				if (TTF_Init() == -1)
				{
					success = false;
				}
			}
		}
	}
	return success;
}

bool LoadMedia()
{
	bool success = true;
	gJump = Mix_LoadWAV("sound/jump_sound.wav");
	gLose = Mix_LoadWAV("sound/Lose.wav");
	gMusic = Mix_LoadMUS("sound/bg_audio.wav");
	if (gMusic == nullptr||gJump == nullptr||gLose == nullptr)
	{

		success = false;
	}
	else
	{
		gFont = TTF_OpenFont("font/OpenSans-Bold.ttf", 28);
		if (gFont == NULL)
		{

			success = false;
		}
		else
		{
			if (!gText1Texture.LoadFromRenderedText("Score", gFont, textColor, gRenderer))
			{

				success = false;
			}

			if (!gText2Texture.LoadFromRenderedText("Best score ", gFont, textColor, gRenderer))
			{

				success = false;
			}

			if (!gMenuTexture.LoadFromFile("imgs/background/menu.png", gRenderer))
			{

				success = false;
			}

			for (int i = 0; i < BACKGROUND_LAYER; ++i)
			{
				if (!gBackgroundTexture[i].LoadFromFile(LAYER[i].c_str(), gRenderer))
				{

					success = false;
				}
			}
			for (int i = 0; i < BACKGROUND_LAYER; ++i)
			{
				if (!gBackgroundTexture2[i].LoadFromFile(LAYER2[i].c_str(), gRenderer))
				{

					success = false;
				}
			}
			if (!gGroundTexture.LoadFromFile("imgs/background/ground.png", gRenderer))
			{
				success = false;
			}
			if (!gCharacterTexture.LoadFromFile("imgs/character/dinosaur.png", gRenderer))
			{
				success = false;
			}
			else
			{
				gCharacterClips[0].x = 68 * 0;
				gCharacterClips[0].y = 0;
				gCharacterClips[0].w = 68;
				gCharacterClips[0].h = 68;

				gCharacterClips[1].x = 68* 1;
				gCharacterClips[1].y = 0;
				gCharacterClips[1].w = 68;
				gCharacterClips[1].h = 68;

				gCharacterClips[2].x = 68 * 2;
				gCharacterClips[2].y = 0;
				gCharacterClips[2].w = 68;
				gCharacterClips[2].h = 68;

				gCharacterClips[3].x = 68 * 3;
				gCharacterClips[3].y = 0;
				gCharacterClips[3].w = 68;
				gCharacterClips[3].h = 68;

				gCharacterClips[4].x = 68 * 4;
				gCharacterClips[4].y = 0;
				gCharacterClips[4].w = 68;
				gCharacterClips[4].h = 68;

				gCharacterClips[5].x = 68 * 5;
				gCharacterClips[5].y = 0;
				gCharacterClips[5].w = 68;
				gCharacterClips[5].h = 68;

				gCharacterClips[6].x = 68 * 6;
				gCharacterClips[6].y = 0;
				gCharacterClips[6].w = 68;
				gCharacterClips[6].h = 68;

				gCharacterClips[7].x = 68 * 7;
				gCharacterClips[7].y = 0;
				gCharacterClips[7].w = 68;
				gCharacterClips[7].h = 68;

			}
			if (!gLoseTexture.LoadFromFile("imgs/background/gameover.png", gRenderer))
			{
				success = false;
			}

		}
	}
	return success;
}
void Close()
{
	gMenuTexture.Free();
	gInstructionTexture.Free();
	gCharacterTexture.Free();
	gGroundTexture.Free();

	gLoseTexture.Free();
	gText1Texture.Free();
	gScoreTexture.Free();
	gText2Texture.Free();
	gHighScoreTexture.Free();

	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		gBackgroundTexture[i].Free();
	}
	Mix_FreeMusic(gMusic);
	Mix_FreeChunk(gLose);
	Mix_FreeChunk(gJump);
	gMusic = nullptr;
	gLose = nullptr;
	gJump = nullptr;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

}

