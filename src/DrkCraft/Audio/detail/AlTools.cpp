#include "AlTools.hpp"

#include "Core/Debug/Profiler.hpp"

#include <al/alc.h>

namespace DrkCraft
{
    //////////////////////////
    //       AlDevice       //
    //////////////////////////

    AlDevice::AlDevice(void)
    {
        DRK_PROFILE_FUNCTION();

        m_device = alcOpenDevice(nullptr);
        DRK_ASSERT_CORE(m_device, "Could not open AL device");

        const ALCchar* name;
        if (alcIsExtensionPresent(m_device, "ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(m_device, ALC_ALL_DEVICES_SPECIFIER);
        else
        {
            DRK_LOG_CORE_WARN("OpenAL extension ALC_ENUMERATE_ALL_EXT not detected");
            name = alcGetString(m_device, ALC_DEVICE_SPECIFIER);
        }
        m_name = name;
    }

    AlDevice::~AlDevice(void)
    {
        DRK_PROFILE_FUNCTION();

        if (m_device)
            alcCloseDevice(m_device);
    }

    ALCdevice* AlDevice::get_raw_device(void)
    {
        return m_device;
    }

    string_view AlDevice::get_name(void) const
    {
        return m_name;
    }

    ///////////////////////////
    //       AlContext       //
    ///////////////////////////

    AlContext::AlContext(AlDevice& device)
    {
        DRK_PROFILE_FUNCTION();

        m_context = alcCreateContext(device.get_raw_device(), nullptr);
        DRK_ASSERT_CORE(m_context, "Could not create AL context");
    }

    AlContext::~AlContext(void)
    {
        DRK_PROFILE_FUNCTION();

        if (m_context)
            alcDestroyContext(m_context);
    }

    ALCcontext* AlContext::get_raw_context(void)
    {
        return m_context;
    }

    void AlContext::make_current(void)
    {
        auto status = alcMakeContextCurrent(m_context);
        DRK_ASSERT_CORE(status, "Could not make AL context current");
    }

    void AlContext::clear_current(void)
    {
        if (alcGetCurrentContext())
            alcMakeContextCurrent(nullptr);
    }
}
