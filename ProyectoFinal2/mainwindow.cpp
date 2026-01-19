#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "telegrambot.h"
#include "mat2qimage.h"
#include<QDebug>
#include <QDirIterator>
#include<QDebug>
#include<QFile>
#include <QTextStream>
#include<QtNetwork/QtNetwork>
#include<QTimer>
#include<opencv2/core/core.hpp>
#include<opencv2/ml/ml.hpp>
#include<opencv/cv.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/imgcodecs.hpp>

#include<QtSql>
#include<QSqlQuery>
#include<QSqlError>
#include<QtNetwork>
#include<QDate>
#include<QDateTime>
#include<QTime>
#include<QDebug>
#include<QSerialPortInfo>
#include<QtSerialPort>
#include<QDebug>

#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
#include<QTextStream>
#include<QMessageBox>

TelegramBot interfacesbot("1908824198:AAGoXf-YDGNm8-qCmTtTX4uzqtV68edVG_A");
using namespace cv;

TelegramBotMessage msgSent;
QSqlDatabase baseDeDatos = QSqlDatabase::addDatabase("QMYSQL");
bool usuarioVerificado = false;
bool usuarioRegistrado = false;
bool ChatIDRegistrado = false;
bool codigoExiste = false;
int contador = 0;
bool TAGValido = false;
bool asistencia = false;
QString PrivilegioEncontrado;
QString nombreEncontrado;
QString TAGEncontrado;
QString imagenEncontrada;
QString chatIDEncontrado;
QString varNivel1Texto;
QString nombreImagen, nombreImagen2;
QString chatID = "";
QString identificador;

QVariant valor;



long int fecha = QDateTime::currentSecsSinceEpoch();
QDateTime fechaCaracteres = QDateTime::currentDateTime();
QString fechaString = QString::number(fecha);
bool IndicadordeAlta = false;

VideoCapture camara(0);
Mat imagen;
Mat imagenChica;

