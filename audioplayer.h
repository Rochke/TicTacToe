#pragma once

#include <QMediaPlayer>
#include <QAudioOutput>

class AudioPlayer : public QObject {
    Q_OBJECT
public:
    AudioPlayer(QObject *parent = nullptr) : QObject(parent) {
        loopurls.resize(3);
        loopplayers.resize(3, nullptr);
    }
    void play(const QUrl&);
    void loop(const QUrl&, const int&);
    void stop(const int&);
    void stopAllPlayers(const int&);
private:
    std::vector<QUrl>loopurls;
    std::vector<QMediaPlayer*>loopplayers;
    bool shouldPlay[3] = {true, true, true};
};
