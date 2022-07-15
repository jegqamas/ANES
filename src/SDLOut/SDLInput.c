// This file is part of ANES (Agile Nes)
//
// A Nintendo Entertainment System / Family Computer (Nes/Famicom)
// Emulator written in C.
//
// Copyright © Alaa Ibrahim Hadid 2021 - 2022
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.If not, see<http://www.gnu.org/licenses/>.
//
// Author email: mailto:alaahadidfreeware@gmail.com
//
#include "SDLInput.h"
#include "../NesEmu/APU.h"
#include <stdio.h>
#include "SDL2/SDL.h"

#include "SDL_RENDERERS_DEFINES.h"

const unsigned char* currentKeyStates;
int Pl1_KeyUp;
int Pl1_KeyDown;
int Pl1_KeyLeft;
int Pl1_KeyRight;
int Pl1_KeyStart;
int Pl1_KeySelect;
int Pl1_KeyA;
int Pl1_KeyB;
int Pl1_KeyTurboA;
int Pl1_KeyTurboB;
char pl1_turbo;

int Pl2_KeyUp;
int Pl2_KeyDown;
int Pl2_KeyLeft;
int Pl2_KeyRight;
int Pl2_KeyStart;
int Pl2_KeySelect;
int Pl2_KeyA;
int Pl2_KeyB;
int Pl2_KeyTurboA;
int Pl2_KeyTurboB;
char pl2_turbo;

char gc_ready;

void(*update_pl1)();
void(*update_pl2)();

SDL_GameControllerButton Pl1_gc_KeyUp;
SDL_GameControllerButton Pl1_gc_KeyDown;
SDL_GameControllerButton Pl1_gc_KeyLeft;
SDL_GameControllerButton Pl1_gc_KeyRight;
SDL_GameControllerButton Pl1_gc_KeyStart;
SDL_GameControllerButton Pl1_gc_KeySelect;
SDL_GameControllerButton Pl1_gc_KeyA;
SDL_GameControllerButton Pl1_gc_KeyB;
SDL_GameControllerButton Pl1_gc_KeyTurboA;
SDL_GameControllerButton Pl1_gc_KeyTurboB;


SDL_GameControllerButton Pl2_gc_KeyUp;
SDL_GameControllerButton Pl2_gc_KeyDown;
SDL_GameControllerButton Pl2_gc_KeyLeft;
SDL_GameControllerButton Pl2_gc_KeyRight;
SDL_GameControllerButton Pl2_gc_KeyStart;
SDL_GameControllerButton Pl2_gc_KeySelect;
SDL_GameControllerButton Pl2_gc_KeyA;
SDL_GameControllerButton Pl2_gc_KeyB;
SDL_GameControllerButton Pl2_gc_KeyTurboA;
SDL_GameControllerButton Pl2_gc_KeyTurboB;

SDL_GameController* gc_1;
SDL_GameController* gc_2;


char IsKeyPressed(int key)
{
    if (currentKeyStates==NULL)
        return 0;
    return currentKeyStates[SDL_GetScancodeFromKey(key)];
}

