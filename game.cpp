#include "game.h"
#include "textureloader.h"



#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rrep(i, a, b) for (int i = (a); i < (b); ++i)

Game::Game(int windowWidth, int windowHeight)
    : PlacementModeCurrent(PlacementMode::wall), 
        level(windowWidth/tileSize, windowHeight/tileSize) 
{
    textureOverlay= *TextureLoader::LoadTextureFromFile("Overlay.bmp");
    
    const float deltaTime= 1.0f/60.0f;
    float accumulator= 0.0f;

    bool running=true;
    SetTargetFPS(60);

    while(running && !WindowShouldClose()){
        float delta=GetFrameTime();
        accumulator += delta;

        while(accumulator >= deltaTime){
            processEvents(running);
            update(deltaTime);
            accumulator-=deltaTime;
        }

        draw();
    }
}

Game::~Game(){
    TextureLoader::DeallocTexture(); // Deallocate all textures
}

void Game::processEvents(bool &running){ //for every frame
    bool mouseDownThisFrame = (mouseDownStatus==0);

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        mouseDownThisFrame=true;
        mouseDownStatus= 1;
        cout << mouseDownStatus << endl;
        cout << "Mouse left button pressed" << endl;
    }
    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
        mouseDownThisFrame=true;
        mouseDownStatus= 2;
        cout << mouseDownStatus << endl;
        cout << "Mouse right button pressed" << endl;
    }

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)){
        mouseDownStatus=0;
        cout << "Mouse button released" << endl;
    }
    if(IsKeyPressed(KEY_ESCAPE)) running=false;
    if(IsKeyPressed(KEY_ONE)){
        cout << "Placement mode: wall" << endl;
        PlacementModeCurrent= PlacementMode::wall;
    }
    if(IsKeyPressed(KEY_TWO)){
        cout << "Placement mode: units" << endl;
        PlacementModeCurrent= PlacementMode::units;
    }
    if(IsKeyPressed(KEY_H)) {
        cout << "Overlay toggled" << endl;
        overlayVisible = !overlayVisible;
    }

    Vector2 mouse= GetMousePosition();
    Vector2 mousePosition= {mouse.x/tileSize, mouse.y/tileSize};

    if(mouseDownStatus>0){
        if(mouseDownStatus==1){
            if(PlacementModeCurrent== PlacementMode::wall){
                level.setTileWall((int) mousePosition.x, (int) mousePosition.y, true);
            }
            else if(PlacementModeCurrent== PlacementMode::units){
                addUnit(mousePosition);
            }
        }
        else if(mouseDownStatus==2){
            level.setTileWall((int) mousePosition.x, (int) mousePosition.y,false);
            removeUnit(mousePosition);
        }
    }
}

void Game::addUnit(Vector2 mousePosition){
    units.push_back(Unit(mousePosition));
}

void Game::removeUnit(Vector2 mousePosition){
    rep(i,units.size()){
        if(units[i].checkOverlap(mousePosition, 0.0f)){
            units.erase(units.begin()+i);
            i--;
        }
    }
}

void Game::update(float deltaTime){
    for(auto it = units.begin(); it != units.end();){
        it->update(deltaTime, level, units);
        if(!it->isAlive()){
            it = units.erase(it); // Remove dead unit
        } else {
            ++it; // Move to the next unit
        }
    }
}

void Game::draw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    level.draw(tileSize);

    for(auto &unit : units){
        unit.draw(tileSize);
    }

    if(overlayVisible) DrawTexture(textureOverlay,40,40,WHITE);

    EndDrawing();
}