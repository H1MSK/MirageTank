#include <cstdio>
#include <functional>
#include <QImage>
#include <QFile>

void forEachPixel(
        QImage* img,
        std::function<void(QRgb* pixel)> function) {
    int width = img->width(),
        height = img->height();
    for (int y = 0; y < height; ++y) {
        QRgb* ipixel = reinterpret_cast<QRgb*>(img->scanLine(y));
        for (int x = 0; x < width; ++x) {
            function(&ipixel[x]);
        }
    }
}

void decolor(QImage *img) {
    forEachPixel(img, [](QRgb* pixel) {
        int grey = static_cast<int>(
                        qRed(*pixel) * 0.3 +
                        qGreen(*pixel) * 0.59 +
                        qBlue(*pixel) * 0.11),
            alpha = qAlpha(*pixel);
        *pixel = qRgba(grey, grey, grey, alpha);
    });
}

void brighten(QImage *img, double ratio) {
    /*
    brighten:   img = img * (1 - ratio) + ratio

        0          1            ratio    1
        |----------|              |------|
        |__________|    ->    |___|______|

    darken:    img = img * (1 - ratio)
        0          1          0    ratio
        |----------|          |------|
        |__________|    ->    |______|___|
  */
    int bright = static_cast<int>(ratio > 0 ? UINT8_MAX * ratio : 0);
    double one_minus_ratio = 1 - (ratio > 0 ? ratio : -ratio);

    forEachPixel(img, [=](QRgb *pixel) {
        int red   = static_cast<int>(qRed  (*pixel) * one_minus_ratio + bright),
            green = static_cast<int>(qGreen(*pixel) * one_minus_ratio + bright),
            blue  = static_cast<int>(qBlue (*pixel) * one_minus_ratio + bright),
            alpha = static_cast<int>(qAlpha(*pixel));
        *pixel = qRgba(red, green, blue, alpha);
    });
}

template<typename T>
inline T max(T a, T b) { return a > b ? a : b; }

QImage* mirage(QString inputFile1, QString inputFile2) {
    QImage *img1 = new QImage(inputFile1), *img2 = new QImage(inputFile2), *img;
    if (img1->isNull() || img2->isNull()) {
        delete img1;
        delete img2;
        return nullptr;
    }
    int width = max(img1->width(), img2->width()),
        height = max(img1->height(), img2->height()),
        offsetx[2], offsety[2];

    if (img1->format() != QImage::Format_RGBA8888)
        img1->convertTo(QImage::Format_RGBA8888);
    if (img2->format() != QImage::Format_RGBA8888)
        img2->convertTo(QImage::Format_RGBA8888);

    if(img1->width() != width || img1->height() != height)
        img1->scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if(img2->width() != width || img2->height() != height)
        img2->scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    decolor(img1);
    decolor(img2);
    brighten(img1, 0.5);
    brighten(img2, -0.5);

    img = new QImage(width, height, QImage::Format_RGBA8888);

    offsetx[0] = (img->width() - img1->width()) / 2;
    offsety[0] = (img->height() - img1->height()) / 2;
    offsetx[1] = (img->width() - img2->width()) / 2;
    offsety[1] = (img->height() - img2->height()) / 2;
    QRgb white = qRgb(255, 255, 255), black = qRgb(0, 0, 0);

    /*
    img0 = img * Opacity / UINT8_MAX + (UINT8_MAX - Opacity)
    img1 = img * Opacity / UINT8_MAX


    Opacity = UINT8_MAX - img0 + img1

    img = img1 / Opacity * UINT8_MAX
    */
    for (int y = 0; y < height; ++y) {
        QRgb *iline[2];
        if(y - offsety[0] >= 0 && y - offsety[0] < img1->height())
            iline[0] = reinterpret_cast<QRgb *>(img1->scanLine(y - offsety[0]));
        else
            iline[0] = nullptr;

        if(y - offsety[1] >= 0 && y - offsety[1] < img2->height())
            iline[1] = reinterpret_cast<QRgb *>(img2->scanLine(y - offsety[1]));
        else
            iline[1] = nullptr;

        QRgb *oline = reinterpret_cast<QRgb *>(img->scanLine(y));

        for (int x = 0; x < width; ++x) {
            QRgb* ipixel[2] = {&white, &black};
            if(iline[0] != nullptr && x - offsetx[0] >= 0 && x - offsetx[0] < img1->width())
                ipixel[0] = &iline[0][x - offsetx[0]];
            if(iline[1] != nullptr && x - offsetx[1] >= 0 && x - offsetx[1] < img2->width())
                ipixel[1] = &iline[1][x - offsetx[1]];

            int opacity = UINT8_MAX - qRed(*ipixel[0]) + qRed(*ipixel[1]);
            if (opacity > UINT8_MAX)
                opacity = UINT8_MAX;
            int greyscale = static_cast<int>(qRed(*ipixel[1]) /
                            static_cast<double>(opacity) *
                            UINT8_MAX);
            if (greyscale > UINT8_MAX)
                greyscale = UINT8_MAX;
            oline[x] = qRgba(greyscale, greyscale, greyscale, opacity);
        }
    }

    delete img1;
    delete img2;
    return img;
}

bool produce(const char* file1, const char* file2, const char *outfile) {
    QImage* mirage(QString inputFile1, QString inputFile2);
    QImage* img = mirage(file1, file2);
    if(img != nullptr) {
        img->save(outfile);
        delete img;
        return true;
    } else {
        return false;
    }
}
