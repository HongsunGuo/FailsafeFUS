/********************************************************************************
** Form generated from reading UI file 'qCheckUSimpedancePage.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCHECKUSIMPEDANCEPAGE_H
#define UI_QCHECKUSIMPEDANCEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qCheckUSimpedancePage
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QListWidget *listWidget;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLineEdit *zmagEdit;
    QLabel *label_6;
    QLineEdit *phaseEdit;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *qCheckUSimpedancePage)
    {
        if (qCheckUSimpedancePage->objectName().isEmpty())
            qCheckUSimpedancePage->setObjectName(QString::fromUtf8("qCheckUSimpedancePage"));
        qCheckUSimpedancePage->resize(552, 421);
        gridLayout = new QGridLayout(qCheckUSimpedancePage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(qCheckUSimpedancePage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(qCheckUSimpedancePage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setScaledContents(false);
        label_3->setWordWrap(true);

        verticalLayout->addWidget(label_3);

        label = new QLabel(qCheckUSimpedancePage);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        listWidget = new QListWidget(qCheckUSimpedancePage);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        label_4 = new QLabel(qCheckUSimpedancePage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_5 = new QLabel(qCheckUSimpedancePage);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        zmagEdit = new QLineEdit(qCheckUSimpedancePage);
        zmagEdit->setObjectName(QString::fromUtf8("zmagEdit"));

        horizontalLayout->addWidget(zmagEdit);

        label_6 = new QLabel(qCheckUSimpedancePage);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        phaseEdit = new QLineEdit(qCheckUSimpedancePage);
        phaseEdit->setObjectName(QString::fromUtf8("phaseEdit"));

        horizontalLayout->addWidget(phaseEdit);

        horizontalSpacer = new QSpacerItem(78, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(qCheckUSimpedancePage);

        QMetaObject::connectSlotsByName(qCheckUSimpedancePage);
    } // setupUi

    void retranslateUi(QWidget *qCheckUSimpedancePage)
    {
        qCheckUSimpedancePage->setWindowTitle(QCoreApplication::translate("qCheckUSimpedancePage", "Form", nullptr));
        label_2->setText(QCoreApplication::translate("qCheckUSimpedancePage", "1) Measure the impedance of the transducer with a vector network analyzer.", nullptr));
        label_3->setText(QCoreApplication::translate("qCheckUSimpedancePage", "2) The impedance should be within a certain range (to be specified after initial calibration). Otherwise, the transducer may have been damaged, and the ongoing experiment should be cancelled.", nullptr));
        label->setText(QCoreApplication::translate("qCheckUSimpedancePage", "Historical transducer impedances", nullptr));
        label_4->setText(QCoreApplication::translate("qCheckUSimpedancePage", "Current impedance:", nullptr));
        label_5->setText(QCoreApplication::translate("qCheckUSimpedancePage", "Zmag (ohm)", nullptr));
        zmagEdit->setText(QString());
        label_6->setText(QCoreApplication::translate("qCheckUSimpedancePage", "Phase (degree)", nullptr));
        phaseEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class qCheckUSimpedancePage: public Ui_qCheckUSimpedancePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCHECKUSIMPEDANCEPAGE_H
