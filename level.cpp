#include "level.h"

#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rrep(i, a, b) for (int i = (a); i < (b); ++i)

Level::Level(int setTileCountX, int setTileCountY)
    : tileCountX(setTileCountX), tileCountY(setTileCountY),
      targetTileX(setTileCountX/2), targetTileY(setTileCountY/2)
{

    // textureTileWall = *TextureLoader::LoadTextureFromFile("Tile Wall.bmp");
    // //"D:\CSE1202_project\Raylib-CPP-Starter-Template-for-VSCODE-V2-main\src\assets\Images\Tile Wall.bmp"
    // textureTileTarget = *TextureLoader::LoadTextureFromFile("Tile Target.bmp");
    // textureTileEmpty = *TextureLoader::LoadTextureFromFile("Tile Empty.bmp");

    Texture2D *wallTex = TextureLoader::LoadTextureFromFile("Tile Empty3.bmp");
    Texture2D *targetTex = TextureLoader::LoadTextureFromFile("Tile Target.bmp");
    Texture2D *emptyTex = TextureLoader::LoadTextureFromFile("Tile Empty4.bmp");

    Texture2D *arrowUpTex = TextureLoader::LoadTextureFromFile("Tile Arrow Up.bmp");
    Texture2D *arrowUpRightTex = TextureLoader::LoadTextureFromFile("Tile Arrow Up Right.bmp");
    Texture2D *arrowRightTex = TextureLoader::LoadTextureFromFile("Tile Arrow Right.bmp");
    Texture2D *arrowDownRightTex = TextureLoader::LoadTextureFromFile("Tile Arrow Down Right.bmp");
    Texture2D *arrowDownTex = TextureLoader::LoadTextureFromFile("Tile Arrow Down.bmp");
    Texture2D *arrowDownLeftTex = TextureLoader::LoadTextureFromFile("Tile Arrow Down Left.bmp");
    Texture2D *arrowLeftTex = TextureLoader::LoadTextureFromFile("Tile Arrow Left.bmp");
    Texture2D *arrowUpLeftTex = TextureLoader::LoadTextureFromFile("Tile Arrow Up Left.bmp");

    if (!wallTex || !targetTex || !emptyTex)
    {
        std::cerr << "Failed to load one or more textures." << std::endl;
        exit(1); // or handle error gracefully
    }

    textureTileWall = *wallTex;
    textureTileTarget = *targetTex;
    textureTileEmpty = *emptyTex;
    textureTileArrowUp = *arrowUpTex;
    textureTileArrowUpRight = *arrowUpRightTex;
    textureTileArrowRight = *arrowRightTex;
    textureTileArrowDownRight = *arrowDownRightTex;
    textureTileArrowDown = *arrowDownTex;
    textureTileArrowDownLeft = *arrowDownLeftTex;
    textureTileArrowLeft = *arrowLeftTex;
    textureTileArrowUpLeft = *arrowUpLeftTex;

    tiles.assign(tileCountX * tileCountY, Tile());
    calculateFlowField();
}

Level::~Level()
{
    UnloadTexture(textureTileWall);
    UnloadTexture(textureTileTarget);
    UnloadTexture(textureTileEmpty);
    UnloadTexture(textureTileArrowUp);
    UnloadTexture(textureTileArrowUpRight);
    UnloadTexture(textureTileArrowRight);
    UnloadTexture(textureTileArrowDownRight);
    UnloadTexture(textureTileArrowDown);
    UnloadTexture(textureTileArrowDownLeft);
    UnloadTexture(textureTileArrowLeft);
    UnloadTexture(textureTileArrowUpLeft);
}

void Level::draw(int tileSize)
{
    rep(i, (int) tiles.size())
    {
        int x = i % tileCountX;
        int y = i / tileCountX;
        drawTile(x, y, tileSize);
    }

    DrawTexture(textureTileTarget,
                targetTileX * tileSize, targetTileY * tileSize, WHITE);

    rep(y, tileCountY)
    {
        rep(x, tileCountX)
        {
            if (isTileWall(x, y))
            {
                // cout << "Tile at (" << x << ", " << y << ") is a wall." << endl;
                DrawTexture(textureTileWall,
                            x * tileSize, y * tileSize, WHITE);
            }
        }
    }
}

void Level::drawTile(int x, int y, int tileSize)
{
    DrawTexture(textureTileEmpty,
                x * tileSize, y * tileSize, WHITE);

    // auto &t = tiles[y * tileCountX + x];
    // Texture2D *arrow = nullptr;

    // if      (t.directionX ==  0 && t.directionY == -1) arrow = &textureTileArrowUp;
    // else if (t.directionX ==  1 && t.directionY == -1) arrow = &textureTileArrowUpRight;
    // else if (t.directionX ==  1 && t.directionY ==  0) arrow = &textureTileArrowRight;
    // else if (t.directionX ==  1 && t.directionY ==  1) arrow = &textureTileArrowDownRight;
    // else if (t.directionX ==  0 && t.directionY ==  1) arrow = &textureTileArrowDown;
    // else if (t.directionX == -1 && t.directionY ==  1) arrow = &textureTileArrowDownLeft;
    // else if (t.directionX == -1 && t.directionY ==  0) arrow = &textureTileArrowLeft;
    // else if (t.directionX == -1 && t.directionY == -1) arrow = &textureTileArrowUpLeft;

    // if (arrow) {
    //     DrawTexture(*arrow,
    //                 x * tileSize, y * tileSize, WHITE);
    // }
}

