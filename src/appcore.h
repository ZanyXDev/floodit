#pragma once

#include <QObject>

#ifdef QT_DEBUG
#include <QDebug>
#endif

class AppCore : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString cipherText READ cipherText WRITE saveCipherText NOTIFY cipherTextChanged)
    explicit AppCore(QObject *parent = nullptr);

    const QString &cipherText() const;
    void saveCipherText(const QString &newCipherText);

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void uninitialize();

signals:
    void cipherTextChanged();

private:
    QString m_cipherText;
    void prepareNewCipherText();
};

