#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#define DEBUG

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
}

void MainWindow::setTimer()
{
    qint64 videoDuration = player->position();
    if (in_Cut_State) {
        if (rsH->GetUpperValue() <= videoDuration) {
            player->pause();
            timer->stop();
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
    if (player->isVideoAvailable()) {
        qint64 videoDuration = player->duration();
        ui->slider->setMaximum(videoDuration);
        if (in_Cut_State) {
            debug("In Cut State\n");
            int start;
            if (player->position() >= rsH->GetUpperValue()) {
                debug("Upper");
                start = rsH->GetLowerValue();
            } else {
                debug("Lower");
                start = ui->slider->value();
            }
            player->setPosition(start);
            player->play();
        } else {
            player->play();
        }
        timer->start(1000);
    } else {
        qDebug() << "No open videos to play, please open the video";
        qDebug() << "File -> Open";
    }
}

void MainWindow::on_Pause_clicked()
{
    debug("on Pause clicked\n");
    player->pause();
    timer->stop();
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

void MainWindow::on_Slider_moved(int pos)
{
    debug("on Slider moved\n");
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

void MainWindow::on_Save_action()
{
    debug("Save Action Activated");
    if (in_Cut_State) {
        std::string savePath = QFileDialog::getSaveFileName(this, "Save Video Directory",currentVideoPath.c_str()).toStdString();
        if (savePath == "") return;
        std::string start = ui->timeEdit->time().toString("hh:mm:ss").toStdString();
        std::string end = ui->timeEdit_2->time().toString("hh:mm:ss").toStdString();
        std::string cutCommand = "ffmpeg -i " + currentVideoPath + " -vcodec libx264 -ss " + start + " -to " + end + " " + savePath;
        debug("cutCommand: " + cutCommand + '\n');
        std::system(cutCommand.c_str());
    }
}

void MainWindow::on_Cut_action()
{
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

void MainWindow::cut_active(bool activ)
{
    in_Cut_State = activ;
    if (activ) {
        ui->timeEdit->show();
        ui->timeEdit_2->show();
        rsH->show();
        ui->Stop->setText("Cancel");
    } else {
        ui->timeEdit->hide();
        ui->timeEdit_2->hide();
        rsH->hide();
        ui->Stop->setText("Stop");
    }
}

void MainWindow::on_upperValueChanged(int value)
{
    player->setPosition(value);
    debug("upperValue: " + std::to_string(value) + '\n');
    int remainder;
    int hours = value / 3600000;
    remainder = value - hours * 3600000;
    int minutes = remainder / 60000;
    remainder = remainder - minutes * 60000;
    int seconds = remainder / 1000;
    ui->timeEdit_2->setTime(QTime(hours, minutes, seconds));
}

void MainWindow::on_lowerValueChanged(int value)
{
    player->setPosition(value);
    ui->slider->setValue(value);
    debug("lowerValue: " + std::to_string(value) + '\n');
    int remainder;
    int hours = value / 3600000;
    remainder = value - hours * 3600000;
    int minutes = remainder / 60000;
    remainder = remainder - minutes * 60000;
    int seconds = remainder / 1000;
    ui->timeEdit->setTime(QTime(hours, minutes, seconds));
}
