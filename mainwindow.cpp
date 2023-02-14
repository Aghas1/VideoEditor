#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer();
    vw = new QVideoWidget(this);
    ui->verticalLayout->insertWidget(0, vw);
    vw->show();
    player->setVideoOutput(vw);
    setConnects();
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
    connect(ui->Play, &QPushButton::clicked, this, &MainWindow::on_Play_clicked);
    connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::on_Pause_clicked);
    connect(ui->Stop, &QPushButton::clicked, this, &MainWindow::on_Stop_clicked);
    connect(ui->slider, &QSlider::sliderMoved, this, &MainWindow::on_Slider_moved);
    connect(timer, &QTimer::timeout, this , &MainWindow::setTimer);
    connect(this, &MainWindow::videoChanged, this, &MainWindow::setVideo);
}

void MainWindow::setTimer()
{
    qint64 videoDuration = player->position();
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

void MainWindow::setVideo()
{
    i_videoLength = player->duration();
    qDebug() << "setVideo: duration: " << i_videoLength;
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
    if (player->isVideoAvailable()) {
        qint64 videoDuration = player->duration();
        ui->slider->setMaximum(videoDuration);
        player->play();
        timer->start(1000);
    } else {
        qDebug() << "No open videos to play, please open the video";
        qDebug() << "File -> Open";
    }
}

void MainWindow::on_Pause_clicked()
{
    player->pause();
    timer->stop();
}

void MainWindow::on_Stop_clicked()
{
    player->stop();
    ui->slider->setValue(0);
    timer->stop();
    setTimer();
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
    qDebug() << "File name: " << videoPath.c_str() << '\n';
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

void MainWindow::on_Save_action()
{
    qDebug() << "Save Action Activated";
}

void MainWindow::on_Cut_action()
{
    if (!player->isVideoAvailable()) {
        qDebug() << "No open videos to cut, please open the video";
        qDebug() << "File -> Open";
        return;
    }
    emit videoChanged();
    VideoCut* cutWindow = new VideoCut(currentVideoPath, l_videoLength, i_videoLength);
    cutWindow->show();
}
