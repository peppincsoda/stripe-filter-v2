#ifndef BASLER_API_H
#define BASLER_API_H

#include "LoggerApi.h"
#include "CameraApi.h"

#ifdef __MINGW32__
// NOTE: This must be a header-only implementation on MINGW32.
// The Basler libraries only support MSVC and therefore we have
// to use them from an external DLL which is loaded at runtime.

#include <Windows.h>

namespace basler {

    static LPTSTR GetLastErrorMessage()
    {
        DWORD dw = GetLastError();
        LPTSTR error_text = NULL;

        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
           | FORMAT_MESSAGE_ALLOCATE_BUFFER
           | FORMAT_MESSAGE_IGNORE_INSERTS,
           NULL,
           dw,
           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
           (LPTSTR)&error_text,
           0,
           NULL);

        return error_text;
    }

    class Api
    {
    public:
        friend class Camera;

        explicit Api(LoggerFn logger)
            : logger_(logger)
            , lib_(NULL)
        {

        }

        ~Api()
        {
            if (lib_ != NULL)
                FreeLibrary(lib_);
        }

        bool init()
        {
            lib_ = LoadLibrary("BaslerLib.dll");
            if (lib_ == NULL) {
                logger_(LogLevel::Critical, __FILE__, __LINE__, __FUNCTION__,
                        "Failed to load BaslerLib.dll:");

                LPTSTR error_text = GetLastErrorMessage();
                if (error_text != NULL) {
                    logger_(LogLevel::Critical, __FILE__, __LINE__, __FUNCTION__,
                            error_text);
                    LocalFree(error_text);
                    error_text = NULL;
                }

                return false;
            }

            set_logger_fn_ = reinterpret_cast<SetLoggerFn>(GetProcAddress(lib_, "BaslerSetLogger"));

            cam_new_fn_ = reinterpret_cast<CameraNewFn>(GetProcAddress(lib_, "BaslerCameraNew"));
            cam_free_fn_ = reinterpret_cast<CameraFreeFn>(GetProcAddress(lib_, "BaslerCameraFree"));
            cam_open_fn_ = reinterpret_cast<CameraOpenFn>(GetProcAddress(lib_, "BaslerCameraOpen"));
            cam_grab_frame_fn_ = reinterpret_cast<CameraGrabFrameFn>(GetProcAddress(lib_, "BaslerCameraGrabFrame"));

            if (set_logger_fn_ == nullptr ||
                cam_new_fn_ == nullptr ||
                cam_free_fn_ == nullptr ||
                cam_open_fn_ == nullptr ||
                cam_grab_frame_fn_ == nullptr) {

                logger_(LogLevel::Critical, __FILE__, __LINE__, __FUNCTION__,
                        "Failed to resolve all imports from BaslerLib.dll");
                return false;
            }

            set_logger_fn_(logger_);
            return true;
        }

    private:
        LoggerFn logger_;

        HMODULE lib_;

        SetLoggerFn set_logger_fn_;

        CameraNewFn cam_new_fn_;
        CameraFreeFn cam_free_fn_;
        CameraOpenFn cam_open_fn_;
        CameraGrabFrameFn cam_grab_frame_fn_;
    };

}

#else // !__MINGW32__

namespace basler {

    class Api
    {
    public:
        explicit Api(LoggerFn logger);
        ~Api();

        bool init();
    };

}

#endif // __MINGW32__

#endif // BASLER_API_H
