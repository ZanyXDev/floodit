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
                                          bool lightmode, QImage *destImage)
{
    // move from void ImageProvider::createGameBoardImage(const QPair<int, int>& params, bool lightmode, QImage *destImage)

    if (!destImage || destImage->height() <= 0 || params.first <= 0)
        return;                 // ошибка

    int cellSize = destImage->height() / params.first;
    if (cellSize <=0) return ;

    int innerCellSize = cellSize - ( 2*BORDER_SIZE );
    if (innerCellSize <=0) return;

    destImage->fill(Qt::transparent); // прозрачный фон

    // Создание новой палитры
    auto m_pallete = std::make_unique<Palette>();
    m_pallete->setMaxColors(params.second);
    m_pallete->setLightMode(lightmode);

    QStringList colors =m_pallete->colors();
    QVector<QPixmap> coloredSquares;  // use array in the color
    coloredSquares.reserve(params.second);

    QPainter painter;

    for (const auto& color : colors) {
        QPixmap pixmap(cellSize, cellSize);
        pixmap.fill(Qt::transparent); // прозрачный фон

        painter.begin(&pixmap);
        painter.fillRect(BORDER_SIZE,BORDER_SIZE, innerCellSize, innerCellSize, color);
        painter.end();

        coloredSquares.append(pixmap);
    }

    // draw game board
    painter.begin(destImage);
    for (int x=0;x<params.first;++x){
        for (int y=0;y<params.first;++y){
            int index = QRandomGenerator::global()->bounded(colors.count());
            painter.drawPixmap(x * cellSize, y * cellSize, coloredSquares[index]);
        }
    }
    painter.end();
}
