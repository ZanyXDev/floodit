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
    static const int MIN_CELLS = 8;
    static const int MAX_CELLS = 24;
    static const int CELL_STEP = 4;

    static const int MIN_COLORS = 4;
    static const int MAX_COLORS = 8;
    static const int COLOR_STEP = 2;
    static const int IMAGE_SIZE = 208;
    static const int numConfigs = ((MAX_CELLS - MIN_CELLS) / CELL_STEP + 1) * ((MAX_COLORS - MIN_COLORS) / COLOR_STEP + 1);
    static const int totalImages = numConfigs * 2; // light and dark modes

    void createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage);
    void createNormalMapImage(const QImage *srcImage,QImage *destImage,float strength = 1.0f);
    void clearCache();
};
