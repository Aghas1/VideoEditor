#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QDebug>
#include <string>
#include <QTimer>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <videocut.h>
#include <QAbstractSlider>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setConnects();
    void setTimer();

signals:
    void videoChanged();

private slots:
    void setVideo();
    void on_Play_clicked();
    void on_Pause_clicked();
    void on_Stop_clicked();
    void on_Slider_moved(int pos);
    void on_Open_action();
    void on_Save_action();
    void on_Cut_action();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* vw;
    QTimer* timer;
    std::string s_videoLength;
    int l_videoLength[3];
    int i_videoLength;
    std::string currentVideoPath;
};
#endif // MAINWINDOW_H
