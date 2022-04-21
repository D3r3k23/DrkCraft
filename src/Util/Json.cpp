#include "Json.hpp"

#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <string>

namespace DrkCraft::Json
{
    Result parse(const fs::path& filename, rapidjson::Document& document)
    {
        DRK_PROFILE_FUNCTION();

        const std::string text = read_file(filename);
        document.Parse(text);
        if (document.HasParseError())
        {
            DRK_ASSERT_DEBUG(false, "Could not parse json file \"{}\"", filename.generic_string());
            return Result::Failure;
        }
        else
            return Result::Success;
    }

    void write(const fs::path& filename, const rapidjson::Document& document)
    {
        DRK_PROFILE_FUNCTION();

        rapidjson::StringBuffer stringBuffer;
        rapidjson::Writer writer(stringBuffer);

        document.Accept(writer);
        write_file(filename, stringBuffer.GetString());
    }
}
