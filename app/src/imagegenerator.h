#pragma once

#include <QObject>

class ImageGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ImageGenerator(QObject *parent = nullptr);

signals:
};
