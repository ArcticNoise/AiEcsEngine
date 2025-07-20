#pragma once

#include <chrono>
#include <cstdio>

#ifdef X2D_WITH_TRACY
#include <Tracy.hpp>
#endif

namespace x2d {

class Profiler {
  public:
    static void SetOutput(const char *path) {
        if (s_File != nullptr && s_File != stdout) {
            std::fclose(s_File);
        }
#ifdef _MSC_VER
        FILE *f = nullptr;
        fopen_s(&f, path, "w");
        s_File = f;
#else
        s_File = std::fopen(path, "w");
#endif
        if (s_File != nullptr) {
            std::fprintf(s_File, "scope,duration_ms\n");
        }
    }

    static void SetEnabled(bool enabled) { s_Enabled = enabled; }

    static bool IsEnabled() { return s_Enabled; }

    static void Flush() {
        if (s_File != nullptr) {
            std::fflush(s_File);
            if (s_File != stdout) {
                std::fclose(s_File);
                s_File = nullptr;
            }
        }
    }

    static FILE *GetFile() { return s_File != nullptr ? s_File : stdout; }

  private:
    static inline FILE *s_File{nullptr};
    static inline bool s_Enabled{false};
};

class ProfileScope {
  public:
    explicit ProfileScope(const char *name)
        : m_Name(name), m_Start(std::chrono::steady_clock::now()) {
#ifdef X2D_WITH_TRACY
        ZoneScopedN(name);
#endif
    }

    ~ProfileScope() {
        const auto end = std::chrono::steady_clock::now();
        const double ms = std::chrono::duration<double, std::milli>(end - m_Start).count();
        if (Profiler::IsEnabled()) {
            std::fprintf(Profiler::GetFile(), "%s,%f\n", m_Name, ms);
        }
    }

  private:
    const char *m_Name;
    std::chrono::steady_clock::time_point m_Start;
};

} // namespace x2d

#define X2D_PROFILE_SCOPE(name) x2d::ProfileScope profileScope##__LINE__(name)
#define X2D_PROFILE_FUNCTION() X2D_PROFILE_SCOPE(__func__)
