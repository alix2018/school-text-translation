#include "mainwindow.h"
#include <string>



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

std::string MainWindow::on_pushButton_clicked()
{
    // Envoyer l'image
    std::cout << fileName.toUtf8().constData() << '\n';


    // Fermer le menu
    QCoreApplication::quit();

    return fileName.toUtf8().constData();

}
