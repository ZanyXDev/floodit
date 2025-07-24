#include <QtConcurrent>
#include <QFuture>

#include <QPainter>
#include <QColor>
#include <QRandomGenerator>
#include <QImage>

#include "imageprovider.h"
#include "palette.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_bordersize(1)
    , m_width(IMAGE_SIZE)
    , m_height(IMAGE_SIZE)
{
    for (int cells = MIN_CELLS; cells <= MAX_CELLS; cells += CELL_STEP){
        m_cells_sizes.append(cells);
    }
    for (int colors = MIN_COLORS; colors <= MAX_COLORS; colors += COLOR_STEP) {
        m_colors_count.append(colors);
    }
}

ImageProvider::~ImageProvider()
{
    qDebug() << Q_FUNC_INFO << " destructor";
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    bool m_parsed = false;
    bool m_picture = false;
    const QString &m_findKey = getImageFindKey(id,m_picture,m_parsed);

    if (m_parsed) {
        QImage *ret_image = (m_picture) ? findImage(m_findKey,m_picturesArray) : findImage(m_findKey,m_normalMapsArray);
        QImage req_image;
        if (ret_image !=nullptr){
            req_image = ret_image->copy();
            if (size) {
                *size = req_image.size();
            }
            if (requestedSize.width() > 0 && requestedSize.height() > 0 ){
                return req_image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
            }
            return req_image;
        }
    }
    return QImage();
}

