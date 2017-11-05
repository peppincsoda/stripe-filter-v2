#ifndef CAMERA_API_H
#define CAMERA_API_H

#include "Definitions.h"

#include <stdint.h>

namespace basler {

    enum class Status
    {
        OK,
        Failed,
    };

    enum class PixelFormat
    {
        Grayscale8bit,
    };

    struct GrabFrameResult
    {
        uint32_t width;
        uint32_t height;
        uint32_t stride;
        PixelFormat format;
        void* data;
    };

    class CameraImpl;
    typedef CameraImpl* (STDCALL *CameraNewFn)();
    typedef void (STDCALL *CameraFreeFn)(CameraImpl*);
    typedef Status (STDCALL *CameraOpenFn)(CameraImpl*);
    typedef Status (STDCALL *CameraGrabFrameFn)(CameraImpl*, GrabFrameResult*);
}

#ifndef __MINGW32__
extern "C" basler::CameraImpl* STDCALL BaslerCameraNew();
extern "C" void STDCALL BaslerCameraFree(basler::CameraImpl* cam);
extern "C" basler::Status STDCALL BaslerCameraOpen(basler::CameraImpl* cam);
extern "C" basler::Status STDCALL BaslerCameraGrabFrame(basler::CameraImpl* cam, basler::GrabFrameResult* result);
#endif // !__MINGW32__

#endif // CAMERA_API_H
