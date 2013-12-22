#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <boost/asio.hpp>
#include "sound_player_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void set_sound_filename();
    void set_movie_filename();
    void start_trials();

signals:
    void play_sound_signal(QString);
    
private:
    Ui::MainWindow *ui;
    sound_player_thread sound_player_;
};

#endif // MAINWINDOW_H
