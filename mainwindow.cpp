#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEBUG

MainWindow::MainWindow(int accesToRedactor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    accesRedactorMode = accesToRedactor;
    ui->setupUi(this);
    timer = new QTimer();
    ui->timeEdit->setDisplayFormat("hh:mm:ss");
    ui->timeEdit_2->setDisplayFormat("hh:mm:ss");

    videoViewer = new QVideoWidget(this);
    ui->verticalLayout->insertWidget(0, videoViewer);
    videoViewer->show();
    player->setVideoOutput(videoViewer);
    rsH = new RangeSlider(Qt::Horizontal, RangeSlider::Option::DoubleHandles, nullptr);
    ui->layout_cut->insertWidget(1, rsH);
    ui->soundSlider->setValue(100);
    ui->soundSlider->hide();
    proc = new QProcess();
    setConnects();
    cut_active(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnects()
{
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_Open_action);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_Save_action);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::on_Cut_action);
    //connect(ui->Play, &QPushButton::clicked, this, &MainWindow::on_Play_clicked);
    //connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::on_Pause_clicked);
    //connect(ui->Stop, &QPushButton::clicked, this, &MainWindow::on_Stop_clicked);
    connect(ui->slider, &QSlider::sliderMoved, this, &MainWindow::on_Slider_moved);
    connect(timer, &QTimer::timeout, this , &MainWindow::setTimer);
    connect(this, &MainWindow::videoChanged, this, &MainWindow::setVideo);
    connect(rsH, &RangeSlider::lowerValueChanged, this, &MainWindow::on_lowerValueChanged);
    connect(rsH, &RangeSlider::upperValueChanged, this, &MainWindow::on_upperValueChanged);
    connect(ui->soundSlider, &QSlider::valueChanged, this, &MainWindow::on_valueChanged);
    connect(ui->check_video, &QCheckBox::stateChanged, this, &MainWindow::on_boxVideoChanged);
    connect(ui->check_audio, &QCheckBox::stateChanged, this, &MainWindow::on_boxAudioChanged);
}

void MainWindow::setTimer()
{
    qint64 videoDuration = player->position();
    if (in_Cut_State) {
        if (rsH->GetUpperValue() <= videoDuration) {
            timer->stop();
            player->pause();
            isPlaying = false;
            ui->Play->setStyleSheet("border-image: url(:/icons/play_icon.jpeg);");
        }
    }
    ui->slider->setValue(videoDuration);
    int remainder;
    int hours = videoDuration / 3600000;
    remainder = videoDuration - hours * 3600000;
    int minutes = remainder / 60000;
    remainder = remainder - minutes * 60000;
    int seconds = remainder / 1000;
    std::string timeText = std::to_string(hours) + "::" + std::to_string(minutes) + "::" + std::to_string(seconds);
    ui->label->setText(timeText.c_str());
}

void MainWindow::debug(std::string message)
{
#ifdef DEBUG
    qDebug() << message.c_str();
#endif
}

void MainWindow::setVideo()
{
    i_videoLength = player->duration();
    debug("setVideo: duration: " + std::to_string(i_videoLength));
    int remainder;
    l_videoLength[0] = i_videoLength / 3600000;
    remainder = i_videoLength - l_videoLength[0] * 3600000;
    l_videoLength[1] = remainder / 60000;
    remainder = remainder - l_videoLength[1] * 60000;
    l_videoLength[2] = remainder / 1000;
    s_videoLength = std::to_string(l_videoLength[0]) + "::" + std::to_string(l_videoLength[1]) + "::" + std::to_string(l_videoLength[2]);
}

void MainWindow::on_Play_clicked()
{
    if (isPlaying) {
        isPlaying = false;
        player->pause();
        timer->stop();
        ui->Play->setStyleSheet("border-image: url(:/icons/play_icon.jpeg);");
        debug("on Pause clicked\n");
    } else if (player->isVideoAvailable()) {
        isPlaying = true;
        qint64 videoDuration = player->duration();
        ui->slider->setMaximum(videoDuration);
        if (in_Cut_State) {
            debug("In Cut State\n");
            int start;
            if (player->position() >= rsH->GetUpperValue()) {
                start = rsH->GetLowerValue();
            } else {
                start = ui->slider->value();
            }
            player->setPosition(start);
            player->play();
        } else {
            player->play();
        }
        timer->start(1000);
        ui->Play->setStyleSheet("border-image: url(:/icons/pause_icon.jpeg);");
    } else {
        qDebug() << "No open videos to play, please open the video";
        qDebug() << "File -> Open";
    }
}

void MainWindow::on_Stop_clicked()
{
    debug("on Stop clicked\n");
    player->stop();
    ui->slider->setValue(0);
    timer->stop();
    setTimer();
    cut_active(false);
}

void MainWindow::on_Sound_clicked()
{
    debug("on_Sound_clicked\n");
    if (ui->soundSlider->isVisible()) {
        ui->soundSlider->hide();
    } else {
        ui->soundSlider->show();
    }
}

void MainWindow::on_Slider_moved(int pos)
{
    player->setPosition(pos);
    setTimer();
}