void MainWindow::Temporizador(){
    TelegramBotMessage msgSent;
    camara >> imagen;
       //Cambiar el tamaño

       cv::resize(imagen, imagenChica, Size(250,150),0,0,INTER_LINEAR);
       //Convertir la imagen Mat a una imagen de QT
       QImage imagenQT = Mat2QImage(imagenChica);
       //Convertir la imagen de QT a un mapa de pixeles de QT
       QPixmap mapaPixeles = QPixmap::fromImage(imagenQT);
       //Limpiar el contenido de la etiqueta
       ui->label->clear();
       //Mostrar el mapa de pixeles en la etiqueta
       ui->label->setPixmap(mapaPixeles);

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conectarArduino();
    //Crear cronometro
        QTimer *cronometro = new QTimer(this);
        //Configurar
        connect(cronometro, SIGNAL(timeout()), this, SLOT(Temporizador()));
        //Iniciar
        cronometro->start(50);

        buffer_interno = "";
            //configuración #1.configurarar la base de datos.
            baseDeDatos.setHostName("localhost");
            baseDeDatos.setPort(3306);
            baseDeDatos.setDatabaseName("2021B2");
            baseDeDatos.setUserName("admin51");
            baseDeDatos.setPassword("hola1234");
            baseDeDatos.setPassword("hola1234");
            if(baseDeDatos.open()){
                           qDebug() << "Abri la base de datos";
                       }
                       else{
                           qDebug() << "Error al abrir la base de datos";
                       }
            ui->label->setVisible(true);
            ui->label_2->setVisible(false);
            ui->label_3->setVisible(false);
            ui->lineEdit_2->setVisible(false);
            ui->lineEdit_3->setVisible(false);
            ui->pushButton_2->setVisible(false);

            QObject::connect(&interfacesbot, &TelegramBot::newMessage, [&interfacesbot](TelegramBotUpdate update) {
                // only handle Messages
                if(update->type != TelegramBotMessageType::Message) return;
                // simplify message access
                TelegramBotMessage& message = *update->message;
                QString mensajeRecibido = message.text;
                //Paso # 8.1 Guardar el identificador del usuario
                identificador = QString::number(message.chat.id);
                //Paso # 8.2 Primero verificar que el usuario este dado de alta.
                if(!usuarioVerificado){
                    //Paso # 8.3 Crear el comando de mysql que permita verificar la fila anterior en la tabla de usuarios.
                    QString comandoTexto = "SELECT * FROM usuarios WHERE chatID = '"+identificador+"'";
                    //Paso # 8.4 Enlazar el comando anterior a un comando de sql
                    QSqlQuery comando;
                    comando.prepare(comandoTexto);
                    //Paso # 8.5 Ejecutar el comando anterior.
                    if(comando.exec()){
                        usuarioVerificado = true;
                        qDebug() << " ChatID: " << identificador;
                        qDebug() << "Ok1";
                        //Paso # 8.6 Leer cada fila que coincida con el campo buscado
                        while (comando.next()) {
                            QString identificadorEncontrado = comando.value(4).toString();
                            if(identificadorEncontrado.isEmpty()){
                                ChatIDRegistrado = false;
                            }
                            else{
                                ChatIDRegistrado = true;
                                qDebug() << "El usuario YA EXISTE!";
                                interfacesbot.sendMessage(message.chat.id,
                                                "Ya estas REGISTRADO!",
                                                0,
                                                TelegramBot::NoFlag,
                                                TelegramKeyboardRequest(),
                                                &msgSent);

                            }

                        }
                    }
                    else{
                        qDebug() << "Error1";

                    }
                    if(usuarioRegistrado == false){
                        qDebug() << "El usuario NO existe, hay que registrarlo";

                    }

                }

                //Paso 8.7 Verificar que el mensaje recibido termine con #:
                //218291193#
                int caracteresEncontrados = mensajeRecibido.endsWith("#");
                qDebug() << "el mensje fue:" << mensajeRecibido;
                //Paso 8.8 Si el mensaje recibido, corresponde al chatID, se guarda en la base de datos.
                if(caracteresEncontrados >= 0 && ChatIDRegistrado == false){
                    QString comandoTexto2 = "UPDATE usuarios SET chatID = '"+identificador+"' WHERE codigoTAG='"+mensajeRecibido+"'";
                    QSqlQuery comando2;
                    comando2.prepare(comandoTexto2);
                    if(comando2.exec()){
                        qDebug() << "OK2 El usuario se registro correctamente";
                        ChatIDRegistrado = true;
                        interfacesbot.sendMessage(message.chat.id,
                                        "Tu REGISTRO se guardo correctamente",
                                        0,
                                        TelegramBot::NoFlag,
                                        TelegramKeyboardRequest(),
                                        &msgSent);

                    }
                    else{
                        qDebug() << "Error en el comando 2";
                    }
                }

                if(ChatIDRegistrado == false){
                     if(usuarioVerificado == false){
                        interfacesbot.sendMessage(message.chat.id,
                                        "Hola, favor de registrarte, escibe tu codigo TAG:",
                                        0,
                                        TelegramBot::NoFlag,
                                        TelegramKeyboardRequest(),
                                        &msgSent);
                    }

                }
       });
        interfacesbot.startMessagePulling();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recepcionSerialAsincrona(){

    QString messageARD, from_serial;
    int pos_n;

     //QByteArray datosLeidos = arduino->readAll();
     //QByteArray datosLeidos = tarjeta->readLine();
   from_serial = tarjeta->readAll();
   qDebug () << "Lo que viene del arduino es: " << from_serial;
   buffer_interno.append(from_serial);

   while (buffer_interno.contains("\n")) {
        pos_n = buffer_interno.indexOf("\n");
        messageARD = buffer_interno.mid(0, pos_n + 1);
        buffer_interno.remove (0, pos_n + 1);

        qDebug () << "Lo que queda en el buffer es: " << buffer_interno;
        qDebug () << "El mensaje ya completo es: " << messageARD;

        qDebug() << "Dato #;" << contador << "recibidos:" << messageARD;
        contador ++;
        int posicion1 = messageARD.indexOf("{");
        int posicion2 = messageARD.indexOf("}");
        if(posicion1 >= 0 && posicion2 >= 0){
            QString mensajeJson = messageARD.mid(posicion1, (posicion2-posicion1)+1);
            QJsonDocument mensajeJSON = QJsonDocument :: fromJson(mensajeJson.toUtf8());
            QJsonValue varNivel1 = mensajeJSON["id"];
            varNivel1Texto = varNivel1.toString();
            qDebug()<< "Variable JSON: " <<varNivel1Texto;
            validarUsuario();
        }
   }

}

void MainWindow::conectarArduino(){
  //Declaracion de las variables
  tarjeta = new QSerialPort(this);
  connect(tarjeta, &QSerialPort::readyRead, this, &MainWindow::recepcionSerialAsincrona);
  QString nombreDispositivoSerial = "";
  int productoID = 0;
  int fabricanteID = 0;

  //Imprimir en la terminal, la cantidad de dispositivos seriales virtuales, conectados a la computadora
  qDebug() << "Esto es un mensaje de terminal" << endl;
  qDebug() << "Puertos disponibles: " << QSerialPortInfo::availablePorts().length();

  foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
      if(serialPortInfo.hasVendorIdentifier()){
          fabricanteID = serialPortInfo.vendorIdentifier();
          productoID = serialPortInfo.productIdentifier();
          nombreDispositivoSerial = serialPortInfo.portName();

          qDebug() << "Fabricante: " << fabricanteID << endl;
          qDebug() << "Producto:   " << productoID << endl;

      }
  }

  //COnexion con Arduino
  if(productoID == ArduinoUNO || productoID == ArduinoMEGA){
      //Establece TTYACMX como nombre del dispositivo
      tarjeta->setPortName(nombreDispositivoSerial);
      tarjeta->open(QIODevice::ReadWrite);
      tarjeta->setDataBits(QSerialPort::Data8);
      tarjeta->setBaudRate(QSerialPort::Baud115200);
      tarjeta->setParity(QSerialPort::NoParity);
      tarjeta->setStopBits(QSerialPort::OneStop);
      tarjeta->setFlowControl(QSerialPort::NoFlowControl);
      tarjeta_conectada = true;
  }
}

