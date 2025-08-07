#include "imagegenerator.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QRandomGenerator>
#include "palette.h"

ImageGenerator::ImageGenerator(QObject *parent)
    : QObject{parent}
{}

void ImageGenerator::createGameBoardImage(const QPair<int, int> &params,
                                          bool lightmode, QImage *destImage,
                                          bool &ok)
{
    // move from void ImageProvider::createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage)
    ok = false;

    if (!destImage || destImage->height() <= 0 || params.first <= 0)
        return;                 // ошибка



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
        painter.fillRect(BORDER_SIZE,BORDER_SIZE, cellSize-(2*BORDER_SIZE), cellSize-(2*m_bordersize), color);
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