void MainWindow::on_Open_action()
{
    QString filter = "All File (*.*) ;; Video (*.mp4 *.avi)";
    std::string  videoPath = QFileDialog::getOpenFileName(this,"Open the file",QDir::homePath(),filter).toStdString();
    debug("File name: " + videoPath + '\n');
    if (videoPath == "") return;
    QMessageBox::StandardButton mesBtn = QMessageBox::question( this, "Chosen file",
                                tr("Do you want to open this file?\n"),
                                QMessageBox::No | QMessageBox::Yes,
                                QMessageBox::Yes);
    if(mesBtn == QMessageBox::Yes) {
        player->setMedia(QUrl::fromLocalFile(videoPath.c_str()));
        currentVideoPath = videoPath;
    } else if (mesBtn == QMessageBox::No) {
        ui->actionOpen->trigger();
    }
}
#include <iostream>
void MainWindow::on_Save_action()
{
    if (accesRedactorMode == 1) {
        QMessageBox::warning(this, "Access Redactor Mode", "Save is for saving video changes,"
                                                           " but you don't have access to the editor mode due"
                                                           " to missing ffmpeg library");
        return;
    }
    debug("Save Action Activated");
    if (in_Cut_State) {
        std::string savePath = QFileDialog::getSaveFileName(this, "Save Video Directory",currentVideoPath.c_str()).toStdString();
        if (savePath == "") return;
        std::string start = ui->timeEdit->time().toString("hh:mm:ss").toStdString();
        std::string end = ui->timeEdit_2->time().toString("hh:mm:ss").toStdString();
        std::string cutCommand = "ffmpeg -i " + currentVideoPath + " -vcodec libx264 -ss " + start + " -to " + end + " -y ";
        if (!ui->check_audio->isChecked()) {
            debug("No audio");
            cutCommand += " -an ";
        } else if (!ui->check_video->isChecked()) {
            debug("No video\n");
            cutCommand += " -vn ";
        }
        cutCommand += savePath;
        debug("Command: " + cutCommand);
        std::thread system_work([&, cutCommand]() {
            std::cout << "cutCommand: " + cutCommand + '\n';
            proc->execute(cutCommand.c_str());
            qDebug() << "ffmpeg video cut proc exit status: " << proc->exitStatus() << " code: " << proc->exitCode() << '\n';
//            std::system(cutCommand.c_str());
        });
        system_work.detach();
        on_Stop_clicked();
    }
}

void MainWindow::on_Cut_action()
{
    if (accesRedactorMode == 1) {
        QMessageBox::warning(this, "Access Redactor Mode", "Cut is designed to cut a part from a video,"
                                                           " but you don't have access to the editor mode due"
                                                           " to missing ffmpeg library");
        return;
    }
    debug("on Cut action\n");
    if (!player->isVideoAvailable()) {
        qDebug() << "No open videos to cut, please open the video";
        qDebug() << "File -> Open";
        return;
    }
    emit videoChanged();
    QTime maxDuration(l_videoLength[0], l_videoLength[1], l_videoLength[2]);
    ui->timeEdit->setMaximumTime(maxDuration);
    ui->timeEdit_2->setMaximumTime(maxDuration);
    ui->timeEdit_2->setTime(maxDuration);
    rsH->setMaximum(i_videoLength);
    cut_active(true);
}

void MainWindow::cut_active(bool state)
{
    in_Cut_State = state;
    if (state) {
        ui->timeEdit->show();
        ui->timeEdit_2->show();
        rsH->show();
        ui->check_video->show();
        ui->check_audio->show();
//        ui->Stop->setText("Cancel");
    } else {
        ui->timeEdit->hide();
        ui->timeEdit_2->hide();
        rsH->hide();
        ui->check_video->hide();
        ui->check_audio->hide();
//        ui->Stop->setText("Stop");
    }
}

void MainWindow::on_upperValueChanged(int value)
{
    player->setPosition(value);
    //debug("upperValue: " + std::to_string(value) + '\n');
    int remainder;
    int hours = value / 3600000;
    remainder = value - hours * 3600000;
    int minutes = remainder / 60000;
    remainder = remainder - minutes * 60000;
    int seconds = remainder / 1000;
    ui->timeEdit_2->setTime(QTime(hours, minutes, seconds));
}

void MainWindow::on_valueChanged(int value)
{
    player->setVolume(value);
    if (value == 0) {
        ui->Sound->setStyleSheet("border-image: url(:/icons/sound_off.jpeg);");
    } else if (value < 50) {
        ui->Sound->setStyleSheet("border-image: url(:/icons/sound_on.jpeg);");
    } else {
        ui->Sound->setStyleSheet("border-image: url(:/icons/sound_max.jpeg);");
    }
}

void MainWindow::on_boxVideoChanged(int state)
{
    if (!state) {
        ui->check_audio->setChecked(true);
    }
}

void MainWindow::on_boxAudioChanged(int state)
{
    if (!state) {
        ui->check_video->setChecked(true);
    }
}

void MainWindow::on_lowerValueChanged(int value)
{
    player->setPosition(value);
    ui->slider->setValue(value);
    //debug("lowerValue: " + std::to_string(value) + '\n');
    int remainder;
    int hours = value / 3600000;
    remainder = value - hours * 3600000;
    int minutes = remainder / 60000;
    remainder = remainder - minutes * 60000;
    int seconds = remainder / 1000;
    ui->timeEdit->setTime(QTime(hours, minutes, seconds));
}
