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
    // Разделяем строку "false_true_20x20x4" по символу '_'
    QStringList parts = id.split('_');
    QString booleanLightPart = parts[0];  // "false"
    QString booleanTypePart = parts[1];  // " true"
    QString numbersPart = parts[2];  // "12x12x4"

    // Преобразуем первую часть в bool

    bool isPicture = (booleanTypePart == "true");  // безопасный способ преобразования

    QString findKey = QString("%1_%2").arg(booleanLightPart).arg(numbersPart);
    QImage res_image;
    if (isPicture){
        auto pic_it = std::find_if(m_picturesArray.begin(), m_picturesArray.end(),
                                   [&findKey](const QPair<QString, QImage*>& pair) {
                                       // qDebug()<< "findKey:"<<findKey<< " pair.first:"<<pair.first;
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

void ImageProvider::generate()
{
    m_picturesArray.clear();
    m_normalMapsArray.clear();

    /// TODO set min and max boarsize from DataManager!!!
    // Сначала собираем все параметры для задач
    QVector<QPair<int, int>> tasks;
    for (int cells = 8; cells <= 24; cells += 4) {
        for (int colors = 4; colors <= 8; colors += 2) {
            tasks.append(qMakePair(cells, colors));
            int m_size = cells * (m_height / cells );
            // "false/20x20x4" ->"[lightmode]_[picType]_[cells x cells x colors]"
            QString desc = QString("%1_%2x%2x%3");           
            m_picturesArray.append(qMakePair(desc.arg("true").arg(cells).arg(colors), new QImage(m_size,m_size,QImage::Format_ARGB32)));
            m_normalMapsArray.append(qMakePair(desc.arg("true").arg(cells).arg(colors), new QImage(m_size,m_size,QImage::Format_ARGB32)));

            m_picturesArray.append(qMakePair(desc.arg("false").arg(cells).arg(colors), new QImage(m_size,m_size,QImage::Format_ARGB32)));
            m_normalMapsArray.append(qMakePair(desc.arg("false").arg(cells).arg(colors), new QImage(m_size,m_size,QImage::Format_ARGB32)));
        }
    }

    QVector<QFuture<void>> futures;
#ifdef QT_DEBUG
    qDebug() <<"tasks.cout():"<<tasks.count();
    QElapsedTimer timer;
    timer.start();
#endif
    int img_id=0;
    // Запускаем задачи в параллельных потоках
    for (int i = 0; i < tasks.size(); ++i) {
        QPair<int, int> params = tasks[i];

        QPair<QString, QImage *>imagesArrayLight = m_picturesArray[img_id];
        QImage* destImageLigth = imagesArrayLight.second;
        // Запускаем createGameBoardImage в отдельном потоке
        futures.append(QtConcurrent::run([this, params, destImageLigth]() {
            this->createGameBoardImage(params, true, destImageLigth);
        }));

        QPair<QString, QImage *>imagesArrayDark = m_picturesArray[img_id+1];
        QImage* destImageDark = imagesArrayDark.second;
        // Запускаем createGameBoardImage в отдельном потоке
        futures.append(QtConcurrent::run([this, params, destImageDark]() {
            this->createGameBoardImage(params, true, destImageDark);
        }));
        img_id +=2;
    }

    // Ожидание завершения  потоков m_pic
    for (QFuture<void>& future : futures) {
        future.waitForFinished();  // Блокирует текущий поток, пока задача не завершится
    }
    futures.clear();

#ifdef QT_DEBUG
    qDebug() << "m_picturesArray.count():"<< m_picturesArray.count();
    qDebug() << "Ожидание завершения  потоков m_pic:" << timer.elapsed() << "ms";

    timer.start();
#endif
    // Создание карты нормалей
    for (int i = 0; i < m_picturesArray.size(); ++i) {
        QPair<QString, QImage *>imagesArray = m_picturesArray[i];
        QPair<QString, QImage *>normalArray = m_normalMapsArray[i];
        QImage* sourceImage = imagesArray.second;
        QImage* destImage = normalArray.second;
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
    qDebug() << "m_normalMapsArray.count():"<< m_normalMapsArray.count();
    qDebug() << "Создание карты нормалей:" << timer.elapsed() << "ms";

    for (int i = 0; i < m_picturesArray.size(); ++i) {
        QPair<QString, QImage *>imagesArray = m_picturesArray[i];
        QPair<QString, QImage *>normalArray = m_normalMapsArray[i];
        QImage* sourceImage = imagesArray.second;
        QImage* destImage = normalArray.second;        
        QString iFileName = QString("/tmp/%1.png");

        sourceImage->save( iFileName.arg(imagesArray.first) );
        //destImage->save( iFileName.arg(normalArray.first) );
    }
#endif
}

void ImageProvider::createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage)
{
    //qDebug() << Q_FUNC_INFO << "params:"<<params;
    if ( destImage->isNull() ) {
        qDebug() << "Error: destImage isNull";
        return;
    }
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
    QImage grayscale = srcImage->convertToFormat(QImage::Format_Grayscale8);
    if (grayscale.isNull()) return;

    const int w = grayscale.width();
    const int h = grayscale.height();
    QImage normalMap(w, h, QImage::Format_RGB32);

    // Блокировка битов изображения для прямого доступа
    grayscale = grayscale.convertToFormat(QImage::Format_Grayscale8);
    normalMap = normalMap.convertToFormat(QImage::Format_RGB32);

    for (int y = 1; y < h - 1; ++y) {
        const uchar *prevLine = grayscale.constScanLine(y - 1);
        const uchar *currLine = grayscale.constScanLine(y);
        const uchar *nextLine = grayscale.constScanLine(y + 1);
        QRgb *destLine = reinterpret_cast<QRgb*>(normalMap.scanLine(y));
        for (int x = 1; x < w - 1; ++x) {
            // Применение ядер Собеля
            float gx = -1 * prevLine[x-1] + 1 * prevLine[x+1] +
                       -2 * currLine[x-1] + 2 * currLine[x+1] +
                       -1 * nextLine[x-1] + 1 * nextLine[x+1];

            float gy = -1 * prevLine[x-1] + -2 * prevLine[x] + -1 * prevLine[x+1] +
                       1 * nextLine[x-1] +  2 * nextLine[x] +  1 * nextLine[x+1];

            // Нормализация и преобразование
            float length = std::sqrt(gx*gx + gy*gy + 255*255);
            float nx = (-gx / 255.0f) / length;
            float ny = (-gy / 255.0f) / length;
            float nz = 1.0f / length;

            destLine[x] = qRgb(
                static_cast<int>((nx * 0.5f + 0.5f) * 255),
                static_cast<int>((ny * 0.5f + 0.5f) * 255),
                static_cast<int>((nz * 0.5f + 0.5f) * 255)
                );
        }
    }
    destImage->swap(normalMap);
}

