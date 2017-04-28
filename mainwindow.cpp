#include "mainwindow.h"
#include <string>
#include "readtranslateandwrite.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Picture Files (*.jpg *.jpeg *.png);;C++ Files (*.cpp *.h)"));

    std::cout << fileName.toStdString() << std::endl;

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);

        QGraphicsScene *scene = new QGraphicsScene;
        QPixmap pixmap(fileName);
        scene->addPixmap(pixmap);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
        ui->graphicsView->ensureVisible ( scene->sceneRect() );
        ui->graphicsView->fitInView( scene->sceneRect(), Qt::KeepAspectRatio);

        file.close();
    }
}

void MainWindow::on_pushButton_clicked()
{
    std::string tradFile = readTranlateAndWrite(fileName.toStdString());
    QString newFile = QString::fromStdString(tradFile);

}
