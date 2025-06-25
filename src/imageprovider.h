#pragma once

#include "palette.h"

#include <QQuickImageProvider>
#include <QList>
#include <QVector3D>

class ImageProvider : public QQuickImageProvider
{
public:

    ImageProvider();
    ~ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
public:
    void generate( bool lightMode );

private:
    QList<QImage> m_pic;
    QList<QImage> m_nmap;
    QImage createImage(Palette *m_pallete,bool v_mode, int v_cellInRow, int v_colors);
    QImage generateNormalMap(const QImage& img);
};
