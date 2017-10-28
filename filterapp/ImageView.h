#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QLabel>

class QPixmap;
class QMouseEvent;

namespace sfv2 {

    class ImageView : public QLabel
    {
        Q_OBJECT
    public:
        explicit ImageView(QWidget* parent = nullptr);
        ~ImageView();

        void updateImage(QPixmap&& pixmap);

        void setRoi(const QPoint& top_left, const QSize& size);

    Q_SIGNALS:
        void roiChanged(const QPoint& top_left, const QSize& size);

    public Q_SLOTS:
        void setSelectRoi(bool active);

    protected:
        void mousePressEvent(QMouseEvent* ev) override;
        void mouseMoveEvent(QMouseEvent* ev) override;
        void mouseReleaseEvent(QMouseEvent* ev) override;

    private:
        bool roi_selection_active_;
        QPoint roi_top_left_;
        QSize roi_size_;
    };

}

#endif // IMAGEVIEW_H
