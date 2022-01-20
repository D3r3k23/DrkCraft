/*/
 * This file copied and modified from lib/openal-soft/examples/common/alhelpers.c
 * Original license:
 *
 *   Copyright (c) 2011 by Chris Robinson <chris.kcat@gmail.com>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
/*/

#include "AlTools.hpp"

#include "Core/Profiler.hpp"

#include <AL/al.h>
#include <AL/alc.h>

namespace DrkCraft
{
    int init_al(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing OpenAL");

        ALCdevice* device = alcOpenDevice(nullptr);
        if (!device)
            return 1;

        ALCcontext* ctx = alcCreateContext(device, nullptr);
        if (!ctx || alcMakeContextCurrent(ctx) == ALC_FALSE)
        {
            if (ctx)
                alcDestroyContext(ctx);
            alcCloseDevice(device);
            return 1;
        }

        const ALCchar* deviceName;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
            deviceName = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!deviceName || alcGetError(device) != AL_NO_ERROR)
            deviceName = alcGetString(device, ALC_DEVICE_SPECIFIER);

        DRK_LOG_CORE_INFO("Audio device: {}", deviceName);

        return 0;
    }

    void shutdown_al(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Shutting down OpenAL");

        if (ALCcontext* ctx = alcGetCurrentContext(); ctx)
        {
            ALCdevice* device = alcGetContextsDevice(ctx);

            alcMakeContextCurrent(nullptr);
            alcDestroyContext(ctx);
            alcCloseDevice(device);
        }
    }
}
