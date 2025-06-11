#include "appcore.h"

AppCore::AppCore(QObject *parent)
    : QObject{parent}
{

}

const QString &AppCore::cipherText() const
{
    return m_cipherText;
}

void AppCore::saveCipherText(const QString &newCipherText)
{
    if (m_cipherText == newCipherText)
        return;
    m_cipherText = newCipherText;
}

void AppCore::initialize()
{
    prepareNewCipherText();
}

void AppCore::uninitialize()
{
    //TODO save to persistent base, for continue work
}

void AppCore::prepareNewCipherText()
{
#ifdef QT_DEBUG
    //НМВ ЖЦТЧТБЙЗМ ЦЗГЗЧТЗ, ЧЗ РКХАЬГЙР ВЕЗ ХМВЦВЧК,
    //КТО ПРИНИМАЕТ РЕШЕНИЕ, НЕ ВЫСЛУШАВ ОБЕ СТОРОНЫ,

    //ЖВХМЬЖЙЗМ ЧЗХЖЦЙРЗИАТРВ, ОВМЛ ЕК ЦЗГЗЧТЗ ФМВ Т
    //ПОСТУПАЕТ НЕСПРАВЕДЛИВО, ХОТЯ БЫ РЕШЕНИЕ ЭТО И
    //ЕКАИ ХЖЦЙРЗИАТРВЗ.
    //БЫЛО СПРАВЕДЛИВОЕ.
    m_cipherText = QString("%1%2%3")
            .arg("НМВ ЖЦТЧТБЙЗМ ЦЗГЗЧТЗ, ЧЗ РКХАЬГЙР ВЕЗ ХМВЦВЧК,")
            .arg("ЖВХМЬЖЙЗМ ЧЗХЖЦЙРЗИАТРВ, ОВМЛ ЕК ЦЗГЗЧТЗ ФМВ Т")
            .arg("ЕКАИ ХЖЦЙРЗИАТРВЗ.");
#endif

    emit cipherTextChanged();
}




