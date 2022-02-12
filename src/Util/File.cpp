#include "File.hpp"

#include "Core/Debug/Profiler.hpp"

#include <fstream>
#include <sstream>

namespace DrkCraft
{
    std::string read_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (is_file(filename))
        {
            std::ifstream file(filename);
            std::stringstream contents;
            contents << file.rdbuf();
            return contents.str();
        }
        else
            return "";
    }

    void write_file(const fs::path& filename, std::string_view text)
    {
        DRK_PROFILE_FUNCTION();

        std::ofstream file(filename);
        file << text;
    }

    void ensure_parent_dir_exists(const fs::path& path)
    {
        if (path_exists(path))
            return;

        const auto parent = path.parent_path();
        if (is_dir(parent))
            return;

        DRK_ASSERT_CORE(!path_exists(parent),
            "Parent directory for file in path \"{}\" already exists and is not a directory",
                        path.generic_string());
        DRK_LOG_CORE_WARN("Parent directory for file in path \"{}\" does not exist",
                        path.generic_string());

        fs::create_directories(parent);
    }

    bool is_file(const fs::path& filename)
    {
        return fs::is_regular_file(filename);
    }

    bool is_dir(const fs::path& dirname)
    {
        return fs::is_directory(dirname);
    }

    bool path_exists(const fs::path& path)
    {
        return fs::exists(path);
    }

    PathType path_type(const fs::path& path)
    {
        if (is_file(path)) return PathType::File;
        if (is_dir(path))  return PathType::Dir;
        return PathType::Unknown;
    }
}