void MainWindow::validarUsuario(){
    if(baseDeDatos.open()){
                               qDebug() << "Abri la base de datos";
                               //EN ESTA SECCIÓN SE CONSULTA QUE EL TAG LEIDO SE ENCUENTRE DADO DE ALTA EN LA BASE DE DATOS
                               QString comandoTexto = "SELECT * FROM usuarios WHERE codigoTAG = '"+varNivel1Texto+"'";
                                        QSqlQuery comando1;
                                        comando1.prepare(comandoTexto);
                                        if(comando1.exec()){
                                          qDebug() << "El comando se ejecuto correctamente";
                                          while (comando1.next()){
                                              //EXTRAIGO INFORMACION DEL USUARIO DADO DE ALTA
                                                           TAGEncontrado = comando1.value(3).toString();
                                                           PrivilegioEncontrado = comando1.value(2).toString();
                                                           nombreEncontrado = comando1.value(1).toString();
                                                           imagenEncontrada = comando1.value(5).toString();
                                                           chatIDEncontrado = comando1.value(4).toString();

                                                           long int identificadorChat = comando1.value(4).toInt();
                                                           valor = QVariant::fromValue(identificadorChat);

                                                           qDebug () << "El privilegio es: " << PrivilegioEncontrado;
                                                           qDebug () << "El nombre es: " << nombreEncontrado;
                                                           qDebug () << "EL chatID: " << chatIDEncontrado;
                                                           qDebug () << "EL TAG es: " << TAGEncontrado;
                                                           if(TAGEncontrado.isEmpty()){
                                                               TAGValido = false;
                                                           }
                                                           else
                                                               TAGValido = true;
                                                           }
                                                   }
                                        else{
                                          qDebug() << "El comando NO ejecuto correctamente";
                                        }

                                        if(TAGValido){
                                        qDebug()<<"El USUARIO existe en la BASE DE DATOS";
                                        if(PrivilegioEncontrado == "A"){
                                            ui->lineEdit->setText("ADMINISTRADOR");
                                            //AQUI HABILITO LA ALTA DE LOS USUARIOS
                                            ui->label->setVisible(true);
                                            ui->label_2->setVisible(true);
                                            ui->label_3->setVisible(true);
                                            ui->lineEdit_2->setVisible(true);
                                            ui->lineEdit_3->setVisible(true);
                                            ui->pushButton_2->setVisible(true);
                                            IndicadordeAlta = true;
                                        }
                                        else {
                                            ui->label->setVisible(true);
                                            ui->label_2->setVisible(false);
                                            ui->label_3->setVisible(false);
                                            ui->lineEdit_2->setVisible(false);
                                            ui->lineEdit_3->setVisible(false);
                                            ui->pushButton_2->setVisible(false);
                                            //ESTA SECCION VALIDA QUE EL USUARIO ESTE EN LA LISTA DE USUARIOS Y GENERA LA ASISTENCIA
                                            ui->lineEdit->setText(TAGEncontrado);
                                            //AQUI REGISTRO LA ASISTENCIA DEL TRABAJADOR QUE YA EXISTE EN LA BASE DE DATOS
                                            QString comandoAsistencia = "INSERT INTO asistenciasMejorado(nombre,timestamp,codigoTAG,imagen) VALUES(?,?,?,?)";
                                                  QSqlQuery comando2;
                                                        comando2.prepare(comandoAsistencia);
                                                        comando2.addBindValue(nombreEncontrado);
                                                        comando2.addBindValue(fechaString);
                                                        comando2.addBindValue(TAGEncontrado);
                                                        comando2.addBindValue(imagenEncontrada);
                                                        if(comando2.exec()){
                                                            //SI EL COMANDO SE EJECUTA DE MANERA CORRECTA SE AGREGA LA ASISTENCIA
                                                            qDebug() << "La información se agrego correctamente";
                                                            QTime reloj = QTime::currentTime();
                                                            QString HoraLLegada = +"Hora de llegada a las :"+ QString::number(reloj.hour())+" Horas con "+QString::number(reloj.minute())+" minutos y "+QString::number(reloj.second())+" segundos";
                                                            QMessageBox::critical (this,tr("Asistencia"),HoraLLegada);
                                                            nombreImagen2 = TAGEncontrado + HoraLLegada+".jpg";
                                                            cv:: imwrite(nombreImagen2.toUtf8().constData(),imagen);
                                                            ui->lineEdit->setText("");
                                                            //AQUI SE MANDA EL MENSAJE A TELEGRAM(u.u)
                                                            interfacesbot.sendMessage(valor,HoraLLegada,0,TelegramBot::NoFlag, TelegramKeyboardRequest(),&msgSent);
                                                        }
                                                        else{
                                                            //SI NO, ARROJA UN MENSAJE EN LA CONSOLA ME AVISA QUE HUBO UN PROBLEMA CON EL COMANDO
                                                            qDebug() << "Error, el comando no se ejecuto (cara triste)";
                                                        }

                                        }
                                        TAGValido = false;
                                        }
                                        else{
                                            if (IndicadordeAlta == false) {
                                            ui->label->setVisible(true);
                                            ui->label_2->setVisible(false);
                                            ui->label_3->setVisible(false);
                                            ui->lineEdit_2->setVisible(false);
                                            ui->lineEdit_3->setVisible(false);
                                            ui->pushButton_2->setVisible(false);
                                        qDebug()<<"El USUARIO NO existe en la BASE DE DATOS";
                                        QMessageBox::critical (this,tr("Error"),tr("Este usuario NO existe, Favor de darlo de alta"));
                                            }
                                            else{
                                                ui->lineEdit->setText(varNivel1Texto);
                                                IndicadordeAlta = false;

                                            }

                                        }

                           }

                           else{
                               qDebug() << "Error al abrir la base de datos";
                           }

}






