#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>

#include "mainwindow.h"
#include "ui_mainwindow.h"

int runGUI(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    if(translator.load(":/" + QLocale::system().name() + ".qm")) {
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return a.exec();
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_isOutputWidgetVisible(false)
{
    ui->setupUi(this);
    ui->ImagePicker1->setBackgroundColor("white");
    ui->ImagePicker2->setBackgroundColor("black");
    ui->outputImage->setBackgroundColor("white");
    ui->outputWidget->setVisible(false);
    setWindowTitle(tr("MirageTank Generator"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleWidgets() {
    if(m_isOutputWidgetVisible) {
        m_isOutputWidgetVisible = false;
        ui->outputWidget->setVisible(false);
        ui->inputWidget->setVisible(true);
    } else {
        m_isOutputWidgetVisible = true;
        ui->outputWidget->setVisible(true);
        ui->inputWidget->setVisible(false);
    }
}

void MainWindow::on_ImagePicker1_clicked()
{
    QString filename =
        QFileDialog::getOpenFileName(
            this, nullptr, nullptr,
            tr("Images (*.png *.jpg *.bmp)"));
    if (filename.length() > 0)
        ui->ImagePicker1->setFilename(filename);
}

void MainWindow::on_ImagePicker2_clicked()
{
    QString filename =
        QFileDialog::getOpenFileName(
            this, nullptr, nullptr,
            tr("Images (*.png *.jpg *.bmp)"));
    if (filename.length() > 0)
        ui->ImagePicker2->setFilename(filename);
}

void MainWindow::on_generateButton_clicked()
{
    QString file1 = ui->ImagePicker1->getFilename(),
            file2 = ui->ImagePicker2->getFilename();
    if(file1.length() != 0 && file2.length() != 0) {
        QImage* mirage(QString inputFile1, QString inputFile2);

        QImage* img = mirage(file1, file2);
        if(img != nullptr) {
            ui->outputImage->setPixmap(QPixmap::fromImage(*img));
            m_isOutputSaved = false;
            toggleWidgets();
            delete img;
        }
    }
}

void MainWindow::on_saveButton_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, nullptr, nullptr,
                                                tr("Images (*.png)"));
    if (file.length()) {
        ui->outputImage->getPixmap().save(file);
        m_isOutputSaved = true;
        if(QMessageBox::question(this, tr("Return to image pick interface?"), tr("Don't you want to have another look?")) == QMessageBox::Yes)
            toggleWidgets();
    }
}

void MainWindow::on_cancelButton_clicked()
{
    if(m_isOutputSaved) {
        if(QMessageBox::question(this, tr("Return to image pick interface?"), tr("Not enjoy it anymore?")) == QMessageBox::Yes)
            toggleWidgets();
    } else {
        if(QMessageBox::question(this, tr("Return to image pick interface?"), tr("Are you sure you don't want to save your masterpiece?")) == QMessageBox::Yes)
            toggleWidgets();
    }
}

void MainWindow::on_outputImage_clicked()
{
    if(ui->outputImage->getBackgroundColor() == "black")
        ui->outputImage->setBackgroundColor("white");
    else
        ui->outputImage->setBackgroundColor("black");
}
