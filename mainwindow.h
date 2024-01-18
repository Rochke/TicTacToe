#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GlobalIncludes.h"
#include "audioplayer.h"
#include "imgbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initButtons();
    void initSideBar();
    void initLayouts();
    void initAnimations();
    void mpError();
    void showRetry();
    AudioPlayer* mediaplayer;

private:
    Ui::MainWindow *ui;

    QMediaPlayer *bpressmp = new QMediaPlayer;
    QAudioOutput *bpressao = new QAudioOutput;
    QMediaPlayer *sfxmp = new QMediaPlayer;
    QAudioOutput *sfxao = new QAudioOutput;

    std::vector<ImageButton*>buttons;
    std::vector<QLabel*>pointIcons;
    std::vector<QLabel*>pointGifs;
    std::vector<QLabel*>pointNoSPFXs;
    QGridLayout* layout; //Grid 3x3
    QHBoxLayout* outerTBT; //Outer Three By Three
    QVBoxLayout* outerSB; //Outer Sidebar
    ImageButton* sbutton1;
    ImageButton* sbutton2;
    ImageButton* sbutton3;
    ImageButton* sbutton4;
    ImageButton *retryYes;
    ImageButton *retryNo;

    QPropertyAnimation *userPointGain;
    QPropertyAnimation *cpuPointGain;
    QPropertyAnimation *retryIn;
    QPropertyAnimation *retryOut;
    QPropertyAnimation *retryBGIn;
    QPropertyAnimation *retryBGOut;
    int previousUserPoints = -1;
    int previousCpuPoints = -1;
    int userPoints{ 0 }, cpuPoints{ 0 };
    bool unsavedhd;
    bool unsavedspfx;
    bool unsavedmusic;
    bool unsavedsfx;
    bool hd;
    bool spfx;
    bool music;
    bool sfx;
    bool winCheck();
    bool tieCheck();
    int winCheckLogic();
    void updatePoints();
    void readSave();
    void loadSave();
    enum boxStatus { EMPTY = 0, PLAYER_OCCUPIED, CPU_OCCUPIED };
    boxStatus getBoxStatus(int index){
        boxStatus temp = static_cast<boxStatus>(buttons[index]->property("boxStatus").toInt());
        return temp;
    }

private slots:
    void buttonClicked();
    void settingsClicked();
    void saveClicked();
    void sButtonClicked();
    void retryButtonClicked();
};
#endif // MAINWINDOW_H
