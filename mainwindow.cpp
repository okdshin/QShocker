#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSound>
#include <thread>

#include "Shocker/shocker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sound_player_(this)
{
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(play_sound_signal(QString)), &sound_player_, SLOT(play_sound(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_sound_filename(){
    QString sound_filename = QFileDialog::getOpenFileName(this, "sound file");
    if(sound_filename.isEmpty()){ return; }
    ui->lineEdit->setText(sound_filename);
}

void MainWindow::set_movie_filename(){
    QString movie_filename = QFileDialog::getOpenFileName(this, "sound file");
    if(movie_filename.isEmpty()){ return; }
    ui->lineEdit_2->setText(movie_filename);
}

void MainWindow::start_trials(){
    auto sound_player = [this](){ emit play_sound_signal(ui->lineEdit->text()); };
    boost::asio::io_service io_service;
    shocker::tms_shocker s(io_service, sound_player,
                  ui->lineEdit_2->text().toStdString(), //movie
                  ui->doubleSpinBox->value(), //finger starts moving at
                  ui->doubleSpinBox_2->value(), //tms shocks relative at
                  ui->lineEdit_3->text().toStdString(), //serial port name
                  ui->spinBox->value() //number of trials
                  );
    boost::asio::io_service::work work(io_service);
    std::thread t1([&io_service](){ io_service.run(); });
    std::thread t2([&io_service](){ io_service.run(); });
    io_service.post([&s](){ s.run(); });
    io_service.run();
    t1.join();
    t2.join();
}
