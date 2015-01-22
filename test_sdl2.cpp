#include <cstdio>
#include <cstring>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

int main(int argc, char *argv[])
{
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
    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    return 0;
}
