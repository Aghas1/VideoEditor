#ifndef VIDEOCUT_H
#define VIDEOCUT_H

#include <QWidget>
#include <QDebug>
#include <thread>
#include <rangeslider.h>

namespace Ui {
class VideoCut;
}

class VideoCut : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCut(std::string currentVideoPath, int videoLength[], int i_video_length, QWidget *parent = nullptr);
    ~VideoCut();
    void setConnects();

private slots:
    void on_pushButton_clicked();
    void on_upperValueChanged(int value);
    void on_lowerValueChanged(int value);

private:
    Ui::VideoCut *ui;
    std::string videoPath;
    int videoDuration;
    RangeSlider *rsH;
};

#endif // VIDEOCUT_H
