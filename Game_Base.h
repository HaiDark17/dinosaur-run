#ifndef GAME_BASE_H_INCLUDED
#define GAME_BASE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#define SDL_ERROR 1
#define IMG_ERROR 2
#define MIX_ERROR 3
#define TTF_ERROR 4

#define MIX_CHANNEL -1

#define TIME_MAX 1000
#define GROUND 430
#define MAX_HEIGHT 250

#define BASE_OFFSET_SPEED 0

#define SPEED_INCREASEMENT 2
#define SCORE_INCREASEMENT 1
#define TIME_INCREASEMENT 1
#define FRAME_INCREASEMENT 1

#define GROUND_SPEED 5
#define ENEMY_SPEED 5
#define MAX_ENEMY_WIDTH 100

#define IN_AIR_ENEMY 1
#define ON_GROUND_ENEMY 0

#define SLOW_FRAME_CHAR 4
#define SLOW_FRAME_ENEMY 4

#define IS_REPEATITIVE -1
#define NOT_REPEATITIVE 0

#define SMALL_BUTTON 1
#define COMMON_BUTTON 2

const std::string WINDOW_TITLE = "Dinosaur Run";

const int SCREEN_WIDTH = 928;
const int SCREEN_HEIGHT = 522;



const int TEXT_1_POSX = 440;
const int TEXT_1_POSY = 20;
const int TEXT_2_POSX = 400;
const int TEXT_2_POSY = 80;
const int SCORE_POSX = 460;
const int SCORE_POSY = 45;
const int HIGH_SCORE_POSX = 455;
const int HIGH_SCORE_POSY = 105;

const double LAYER_1_SPEED = 0.0;


const int RUNNING_FRAMES = 8;
const int FLYING_FRAMES = 5;
const int BACKGROUND_LAYER = 1;


void LogError(std::string msg, int error_code = SDL_ERROR);

#endif // GAME_BASE_H_INCLUDED
