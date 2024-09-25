
#include "TestApplication.h"

#include "../TouhouBattleTheatre.h"
#include "Terrain.h"
#include <glm/glm.hpp>

#include <SDL3/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include <SDL3/SDL.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

using namespace std;

SDL_Window* window = nullptr;

struct Matrices 
{
    glm::mat4 projection;
    glm::mat4 view;
};



void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    auto const src_str = [source]() {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
        }();

    auto const type_str = [type]() {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
        }();

    auto const severity_str = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
        }();
    spdlog::warn("{}, {}, {}, {}, {}", src_str, type_str, severity_str, id, message);
}



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
        window = SDL_CreateWindow(" бл Touhou Battle Theatre", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_CLAMP);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glAlphaFunc(GL_GREATER, 0.1);
    glDebugMessageCallback(message_callback, nullptr);

    SDL_Event event;

    Terrain terrain = Terrain(10, 10, 3);

    //ShaderInfo shaderInfoBasic, shaderInfoTiles;
    //shaderInfoBasic.shaderCode = "basic";
    //ShaderManager::getInstance().compileShader(shaderInfoBasic);

 //   shaderInfoTiles.shaderCode = "tiles";
	//shaderInfoTiles.hasGeom = true;
 //   ShaderManager::getInstance().compileShader(shaderInfoTiles);

    ShaderManager::GetInstance();
    



    
	//ShaderManager::getInstance().setUniform("basic", "model", UniformType::MAT4, (void*)&matrices.projection);

    int lastFrame = SDL_GetTicks();
    float deltaTime;

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), // Eye position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Viewpoint position
        glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector

    glm::mat4 projectionMatrix = glm::perspective(
        45.0f, // field of view angle (in degrees)
        float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), // aspect ratio
        0.5f, // near plane distance
        1000.0f); // far plane distance

    
    Matrices matrices;
    matrices.projection = projectionMatrix;
    matrices.view = viewMatrix;
    ShaderManager::GetInstance().SetUniformBlock("Matrices", (void*)&matrices);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                goto END;
            }
        }


        //render 
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        


		
        terrain.Render();

        SDL_Delay(1);
        SDL_GL_SwapWindow(window);
    }

END:
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}

TestApplication& TestApplication::getInstance()
{
    static TestApplication ta;
    return ta;
}

TestApplication::TestApplication() 
{
    std::string target = SDL_GetBasePath();
    BasePath = target + "res/";
    spdlog::warn("BasePath inited, BasePath = {}", BasePath);
}
