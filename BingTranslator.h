//
// Created by thibaut on 26/04/17.
//

#ifndef OPENCV_TESTS_BINGTRANSLATOR_H
#define OPENCV_TESTS_BINGTRANSLATOR_H


#include <string>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkAccessManager>

class BingAuthReply : public QObject
{
    Q_OBJECT

public:
    BingAuthReply(QNetworkReply *reply);
    BingAuthReply(QByteArray array);

signals:
    void tokenSent(QByteArray token);
    void authError(QString message = "");
};

class BingTranslator : public QObject
{
    Q_OBJECT

public:
    BingTranslator();

    void translate(std::string text, std::string src, std::string dest, int id);

signals:
    void translationSent(std::string response, int id);
    void translationError(QString message = "");

private:
    BingAuthReply *requestToken();

    QByteArray m_token{};
    QTime m_last_token_update{1, 0};

    QNetworkAccessManager m_manager{};
};


#endif //OPENCV_TESTS_BINGTRANSLATOR_H
