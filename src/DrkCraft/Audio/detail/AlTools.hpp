#ifndef DRK_AUDIO_DETAIL_AL_TOOLS_HPP
#define DRK_AUDIO_DETAIL_AL_TOOLS_HPP

#include "Core/Base.hpp"

#include "lib/string.hpp"
#include "lib/string_view.hpp"

struct ALCdevice;
struct ALCcontext;

namespace DrkCraft
{
    class AlDevice
    {
    public:
        AlDevice(void);
        ~AlDevice(void);

        AlDevice(const AlDevice&) = delete;
        AlDevice& operator=(const AlDevice&) = delete;

        ALCdevice* get_raw_device(void);
        string_view get_name(void) const;

    private:
        ALCdevice* m_device;
        string m_name;
    };

    class AlContext
    {
    public:
        AlContext(AlDevice& device);
        ~AlContext(void);

        AlContext(const AlContext&) = delete;
        AlContext& operator=(const AlContext&) = delete;

        ALCcontext* get_raw_context(void);
        void make_current(void);

        static void clear_current(void);

    private:
        ALCcontext* m_context;
    };
}

#endif // DRK_AUDIO_DETAIL_AL_TOOLS_HPP
