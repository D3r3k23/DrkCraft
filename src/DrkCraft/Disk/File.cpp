#include "File.hpp"

#include "Core/Debug/Profiler.hpp"

#include <fstream>
#include <sstream>

namespace DrkCraft
{
    string read_file(fs::path filename)
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

    void write_file(fs::path filename, string_view text)
    {
        DRK_PROFILE_FUNCTION();

        std::ofstream file(filename);
        file << text;
    }

    void make_dir(const fs::path& dirname)
    {
        fs::create_directory(dirname);
    }

    void make_dirs(const fs::path& dirname)
    {
        fs::create_directories(dirname);
    }

    void ensure_dir_exists(const fs::path& dirname)
    {
        if (is_dir(dirname))
            return;

        DRK_ASSERT_CORE(!path_exists(dirname), "Directory \"{}\" already exists and is not a directory", dirname.generic_string());
        make_dirs(dirname);
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
