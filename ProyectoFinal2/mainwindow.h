#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Temporizador();
    void recepcionSerialAsincrona();
    void conectarArduino();
    void validarUsuario();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
       QSerialPort *tarjeta;
       bool tarjeta_conectada = false;
       int ArduinoUNO = 67;
       int ArduinoMEGA = 66;
       int ESP32 = 60000;
       QString buffer_interno;
};
#endif // MAINWINDOW_H
