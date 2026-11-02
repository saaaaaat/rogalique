#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <memory> 

namespace Engine 
{
    enum class LogLevel { INFO, WARNING, Error };

    class LogSink {
    public:
        virtual void log(LogLevel level, const std::string& message) = 0;
        virtual ~LogSink() = default;
    };

    class ConsoleSink : public LogSink {
    public:
        void log(LogLevel level, const std::string& message) override {
            std::cout << logLevelToString(level) << " " << message << std::endl;
        }

    private:
        std::string logLevelToString(LogLevel level) {
            switch (level) {
            case LogLevel::INFO: return "[INFO]";
            case LogLevel::WARNING: return "[WARNING]";
            case LogLevel::Error: return "[ERROR]";
            default: return "[UNKNOWN]";
            }
        }
    };

    class FileSink : public LogSink {
    private:
        std::ofstream logFile;

    public:
        FileSink(const std::string& filename) 
        {
            logFile.open(filename, std::ios::app | std::ios::binary);
        
            if (logFile.tellp() == 0) {
                const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
                logFile.write((const char*)bom, sizeof(bom));
            }
        }

        void log(LogLevel level, const std::string& message) override {
            if (logFile) {
                logFile << logLevelToString(level) << " " << message << std::endl;
            }
        }

        ~FileSink() {
            if (logFile.is_open()) logFile.close();
        }
      

    private:
        std::string logLevelToString(LogLevel level) {
            switch (level) {
            case LogLevel::INFO: return "[INFO]";
            case LogLevel::WARNING: return "[WARNING]";
            case LogLevel::Error: return "[ERROR]";
            default: return "[UNKNOWN]";
            }
        }
    };

    class Logger {
    private:
        std::vector<std::shared_ptr<LogSink>> sinks;


    public:
        void addSink(std::shared_ptr<LogSink> sink) {
            sinks.push_back(sink);
        }

        void log(LogLevel level, const std::string& message) {

            if (sinks.empty()) return;

            for (auto& sink : sinks) {
                sink->log(level, message);
            }
        }

        void info(const std::string& message) { log(LogLevel::INFO, message); }
        void warn(const std::string& message) { log(LogLevel::WARNING, message); }
        void error(const std::string& message) { log(LogLevel::Error, message); }
    };

    class LoggerRegistry {
    private:
        std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
        std::shared_ptr<Logger> defaultLogger;
        std::mutex registryMutex;

        LoggerRegistry() {} 

    public:
        static LoggerRegistry& getInstance() {
            static LoggerRegistry instance;
            return instance;
        }


        void setDefaultLogger(std::shared_ptr<Logger> logger) {
            defaultLogger = logger;
        }

        void registerLogger(const std::string& name, std::shared_ptr<Logger> logger) {
            std::lock_guard<std::mutex> lock(registryMutex);
            loggers[name] = logger;
        }


        std::shared_ptr<Logger> getLogger(const std::string& name) {
            std::lock_guard<std::mutex> lock(registryMutex);
            if (loggers.find(name) != loggers.end()) {
                return loggers[name];
            }
            if (defaultLogger) return defaultLogger;

         
            static std::shared_ptr<Logger> dummyLogger = std::make_shared<Logger>();
            return dummyLogger;
        }
    };
}

#define LOG_INFO(message)  ::Engine::LoggerRegistry::getInstance().getLogger("global")->info(message)
#define LOG_WARN(message)  ::Engine::LoggerRegistry::getInstance().getLogger("global")->warn(message)
#define LOG_ERROR(message) ::Engine::LoggerRegistry::getInstance().getLogger("global")->error(message)