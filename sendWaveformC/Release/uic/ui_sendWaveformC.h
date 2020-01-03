/********************************************************************************
** Form generated from reading UI file 'sendWaveformC.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDWAVEFORMC_H
#define UI_SENDWAVEFORMC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_sendWaveformCClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QPushButton *testIsptaBtn;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *testIsppaBtn;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *checkVBtn;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *sendWaveformCClass)
    {
        if (sendWaveformCClass->objectName().isEmpty())
            sendWaveformCClass->setObjectName(QString::fromUtf8("sendWaveformCClass"));
        sendWaveformCClass->resize(522, 564);
        sendWaveformCClass->setMinimumSize(QSize(100, 100));
        verticalLayout_2 = new QVBoxLayout(sendWaveformCClass);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        testIsptaBtn = new QPushButton(sendWaveformCClass);
        testIsptaBtn->setObjectName(QString::fromUtf8("testIsptaBtn"));
        testIsptaBtn->setMinimumSize(QSize(200, 150));
        QFont font;
        font.setPointSize(16);
        testIsptaBtn->setFont(font);

        verticalLayout_2->addWidget(testIsptaBtn);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        testIsppaBtn = new QPushButton(sendWaveformCClass);
        testIsppaBtn->setObjectName(QString::fromUtf8("testIsppaBtn"));
        testIsppaBtn->setMinimumSize(QSize(200, 150));
        testIsppaBtn->setFont(font);

        horizontalLayout_6->addWidget(testIsppaBtn);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        checkVBtn = new QPushButton(sendWaveformCClass);
        checkVBtn->setObjectName(QString::fromUtf8("checkVBtn"));
        checkVBtn->setMinimumSize(QSize(200, 150));
        checkVBtn->setFont(font);

        horizontalLayout_3->addWidget(checkVBtn);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(248, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        okBtn = new QPushButton(sendWaveformCClass);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));

        horizontalLayout_2->addWidget(okBtn);

        cancelBtn = new QPushButton(sendWaveformCClass);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        horizontalLayout_2->addWidget(cancelBtn);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(sendWaveformCClass);
        QObject::connect(cancelBtn, SIGNAL(clicked()), sendWaveformCClass, SLOT(reject()));
        QObject::connect(okBtn, SIGNAL(clicked()), sendWaveformCClass, SLOT(accept()));

        QMetaObject::connectSlotsByName(sendWaveformCClass);
    } // setupUi

    void retranslateUi(QDialog *sendWaveformCClass)
    {
        sendWaveformCClass->setWindowTitle(QCoreApplication::translate("sendWaveformCClass", "sendWaveformC", nullptr));
        testIsptaBtn->setText(QCoreApplication::translate("sendWaveformCClass", "Sanity test of averaged voltage threshold (for Ispta)", nullptr));
        testIsppaBtn->setText(QCoreApplication::translate("sendWaveformCClass", "Sanity test of peak voltage threshold (for Isppa)", nullptr));
        checkVBtn->setText(QCoreApplication::translate("sendWaveformCClass", "Check current output voltage", nullptr));
        okBtn->setText(QCoreApplication::translate("sendWaveformCClass", "OK", nullptr));
        cancelBtn->setText(QCoreApplication::translate("sendWaveformCClass", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sendWaveformCClass: public Ui_sendWaveformCClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDWAVEFORMC_H
