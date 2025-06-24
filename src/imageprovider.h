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
public slots:
    void generate();
signals:
    void generationImagesDone();
private:
    QList<QImage> m_pic;
    QList<QImage> m_nmap;
    QImage createImage(Palette *m_pallete,bool v_mode, int v_cellInRow, int v_colors);
    // Преобразование HEX-цвета в вектор нормали
    QVector3D hexColorToNormal(const QColor &color);
    // Преобразование вектора нормали в цвет для normal map
    QColor normalToColor(const QVector3D& normal);
};
