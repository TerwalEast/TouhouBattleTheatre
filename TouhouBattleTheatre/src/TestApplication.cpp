#pragma execution_character_set("utf-8")

#include "TestApplication.h"

#include "WorldMap/WorldMap.h"

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;

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

TestApplication::TestApplication() 
{
    std::string target = SDL_GetBasePath();
    BasePath = target + "res/";
    spdlog::set_level(spdlog::level::debug);
    spdlog::warn("BasePath inited, BasePath = {}", BasePath);
}

TestApplication::~TestApplication()
{
    Cleanup();
}

bool TestApplication::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        spdlog::error("Failed to init SDL. SDL_Error : {}", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    _window = SDL_CreateWindow("東方夢鬥劇 ～ Touhou Battle Theatre", _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if (_window == nullptr)
    {
        spdlog::error("Window creation failed. SDL_Error: {}", SDL_GetError());
        return false;
    }
    spdlog::info("Window init success, Basepath: {}", SDL_GetBasePath());

    SDL_GLContext context = SDL_GL_CreateContext(_window);
    SDL_GL_MakeCurrent(_window, context);
    SDL_SetWindowResizable(_window, false);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        spdlog::error("GLAD初始化失败，无法启动");
        return false;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_CLAMP);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDebugMessageCallback(message_callback, nullptr);

    return true;
}

void TestApplication::Cleanup()
{
    if (_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    SDL_Quit();
}

int TestApplication::run()
{
    if (!Init())
    {
        return -1; // Indicate initialization failure
    }

    Uint8* KeyStates = (Uint8*)SDL_GetKeyboardState(NULL);

	WorldMap worldMap = WorldMap();

    //------------------------------------------------
    // Main Loop
    //------------------------------------------------
    worldMap.Start(_window, KeyStates);

    return 0;
}

TestApplication& TestApplication::GetInstance()
{
    static TestApplication ta;
    return ta;
}
