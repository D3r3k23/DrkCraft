#include "Json.hpp"

#include "Core/Debug/Profiler.hpp"

#include <string>

namespace DrkCraft::Json
{
    Result<Document> parse(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        Document document;

        const std::string text = read_file(filename);
        document.Parse(text);
        if (document.HasParseError())
        {
            DRK_ASSERT_DEBUG_FALSE("Could not parse json file \"{}\"", filename.generic_string());
            return {};
        }
        else
            return { std::move(document) };
    }

    void print(const Document& document, const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        FileWriter<Writer> writer(filename);
        document.Accept(writer.get_writer());
    }

    void pprint(const Document& document, const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        FileWriter<PrettyWriter> writer(filename);
        document.Accept(writer.get_writer());
    }
}
