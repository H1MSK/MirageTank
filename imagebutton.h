#ifndef IMAGEPICKER_H
#define IMAGEPICKER_H

#include <QObject>
#include <QLabel>
#include <QPushButton>

class ImageButton : public QPushButton
{
    QLabel m_displayLabel;

    QString m_fileChoosed;
    QPixmap m_image;
    QString m_backgroundColor;

    void paintEvent(QPaintEvent* event) override;
    void showPixmap();
    void hidePixmap();
public:
    ImageButton(QWidget* parent);

    void setBackgroundColor(QString background);
    const QString& getBackgroundColor() const;
    void setFilename(QString filename);
    const QString& getFilename() const;

    void setPixmap(const QPixmap& image);
    const QPixmap& getPixmap() const;
};

#endif // IMAGEPICKER_H
