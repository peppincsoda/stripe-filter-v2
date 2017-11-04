#ifndef CAMERA_H
#define CAMERA_H

#include "BaslerApi.h"

#ifdef __MINGW32__

namespace basler {

    class Camera
    {
    public:
        explicit Camera(Api* api)
            : api_(api)
            , pimpl_(api->cam_new_fn_())
        {

        }

        ~Camera()
        {
            api_->cam_free_fn_(pimpl_);
        }

        bool open()
        {
            return api_->cam_open_fn_(pimpl_) == basler::Status::OK;
        }

        bool grabFrame(GrabFrameResult& result)
        {
            return api_->cam_grab_frame_fn_(pimpl_, &result) == basler::Status::OK;
        }

    private:
        Api* api_;
        CameraImpl* pimpl_;
    };

}

#else // !__MINGW32__

namespace basler {

    class Camera
    {
    public:
        explicit Camera(Api* api);
        ~Camera();

        bool open();
        bool grabFrame(GrabFrameResult& result);

    private:
        CameraImpl* pimpl_;
    };

}

#endif // __MINGW32__

#endif // CAMERA_H
