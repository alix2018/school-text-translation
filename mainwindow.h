#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ui_mainwindow.h"
#include "BingTranslator.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPixmap>
#include <iostream>
#include <fstream>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void text_translated(std::string text, int id);

private:
    Ui::MainWindow *ui;
    QString fileName;

    std::vector<cv::Rect> m_rectangles{};
    std::vector<std::string> m_rectanglesTexts{};
    std::vector<std::string> m_translatedRectangles{};

    BingTranslator *m_translator{NULL};
    cv::Mat m_srcImg{};
    cv::Mat m_resImg{};

    unsigned long m_translationCounter{0};
    QGraphicsScene *m_scene;

    void displayResults() const;
};


#endif // MAINWINDOW_H
