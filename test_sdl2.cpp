#include <cstdio>
#include <cstring>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <al.h>
#include <alc.h>
#include "load_png.h"

ALCdevice *device;
ALCcontext *context;
ALuint buffer;
ALuint source;
void openalStart()
{
    device = alcOpenDevice(0);
    context = 0;
    buffer = 0;
    source = 0;

    if(device) context = alcCreateContext(device, 0);
    if(context) alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    ALfloat listenerPos[]={0.0f, 0.0f, 4.0f};
    ALfloat listenerVel[]={0.0f, 0.0f, 0.0f};
    ALfloat listenerOri[]={0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerPos);
    alListenerfv(AL_ORIENTATION, listenerPos);
}
void openalEnd()
{
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(0);
    if(context) alcDestroyContext(context);
    if(device) alcCloseDevice(device);

    context = 0;
    device = 0;
}

int main(int argc, char *argv[])
{
    openalStart();

    testLoadPNG("test.png");
    //testLoadPNG("dog.png");
    testLoadPNG("pretty_pal.png");
    printf("Test SDL2\n");
    SDL_SetMainReady();
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO))
    {
        printf("SDL_Init failed [%s]\n", SDL_GetError());
    }

    int initMixFlag = MIX_INIT_MP3|MIX_INIT_OGG;
    if(Mix_Init(initMixFlag)!=initMixFlag)
    {
        printf("Mix_Init has ERROR [%s]\n", Mix_GetError());
    }
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024))
    {
        printf("Mix_OpenAudio failed\n");
    }

    if(TTF_Init())
    {
        printf("TTF_Init has ERROR [%s]\n", TTF_GetError());
    }

    TTF_Font *font = TTF_OpenFont("arialuni.ttf", 16);
    if(!font)
    {
        printf("Font open failed [%s]\n", TTF_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("Test SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *pPNGSurface = SDL_CreateSurfaceFromPNG("pretty_pal.png");
    if(pPNGSurface)
    {
        printf("Create PNG surface success\n");
    }

    Mix_Chunk *pChunk = Mix_LoadWAV("cgbgm_b0.ogg");
    if(pChunk)
    {
        alBufferData(buffer, AL_FORMAT_STEREO16, pChunk->abuf, pChunk->alen, 22050);
        printf("Load Chunk\n");
        Mix_FreeChunk(pChunk);

        ALfloat sourcePos[] = {-2.0, 0.0, 0.0};
        ALfloat sourceVel[] = {0.0, 0.0, 0.0};
        alSourcef(source, AL_PITCH, 1.0f);
        alSourcef(source, AL_GAIN, 1.0f);
        alSourcefv(source, AL_POSITION, sourcePos);
        alSourcefv(source, AL_VELOCITY, sourceVel);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcei(source, AL_LOOPING, AL_TRUE);
        alSourcePlay(source);
    }
    else
    {
        printf("Mix ERROR [%s]\n", Mix_GetError());
    }

    SDL_Rect textRect = {0, 0, 120, 32};
    SDL_SetTextInputRect(&textRect);
    SDL_StartTextInput();
    bool bRunning = true;
    fflush(stdout);
    char buf[256];
    memset(buf, 0, sizeof(buf));
    std::string inputText;
    while(bRunning)
    {
        // Drawing
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &textRect);

        // Event process
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_KEYDOWN:
                printf("Key down\n");
                fflush(stdout);
                break;
            case SDL_QUIT:
                printf("Quit program\n");
                fflush(stdout);
                bRunning = false;
                break;
            case SDL_TEXTINPUT:
                printf("SDL_TEXTINPUT event\n");
                fflush(stdout);
                inputText += event.text.text;
                break;
            case SDL_TEXTEDITING:
                {
                    int start = event.edit.start;
                    int length = event.edit.length;
                    int timestamp = event.edit.timestamp;
                    int winId = event.edit.windowID;
                    printf("SDL_TEXTEDITING event id[%d] ts[%d] start[%d] length[%d]\n", winId, timestamp, start, length);
                    fflush(stdout);
                    strcpy(buf, event.edit.text);
                }
                break;
            default:
                break;
            }
        }

        if(strlen(buf))
        {
            // Edit text
            SDL_Color editColor = {0xFF, 0xFF, 0xFF};
            SDL_Surface *editText = TTF_RenderUTF8_Solid(font, buf, editColor);
            if(editText)
            {
                SDL_Texture *editTexture = SDL_CreateTextureFromSurface(renderer, editText);
                if(editTexture)
                {
                    SDL_Rect rect = {0, 0, editText->w, editText->h};
                    SDL_RenderCopy(renderer, editTexture, 0, &rect);
                    SDL_DestroyTexture(editTexture);
                }
                SDL_FreeSurface(editText);
            }
        }
        if(pPNGSurface)
        {
            SDL_Texture *pngTexture = SDL_CreateTextureFromSurface(renderer, pPNGSurface);
            if(pngTexture)
            {
                SDL_Rect rect = {0, 0, pPNGSurface->w, pPNGSurface->h};
                SDL_RenderCopy(renderer, pngTexture, 0, &rect);
                SDL_DestroyTexture(pngTexture);
            }
        }

        if(inputText != "")
        {
            // Show text
            SDL_Color color = {0xFF, 0, 0};
            SDL_Surface *text = TTF_RenderUTF8_Solid(font, inputText.c_str(), color);
            if(text)
            {
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);

                if(texture)
                {
                    SDL_Rect rect = {320, 240, text->w, text->h};
                    SDL_RenderCopy(renderer, texture, 0, &rect);
                    SDL_DestroyTexture(texture);
                }
                else
                {
                    printf("SDL_CreateTextureFromSurface failed [%s]\n", TTF_GetError());
                }

                SDL_FreeSurface(text);
            }
            else
            {
                printf("TTF_RenderUTF8_Solid failed [%s]\n", TTF_GetError());
            }
        }

        // Flip
        SDL_RenderPresent(renderer);

        // Sleep for 16.6ms
        SDL_Delay(16);
    }

    if(pPNGSurface)
    {
        SDL_FreeSurface(pPNGSurface);
    }

    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    alSourceStop(source);
    openalEnd();
    printf("Program exit\n");
    fflush(stdout);
    return 0;
}
