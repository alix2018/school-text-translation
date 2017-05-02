#include "mainwindow.h"
#include <string>
#include "readtranslateandwrite.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_translator = new BingTranslator(this);

    m_scene = new QGraphicsScene(ui->graphicsView);

    connect(m_translator, &BingTranslator::translationSent, this, &MainWindow::text_translated);

    connect(m_translator, &BingTranslator::translationError, [this](QString message) {
        cout << message.toStdString() << endl;

        m_translationCounter++;

        if (m_translationCounter == m_rectanglesTexts.size()) {
            displayResults();
        } else {
            m_translator->translate(m_rectanglesTexts[m_translationCounter], "en", "fr", m_translationCounter);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::text_translated(std::string text, int id)
{
    m_translatedRectangles[id] = text;

    cout << "Traduction rectangle : " << id << endl;

    rewriteImage(fileName.toStdString(), m_srcImg, m_resImg, m_rectangles, m_translatedRectangles, id);
    m_translationCounter++;
    
    if (m_translationCounter == m_rectanglesTexts.size()) {
        for (string s : m_translatedRectangles) {
            cout << s << endl;
        }
        displayResults();
    } else {
        m_translator->translate(m_rectanglesTexts[m_translationCounter], "en", "fr", m_translationCounter);
    }
}

void MainWindow::displayResults() const
{
    const string resFileName = writeToFile(fileName.toStdString(), m_resImg);

    m_scene->addPixmap(QPixmap(QString::fromStdString(resFileName)));
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

        QPixmap pixmap(fileName);
        m_scene->addPixmap(pixmap);
        ui->graphicsView->setScene(m_scene);
        ui->graphicsView->show();
        ui->graphicsView->ensureVisible ( m_scene->sceneRect() );
        ui->graphicsView->fitInView( m_scene->sceneRect(), Qt::KeepAspectRatio);

        file.close();
    }
}

void MainWindow::on_pushButton_clicked()
{
    m_srcImg = readTranslateAndWrite(fileName.toStdString(), m_rectangles, m_rectanglesTexts);
    m_resImg = m_srcImg.clone();

    m_translatedRectangles = m_rectanglesTexts;

    m_translator->translate(m_rectanglesTexts[m_translationCounter], "en", "fr", (int) m_translationCounter);
}
