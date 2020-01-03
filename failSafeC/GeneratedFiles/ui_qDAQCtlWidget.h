/********************************************************************************
** Form generated from reading UI file 'qDAQCtlWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDAQCTLWIDGET_H
#define UI_QDAQCTLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qDAQCtlWidget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *isilineEdit;
    QPushButton *setISIBtn;
    QLabel *label_2;
    QDoubleSpinBox *yLimitSpinBox;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPushButton *quitBtn;
    QLCDNumber *lcdNumber;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *qDAQCtlWidget)
    {
        if (qDAQCtlWidget->objectName().isEmpty())
            qDAQCtlWidget->setObjectName(QString::fromUtf8("qDAQCtlWidget"));
        qDAQCtlWidget->resize(153, 696);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qDAQCtlWidget->sizePolicy().hasHeightForWidth());
        qDAQCtlWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(qDAQCtlWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(qDAQCtlWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        isilineEdit = new QLineEdit(qDAQCtlWidget);
        isilineEdit->setObjectName(QString::fromUtf8("isilineEdit"));
        isilineEdit->setMouseTracking(false);

        verticalLayout->addWidget(isilineEdit);

        setISIBtn = new QPushButton(qDAQCtlWidget);
        setISIBtn->setObjectName(QString::fromUtf8("setISIBtn"));

        verticalLayout->addWidget(setISIBtn);

        label_2 = new QLabel(qDAQCtlWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        yLimitSpinBox = new QDoubleSpinBox(qDAQCtlWidget);
        yLimitSpinBox->setObjectName(QString::fromUtf8("yLimitSpinBox"));
        yLimitSpinBox->setMinimum(0.010000000000000);
        yLimitSpinBox->setMaximum(10.000000000000000);
        yLimitSpinBox->setSingleStep(0.500000000000000);
        yLimitSpinBox->setValue(3.000000000000000);

        verticalLayout->addWidget(yLimitSpinBox);

        startBtn = new QPushButton(qDAQCtlWidget);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setFlat(false);

        verticalLayout->addWidget(startBtn);

        stopBtn = new QPushButton(qDAQCtlWidget);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));

        verticalLayout->addWidget(stopBtn);

        quitBtn = new QPushButton(qDAQCtlWidget);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));

        verticalLayout->addWidget(quitBtn);

        lcdNumber = new QLCDNumber(qDAQCtlWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setFrameShape(QFrame::Box);
        lcdNumber->setSmallDecimalPoint(false);
        lcdNumber->setProperty("value", QVariant(20.222000000000001));

        verticalLayout->addWidget(lcdNumber);

        verticalSpacer = new QSpacerItem(20, 700, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(qDAQCtlWidget);
        QObject::connect(startBtn, SIGNAL(clicked()), qDAQCtlWidget, SLOT(onStartDAQ()));
        QObject::connect(stopBtn, SIGNAL(clicked()), qDAQCtlWidget, SLOT(onStopDAQ()));
        QObject::connect(yLimitSpinBox, SIGNAL(valueChanged(double)), qDAQCtlWidget, SLOT(onChangeYLimit(double)));

        QMetaObject::connectSlotsByName(qDAQCtlWidget);
    } // setupUi

    void retranslateUi(QWidget *qDAQCtlWidget)
    {
        qDAQCtlWidget->setWindowTitle(QCoreApplication::translate("qDAQCtlWidget", "qDAQCtlWidget", nullptr));
        label->setText(QCoreApplication::translate("qDAQCtlWidget", "Interstimulus interval (s):", nullptr));
        setISIBtn->setText(QCoreApplication::translate("qDAQCtlWidget", "Set ISI", nullptr));
        label_2->setText(QCoreApplication::translate("qDAQCtlWidget", "YLimit (V)", nullptr));
        startBtn->setText(QCoreApplication::translate("qDAQCtlWidget", "Start", nullptr));
        stopBtn->setText(QCoreApplication::translate("qDAQCtlWidget", "Stop", nullptr));
        quitBtn->setText(QCoreApplication::translate("qDAQCtlWidget", "Quit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qDAQCtlWidget: public Ui_qDAQCtlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDAQCTLWIDGET_H
