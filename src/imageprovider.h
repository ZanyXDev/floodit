#pragma once

#include "palette.h"
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
    std::unique_ptr<Palette> m_pallete;
    QVector<QImage> m_pic;
    QVector<QImage> m_nmap;
    QImage createImage(Palette *m_pallete,bool v_mode, int v_cellInRow, int v_colors);
    QImage createNormalMap(const QImage& img);
};
