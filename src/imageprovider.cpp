#include <QPainter>
#include <QColor>
#include <QRandomGenerator>
#include <QtConcurrent>
#include <QFuture>
#include <QImage>
#include "imageprovider.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{

}

ImageProvider::~ImageProvider()
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = 200;
    int height = 200;

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::white);

    return image;
}

void ImageProvider::generate( bool lightMode)
{   
    m_pic.clear();
    m_nmap.clear();

    // Создание новой палитры
    m_pallete = std::make_unique<Palette>();

    // Сначала собираем все параметры для задач
    QVector<QPair<int, int>> tasks;
    for (int cellSize = 8; cellSize <= 20; cellSize += 4) {
        for (int colors = 3; colors <= 7; colors += 2) {
            tasks.append(qMakePair(cellSize, colors));
            QImage img = createImage(m_pallete.get(), lightMode, cellSize, colors);
            QImage normalMap = createNormalMap(img);
            m_pic.append(img);
            m_nmap.append(normalMap);
#ifdef QT_DEBUG
            QString fileName= QString("%1x%1_%2_%3").arg(cellSize).arg(colors).arg(lightMode);
            img.save(fileName, "PNG");
            normalMap.save(fileName+"_nmap", "PNG");
#endif
        }
    }
 }

QImage ImageProvider::createImage(Palette *m_pallete, bool v_mode, int v_cellInRow, int v_colors)
{
    QStringList m_colors;
    QVector<QPixmap> coloredSquares;  // use array in the color squares
    QPainter painter;

    int imgSize = 200;
    int cellSize = imgSize / v_cellInRow;
    int borderSize = 1;

    m_pallete->setColorMode(v_mode);
    m_pallete->setMaxColors(v_colors);
    m_colors = m_pallete->colors();

    for (int i=0;i<m_colors.count();++i) {
        QColor tmp_color = m_colors.at(i);
        QPixmap pixmap(cellSize, cellSize);
        pixmap.fill(Qt::transparent); // прозрачный фон

        painter.begin(&pixmap);
        painter.fillRect(borderSize,borderSize, cellSize-(2*borderSize), cellSize-(2*borderSize), tmp_color);
        painter.end();
        coloredSquares.append(pixmap);
    }

    // draw game board
    QImage image(cellSize *v_cellInRow, cellSize*v_cellInRow, QImage::Format_ARGB32);
    image.fill(Qt::transparent); // прозрачный фон
    painter.begin(&image);

    for (int i=0;i<v_cellInRow;++i){
        for (int j=0;j<v_cellInRow;++j){
            int index = QRandomGenerator::global()->bounded(v_colors);
            painter.drawPixmap(i * cellSize, j * cellSize, coloredSquares[index]);
        }
    }
    painter.end();

    return image;
}


QImage ImageProvider::createNormalMap(const QImage &img)
{
    // Преобразование img в яркость
    // Convert to a different format, for example, Format_Grayscale8
    QImage grayscale = img.convertToFormat(QImage::Format_Grayscale8);
    if (grayscale.isNull()) return QImage();

    int w = grayscale.width();
    int h = grayscale.height();
    QImage normalMap(w, h, QImage::Format_RGB32);

    // Простые ядра Собеля
    int sobelX[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };
    int sobelY[3][3] = {
        { -1, -2, -1 },
        {  0,  0,  0 },
        {  1,  2,  1 }
    };

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            float gx = 0, gy = 0;

            // Применить ядра
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int val = qGray(grayscale.pixel(x + i, y + j));
                    gx += val * sobelX[j + 1][i + 1];
                    gy += val * sobelY[j + 1][i + 1];
                }
            }

            // Вычислить нормаль
            float nx = -gx / 255.0f;
            float ny = -gy / 255.0f;
            float nz = 1.0f;

            // Нормализовать вектор
            float length = std::sqrt(nx * nx + ny * ny + nz * nz);
            nx /= length;
            ny /= length;
            nz /= length;
            // Перевести [-1,1] в [0,255]
            int r = static_cast<int>((nx * 0.5f + 0.5f) * 255);
            int g = static_cast<int>((ny * 0.5f + 0.5f) * 255);
            int b = static_cast<int>((nz * 0.5f + 0.5f) * 255);

            normalMap.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return normalMap;
}



