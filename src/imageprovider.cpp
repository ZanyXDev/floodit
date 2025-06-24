#include <QPainter>
#include <QColor>
#include <QRandomGenerator>

#include "imageprovider.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    generate();
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

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r = x % 255;
            int g = y % 255;
            int b = (x + y) % 255;
            image.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    if (size)
        *size = image.size();

    return image;
}

void ImageProvider::generate()
{
    /// Move to DataManager
    m_pic.clear();
    m_nmap.clear();
    Palette *m_pallete = new Palette();

    // i = cell size 8,12,16,20
    // c = color range 3,5,7 start with zero
    for (int i=8;i<24; i+=4){
        for (int c=3;c<8;c+=2){
            m_pic.append(createImage(m_pallete,true,i,c));
            m_pic.append(createImage(m_pallete,false,i,c));
        }
    }

    m_pallete->deleteLater();
}

QImage ImageProvider::createImage(Palette *m_pallete, bool v_mode, int v_cellInRow, int v_colors)
{
    QStringList m_colors;
    QVector<QPixmap> coloredSquares;  // use array in the color squares
    QVector<QPixmap> normalMapSquares;
    QPainter painter;
    QColor pixelColor;
    int imgSize = 200;
    int cellSize = imgSize / v_cellInRow;
    int borderSize =2;

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

        // Рисуем карту нормалей для блока
        QPixmap pixmapNormal(cellSize, cellSize);
        pixmapNormal.fill(QColor(128,128,200)); // # По умолчанию (плоская поверхность)
        pixelColor.setRgb(50,50,200); // / Границы  - углубление
        QPen pen;
        pen.setWidth(borderSize);
        pen.setColor(pixelColor);

        painter.begin(&pixmapNormal);
        painter.setPen(pen);
        painter.drawRect(0,0, cellSize, cellSize);
        painter.fillRect(borderSize,borderSize, (2*borderSize), cellSize-(2*borderSize), normalToColor(hexColorToNormal(tmp_color)));
        painter.end();
        normalMapSquares.append(pixmapNormal);
    }

    // draw game board
    QImage image(cellSize *v_cellInRow, cellSize*v_cellInRow, QImage::Format_ARGB32);
    image.fill(Qt::transparent); // прозрачный фон
    painter.begin(&image);

    // create normal map
    QImage normalMap(cellSize *v_cellInRow, cellSize*v_cellInRow, QImage::Format_ARGB32);
    normalMap.fill(QColor(128,128,200)); // # По умолчанию (плоская поверхность)
    QPainter painterNormal;
    painterNormal.begin(&normalMap);

    for (int i=0;i<v_cellInRow;++i){
        for (int j=0;j<v_cellInRow;++j){
            int index = QRandomGenerator::global()->bounded(v_colors);
            painter.drawPixmap(i * cellSize, j * cellSize, coloredSquares[index]);
            painterNormal.drawPixmap(i * cellSize, j * cellSize, normalMapSquares[index]);
        }
    }
    painter.end();
    painterNormal.end();

#ifdef QT_DEBUG
    image.save(QString("%1x%1x%2x%3.png").arg(v_cellInRow).arg(v_colors).arg(v_mode), "PNG");
    normalMap.save(QString("%1x%1x%2x%3_nmap.png").arg(v_cellInRow).arg(v_colors).arg(v_mode), "PNG");

     for (int i=0;i<normalMapSquares.count();++i){
        normalMapSquares.at(i).save(QString("normal_map_%1_%2.png").arg(v_cellInRow).arg(i), "PNG");
    }
#endif
    m_nmap.append(normalMap);
    return image;
}

QVector3D ImageProvider::hexColorToNormal(const QColor &color)
{
    auto r = color.redF() / 255.0f;
    auto g = color.greenF() / 255.0f;
    auto b = color.blueF() / 255.0f;
    // Переводим в диапазон [-1, 1]
    QVector3D normal(
        (r - 0.5f) * 2.0f,
        (g - 0.5f) * 2.0f,
        (b - 0.5f) * 2.0f
        );
    // Нормализуем вектор
    normal.normalize();
    return normal;
}

QColor ImageProvider::normalToColor(const QVector3D &normal)
{
    // Переводим из [-1, 1] в [0, 255]
    int r = static_cast<int>((normal.x() + 1.0f) * 127.5f);
    int g = static_cast<int>((normal.y() + 1.0f) * 127.5f);
    int b = static_cast<int>((normal.z() + 1.0f) * 127.5f);
    return QColor(r, g, b);
}