void MainWindow::on_pushButton_2_clicked()
{
    QString privilegio = "U";
        if(varNivel1Texto != TAGEncontrado){
        QDate tiempo = QDate::currentDate();
        nombreImagen = varNivel1Texto +"-"+QString::number(tiempo.year())+QString::number(tiempo.month())+QString::number(tiempo.day())+".jpg";
        ui->lineEdit_3->setText(nombreImagen);
        QString nombreFormulario = ui->lineEdit_2->text();
        QString comandoNewUser = "INSERT INTO usuarios(nombre,privilegios,codigoTAG,chatID,imagen) VALUES(?,?,?,?,?)";
           QSqlQuery comando3;
            comando3.prepare(comandoNewUser);
            comando3.addBindValue(nombreFormulario);
            comando3.addBindValue(privilegio);
            comando3.addBindValue(varNivel1Texto);
            comando3.addBindValue(chatID);
            comando3.addBindValue(nombreImagen);

                       //Paso # 2.4 Ejecutar el comando de mysql.
                       if(comando3.exec()){
                           cv:: imwrite(nombreImagen.toUtf8().constData(),imagen);
                           QMessageBox::critical (this,tr("Hecho"),tr("El usuario fue dado de alta"));
                           qDebug()<<"La fila se inserto";
                           ui->label->setVisible(false);
                           ui->label_2->setVisible(false);
                           ui->label_3->setVisible(false);
                           ui->lineEdit_2->setVisible(false);
                           ui->lineEdit_3->setVisible(false);
                           ui->pushButton_2->setVisible(false);
                           ui->lineEdit->clear();
                           ui->lineEdit_2->clear();
                           ui->lineEdit_3->clear();
                       }
                       else{
                           qDebug()<<"Error_unu";
                       }
        }
        else{
            QMessageBox::critical (this,tr("Error"),tr("Error, Favor de acercar el TAG del usuario"));
        }
}



