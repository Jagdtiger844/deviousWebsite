#include <raylib.h>
#include <emscripten/emscripten.h>
#include <raymath.h>
#include "string.h"


int screenWidth = 800;
int screenHeight = 450;

Camera mainCamera = { 
    .position = (Vector3){0.0f, 10.0f, 100.0f},
    .target = (Vector3){0.0f, 0.0f, 0.0f},
    .up = (Vector3){0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE
};

Model pix = { 0 };
Vector3 pixPosition = { 0.0f, 0.0f, 0.0f };

float pixRotation = 0.0f;
Font font = { 0 };
Music razormind = { 0 };

void UpdateDrawFrame(void);

int main(void) {
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    InitAudioDevice();
    
    pix = LoadModel("./resources/pix.glb");
    
    font = LoadFont("./resources/CascadiaMono-Regular.ttf");
    
    razormind = LoadMusicStream("./resources/razormind.mp3");
    
    PlayMusicStream(razormind);
    
    SetMusicVolume(razormind, .5f);
    
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    
    CloseWindow();
    
    UnloadMusicStream(razormind);
    UnloadModel(pix);
    
    CloseAudioDevice();
    
    return 0;
}

Vector3 pixRotationEuler = (Vector3){270.0f, 0.0f, 0.0f };
float floatation = 0.0f;
bool razormindStopped = false;

void UpdateDrawFrame(void) {
    
    UpdateMusicStream(razormind);
    
    MaximizeWindow();
    
    if (IsKeyPressed(KEY_SPACE)) {
        if (razormindStopped)
            PlayMusicStream(razormind);
        else
            StopMusicStream(razormind);
        
        razormindStopped = !razormindStopped;
    } else if (IsKeyPressed(KEY_BACKSPACE)) {
        StopMusicStream(razormind);
        PlayMusicStream(razormind);
    }
    
    floatation += 120.0f * GetFrameTime();
    pixRotationEuler.y += 120.0f * GetFrameTime();
    
    if (pixRotationEuler.y >= 360.0f)
        pixRotationEuler.y -= 360.0f;
    
    if (floatation >= 360.0f)
        floatation -= 360.0f;
    
    Quaternion pixRotationQuaternion = QuaternionFromEuler(DEG2RAD * pixRotationEuler.x, DEG2RAD * pixRotationEuler.y, DEG2RAD * pixRotationEuler.z);
    
    Vector3 outAxis = Vector3Zero();
    float outAngle = 0.0f;
    
    QuaternionToAxisAngle(pixRotationQuaternion, &outAxis, &outAngle);
    
    BeginDrawing();
        
        ClearBackground(BLACK);

        BeginMode3D(mainCamera);
        
           DrawModelEx(pix, pixPosition, outAxis, outAngle * RAD2DEG, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
           
            
        EndMode3D();
        
        const char* loadingText = "ASSAULT IN PROGRESS";
        float loadingTextSize = MeasureText(loadingText, 50);
        float initialTextPos = (GetScreenWidth() - loadingTextSize) / 2;
        float spaceOffset = loadingTextSize / (float)strlen(loadingText);
        Color c = { 0 };
        c.r = 255 * (sinf(DEG2RAD * (floatation)) + 1) / 2;
        c.g = 255 * (sinf(DEG2RAD * (floatation + 120)) + 1) / 2;
        c.b = 255 * (sinf(DEG2RAD * (floatation + 240)) + 1) / 2;
        c.a = 255;
        
        for (int i = 0; i < strlen(loadingText); i++) {
            DrawTextCodepoint(font, (int)(loadingText[i]), (Vector2){initialTextPos, GetScreenHeight() / 2 + 60 + (10 * sinf(DEG2RAD * (floatation + (10 * i))))}, 50.0f, c);
            initialTextPos += spaceOffset + 2;
        }
        
        DrawText("Space to STOP and PLAY. Backspace to RESTART.", 10, 10, 20, WHITE);

    EndDrawing();
}