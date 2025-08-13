#pragma once

#include <QObject>

class ImageGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ImageGenerator(QObject *parent = nullptr);
public slots:
    void createGameBoardImage(const QPair<int, int>& params,
                              bool lightmode, QImage *destImage);
private:
    static const int BORDER_SIZE = 8;
};
