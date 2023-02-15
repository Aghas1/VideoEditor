#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
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
#include <QHBoxLayout>

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
    void debug(std::string message);

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
    void cut_active(bool activ);
    void on_lowerValueChanged(int value);
    void on_upperValueChanged(int value);


private:
    Ui::MainWindow *ui;
    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* videoViewer;
    QTimer* timer;
    std::string s_videoLength;
    int l_videoLength[3];
    int i_videoLength;
    std::string currentVideoPath;
    RangeSlider* rsH;
    bool in_Cut_State = false;
};
#endif // MAINWINDOW_H