bool Level::isTileWall(int x, int y)
{
    if (x < 0 || x >= tileCountX || y < 0 || y >= tileCountY || x + y * tileCountX < 0 || x + y * tileCountX >= tiles.size())
        return false;
    return tiles[y * tileCountX + x].isWall;
}

void Level::setTileWall(int x, int y, bool isWall)
{
    if (x < 0 || x >= tileCountX || y < 0 || y >= tileCountY || x + y * tileCountX < 0 || x + y * tileCountX >= tiles.size())
        return;
    tiles[y * tileCountX + x].isWall = isWall;
    calculateFlowField();
}

Vector2 Level::getTargetPosition()
{
    return Vector2{(float)(targetTileX+ 0.5f), (float)(targetTileY+ 0.5f)};
}

Vector2 Level::getFlowNormal(int x, int y)
{
    if (x < 0 || x >= tileCountX || y < 0 || y >= tileCountY || x + y * tileCountX < 0 || x + y * tileCountX >= tiles.size())
        return Vector2{0, 0};
    Tile &tile = tiles[y * tileCountX + x];
    return Vector2{(float)tile.directionX, (float)tile.directionY};
}

void Level::calculateFlowField()
{
    // int targetIndex = targetTileY * tileCountX + targetTileX;
    // if (targetIndex >= 0 && targetIndex < tiles.size() && targetTileX >= 0 && targetTileX < tileCountX && targetTileY >= 0 && targetTileY < tileCountY)
    // {
    //     for (auto &tile : tiles)
    //     {
    //         tile.flowDistance = MAX_LEVELS;
    //         tile.directionX = 0;
    //         tile.directionY = 0;
    //     }
    // }

    // calculateFlowDistances();
    // CalculateFlowFieldForTile(); // flow directions

    for (auto &tile : tiles) {
        tile.flowDistance = MAX_LEVELS;
        tile.directionX  = 0;
        tile.directionY  = 0;
    }
    calculateFlowDistances();
    CalculateFlowFieldForTile(); // flow directions
}

void Level::calculateFlowDistances()
{
    int targetIndex = targetTileY * tileCountX + targetTileX;
    if (targetIndex < 0 || targetIndex >= tiles.size())
        return;

    tiles[targetIndex].flowDistance = 0;

    queue<int> q;
    q.push(targetIndex);

    while (!q.empty())
    {
        int index = q.front();
        q.pop();

        int x = index % tileCountX;
        int y = index / tileCountX;

        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (abs(dx) + abs(dy) != 1)
                    continue; // Only orthogonal directions
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || nx >= tileCountX || ny < 0 || ny >= tileCountY)
                    continue;

                int neighborIndex = ny * tileCountX + nx;
                if (tiles[neighborIndex].isWall || tiles[neighborIndex].flowDistance < MAX_LEVELS)
                    continue;

                tiles[neighborIndex].flowDistance = tiles[index].flowDistance + 1;
                q.push(neighborIndex);
            }
        }
    }
}

void Level::CalculateFlowFieldForTile()
{
    static const int directions[8][2] = {
        {0, -1}, // Up
        {1, -1}, // Up-Right
        {1, 0},  // Right
        {1, 1},  // Down-Right
        {0, 1},  // Down
        {-1, 1}, // Down-Left
        {-1, 0}, // Left
        {-1, -1} // Up-Left
    };

    // flow field calculation and direction 
    int N = tileCountX * tileCountY;
    for (int idx = 0; idx < N; ++idx) {
        auto &tile = tiles[idx];
        if (tile.flowDistance == MAX_LEVELS)
            continue;   // unreachable

        int x = idx % tileCountX;
        int y = idx / tileCountX;

        unsigned char bestDist = tile.flowDistance;
        int bestDx = 0, bestDy = 0;

        // check all 8 neighbors
        for (int d = 0; d < 8; ++d) {
            int nx = x + directions[d][0];
            int ny = y + directions[d][1];
            if (nx < 0 || nx >= tileCountX || ny < 0 || ny >= tileCountY)
                continue;

            int nidx = ny * tileCountX + nx;
            auto &nbr = tiles[nidx];
            if (nbr.flowDistance < bestDist) {
                bestDist = nbr.flowDistance;
                bestDx   = directions[d][0];
                bestDy   = directions[d][1];
            }
        }

        tile.directionX = bestDx;
        tile.directionY = bestDy;
    }
}