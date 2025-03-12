#ifndef LOGGER_H
#define LOGGER_H
#include <cstdint>
#include <iostream>
#include <mutex>
#include <sstream>

namespace common {
enum class Severity : uint8_t {
    VERBOSE = 0,
    DEBUG = 1,
    INFO = 2,
    WARNING = 3,
    BYPASS = 4
};

/**
 * @class Logger
 * @brief singleton class
 *
 */
class Logger {
    Severity defaultSeverity_{Severity::WARNING};
    Severity currentSeverity_{Severity::BYPASS};
    std::mutex m_{};
    bool isFirst{true};

    Logger() {};

   public:
    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(const Logger&&) = delete;

    static Logger& getLogger() {
        static Logger logger;
        return logger;
    }

    void setSeverity(std::string_view severity) {
        if (severity.find("I") != std::string::npos) {
            defaultSeverity_ = Severity::INFO;
        } else if (severity.find("W") != std::string::npos) {
            defaultSeverity_ = Severity::WARNING;
        } else if (severity.find("V") != std::string::npos) {
            defaultSeverity_ = Severity::VERBOSE;
        } else if (severity.find("D") != std::string::npos) {
            defaultSeverity_ = Severity::DEBUG;
        } else {
            defaultSeverity_ = Severity::INFO;
        }
    }

    Logger& log(Severity&& sev) {
        currentSeverity_ = sev;

        if (currentSeverity_ >= defaultSeverity_) {
            if (!isFirst) {
                std::cout << "\n";
            } else {
                isFirst = false;
            }

            if (sev == Severity::VERBOSE) {
                std::cout << "[VRB] ";
            } else if (sev == Severity::DEBUG) {
                std::cout << "[DBG] ";

            } else if (sev == Severity::INFO) {
                std::cout << "[INF] ";
            } else if (sev == Severity::WARNING) {
                std::cout << "[WRN] ";
            } else {
                // bypass
            }
        }
        return *this;
    }

    template <class T>
    Logger& operator<<(const T& message) {
        std::lock_guard<std::mutex> guard{m_};
        std::ostringstream oss;
        oss << message;

        if (currentSeverity_ >= defaultSeverity_) {
            std::cout << oss.str();
        }
        return *this;
    }
};

/// TODO: add module name
#define LOG_B() common::Logger::getLogger().log(common::Severity::BYPASS)
#define LOG_V() common::Logger::getLogger().log(common::Severity::VERBOSE)
#define LOG_D() common::Logger::getLogger().log(common::Severity::DEBUG)
#define LOG_I() common::Logger::getLogger().log(common::Severity::INFO)
#define LOG_W() common::Logger::getLogger().log(common::Severity::WARNING)

class ScopedLog {
    std::string_view fname_{};

   public:
    ScopedLog(std::string_view fname) : fname_{fname} {
        LOG_V() << "Enter: " << fname_;
    }
    ~ScopedLog() { LOG_V() << "Exit: " << fname_; }
};

#define LOG_ENTRYPOINT \
    common::ScopedLog { __FUNCTION__ }
};  // namespace common

#endif
