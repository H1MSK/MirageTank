#include <QDebug>
#include "imagebutton.h"

ImageButton::ImageButton(QWidget* parent)
    : QPushButton(parent), m_displayLabel(this) {
    hidePixmap();
    m_displayLabel.setAlignment(Qt::AlignCenter);
    m_displayLabel.setScaledContents(true);
}

void ImageButton::paintEvent(QPaintEvent *event) {
    if (!m_image.isNull()) {
        double windowRatio = width() * 1.0 / height(),
               imageRatio = m_image.width() * 1.0 / m_image.height();
        if(windowRatio > imageRatio) {
            m_displayLabel.setGeometry(
                static_cast<int>((width() - height() * imageRatio) / 2.0),
                0,
                static_cast<int>(height() * imageRatio),
                height());
        } else {
            m_displayLabel.setGeometry(
                0,
                static_cast<int>((height() - width() / imageRatio) / 2.0),
                width(),
                static_cast<int>(width() / imageRatio));
        }
    }
    QPushButton::paintEvent(event);
}

void ImageButton::setBackgroundColor(QString background) {
    m_backgroundColor = background;
    if(m_displayLabel.isVisible()) {
        setStyleSheet("border:none;background-color:"+m_backgroundColor);
    }
}

const QString& ImageButton::getBackgroundColor() const {
    return m_backgroundColor;
}

const QString& ImageButton::getFilename() const {
    return m_fileChoosed;
}

void ImageButton::setFilename(QString filename) {
    m_fileChoosed = filename;
    if(filename.length() > 0) {
        m_fileChoosed = filename;
        m_image.load(filename);
        showPixmap();
    } else {
        hidePixmap();
    }
}

void ImageButton::setPixmap(const QPixmap& image) {
    m_image = image;
    showPixmap();
}

const QPixmap& ImageButton::getPixmap() const {
    return m_image;
}

void ImageButton::showPixmap() {
    m_displayLabel.setPixmap(m_image);
    m_displayLabel.setVisible(true);
    setStyleSheet("border:none;background-color:"+m_backgroundColor);
}

void ImageButton::hidePixmap() {
    m_displayLabel.setVisible(false);
    setStyleSheet("");
}
