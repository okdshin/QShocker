#ifndef SOUND_PLAYER_THREAD_H
#define SOUND_PLAYER_THREAD_H

#include <QThread>

class sound_player_thread : public QThread
{
    Q_OBJECT
public:
    explicit sound_player_thread(QObject *parent = 0);
    
signals:
    
public slots:
    void play_sound(QString sound_filename);
    
};

#endif // SOUND_PLAYER_THREAD_H
