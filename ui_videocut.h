/********************************************************************************
** Form generated from reading UI file 'videocut.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOCUT_H
#define UI_VIDEOCUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoCut
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QTimeEdit *timeEdit;
    QLabel *label_2;
    QTimeEdit *timeEdit_2;

    void setupUi(QWidget *VideoCut)
    {
        if (VideoCut->objectName().isEmpty())
            VideoCut->setObjectName(QString::fromUtf8("VideoCut"));
        VideoCut->resize(400, 300);
        pushButton = new QPushButton(VideoCut);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(70, 190, 80, 23));
        pushButton_2 = new QPushButton(VideoCut);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(220, 190, 80, 23));
        verticalLayoutWidget = new QWidget(VideoCut);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 371, 121));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        timeEdit = new QTimeEdit(verticalLayoutWidget);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));

        horizontalLayout->addWidget(timeEdit);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setLayoutDirection(Qt::LeftToRight);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        timeEdit_2 = new QTimeEdit(verticalLayoutWidget);
        timeEdit_2->setObjectName(QString::fromUtf8("timeEdit_2"));

        horizontalLayout->addWidget(timeEdit_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(VideoCut);

        QMetaObject::connectSlotsByName(VideoCut);
    } // setupUi

    void retranslateUi(QWidget *VideoCut)
    {
        VideoCut->setWindowTitle(QApplication::translate("VideoCut", "Form", nullptr));
        pushButton->setText(QApplication::translate("VideoCut", "Cut", nullptr));
        pushButton_2->setText(QApplication::translate("VideoCut", "Cancel", nullptr));
        label->setText(QApplication::translate("VideoCut", "Start", nullptr));
        label_2->setText(QApplication::translate("VideoCut", "End", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoCut: public Ui_VideoCut {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOCUT_H
