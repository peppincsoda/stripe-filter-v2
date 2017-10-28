#include "ImageView.h"

#include <QMouseEvent>
#include <QPainter>

namespace sfv2 {

    ImageView::ImageView(QWidget* parent)
        : QLabel(parent)
        , roi_selection_active_(false)
        , roi_top_left_()
        , roi_size_()
    {

    }

    ImageView::~ImageView()
    {

    }

    void ImageView::updateImage(QPixmap&& pixmap)
    {
        QPainter p(&pixmap);
        p.setPen(Qt::white);

        p.drawRect(roi_top_left_.x(),
                   roi_top_left_.y(),
                   roi_size_.width(),
                   roi_size_.height());
        p.end();

        setPixmap(pixmap);
    }

    void ImageView::setRoi(const QPoint& top_left, const QSize& size)
    {
        roi_top_left_ = top_left;
        roi_size_ = size;
    }

    void ImageView::setSelectRoi(bool active)
    {
        roi_selection_active_ = active;
        setCursor(roi_selection_active_ ? Qt::CrossCursor : Qt::ArrowCursor);
    }

    void ImageView::mousePressEvent(QMouseEvent* ev)
    {
        if (roi_selection_active_) {
            roi_top_left_ = ev->pos();
            ev->accept();
        }
    }

    void ImageView::mouseMoveEvent(QMouseEvent* ev)
    {
        if (roi_selection_active_) {
            const auto diff = ev->pos() - roi_top_left_;
            roi_size_ = QSize(diff.x(), diff.y());
            ev->accept();
        }
    }

    void ImageView::mouseReleaseEvent(QMouseEvent* ev)
    {
        if (roi_selection_active_) {
            setSelectRoi(false);
            emit roiChanged(roi_top_left_, roi_size_);
            ev->accept();
        }
    }

}
