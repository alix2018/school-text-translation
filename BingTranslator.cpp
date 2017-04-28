//
// Created by thibaut on 26/04/17.
//

#include <QtNetwork/QNetworkReply>
#include <QXmlStreamWriter>
#include <iostream>
#include <QtXmlPatterns/QXmlQuery>
#include "BingTranslator.h"

const QByteArray app_key("BDM e47933adbd504b29909b02e60d6fe1d1");

BingAuthReply::BingAuthReply(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::finished, [=]() {
        emit tokenSent(reply->readAll());
    });

    reply->setParent(this);
}

BingAuthReply::BingAuthReply(QByteArray array)
{
    emit tokenSent(array);
}

BingTranslator::BingTranslator()
{
    m_last_token_update.start();
}

void BingTranslator::translate(std::string text, std::string src, std::string dest)
{
    BingAuthReply *authReply = requestToken();
    authReply->deleteLater();
    connect(authReply, &BingAuthReply::tokenSent, [=](QByteArray token) {
        QByteArray data;
        QXmlStreamWriter writer(&data);
        const QString xmlns("http://schemas.microsoft.com/2003/10/Serialization/Arrays");

        writer.writeStartDocument();
        writer.writeStartElement("translatearrayrequest");
        writer.writeTextElement("appid", QString("BDM %1").arg(QString(token)));
        writer.writeTextElement("from", QString(src.c_str()));
        writer.writeTextElement("options", "");
        writer.writeStartElement("texts");
        writer.writeStartElement("string");
        writer.writeAttribute("xmlns", xmlns);
        writer.writeCharacters(QString(src.c_str()));
        writer.writeEndElement(); // string
        writer.writeEndElement(); // texts
        writer.writeTextElement("to", QString(dest.c_str()));
        writer.writeEndElement(); // translatearrayrequest
        writer.writeEndDocument();

        std::cout << data.constData() << std::endl;

        QNetworkReply *reply = m_manager.post(
                QNetworkRequest(QUrl("https://api.microsofttranslator.com/V2/Http.svc/TranslateArray")),
                data
        );

        connect(reply, &QNetworkReply::finished, [=] {
            QXmlQuery query;
            QString out;

            query.setFocus(reply->readAll());
            query.setQuery("//translatedtext[1]/text()");
            query.evaluateTo(&out);

            emit translationSent(out.toStdString());

            reply->deleteLater();
        });

    });

}

BingAuthReply *BingTranslator::requestToken()
{
    if (m_last_token_update >= QTime(0, 10)) {
        QNetworkRequest request = QNetworkRequest(QUrl("https://api.cognitive.microsoft.com/sts/v1.0/issueToken"));
        request.setRawHeader("Ocp-Apim-Subscription-Key", app_key);

        QNetworkReply *reply = m_manager.post(request, QByteArray());
        BingAuthReply *authReply = new BingAuthReply(reply);

        connect(authReply, &BingAuthReply::tokenSent, [=](QByteArray token) {
            m_token = token;
            m_last_token_update.restart();
        });
        
        return authReply;
    } 
    else {
        return new BingAuthReply(m_token);
    }
}