void UpdateBlank() {}
void UpdateKeyboardPlayer1()
{
    pl1_turbo = !pl1_turbo;

    PLAYER1_DATA = 0;

    if (IsKeyPressed(Pl1_KeyA))
        PLAYER1_DATA |= 1;

    if (IsKeyPressed(Pl1_KeyB))
        PLAYER1_DATA |= 2;

    if (IsKeyPressed(Pl1_KeyTurboA) && pl1_turbo)
        PLAYER1_DATA |= 1;

    if (IsKeyPressed(Pl1_KeyTurboB) && pl1_turbo)
        PLAYER1_DATA |= 2;

    if (IsKeyPressed(Pl1_KeySelect))
        PLAYER1_DATA |= 4;

    if (IsKeyPressed(Pl1_KeyStart))
        PLAYER1_DATA |= 8;

    if (IsKeyPressed(Pl1_KeyUp))
        PLAYER1_DATA |= 0x10;

    if (IsKeyPressed(Pl1_KeyDown))
        PLAYER1_DATA |= 0x20;

    if (IsKeyPressed(Pl1_KeyLeft))
        PLAYER1_DATA |= 0x40;

    if (IsKeyPressed(Pl1_KeyRight))
        PLAYER1_DATA |= 0x80;
}
void UpdateKeyboardPlayer2()
{
    pl2_turbo = !pl2_turbo;

    PLAYER2_DATA = 0;

    if (IsKeyPressed( Pl2_KeyA))
        PLAYER2_DATA |= 1;

    if (IsKeyPressed( Pl2_KeyB))
        PLAYER2_DATA |= 2;

    if (IsKeyPressed( Pl2_KeyTurboA) && pl1_turbo)
        PLAYER2_DATA |= 1;

    if (IsKeyPressed( Pl2_KeyTurboB) && pl1_turbo)
        PLAYER2_DATA |= 2;

    if (IsKeyPressed( Pl2_KeySelect))
        PLAYER2_DATA |= 4;

    if (IsKeyPressed( Pl2_KeyStart))
        PLAYER2_DATA |= 8;

    if (IsKeyPressed( Pl2_KeyUp))
        PLAYER2_DATA |= 0x10;

    if (IsKeyPressed( Pl2_KeyDown))
        PLAYER2_DATA |= 0x20;

    if (IsKeyPressed( Pl2_KeyLeft))
        PLAYER2_DATA |= 0x40;

    if (IsKeyPressed( Pl2_KeyRight))
        PLAYER2_DATA |= 0x80;
}

void CheckDirections(SDL_GameController* joystick, unsigned char * data)
{
    if (SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTY) <= -8000)
        (*data) |= 0x10;// Up

    if (SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTY) >= 8000)
        (*data) |= 0x20;// Down

    if (SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTX) <= -8000)
        (*data) |= 0x40;// Left

    if (SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTX) >= 8000)
        (*data) |= 0x80;// Right
}
void UpdateGCPlayer1()
{
    pl1_turbo = !pl1_turbo;

    PLAYER1_DATA = 0;

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyA) != 0)
        PLAYER1_DATA |= 1;

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyB) != 0)
        PLAYER1_DATA |= 2;

    if ((SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyTurboA) != 0) && pl1_turbo)
        PLAYER1_DATA |= 1;

    if ((SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyTurboB) != 0) && pl1_turbo)
        PLAYER1_DATA |= 2;

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeySelect) != 0)
        PLAYER1_DATA |= 4;

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyStart) != 0)
        PLAYER1_DATA |= 8;

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyUp) != 0)
        PLAYER1_DATA |= 0x10;// Up

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyDown) != 0)
        PLAYER1_DATA |= 0x20;// Down

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyLeft) != 0)
        PLAYER1_DATA |= 0x40;// Left

    if (SDL_GameControllerGetButton(gc_1, Pl1_gc_KeyRight) != 0)
        PLAYER1_DATA |= 0x80;// Right

    CheckDirections(gc_1, &PLAYER1_DATA);
}
void UpdateGCPlayer2()
{
    pl2_turbo = !pl2_turbo;

    PLAYER2_DATA = 0;

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyA) != 0)
        PLAYER2_DATA |= 1;

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyB) != 0)
        PLAYER2_DATA |= 2;

    if ((SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyTurboA) != 0) && pl2_turbo)
        PLAYER2_DATA |= 1;

    if ((SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyTurboB) != 0) && pl2_turbo)
        PLAYER2_DATA |= 2;

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeySelect) != 0)
        PLAYER2_DATA |= 4;

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyStart) != 0)
        PLAYER2_DATA |= 8;

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyUp) != 0)
        PLAYER2_DATA |= 0x10;// Up

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyDown) != 0)
        PLAYER2_DATA |= 0x20;// Down

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyLeft) != 0)
        PLAYER2_DATA |= 0x40;// Left

    if (SDL_GameControllerGetButton(gc_2, Pl2_gc_KeyRight) != 0)
        PLAYER2_DATA |= 0x80;// Right

    CheckDirections(gc_2, &PLAYER2_DATA);
}

void SDLInputUpdate()
{
    currentKeyStates = SDL_GetKeyboardState(NULL);
    update_pl1();
    update_pl2();
}

