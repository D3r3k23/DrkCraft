#include "Assert.hpp"

#if defined(DRK_EN_ASSERTS)

    #include "Base.hpp"

    #include <filesystem>
    #include <format>

    namespace DrkCraft
    {
        void assert_failed(std::string_view cond, std::string_view msg, std::string_view file, int line)
        {
            const auto fileName = std::filesystem::path(file).filename().string();
            DRK_LOG_ERROR("[{0}:{1}] Assert ({2}) failed: {3}", fileName, line, cond, msg);
            DRK_LOGGER_SAVE();
            DRK_DEBUG_BREAK();
        }
    }

#endif
