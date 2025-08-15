#pragma once

#include <QObject>

class Validator : public QObject
{
    Q_OBJECT
public:
    explicit Validator(QObject *parent = nullptr);
private:
};
