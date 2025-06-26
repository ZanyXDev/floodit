#pragma once

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{

public:

    ImageProvider();
    ~ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
public:
    void generate( bool lightMode );

private:

    QVector<QImage *> m_pic;
    QVector<QImage *> m_nmap;
    int m_bordersize;
    int m_width;
    int m_height;

    void createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage);
    void createNormalMapImage(const QImage *srcImage,QImage *destImage);
};
