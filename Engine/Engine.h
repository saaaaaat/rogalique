#pragma once
#define NOMINMAX
#include "SFML/Graphics.hpp"
#include <functional>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    class ENGINE_API Engine
    {
    public:
        Engine(const Engine& app) = delete;
        Engine& operator=(const Engine&) = delete;

        static Engine* Instance();
        void Run();

        void SetUpdateFunction(std::function<void(float)> updateFunc);

    private:
        Engine();
        ~Engine() = default;

        std::function<void(float)> customUpdateFunc;
    };
}