#ifndef DRK_DISK_FILE_HPP
#define DRK_DISK_FILE_HPP

#include "Core/Base.hpp"

#include "Lib/fs.hpp"
#include "Lib/string.hpp"
#include "Lib/string_view.hpp"

#include <cstdio>

namespace DrkCraft
{
    using DirEntry       = fs::directory_entry;
    using DirIterator    = fs::directory_iterator;
    using RecDirIterator = fs::recursive_directory_iterator;

    enum class PathType
    {
        Unknown = 0,
        File,
        Dir
    };

    string read_file(const fs::path& filename);
    void write_file(const fs::path& filename, string_view text);

    void make_dir(const fs::path& dirname);
    void make_dirs(const fs::path& dirname);
    void ensure_dir_exists(const fs::path& dirname);

    bool is_file(const fs::path& filename);
    bool is_dir(const fs::path& dirname);

    bool path_exists(const fs::path& path);
    PathType find_path_type(const fs::path& path);
}

#endif // DRK_DISK_FILE_HPP
