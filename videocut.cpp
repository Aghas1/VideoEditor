#include "videocut.h"
#include "ui_videocut.h"
#include <cstdlib>
#include <unistd.h>

VideoCut::VideoCut(std::string currentVideoPath, int videoLength[], QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCut)
{
    ui->setupUi(this);
    videoPath = currentVideoPath;
    ui->timeEdit->setDisplayFormat("hh:mm:ss");
    ui->timeEdit_2->setDisplayFormat("hh:mm:ss");
    qDebug() << "hh: " << videoLength[0] << "mm: " << videoLength[1] << "ss: " << videoLength[2];
    QTime maxDuration(videoLength[0], videoLength[1], videoLength[2]);
    ui->timeEdit->setMaximumTime(maxDuration);
    ui->timeEdit_2->setMaximumTime(maxDuration);
}

VideoCut::~VideoCut()
{
    delete ui;
}

void VideoCut::on_pushButton_clicked()
{
    std::thread th1([&]() {
        qDebug() << ui->timeEdit->time().toString("hh:mm:ss");
        std::string start = ui->timeEdit->time().toString("hh:mm:ss").toStdString();
        std::string end = ui->timeEdit_2->time().toString("hh:mm:ss").toStdString();
        std::string cutCommand = "ffmpeg -i " + videoPath + " -vcodec libx264 -ss " + start + " -to " + end + " out.avi";
        qDebug() << "cutCommand: " << cutCommand.c_str() << '\n';
        std::system(cutCommand.c_str());
    });
    th1.detach();
}
