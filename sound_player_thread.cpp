#include "sound_player_thread.h"
#include <QSound>
#include <iostream>

sound_player_thread::sound_player_thread(QObject *parent) :
    QThread(parent)
{
    //std::cout << "QSound is available? : " << (QSound::isAvailable() ? "Yes" : "No") << std::endl;
}

void sound_player_thread::play_sound(QString sound_filename){
    std::cout << "play sound" << std::endl;
    QSound::play(sound_filename);
}
