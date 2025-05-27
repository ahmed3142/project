#pragma once
#include "level.h"
#include "unit.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

using namespace std;


class Game {
    enum class PlacementMode{
        wall,
        units
    } PlacementModeCurrent ; // variable

    void processEvents(bool& running);
    void update(float deltaTime);
    void draw();
    void addUnit(Vector2 mousePostion);
    void removeUnit(Vector2 mousePosition);

    int mouseDownStatus=0;

    const int tileSize= 48;
    Level level;
    
    vector<Unit> units;

    Texture2D textureOverlay;
    bool overlayVisible=false;

    public:
        Game(int windowWidth, int windowHeight);
        ~Game();
};