#include <fstream>
#include <sstream>
#include "mainwindow.h"
#include "error.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Initializing UI Elements
    ui->setupUi(this);
    this->setStyleSheet("background-color: #1e1e1e;");
    this->setFixedSize(724, 489);
    mediaplayer = new AudioPlayer(this);

    readSave();
    initLayouts();
    initSideBar();
    initButtons();
    initAnimations();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Main Window Class Members

void MainWindow::initLayouts() {
    layout = new QGridLayout();
    outerTBT = new QHBoxLayout();
    outerSB = new QVBoxLayout();
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(outerTBT);
    setCentralWidget(centralWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

void MainWindow::initButtons() {
    for(int i = 0; i < 9; i++){
        QString bName = QString("button%1").arg(i + 1);
        ImageButton *button = new ImageButton(QPixmap(":/Resources/buttonBG.png"));
        button->setObjectName(bName);
        button->setProperty("boxStatus", EMPTY);
        button->setMinimumSize(150, 150);

        int row = i / 3;
        int column = i % 3;

        layout->addWidget(button, row, column);
        connect(button, &ImageButton::clicked, this, &MainWindow::buttonClicked);
        buttons.push_back(button);
    }

    outerTBT->addLayout(layout);
}

void MainWindow::initSideBar() {
    QSpacerItem *spacer = new QSpacerItem(10, 1000, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ImageButton *settings = new ImageButton(QPixmap(":/Resources/Settings.png"));
    QLabel *scoreboard = new QLabel(this);
    QLabel *titlecard = new QLabel(this);
    settings->setFixedSize(250, 100);
    scoreboard->setFixedSize(250, 100);
    titlecard->setFixedSize(250, 100);
    QPixmap tcPixmapUnscaled(":/Resources/ttt-title.png");
    QPixmap tcPixmap = tcPixmapUnscaled.scaled(titlecard->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    titlecard->setPixmap(tcPixmap);
    QPixmap sbPixmapUnscaled(":/Resources/scoreboard.png");
    QPixmap sbPixmap = sbPixmapUnscaled.scaled(scoreboard->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scoreboard->setPixmap(sbPixmap);
    connect(settings, &ImageButton::clicked, this, &MainWindow::settingsClicked);

    outerTBT->addLayout(outerSB);
    outerSB->addWidget(titlecard);
    outerSB->addSpacerItem(spacer);
    outerSB->addWidget(scoreboard);
    outerSB->addWidget(settings);
    QPixmap pointPixmap(":/Resources/pointBG.png");
    QPixmap pointNoSPFXPixmap(":/Resources/point.png");
    QMovie *p = new QMovie(":/Resources/point.gif");
    for(int i = 0; i < 6; i++){
        QString pointName = QString("point%1").arg(i + 1);
        QString pointGifName = QString("pointGif%1").arg(i + 1);
        QString pointNoSPFXName = QString("pointNoSPFX%1").arg(i + 1);
        QLabel *point = new QLabel(this);
        QLabel *pointNoSPFX = new QLabel(this);
        point->setScaledContents(true);
        point->setObjectName(pointName);
        point->setPixmap(pointPixmap);
        pointNoSPFX->setScaledContents(true);
        pointNoSPFX->setObjectName(pointNoSPFXName);
        pointNoSPFX->setPixmap(pointNoSPFXPixmap);
        if(i < 3){
            point->setGeometry(20 + (i * 35), 304, pointPixmap.width() / 3, pointPixmap.height() / 3);
            pointNoSPFX->setGeometry(20 + (i * 35), 304, pointPixmap.width() / 3, pointPixmap.height() / 3);
        } else {
            point->setGeometry(40 + (i * 35), 304, pointPixmap.width() / 3, pointPixmap.height() / 3);
            pointNoSPFX->setGeometry(40 + (i * 35), 304, pointPixmap.width() / 3, pointPixmap.height() / 3);
        }
        point->show();
        pointIcons.push_back(point);
        pointNoSPFX->hide();
        pointNoSPFXs.push_back(pointNoSPFX);
        QLabel *pointGif = new QLabel(this);
        pointGif->setScaledContents(true);
        pointGif->setObjectName(pointGifName);
        if(i < 3){
            pointGif->setGeometry(20 + (i * 35), 304, pointPixmap.width() / 3, pointPixmap.height() / 3);
        } else {
            pointGif->setGeometry(40 + (i * 35), 304, pointPixmap.width() / 3, pointPixmap.height() / 3);
        }
        pointGif->setMovie(p);
        p->start();
        pointGif->hide();
        pointGifs.push_back(pointGif);
    }
}

void MainWindow::settingsClicked() {
    if(sfx) mediaplayer->play(QUrl("qrc:/Resources/bpress.mp3"));
    QDialog *settings = new QDialog;
    settings->setWindowTitle("Settings");
    settings->setModal(true);
    QLabel *settingsui = new QLabel(settings);
    QPixmap uipixmap(":/Resources/SettingsUI.png");
    settingsui->setPixmap(uipixmap);
    settings->setFixedSize(uipixmap.size());
    QPixmap savepixmap(":/Resources/Save.png");
    QPixmap okpixmap(":/Resources/Ok.png");
    QPixmap sbuttonpixmap(":/Resources/SettingsButton1.png");
    QPixmap sbuttonfadedpixmap(":/Resources/SettingsButtonFaded.png");
    ImageButton *save = new ImageButton(savepixmap, settings);
    ImageButton *ok = new ImageButton(okpixmap, settings);
    //Ordered As : 1. Graphics 2. Audio (Top To Bottom)
    sbutton1 = new ImageButton(sbuttonfadedpixmap, settings);
    sbutton2 = new ImageButton(sbuttonpixmap, settings);
    sbutton3 = new ImageButton(sbuttonpixmap, settings);
    sbutton4 = new ImageButton(sbuttonpixmap, settings);
    if(hd) sbutton1->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    else sbutton1->setNewPixmap(QPixmap(":/Resources/SettingsButtonFaded.png"));    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Change For HD Update
    if(spfx) sbutton2->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    else sbutton2->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
    if(music) sbutton3->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    else sbutton3->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
    if(sfx) sbutton4->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    else sbutton4->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
    unsavedhd = hd;
    unsavedspfx = spfx;
    unsavedmusic = music;
    unsavedsfx = sfx;
    connect(ok, &ImageButton::clicked, settings, &QDialog::close);
    connect(save, &ImageButton::clicked, this, &MainWindow::saveClicked);
    connect(sbutton1, &ImageButton::clicked, this, &MainWindow::sButtonClicked);
    connect(sbutton2, &ImageButton::clicked, this, &MainWindow::sButtonClicked);
    connect(sbutton3, &ImageButton::clicked, this, &MainWindow::sButtonClicked);
    connect(sbutton4, &ImageButton::clicked, this, &MainWindow::sButtonClicked);
    sbutton1->setObjectName("sbutton1");
    sbutton2->setObjectName("sbutton2");
    sbutton3->setObjectName("sbutton3");
    sbutton4->setObjectName("sbutton4");
    save->setGeometry(1076, 570, 150, 100);
    ok->setGeometry(1076, 450, 150, 100);
    sbutton1->setGeometry(44, 184, 50, 50);
    sbutton2->setGeometry(44, 252, 50, 50);
    sbutton3->setGeometry(655, 183, 50, 50);
    sbutton4->setGeometry(655, 253, 50, 50);
    settings->exec();
}

void MainWindow::sButtonClicked(){
    ImageButton* clickedButton = qobject_cast<ImageButton*>(sender());
    /*if (clickedButton == sbutton1) {
        if(unsavedhd) unsavedhd = false;
        else unsavedhd = true;
        if(!unsavedhd) sbutton1->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
        else sbutton1->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    }*/if (clickedButton == sbutton2) {
        if(unsavedspfx) unsavedspfx = false;
        else unsavedspfx = true;
        if(!unsavedspfx) sbutton2->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
        else sbutton2->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    }if (clickedButton == sbutton3) {
        if(unsavedmusic) unsavedmusic = false;
        else unsavedmusic = true;
        if(!unsavedmusic) sbutton3->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
        else sbutton3->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    }if (clickedButton == sbutton4) {
        if(unsavedsfx) unsavedsfx = false;
        else unsavedsfx = true;
        if(!unsavedsfx) sbutton4->setNewPixmap(QPixmap(":/Resources/SettingsButton1.png"));
        else sbutton4->setNewPixmap(QPixmap(":/Resources/SettingsButton2.png"));
    }
}

void MainWindow::saveClicked(){
    hd = unsavedhd;
    spfx = unsavedspfx;
    music = unsavedmusic;
    sfx = unsavedsfx;
    std::ofstream save(saveFilePath.toStdString(), std::ios::out | std::ios::trunc);
    if(!save) {
        error("save.txt failed to open! (saveClicked())");
    }
    save << "hd = " << hd << ";\n";
    save << "spfx = " << spfx << ";\n";
    save << "music = " << music << ";\n";
    save << "sfx = " << sfx << ';';

    save.close();
    loadSave();
    if(sfx) mediaplayer->play(QUrl("qrc:/Resources/save.mp3"));
}

void MainWindow::readSave(){
    std::ifstream save(saveFilePath.toStdString());
    if(!save){
        error("save.txt failed to open! (readSave()");
    }
    std::vector<std::string>options{ "hd", "spfx", "music", "sfx" };
    std::string word;
    std::string line;
    std::string str;
    char c;
    bool chosen{ false };
    int option{ 0 };
    while(true) {
        getline(save, line);
        std::istringstream iss(line);
        while(iss >> word){

            if(chosen == false) {
                for(unsigned long long i = 0; i < options.size(); i++) {
                    if(word == options[i]) {
                        option = i;
                        chosen = true;
                        break;
                    }
                    else if (word != options[i] && i == options.size())
                        error("No options found!");
                }
            }
            else if (chosen == true){
                if(word != "=") error("Missing '=' in save.txt");
                iss >> word;
                c = word.at(0);
                str = c;
                switch(option) {
                case 0 :
                    hd = std::stoi(str);
                    break;
                case 1 :
                    spfx = std::stoi(str);
                    break;
                case 2 :
                    music = std::stoi(str);
                    break;
                case 3 :
                    sfx = std::stoi(str);
                    break;
                }
                if(word.at(1) == ';') chosen = false;
                else error("Missing ';' in save.txt");
            }
        }

        if(save.eof()) break;
    }
    save.close();
    loadSave();
}

void MainWindow::loadSave() {
    if(music) {
        mediaplayer->stopAllPlayers(0);
        mediaplayer->loop(QUrl("qrc:/Resources/Investigations.mp3"), 0);
    }
    else if (!music) {
        mediaplayer->stop(0);
    }
    for(unsigned long long i = 0; i < pointGifs.size(); i++) {
        pointGifs[i]->hide();
        pointNoSPFXs[i]->hide();
    }
    if(spfx) {
        for(int i = 0; i < userPoints; i++) {
            pointGifs[i]->show();

        }
        for(int i = 0; i < cpuPoints; i++) {
            pointGifs[i + 3]->show();
        }
    } else {
        for(int i = 0; i < userPoints; i++) {
            pointNoSPFXs[i]->show();
        }
        for(int i = 0; i < cpuPoints; i++) {
            pointNoSPFXs[i + 3]->show();
        }
    }
}

void MainWindow::initAnimations() {
    QLabel *cpuPointLabel = new QLabel(this);
    QPixmap cpuPixmap(":/Resources/spfxCpuPoint.png");
    cpuPointLabel->setScaledContents(true);
    cpuPointLabel->setPixmap(cpuPixmap);
    cpuPointLabel->setAttribute(Qt::WA_TranslucentBackground);
    cpuPointLabel->setGeometry(265, 10, 900 / 2, 900 / 2);
    QGraphicsOpacityEffect* opacityEffectCpu = new QGraphicsOpacityEffect(cpuPointLabel);
    cpuPointLabel->setGraphicsEffect(opacityEffectCpu);
    cpuPointLabel->hide();
    cpuPointGain = new QPropertyAnimation(opacityEffectCpu, "opacity");
    cpuPointGain->setDuration(300);
    cpuPointGain->setStartValue(0);
    cpuPointGain->setKeyValueAt(0.4, 0.8);
    cpuPointGain->setEndValue(0);
    cpuPointGain->setEasingCurve(QEasingCurve::InOutQuad);
    cpuPointGain->setLoopCount(1);

    QLabel *userPointLabel = new QLabel(this);
    QPixmap userPixmap(":/Resources/spfxUserPoint.png");
    userPointLabel->setScaledContents(true);
    userPointLabel->setPixmap(userPixmap);
    userPointLabel->setAttribute(Qt::WA_TranslucentBackground);
    userPointLabel->setGeometry(265, 10, 900 / 2, 900 / 2);
    QGraphicsOpacityEffect* opacityEffectUser = new QGraphicsOpacityEffect(userPointLabel);
    userPointLabel->setGraphicsEffect(opacityEffectUser);
    userPointLabel->hide();
    userPointGain = new QPropertyAnimation(opacityEffectUser, "opacity");
    userPointGain->setDuration(300);
    userPointGain->setStartValue(0);
    userPointGain->setKeyValueAt(0.4, 0.8);
    userPointGain->setEndValue(0);
    userPointGain->setEasingCurve(QEasingCurve::InOutQuad);
    userPointGain->setLoopCount(1);

    QPixmap retryBGPixmap(":/Resources/retryBG.png");
    QLabel* retryBGLabel = new QLabel(this);
    retryBGLabel->setPixmap(retryBGPixmap);
    retryBGLabel->setScaledContents(true);
    retryBGLabel->setAttribute(Qt::WA_TranslucentBackground);
    retryBGLabel->setGeometry(0, 0, 724, 489);
    QGraphicsOpacityEffect* opacityEffectRetry = new QGraphicsOpacityEffect(retryBGLabel);
    retryBGLabel->setGraphicsEffect(opacityEffectRetry);
    retryBGLabel->hide();
    retryBGIn = new QPropertyAnimation(opacityEffectRetry, "opacity");
    retryBGIn->setDuration(2000);
    retryBGIn->setStartValue(0);
    retryBGIn->setEndValue(1);
    retryBGIn->setEasingCurve(QEasingCurve::InOutQuad);
    retryBGIn->setLoopCount(1);
    retryBGOut = new QPropertyAnimation(opacityEffectRetry, "opacity");
    retryBGOut->setDuration(750);
    retryBGOut->setStartValue(1);
    retryBGOut->setEndValue(0);
    retryBGOut->setEasingCurve(QEasingCurve::InOutQuad);
    retryBGOut->setLoopCount(1);

    QLabel *retryLabel = new QLabel(this);
    QPixmap retryPixmap(":/Resources/RetryUI.png");
    retryLabel->setScaledContents(true);
    retryLabel->setPixmap(retryPixmap);
    retryLabel->setAttribute(Qt::WA_TranslucentBackground);
    retryLabel->setGeometry(290, -300, 800 / 2, 600 / 2);

    retryIn = new QPropertyAnimation(retryLabel, "geometry");
    retryIn->setDuration(1000);
    retryIn->setStartValue(QRect(290, -300, 800 / 2, 600 / 2));
    retryIn->setEndValue(QRect(290, 0, 800 / 2, 600 / 2));
    retryIn->setEasingCurve(QEasingCurve::OutBounce);

    retryOut = new QPropertyAnimation(retryLabel, "geometry");
    retryOut->setDuration(400);
    retryOut->setStartValue(QRect(290, 0, 800 / 2, 600 / 2));
    retryOut->setEndValue(QRect(290, -300, 800 / 2, 600 / 2));
    retryOut->setEasingCurve(QEasingCurve::OutCurve);

    connect(cpuPointGain, &QPropertyAnimation::stateChanged, this, [cpuPointLabel](QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
        if (newState == QAbstractAnimation::Running && oldState != QAbstractAnimation::Running) {
            cpuPointLabel->show();
        }
    });
    connect(cpuPointGain, &QPropertyAnimation::finished, cpuPointLabel, &QLabel::hide);

    connect(userPointGain, &QPropertyAnimation::stateChanged, this, [userPointLabel](QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
        if (newState == QAbstractAnimation::Running && oldState != QAbstractAnimation::Running) {
            userPointLabel->show();
        }
    });
    connect(userPointGain, &QPropertyAnimation::finished, userPointLabel, &QLabel::hide);

    connect(retryBGIn, &QPropertyAnimation::stateChanged, this, [retryBGLabel](QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
        if (newState == QAbstractAnimation::Running && oldState != QAbstractAnimation::Running) {
            retryBGLabel->show();
        }
    });
    connect(retryBGOut, &QPropertyAnimation::finished, retryBGLabel, &QLabel::hide);
}

//Game Logic

int MainWindow::winCheckLogic() {
    const std::vector<std::vector<int>> winCombinations = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (const std::vector<int>& combination : winCombinations) {
        boxStatus firstStatus = static_cast<boxStatus>(buttons[combination[0]]->property("boxStatus").toInt());

        if (firstStatus != EMPTY) {
            bool win = true;
            for (int i = 1; i < 3; i++) {
                boxStatus currentStatus = static_cast<boxStatus>(buttons[combination[i]]->property("boxStatus").toInt());
                if (currentStatus != firstStatus) {
                    win = false;
                    break;
                }
            }

            if (win) {
                return (firstStatus == PLAYER_OCCUPIED) ? 1 : 0;
            }
        }
    }
    return 2;
}

bool MainWindow::winCheck() {
    int win = winCheckLogic();
    if(win == 1) {
        userPoints++;
        updatePoints();
        showRetry();
        return 1;
    }
    else if(win == 0) {
        cpuPoints++;
        updatePoints();
        showRetry();
        return 1;
    }
    bool tie = tieCheck();
    if(tie){
        updatePoints();
        showRetry();
        return 1;
    }

    return 0;
}

void MainWindow::updatePoints() {
    if(userPoints != previousUserPoints && userPoints != 0) {
        if(spfx) pointGifs[userPoints - 1]->show();
        else pointNoSPFXs[userPoints - 1]->show();
        if(userPoints != 3) {
            if(sfx) mediaplayer->play(QUrl("qrc:/Resources/coin.wav"));
        }
        if(spfx) userPointGain->start();
        previousUserPoints = userPoints;
    }
    if(cpuPoints != previousCpuPoints && cpuPoints != 0 ) {
        if(spfx) pointGifs[cpuPoints + 2]->show();
        else pointNoSPFXs[cpuPoints + 2]->show();
        if(cpuPoints != 3) {
            if(sfx) mediaplayer->play(QUrl("qrc:/Resources/coincpu.mp3"));
        }
        if(spfx) cpuPointGain->start();
        previousCpuPoints = cpuPoints;
    }
}

bool MainWindow::tieCheck() {
    for (auto& button : buttons) {
        if (static_cast<boxStatus>(button->property("boxStatus").toInt()) == EMPTY) {
            return false;
        }
    }
    return true;
}

void MainWindow::buttonClicked() {
    if(sfx) mediaplayer->play(QUrl("qrc:/Resources/bpress.mp3"));
    ImageButton* clickedButton = qobject_cast<ImageButton*>(sender());
    QPixmap xIcon(":/Resources/X.png");
    QPixmap oIcon(":/Resources/Circle.png");
    while(true) {
        if(clickedButton->property("boxStatus").toInt() == EMPTY) {
            clickedButton->setProperty("boxStatus", PLAYER_OCCUPIED);
            clickedButton->setNewPixmap(xIcon);

            bool outcomeFound = winCheck();
            if(outcomeFound){
                break;
            }

            ImageButton* cpuButton;
            srand(time(NULL));
            while(true) {
                int ran = (rand() % 9);
                cpuButton = buttons[ran];
                if(buttons[ran]->property("boxStatus").toInt() == EMPTY)
                    break;
            }
            cpuButton->setProperty("boxStatus", CPU_OCCUPIED);
            cpuButton->setNewPixmap(oIcon);

            outcomeFound = winCheck();
            if(outcomeFound){
                break;
            }
        }
        break;
    }
}

void MainWindow::showRetry() {
    if(userPoints == 3 || cpuPoints == 3) {
        retryBGIn->start();
        retryIn->start();
        QPixmap retryYesPixmap(":/Resources/RetryYes.png");
        QPixmap retryNoPixmap(":/Resources/RetryNo.png");
        retryYes = new ImageButton(retryYesPixmap, this);
        retryNo = new ImageButton(retryNoPixmap, this);
        retryYes->setObjectName("retryYes");
        retryYes->setGeometry(304, 210, 262 / 2, 130 / 2);
        retryNo->setGeometry(545, 158, 262 / 2, 130 / 2);

        connect(retryYes, &QPushButton::clicked, this, &MainWindow::retryButtonClicked);
        connect(retryNo, &QPushButton::clicked, this, &MainWindow::retryButtonClicked);

        connect(retryIn, &QPropertyAnimation::finished, this, [this]() {
            retryYes->show();
            retryNo->show();
        });
    }
    else {
        for(unsigned long long i = 0; i < buttons.size(); i++) {
        buttons[i]->setProperty("boxStatus", EMPTY);
        buttons[i]->setNewPixmap(QPixmap(":/Resources/buttonBG.png"));
        }
    }
}

void MainWindow::retryButtonClicked() {
    ImageButton* clickedButton = qobject_cast<ImageButton*>(sender());
    if(clickedButton->objectName() == "retryYes") {
        retryBGOut->start();
        retryOut->start();
        for(unsigned long long i = 0; i < buttons.size(); i++) {
            buttons[i]->setProperty("boxStatus", EMPTY);
            buttons[i]->setNewPixmap(QPixmap(":/Resources/buttonBG.png"));
            cpuPoints = 0;
            userPoints = 0;
            for(int i = 0; i < 6; i++) {
                pointGifs[i]->hide();
                pointNoSPFXs[i]->hide();
            }
        }
        retryYes->hide();
        retryNo->hide();
    } else {
        exit(0);
    }
}
