#ifndef GAME_UTILS_H_INCLUDED
#define GAME_UTILS_H_INCLUDED

#include "Game_Base.h"

#include "Enemy.h"
#include "Character.h"

bool Init();
bool LoadMedia();
void Close();

std::string GetHighScoreFromFile(std::string path);

void UpdateHighScore(std::string path,
    const int& score,
    const std::string& old_high_score);

int UpdateGameTimeAndScore(int& time, int& speed, int& score);

void RenderScrollingBackground(std::vector <double>& offsetSpeed,
    LTexture(&gBackgroundTexture)[BACKGROUND_LAYER],
    SDL_Renderer* gRenderer);

void RenderScrollingGround(int& speed,
    const int acceleration,
    LTexture gGroundTexture,
    SDL_Renderer* gRenderer);


void GenerateEnemy(Enemy& enemy1,
    Enemy& enemy2,
    Enemy& enemy3,
    SDL_Rect(&gEnemyClips)[FLYING_FRAMES],
    SDL_Renderer* gRenderer);

bool CheckColission(Character character,
    SDL_Rect* char_clip,
    Enemy enemy,
    SDL_Rect* enemy_clip = nullptr);

bool CheckEnemyColission(Character character,
    Enemy enemy1,
    Enemy enemy2,
    Enemy enemy3,
    SDL_Rect* char_clip,
    SDL_Rect* enemy_clip = nullptr);

void ControlCharFrame(int& frame);

void ControlEnemyFrame(int& frame);

void DrawPlayerScore(LTexture gTextTexture,
    LTexture gScoreTexture,
    SDL_Color textColor,
    SDL_Renderer* gRenderer,
    TTF_Font* gFont,
    const int& score);

void DrawPlayerHighScore(LTexture gTextTexture,
    LTexture gHighScoreTexture,
    SDL_Color textColor,
    SDL_Renderer* gRenderer,
    TTF_Font* gFont,
    const std::string& HighScore);

void DrawEndGameSelection(LTexture gLoseTexture,
    SDL_Event* e,
    SDL_Renderer* gRenderer,
    bool& Play_Again);


#endif // GAME_UTILS_H_INCLUDED
