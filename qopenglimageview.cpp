#include <QMouseEvent>
#include "qopenglimageview.h"

QOpenGLImageView::QOpenGLImageView(QWidget* parent) :
    QOpenGLWidget (parent), m_texture(0), m_background{ 1.0f, 1.0f, 1.0f },
    m_valid(0)
{ }

void QOpenGLImageView::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        if(!m_valid) initializeGL();
        m_background[0] = 0;
        m_background[1] = 0;
        m_background[2] = 0;
        emit repaint();
    }
}

void QOpenGLImageView::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        m_background[0] = 1;
        m_background[1] = 1;
        m_background[2] = 1;
        emit repaint();
    }
}

void QOpenGLImageView::initializeGL() {
    if(!m_valid) {
        glGenTextures(1, &m_texture);
        if(m_texture != 0) {
            m_valid = 1;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void QOpenGLImageView::paintGL() {
    if(m_valid == 2) {
        double windowRatio = width() * 1.0 / height(),
               imageRatio = m_image.width() * 1.0 / m_image.height();
        int l, r, u, d;
        if(windowRatio > imageRatio) {
            l = (width() - height() * imageRatio) / 2.0,
            d = 0,
            r = l + height() * imageRatio,
            u = height();
        } else {
            l = 0,
            d = (height() - width() / imageRatio) / 2.0,
            r = width(),
            u = d + width() / imageRatio;
        }

        glClearColor(m_background[0], m_background[1], m_background[2], 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!glIsEnabled(GL_TEXTURE_2D)) {
            glEnable(GL_TEXTURE_2D);
        }
        if(!glIsEnabled(GL_BLEND)) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glLoadIdentity();
        glOrtho(0, width(), 0, height(), -1, 1);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(l, d);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(r, d);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(r, u);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(l, u);
        glEnd();
    }
}

void QOpenGLImageView::setImage(const QImage& image) {
    m_image = image;
    if (!glIsEnabled(GL_TEXTURE_2D)) {
        glEnable(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 m_image.width(),
                 m_image.height(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_INT_8_8_8_8_REV,
                 m_image.bits()
    );
}

const QImage& QOpenGLImageView::getImage() const { return m_image; }
