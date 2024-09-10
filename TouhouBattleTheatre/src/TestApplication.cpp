
#include "TestApplication.h"

#include "../TouhouBattleTheatre.h"
#include "Terrain.h"

#include <SDL3/SDL.h>


using namespace std;

SDL_Window* window = nullptr;

int TestApplication::run()
{
    spdlog::set_level(spdlog::level::debug);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        spdlog::error("Failed to init SDL. SDL_Error : {}", SDL_GetError());
        return false;
    }
    else
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        window = SDL_CreateWindow(" бл Touhou Battle Theatre", 1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (window == NULL)
        {
            spdlog::error("Window creation failed. SDL_Error: {}", SDL_GetError());
            return false;
        }
        spdlog::info("Window init success, Basepath: {}", SDL_GetBasePath());
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    SDL_SetWindowMouseGrab(window, SDL_FALSE);
    SDL_ShowCursor();
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return false;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_CLAMP);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0.1);
    //glDebugMessageCallback(message_callback, nullptr);

    SDL_Event event;

    Terrain terrain = Terrain(10, 10, 2);

    ShaderInfo shaderInfoBasic, shaderInfoTiles;
    shaderInfoBasic.shaderCode = "basic";
    shaderInfoBasic.uniformList.push_back("color");
    shaderInfoBasic.uniformList.push_back("model");
    shaderInfoBasic.uniformList.push_back("view");
    shaderInfoBasic.uniformList.push_back("projection");
    ShaderManager::getInstance().compileShader(shaderInfoBasic);

    shaderInfoTiles.shaderCode = "tiles";
    shaderInfoTiles.uniformList.push_back("tileSheet");
    shaderInfoTiles.uniformList.push_back("Touhou");
    ShaderManager::getInstance().compileShader(shaderInfoTiles);


    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                goto END;
            }
        }

        SDL_Delay(1);
        SDL_GL_SwapWindow(window);
    }

END:
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