void ImageProvider::generate()
{
    clearCache();
    m_picturesArray.reserve(totalImages);
    m_normalMapsArray.reserve(totalImages);

    /// TODO set min and max boarsize from DataManager!!!
    // Сначала собираем все параметры для задач
    QVector<QPair<int, int>> tasks;
    tasks.reserve(numConfigs);
    const QString descTemplate = "%1_%2x%2x%3";

    for (int cells = MIN_CELLS; cells <= MAX_CELLS; cells += CELL_STEP) {
        const int m_size = cells * (m_height / cells);
        for (int colors = MIN_COLORS; colors <= MAX_COLORS; colors += COLOR_STEP) {
            tasks.append(qMakePair(cells, colors));            
            // Создаем изображения заранее
            auto createImagePair = [&](bool lightMode) {
                QString key = descTemplate.arg(lightMode ? "true" : "false").arg(cells).arg(colors);
                m_picturesArray.append(qMakePair(key, new QImage(m_size, m_size, QImage::Format_ARGB32)));
                m_normalMapsArray.append(qMakePair(key, new QImage(m_size, m_size, QImage::Format_Indexed8 )));
            };

            createImagePair(true);  // light mode
            createImagePair(false); // dark mode
        }
    }

    QVector<QFuture<void>> futures;
    futures.reserve(totalImages);

#ifdef QT_DEBUG
    qDebug() <<"tasks.cout():"<<tasks.count();
    QElapsedTimer timer;
    timer.start();
#endif
    // Обрабатываем light и dark режимы в одной задаче
    for (int i = 0; i < tasks.size(); ++i) {
        const int lightIndex = i * 2;
        const int darkIndex = i * 2 + 1;

        const auto& params = tasks[i];
        QImage* lightImage = m_picturesArray[lightIndex].second;
        QImage* darkImage = m_picturesArray[darkIndex].second;

        futures.append(QtConcurrent::run([this, params, lightImage, darkImage]() {
            this->createGameBoardImage(params, true, lightImage);
            this->createGameBoardImage(params, false, darkImage);
        }));
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
    futures.reserve(totalImages);
    for (int i = 0; i < totalImages; ++i) {
        QImage* sourceImage = m_picturesArray[i].second;
        QImage* destImage = m_normalMapsArray[i].second;

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

    for (int i = 0; i < totalImages; ++i) {
        const auto& imagePair = m_picturesArray[i];
        const auto& normalPair = m_normalMapsArray[i];

        imagePair.second->save("/tmp/" + imagePair.first + ".png");
        normalPair.second->save("/tmp/normal_" + normalPair.first + ".png");
    }
#endif
}

QString ImageProvider::getImageFindKey(const QString &id, bool &isPicture, bool &ok) const
{
    ok = false;
    isPicture = false;

    // Быстрая проверка очевидных ошибок
    if (id.isEmpty() || id.count('_') != 2 || id.count('x') != 2) {
        return QString();
    }

    // Разделяем строку "false_true_20x20x4" по символу '_'
    QStringList parts = id.split('_');
    if (parts.size() < 3) {
        return QString();
    }

    const QString &booleanLightPart = parts[0].toLower(); // Нет копирования QString !!!!
    const QString &booleanTypePart = parts[1].toLower();
    const QString &numbersPart = parts[2];

    // Проверка булевых значений
    if (booleanLightPart != "true" && booleanLightPart != "false") {
        return QString();
    }
    if (booleanTypePart != "true" && booleanTypePart != "false") {
        return QString();
    }

    // Преобразуем первую часть в bool
    isPicture = (booleanTypePart == "true");  // безопасный способ преобразования

    QStringList cellsColors = numbersPart.split('x');
    if (cellsColors.size() < 3) {
        return QString();
    }

    bool conversionOk;
    int cell1 = cellsColors[0].toInt(&conversionOk);
    if (!conversionOk) return QString();

    int cell2 = cellsColors[1].toInt(&conversionOk);
    if (!conversionOk || cell1 != cell2) return QString();

    int colors = cellsColors[2].toInt(&conversionOk);
    if (!conversionOk) return QString();

    ok = ( m_cells_sizes.contains(cell1) && (m_colors_count.contains(colors)) );
    return QString("%1_%2").arg(booleanLightPart).arg(numbersPart);
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

void ImageProvider::createNormalMapImage(const QImage *srcImage,QImage *destImage,float strength )
{
    if (srcImage->isNull() || destImage->isNull()) return;

    // Конвертируем в grayscale, если нужно
    QImage grayscale;
    if (srcImage->format() != QImage::Format_Grayscale8) {
        grayscale = srcImage->convertToFormat(QImage::Format_Grayscale8);
    } else {
        grayscale = *srcImage;
    }

    if (grayscale.isNull()) return;


    const int width = grayscale.width();
    const int height = grayscale.height();

    // Создаем нормальную карту в правильном формате
    QImage normalMap(width, height, QImage::Format_RGB32);
    // Обработка краевых пикселей (заполняем нейтральным цветом)
    normalMap.fill(QColor(127, 127, 255).rgb());
    for (int y = 1; y < height - 1; ++y) {
        const uchar *prevLine = grayscale.constScanLine(y - 1);
        const uchar *currLine = grayscale.constScanLine(y);
        const uchar *nextLine = grayscale.constScanLine(y + 1);

        QRgb *destLine = reinterpret_cast<QRgb*>(normalMap.scanLine(y));

        for (int x = 1; x < width - 1; ++x) {
            // Вычисление градиентов по Собелю
            float gx = -1.0f * prevLine[x-1] + 1.0f * prevLine[x+1] +
                       -2.0f * currLine[x-1] + 2.0f * currLine[x+1] +
                       -1.0f * nextLine[x-1] + 1.0f * nextLine[x+1];

            float gy = -1.0f * prevLine[x-1] + -2.0f * prevLine[x] + -1.0f * prevLine[x+1] +
                       1.0f * nextLine[x-1] +  2.0f * nextLine[x] +  1.0f * nextLine[x+1];

            // Нормализация вектора
            float length = qSqrt(gx*gx + gy*gy + 255.0f*255.0f);
            float nx = (-gx * strength) / length;
            float ny = (-gy * strength) / length;
            float nz = 255.0f / length;
            // Преобразование в цветовое пространство [0,255]
            destLine[x] = qRgb(
                static_cast<int>((nx + 1.0f) * 127.5f),
                static_cast<int>((ny + 1.0f) * 127.5f),
                static_cast<int>(nz)
                );
        }
    }
    destImage->swap(normalMap);
}

void ImageProvider::clearCache()
{
    if ( !m_picturesArray.isEmpty()) {
        for (auto& pair : m_picturesArray) {
            delete pair.second;
        }
        m_picturesArray.clear();
    }

    if (!m_normalMapsArray.isEmpty()){
        for (auto& pair : m_normalMapsArray) {
            delete pair.second;
        }
        m_normalMapsArray.clear();
    }
}

QImage *ImageProvider::findImage(const QString &key, const QVector<QPair<QString, QImage *> > &array)
{
    auto it = std::find_if(array.begin(), array.end(), [&key](const auto& p) {
        return p.first == key;
    });
    return it != array.end() ? it->second : nullptr;
}

