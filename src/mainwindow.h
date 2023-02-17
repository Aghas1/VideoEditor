#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <string>
#include <thread>

#include <QMainWindow>
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <rangeslider.h>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int accessToRedactor, QWidget *parent = nullptr);
    ~MainWindow();
    void setConnects();
    void setTimer();
    void debug(std::string message);

signals:
    void videoChanged();

private slots:
    void setVideo();
    void on_Play_clicked();
    void on_Stop_clicked();
    void on_Sound_clicked();
    void on_Slider_moved(int pos);
    void on_Open_action();
    void on_Save_action();
    void on_Cut_action();
    void cut_active(bool state);
    void on_lowerValueChanged(int value);
    void on_upperValueChanged(int value);
    void on_valueChanged(int value);
    void on_boxVideoChanged(int state);
    void on_boxAudioChanged(int state);


private:
    QProcess* proc;
    Ui::MainWindow *ui;
    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* videoViewer;
    QTimer* timer;
    int accesRedactorMode;

    std::string currentVideoPath;
    std::string s_videoLength;
    int l_videoLength[3];
    int i_videoLength;

    bool in_Cut_State = false;
    bool isPlaying = false;
    RangeSlider* rsH;
};
#endif // MAINWINDOW_H
