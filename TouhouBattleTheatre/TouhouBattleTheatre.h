﻿// TouhouBattleTheatre.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#pragma execution_character_set("utf-8")


//Log
#include <spdlog/spdlog.h>
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include <glad/glad.h>


// TODO: 在此处引用程序需要的其他标头。

//const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

const std::string BasePath = "D:/Repo/TouhouBattleTheatre/TouhouBattleTheatre/res/";
