#include <raylib.h>
#include <emscripten/emscripten.h>
#include <raymath.h>
#include "string.h"


int screenWidth = 800;
int screenHeight = 450;
int monitor = 0;

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

void UpdateDrawFrame(void);

int main(void) {
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    pix = LoadModel("./resources/pix.glb");
    
    font = LoadFont("./resources/CascadiaMono-Regular.ttf");
    
    monitor = GetCurrentMonitor();
    
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    
    CloseWindow();
    
    
    return 0;
}

Vector3 pixRotationEuler = (Vector3){270.0f, 0.0f, 0.0f };
float floatation = 0.0f;

void UpdateDrawFrame(void) {
    
    MaximizeWindow();
    
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
        
        const char* loadingText = "WEBSITE IN PROGRESS";
        float loadingTextSize = MeasureText(loadingText, 50);
        float initialTextPos = (GetScreenWidth() - loadingTextSize) / 2;
        float spaceOffset = loadingTextSize / (float)strlen(loadingText);
        
        for (int i = 0; i < strlen(loadingText); i++) {
            DrawTextCodepoint(font, (int)(loadingText[i]), (Vector2){initialTextPos, GetScreenHeight() / 2 + 60 + (10 * sinf(DEG2RAD * (floatation + (10 * i))))}, 50.0f, WHITE);
            initialTextPos += spaceOffset + 2;
        }

    EndDrawing();
}