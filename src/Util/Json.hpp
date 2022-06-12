#ifndef DRK_UTIL_JSON_HPP
#define DRK_UTIL_JSON_HPP

#include "Core/Base.hpp"
#include "Util/File.hpp"

#include "lib/fs.hpp"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

namespace DrkCraft::Json
{
    using rapidjson::Document;
    using rapidjson::StringBuffer;

    using Writer       = rapidjson::Writer<StringBuffer>;
    using PrettyWriter = rapidjson::PrettyWriter<StringBuffer>;

    Result<Document> parse(const fs::path& filename);

    void write(const Document& document, const fs::path& filename);
    void pretty_write(const Document& document, const fs::path& filename);

    template <typename W=Writer>
    class FileWriter
    {
    public:
        FileWriter(fs::path filename)
          : filename(std::move(filename)),
            writer(buf),
            open(true)
        {
            writer.StartObject();
        }

        ~FileWriter(void)
        {
            close();
        }

        void close(void)
        {
            if (open)
            {
                writer.EndObject();
                write_file(filename, buf.GetString());
                open = false;
            }
        }

        W& get_writer(void)
        {
            return writer;
        }

    private:
        fs::path filename;
        StringBuffer buf;
        W writer;
        bool open;
    };
}

#endif // DRK_UTIL_JSON_HPP
