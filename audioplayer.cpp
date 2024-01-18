#include "audioplayer.h"
#include "error.h"

void AudioPlayer::play(const QUrl &url) {
    QMediaPlayer* player = new QMediaPlayer;
    QAudioOutput* ao = new QAudioOutput;
    player->setAudioOutput(ao);
    player->setSource(url);
    if(player->mediaStatus() == QMediaPlayer::InvalidMedia){
        error("Player Resource Error");
    }
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [player](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) {
            player->play();
        }
    });

    connect(player, &QMediaPlayer::playbackStateChanged, player, [player](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::StoppedState) {
            player->deleteLater();
        }
    });
}

void AudioPlayer::loop(const QUrl &url, const int &index) {
    if(index < 0 || index > 2) {
        error("Invalid Index (loop())");
    }
    QMediaPlayer* lplayer = new QMediaPlayer;
    QAudioOutput* loutput = new QAudioOutput;
    lplayer->setAudioOutput(loutput);
    lplayer->setSource(url);
    loopurls[index] = url;
    loopplayers[index] = lplayer;
    shouldPlay[index] = true;

    connect(lplayer, &QMediaPlayer::mediaStatusChanged, this, [this, lplayer, index](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia && shouldPlay[index]) {
            lplayer->play();
        }
    });

    connect(lplayer, &QMediaPlayer::playbackStateChanged, lplayer, [this, lplayer, index](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::StoppedState && shouldPlay[index]) {
            lplayer->play();
        }
    });
}

void AudioPlayer::stop(const int &index) {
    if(index < 0 || index > 2) {
        error("Invalid Index (stop())");
    }
    if(loopplayers[index] != nullptr) {
        shouldPlay[index] = false;
        loopplayers[index]->stop();
    }
}

void AudioPlayer::stopAllPlayers(const int &index) {
    for (int i = 0; i < 3; ++i) {  // Assuming you have 3 players
        if (loopplayers[i] != nullptr) {
            shouldPlay[index] = true;
            loopplayers[i]->stop();
            delete loopplayers[i];
            loopplayers[i] = nullptr;
        }
    }
}
