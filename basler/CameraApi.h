#ifndef CAMERA_API_H
#define CAMERA_API_H

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
    typedef CameraImpl* (_stdcall *CameraNewFn)();
    typedef void (_stdcall *CameraFreeFn)(CameraImpl*);
    typedef Status (_stdcall *CameraOpenFn)(CameraImpl*);
    typedef Status (_stdcall *CameraGrabFrameFn)(CameraImpl*, GrabFrameResult*);
}

#ifndef __MINGW32__
extern "C" basler::CameraImpl* _stdcall BaslerCameraNew();
extern "C" void _stdcall BaslerCameraFree(basler::CameraImpl* cam);
extern "C" basler::Status _stdcall BaslerCameraOpen(basler::CameraImpl* cam);
extern "C" basler::Status _stdcall BaslerCameraGrabFrame(basler::CameraImpl* cam, basler::GrabFrameResult* result);
#endif // !__MINGW32__

#endif // CAMERA_API_H
