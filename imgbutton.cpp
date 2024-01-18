#include "imgbutton.h"

//For Context : This was originally written with window expansion in mind, but it seems I was out of my depth, because aspect ratio and layouts are hard to mix!

ImageButton::ImageButton(const QPixmap& pixmap, QWidget *parent)
    : QPushButton(parent), mPixmap(pixmap) {

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ImageButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap scaledPixmap = mPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QRect pixmapRect(QPoint(0, 0), scaledPixmap.size());
    pixmapRect.moveCenter(rect().center());
    painter.drawPixmap(pixmapRect.topLeft(), scaledPixmap);
}

void ImageButton::setNewPixmap(const QPixmap& newPixmap) {
    mPixmap = newPixmap;
    update();
}
