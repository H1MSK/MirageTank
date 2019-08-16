#ifndef QOPENGLIMAGEVIEW_H
#define QOPENGLIMAGEVIEW_H

#include <QOpenGLWidget>
#include <QImage>
#include <gl/GL.h>

class QOpenGLImageView : public QOpenGLWidget
{
    QImage m_image;
    GLuint m_texture;
    GLfloat m_background[3];
    
    /**
     * 0 : Not initialised
     * 1 : Initialized, not contain image
     * 2 : Contains image
     */
    int m_valid;
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
public:
    void initializeGL();
    void paintGL();
    void setImage(const QImage& image);
    const QImage& getImage() const;
    QOpenGLImageView(QWidget* parent);
};

#endif // QOPENGLIMAGEVIEW_H
