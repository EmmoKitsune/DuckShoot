#include <iostream>
#include <raylib.h>

using namespace std;

int score;

class Player{
public:
    Vector2 cursorpos,sprpos;
    Image img,weapImg;
    Texture2D texture,weapTexture;

    void draw(){
        DrawTexture(weapTexture,sprpos.x,sprpos.y,WHITE);
        DrawTexture(texture,cursorpos.x,cursorpos.y,WHITE);
    }

    void update(){
        cursorpos = Vector2{float(GetMouseX()-24),float(GetMouseY()-24)};
    }
};

class Duck{
public:
    Vector2 pos,gameMousePos;
    bool death = false;
    Texture2D texture;
    Image img;
    Color col;
    Sound shootSFX = LoadSound("SFX/shoot.wav");
    Sound scoreSFX = LoadSound("SFX/score.wav");

    Duck(){
        col = WHITE;
    }

    ~Duck(){
        UnloadTexture(texture);
    }

    void draw(){
        DrawTexture(texture,pos.x,pos.y,col);
    }

    void update(){
        if(CheckCollisionCircleRec(pos,25,Rectangle{gameMousePos.x,gameMousePos.y,32,32})){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                death = true;
                score += 1;
                death = false;
                getThePos();
                PlaySound(shootSFX);
                PlaySound(scoreSFX);
            }
        }
    }
    
    void afterwinStart(){
        img = LoadImage("Sprites/spr1.png");
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
        pos = generateRandomPos();
    }

    Vector2 generateRandomPos(){
        float x = GetRandomValue(68,800-68);
        float y = GetRandomValue(68,600-68);

        return Vector2{x,y};
    }

    void getThePos(){
        pos = generateRandomPos();
    }
};

class Game{
public:
    Player player = Player();
    Duck duck1 = Duck();
    Texture2D bgtexture;

    ~Game(){
        UnloadTexture(bgtexture);
        UnloadTexture(player.texture);
        UnloadTexture(player.weapTexture);
        UnloadSound(duck1.scoreSFX);     // Unload sound data
        UnloadSound(duck1.shootSFX);     // Unload sound data

        CloseAudioDevice();     
    }

    Game(){
        player.sprpos = Vector2{float(320),float(440)};
        duck1.pos = Vector2{500,200};
    }

    void draw(){
        DrawTexture(bgtexture,0,0,WHITE);
        duck1.draw();
        player.draw();
        DrawText(TextFormat("Points: %i",score),20,20,50,WHITE);
        DrawTexture(bgtexture,0,0,Color{0,0,0,0});
    }

    void update(){
        player.update();
        duck1.gameMousePos = player.cursorpos;
        duck1.update();
    }

    void afterWinStart(){
        Image imagebg = LoadImage("Sprites/bg.png");
        player.weapImg = LoadImage("Sprites/spr2.png");
        player.weapTexture = LoadTextureFromImage(player.weapImg);

        player.img = LoadImage("Sprites/spr3.png");
        duck1.afterwinStart();
        bgtexture = LoadTextureFromImage(imagebg);
        player.texture = LoadTextureFromImage(player.img);
        UnloadImage(imagebg);
        UnloadImage(player.img);
        UnloadImage(player.weapImg);
    }
};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "DuckShoot!");

    InitAudioDevice();

    SetTargetFPS(60);

    Game game = Game();
    game.afterWinStart();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        game.update();

        ClearBackground(BLACK);
        game.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}