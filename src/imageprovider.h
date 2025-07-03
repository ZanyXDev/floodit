#pragma once

#include <QQuickImageProvider>
#include <QPair>

class ImageProvider : public QQuickImageProvider
{

public:

    ImageProvider();
    ~ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
public:
    void generate();

private:
    QVector<QPair<QString, QImage *>> m_picturesArray;
    QVector<QPair<QString, QImage *>> m_normalMapsArray;

    int m_bordersize;
    int m_width;
    int m_height;

    void createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage);
    void createNormalMapImage(const QImage *srcImage,QImage *destImage,float strength = 1.0f);
};