void SDLInputInitialize()
{
    Pl1_KeyUp =  SDL_GetKeyFromName("Up");
    Pl1_KeyDown = SDL_GetKeyFromName("Down");
    Pl1_KeyLeft = SDL_GetKeyFromName("Left");
    Pl1_KeyRight = SDL_GetKeyFromName("Right");
    Pl1_KeyStart =SDL_GetKeyFromName("B");
    Pl1_KeySelect = SDL_GetKeyFromName("V");
    Pl1_KeyA = SDL_GetKeyFromName("C");
    Pl1_KeyB = SDL_GetKeyFromName("X");
    Pl1_KeyTurboA = SDL_GetKeyFromName("D");
    Pl1_KeyTurboB = SDL_GetKeyFromName("S");

    Pl2_KeyUp =  SDL_GetKeyFromName("Up");
    Pl2_KeyDown = SDL_GetKeyFromName("Down");
    Pl2_KeyLeft = SDL_GetKeyFromName("Left");
    Pl2_KeyRight = SDL_GetKeyFromName("Right");
    Pl2_KeyStart =SDL_GetKeyFromName("B");
    Pl2_KeySelect = SDL_GetKeyFromName("V");
    Pl2_KeyA = SDL_GetKeyFromName("C");
    Pl2_KeyB = SDL_GetKeyFromName("X");
    Pl2_KeyTurboA = SDL_GetKeyFromName("D");
    Pl2_KeyTurboB = SDL_GetKeyFromName("S");


    Pl1_gc_KeyUp = SDL_CONTROLLER_BUTTON_DPAD_UP;
    Pl1_gc_KeyDown=SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    Pl1_gc_KeyLeft=SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    Pl1_gc_KeyRight=SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    Pl1_gc_KeyStart=SDL_CONTROLLER_BUTTON_START;
    Pl1_gc_KeySelect=SDL_CONTROLLER_BUTTON_BACK;
    Pl1_gc_KeyA=SDL_CONTROLLER_BUTTON_A;
    Pl1_gc_KeyB=SDL_CONTROLLER_BUTTON_X;
    Pl1_gc_KeyTurboA=SDL_CONTROLLER_BUTTON_B;
    Pl1_gc_KeyTurboB=SDL_CONTROLLER_BUTTON_Y;


    Pl2_gc_KeyUp = SDL_CONTROLLER_BUTTON_DPAD_UP;
    Pl2_gc_KeyDown=SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    Pl2_gc_KeyLeft=SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    Pl2_gc_KeyRight=SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    Pl2_gc_KeyStart=SDL_CONTROLLER_BUTTON_START;
    Pl2_gc_KeySelect=SDL_CONTROLLER_BUTTON_BACK;
    Pl2_gc_KeyA=SDL_CONTROLLER_BUTTON_A;
    Pl2_gc_KeyB=SDL_CONTROLLER_BUTTON_X;
    Pl2_gc_KeyTurboA=SDL_CONTROLLER_BUTTON_B;
    Pl2_gc_KeyTurboB=SDL_CONTROLLER_BUTTON_Y;

    if (Player1_ControlMode==SDLInput_ControlMode_GameController || Player2_ControlMode==SDLInput_ControlMode_GameController)
        SDL_Init(SDL_INIT_GAMECONTROLLER);

    switch(Player1_ControlMode)
    {
    case SDLInput_ControlMode_Keyboard:
    {
        update_pl1=UpdateKeyboardPlayer1;
        break;
    }
    case SDLInput_ControlMode_GameController:
    {
        update_pl1=UpdateGCPlayer1;

        gc_1= SDL_GameControllerOpen(Player1_GC_Index);
        break;
    }
    case SDLInput_ControlMode_NONE:
    {
        update_pl1=UpdateBlank;
        break;
    }
    }

    switch(Player2_ControlMode)
    {
    case SDLInput_ControlMode_Keyboard:
    {
        update_pl2=UpdateKeyboardPlayer2;
        break;
    }
    case SDLInput_ControlMode_GameController:
    {
        update_pl2=UpdateGCPlayer2;
        gc_2= SDL_GameControllerOpen(Player2_GC_Index);
        break;
    }
    case SDLInput_ControlMode_NONE:
    {
        update_pl2=UpdateBlank;
        break;
    }
    }
}
