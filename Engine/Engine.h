#pragma once
#define NOMINMAX

#include "SFML/Graphics.hpp"

// 1. Добавляем этот блок СРАЗУ ПОСЛЕ инклудов
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    // 2. Пишем ENGINE_API вместо __declspec(dllexport)
    class ENGINE_API Engine
    {
    public:
        Engine(const Engine& app) = delete;
        Engine& operator=(const Engine&) = delete;

        static Engine* Instance();
        void Run();

    private:
        Engine();
        ~Engine() = default;
    };
}