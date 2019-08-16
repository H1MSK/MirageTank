#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ImagePicker1_clicked();

    void on_ImagePicker2_clicked();

    void on_generateButton_clicked();

    void on_saveButton_clicked();

    void on_cancelButton_clicked();

    void on_outputImage_clicked();

private:
    Ui::MainWindow *ui;
    bool m_isOutputWidgetVisible;
    bool m_isOutputSaved;

    void toggleWidgets();
};

#endif // MAINWINDOW_H
