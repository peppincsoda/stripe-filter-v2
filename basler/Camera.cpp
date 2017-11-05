#include "Camera.h"
#include "Logger.h"

#include <memory>

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>

using namespace Pylon;

namespace basler {

    class CameraImpl
    {
    public:
        CameraImpl();
        ~CameraImpl();

        bool open();
        bool grabFrame(GrabFrameResult& result);

    private:
        PylonAutoInitTerm auto_init_term_;
        std::unique_ptr<CInstantCamera> camera_;
        CGrabResultPtr grab_result_;
    };

    CameraImpl::CameraImpl()
        : auto_init_term_()
        , camera_()
        , grab_result_()
    {
    }

    CameraImpl::~CameraImpl()
    {
    }

    bool CameraImpl::open()
    {
        // Return true if the camera has already been opened
        if (camera_ != nullptr)
            return true;

        try {
            std::unique_ptr<IPylonDevice> device(CTlFactory::GetInstance().CreateFirstDevice());
            if (device->GetDeviceInfo().GetDeviceClass() == BaslerGigEDeviceClass) {
                std::unique_ptr<CBaslerGigEInstantCamera> camera(new CBaslerGigEInstantCamera(device.get()));
                device.release();

                //camera_->PixelFormat.SetValue(Basler_GigECamera::PixelFormat_Mono8);
                camera->AcquisitionMode.SetValue(Basler_GigECameraParams::AcquisitionMode_Continuous);
                camera->GevSCPSPacketSize = 1500;
                camera->MaxNumBuffer = 2;

                camera_ = std::move(camera);

            } else {
                std::unique_ptr<CInstantCamera> camera(new CInstantCamera(device.get()));
                device.release();

                camera_ = std::move(camera);
            }

            LOG_INFO("Using device: %s", camera_->GetDeviceInfo().GetModelName().c_str());

            camera_->StartGrabbing(GrabStrategy_LatestImages);

        } catch (GenericException& e) {
            LOG_WARNING("Failed to open camera device: %s", e.GetDescription());
            camera_ = nullptr;
            return false;
        }

        return true;
    }

    bool CameraImpl::grabFrame(GrabFrameResult& result)
    {
        try {
            if (!camera_->RetrieveResult(1000, grab_result_, TimeoutHandling_Return)) {
                LOG_WARNING("RetrieveResult timed out");
                return false;
            }

            if (!grab_result_->GrabSucceeded()) {
                LOG_WARNING("GrabResult error: %08X %s",
                    grab_result_->GetErrorCode(),
                    grab_result_->GetErrorDescription().c_str());
                return false;
            }

            size_t stride = 0;
            if (!grab_result_->GetStride(stride)) {
                LOG_WARNING("GetStride failed");
                return false;
            }
            result.width = grab_result_->GetWidth();
            result.height = grab_result_->GetHeight();
            result.stride = stride;

            switch (grab_result_->GetPixelType()) {
            case PixelType_Mono8:
                result.format = PixelFormat::Grayscale8bit;
                break;
            default:
                LOG_WARNING("The returned frame has unsupported pixel format;"
                    " Please configure the camera to capture in Mono8 pixel format");
                return false;
            }

            result.data = grab_result_->GetBuffer();

        } catch (GenericException& e) {
            LOG_WARNING("Failed to grab a frame: %s", e.GetDescription());
            return false;
        }

        return true;
    }


    Camera::Camera(Api* /*api*/)
        : pimpl_(new CameraImpl)
    {

    }

    Camera::~Camera()
    {
        delete pimpl_;
        pimpl_ = nullptr;
    }

    bool Camera::open()
    {
        return pimpl_->open();
    }

    bool Camera::grabFrame(GrabFrameResult& result)
    {
        return pimpl_->grabFrame(result);
    }

}

extern "C" basler::CameraImpl* STDCALL BaslerCameraNew()
{
    return new basler::CameraImpl;
}

extern "C" void STDCALL BaslerCameraFree(basler::CameraImpl* cam)
{
    delete cam;
}

extern "C" basler::Status STDCALL BaslerCameraOpen(basler::CameraImpl* cam)
{
    if (!cam->open())
        return basler::Status::Failed;
    return basler::Status::OK;
}

extern "C" basler::Status STDCALL BaslerCameraGrabFrame(basler::CameraImpl* cam, basler::GrabFrameResult* result)
{
    if (!cam->grabFrame(*result))
        return basler::Status::Failed;
    return basler::Status::OK;
}
