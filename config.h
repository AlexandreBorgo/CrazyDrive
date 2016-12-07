#ifndef CONFIG_H
#define CONFIG_H

#define GAME_TITLE "Crazy Drive"

#define WINDOW_W 1080
#define WINDOW_H 720

#define ON 1
#define OFF 0

#define PROGRAM_RUN 0
#define PROGRAM_STOP 1

#define MENU_STATE 0
#define GAME_STATE 1
#define OPTION_STATE 2
#define TOPSCORE_STATE 3

#define GAME_BEGIN 0
#define GAME_PLAY 1
#define GAME_PAUSE 2
#define GAME_TOOVER 3
#define GAME_OVER 4

#define MODE_CLASSIC 0
#define MODE_RETRO 1
#define MODE_SECRET 2

#define RANDOM 0
#define EASY 1
#define MEDIUM 2
#define HARD 3

#define DELAY 0

#define IMG_ROAD_3 "images/road_3_2.png"
#define IMG_ROAD_4 "images/road_4_2.png"
#define IMG_ROAD_5 "images/road_5_2.png"
#define BACKGROUND "images/button_off.png"

#define IMG_CAR_YELLOW "images/yellow_car.png"
#define IMG_CAR_RED "images/red_car.png"

#define IMG_SPEED "images/speed.png"
#define IMG_NEEDLE "images/needle.png"

#define IMG_MENU_BACKGROUND "images/background.png"
#define IMG_BUTTON_ON "images/button_on.png"
#define IMG_BUTTON_OFF "images/button_off.png"

#define IMG_ANIM "images/sprite_explo.png"

#define CRASH_PATH "sounds/crash.wav"
#define BACKGROUND_MUSIC "sounds/background.wav"
#define START "sounds/start.wav"

#define CAR_W 60//50
#define CAR_H 130//114

#define SPEED_W 200
#define SPEED_H 200
#define NEEDLE_W 175
#define NEEDLE_H 175

#define CAR_POS_Y 580

#define VOIES_3 3
#define VOIES_4 4
#define VOIES_5 5

#define CAR_POS_1_3 424
#define CAR_POS_2_3 540
#define CAR_POS_3_3 656

#define CAR_POS_1_4 365
#define CAR_POS_2_4 481
#define CAR_POS_3_4 597
#define CAR_POS_4_4 713

#define CAR_POS_1_5 308
#define CAR_POS_2_5 424
#define CAR_POS_3_5 540
#define CAR_POS_4_5 656
#define CAR_POS_5_5 772

#define BG_1_POS_INIT_Y 0
#define BG_2_POS_INIT_Y -720
#define BG_3_POS_INIT_Y -1440

#define INIT_POS 3
#define POS_1 1
#define POS_2 2
#define POS_3 3
#define POS_4 4
#define POS_5 5

#define MAX_SPEED 30
#define ACC 0.5
#define DECC 0.5
#define BRAKE 0.8

#define COLLISION_LIMIT 100

#endif
