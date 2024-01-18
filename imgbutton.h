#pragma once

#include <QObject>
#include <QPushButton>
#include <QPainter>

class ImageButton : public QPushButton {
    Q_OBJECT
public:
    ImageButton(const QPixmap& pixmap, QWidget *parent = nullptr);
    void setNewPixmap(const QPixmap&);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap mPixmap;
};
