#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "Sound.h"

void PlayBGM() {
    PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void StopBGM() {
    PlaySound(NULL, 0, 0);
}
