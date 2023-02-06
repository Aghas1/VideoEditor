#ifndef VIDEOCUT_H
#define VIDEOCUT_H

#include <QWidget>
#include <QDebug>
#include <thread>

namespace Ui {
class VideoCut;
}

class VideoCut : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCut(std::string currentVideoPath, int videoLength[], QWidget *parent = nullptr);
    ~VideoCut();

private slots:
    void on_pushButton_clicked();

private:
    Ui::VideoCut *ui;
    std::string videoPath;
};

#endif // VIDEOCUT_H
