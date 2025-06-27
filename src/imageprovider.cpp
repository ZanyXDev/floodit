#include <QPainter>
#include <QColor>
#include <QRandomGenerator>
#include <QtConcurrent>
#include <QFuture>
#include <QImage>

#include "imageprovider.h"
#include "palette.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_bordersize(1)
    , m_width(208)
    , m_height(208)
{

}

ImageProvider::~ImageProvider()
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    // Разделяем строку "false/true/20x20x4" по символу '/'
    QStringList parts = id.split('/');
    QString booleanLightPart = parts[0];  // "false"
    QString booleanTypePart = parts[1];  // " true"
    QString numbersPart = parts[2];  // "12x12x4"

    // Преобразуем первую часть в bool

    bool isPicture = (booleanTypePart == "true");  // безопасный способ преобразования

    QString findKey = QString("%1/%2").arg(booleanLightPart).arg(numbersPart);
    QImage res_image;
    if (isPicture){
        auto pic_it = std::find_if(m_picturesArray.begin(), m_picturesArray.end(),
                                   [&findKey](const QPair<QString, QImage*>& pair) {
                                       qDebug()<< "findKey:"<<findKey<< " pair.first:"<<pair.first;
                                       return pair.first == findKey;
                                   });
        if (pic_it != m_picturesArray.end()){
            res_image = *pic_it->second;
        }
    }else{
        auto norm_it = std::find_if(m_normalMapsArray.begin(), m_normalMapsArray.end(),
                                    [&findKey](const QPair<QString, QImage*>& pair) {
                                        return pair.first == findKey;
                                    });
        if (norm_it != m_normalMapsArray.end()){
            res_image = *norm_it->second;
        }
    }

    return res_image;
}

void ImageProvider::generate( bool lightMode )
{
    m_pic.clear();
    m_nmap.clear();
    /// TODO set min and max boarsize from DataManager!!!
    // Сначала собираем все параметры для задач
    QVector<QPair<int, int>> tasks;
    for (int cells = 8; cells <= 24; cells += 4) {
        for (int colors = 3; colors <= 7; colors += 2) {
            tasks.append(qMakePair(cells, colors));
            int m_size = cells * (m_height / cells );
            m_pic.append( new QImage(m_size,m_size,QImage::Format_ARGB32));
            m_nmap.append( new QImage(m_size,m_size,QImage::Format_ARGB32));
        }
    }

    QVector<QFuture<void>> futures;

    // Запускаем задачи в параллельных потоках
    for (int i = 0; i < tasks.size(); ++i) {
        QPair<int, int> params = tasks[i];
        QImage* destImage = m_pic[i];
        // Запускаем createGameBoardImage в отдельном потоке
        futures.append(QtConcurrent::run([this, params, lightMode, destImage]() {
            this->createGameBoardImage(params, lightMode, destImage);
        }));
    }

    // Ожидание завершения  потоков m_pic
    for (QFuture<void>& future : futures) {
        future.waitForFinished();  // Блокирует текущий поток, пока задача не завершится
    }
    futures.clear();
    // Создание карты нормалей
    for (int i = 0; i < tasks.size(); ++i) {
        QImage* sourceImage = m_pic[i];
        QImage* destImage = m_nmap[i];
        // Запускаем createGameBoardImage в отдельном потоке
        futures.append(QtConcurrent::run([this, sourceImage, destImage]() {
            this->createNormalMapImage(sourceImage, destImage);
        }));
    }
    // Ожидание завершения  потоков m_nmap
    for (QFuture<void>& future : futures) {
        future.waitForFinished();  // Блокирует текущий поток, пока задача не завершится
    }
#ifdef QT_DEBUG
    int index;
    index = 0;
    for (const auto& map : m_pic) {
        QString filename = QStringLiteral("m_pic_%1.png").arg(index++);
map->save(filename);
    }
    index = 0;
    for (const auto& map : m_nmap) {
        QString filename = QStringLiteral("m_nmap%1.png").arg(index++);
map->save(filename);
    }
#endif
}


void ImageProvider::createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage)
{
    if ( destImage->isNull() ) return;

    // Создание новой палитры
    auto m_pallete = std::make_unique<Palette>();
    m_pallete->setMaxColors(params.second);
    m_pallete->setLightMode(lightmode);

    QStringList colors =m_pallete->colors();
    QVector<QPixmap> coloredSquares;  // use array in the color

    QPainter painter;
    int cellSize = destImage->height() / params.first;
    for (const auto& color : colors) {
        QPixmap pixmap(cellSize, cellSize);
        pixmap.fill(Qt::transparent); // прозрачный фон

        painter.begin(&pixmap);
        painter.fillRect(m_bordersize,m_bordersize, cellSize-(2*m_bordersize), cellSize-(2*m_bordersize), color);
        painter.end();

        coloredSquares.append(pixmap);
    }
    // draw game board
    destImage->fill(Qt::transparent); // прозрачный фон
    painter.begin(destImage);

    for (int x=0;x<params.first;++x){
        for (int y=0;y<params.first;++y){
            int index = QRandomGenerator::global()->bounded(colors.count());
            painter.drawPixmap(x * cellSize, y * cellSize, coloredSquares[index]);
        }
    }
    painter.end();
}

void ImageProvider::createNormalMapImage(const QImage *srcImage,QImage *destImage)
{
    // Преобразование img в яркость
    QImage grayscale = srcImage->convertToFormat(QImage::Format_Grayscale8);

    if (grayscale.isNull()) return ;
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

    int w = grayscale.width();
    int h = grayscale.height();
    QImage normalMap(w, h, QImage::Format_RGB32);
    //normalMap.fill("transparent");
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
    destImage->swap(normalMap);
}
