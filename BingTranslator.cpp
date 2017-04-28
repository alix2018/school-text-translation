//
// Created by thibaut on 26/04/17.
//

#include <QtNetwork/QNetworkReply>
#include <QXmlStreamWriter>
#include <iostream>
#include <QtXmlPatterns/QXmlQuery>
#include <QtCore/QUrlQuery>
#include "BingTranslator.h"

const QByteArray app_key("e47933adbd504b29909b02e60d6fe1d1");

BingAuthReply::BingAuthReply(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::finished, [=]() {
        if (!reply->error())
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

void BingTranslator::translate(std::string text, std::string src, std::string dest, int id)
{
    BingAuthReply *authReply = requestToken();
    connect(authReply, &BingAuthReply::tokenSent, [=](QByteArray token) {
        QUrl url = QUrl(
                QString("https://api.microsofttranslator.com/V2/Http.svc/Translate?from=%1&to=%2&text=%3")
                        .arg(QString(src.c_str()), QString(dest.c_str()), QString(text.c_str()))
        );

        std::cout << url.toString().toStdString() << std::endl;

        QNetworkRequest request = QNetworkRequest(url);
        request.setRawHeader("Authorization", QByteArray("Bearer ") + token);

        QNetworkReply *reply = m_manager.get(request);

        connect(reply, &QNetworkReply::finished, [=] {
            if (!reply->error()) {
                QXmlQuery query;
                QString out;

                QByteArray xmlData = reply->readAll();
                query.setFocus(xmlData);
                query.setQuery(
                        "declare namespace s=\"http://schemas.microsoft.com/2003/10/Serialization/\";"
                        "//s:string/text()"
                );
                query.evaluateTo(&out);

                emit translationSent(out.toStdString(), id);
            }

            reply->deleteLater();
        });

        connect(
                reply,
                static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
                [=] (QNetworkReply::NetworkError error) {
                    emit translationError(reply->errorString());

                    reply->deleteLater();
                }
        );

        authReply->deleteLater();
    });

    connect(authReply, &BingAuthReply::authError, [this, authReply](QString message) {
        emit translationError(message);

        authReply->deleteLater();
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
