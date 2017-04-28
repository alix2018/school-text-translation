#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/text.hpp>
#include <QtWidgets/QApplication>
#include "BingTranslator.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    BingTranslator translator;
    QObject::connect(&translator, &BingTranslator::translationSent, [] (std::string res, int id) {
        std::cout << res << std::endl;
        std::cout << id << std::endl;
    });

    QObject::connect(&translator, &BingTranslator::translationError, [] (QString messsage) {
        std::cout << messsage.toStdString() << std::endl;
    });

    translator.translate("Les chaussettes de la duchesse sont-elles sèches ? Oui, archi-sèches.", "fr", "en", 0);

    return app.exec();
}
