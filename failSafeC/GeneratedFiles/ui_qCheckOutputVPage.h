/********************************************************************************
** Form generated from reading UI file 'qCheckOutputVPage.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCHECKOUTPUTVPAGE_H
#define UI_QCHECKOUTPUTVPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_qCheckOutputVPage
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWizardPage *qCheckOutputVPage)
    {
        if (qCheckOutputVPage->objectName().isEmpty())
            qCheckOutputVPage->setObjectName(QString::fromUtf8("qCheckOutputVPage"));
        qCheckOutputVPage->resize(494, 411);
        gridLayout = new QGridLayout(qCheckOutputVPage);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(qCheckOutputVPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        label_2 = new QLabel(qCheckOutputVPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(qCheckOutputVPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(qCheckOutputVPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        listWidget = new QListWidget(qCheckOutputVPage);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_5 = new QLabel(qCheckOutputVPage);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        lineEdit = new QLineEdit(qCheckOutputVPage);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        horizontalSpacer = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(qCheckOutputVPage);

        QMetaObject::connectSlotsByName(qCheckOutputVPage);
    } // setupUi

    void retranslateUi(QWizardPage *qCheckOutputVPage)
    {
        qCheckOutputVPage->setWindowTitle(QCoreApplication::translate("qCheckOutputVPage", "qCheckOutputVPage", nullptr));
        label->setText(QCoreApplication::translate("qCheckOutputVPage", "1) Before using the transducer on human subjects, please test the transducer in a  beake filled with water.", nullptr));
        label_2->setText(QCoreApplication::translate("qCheckOutputVPage", "2) Measure the output voltage delivered to the transducer.", nullptr));
        label_3->setText(QCoreApplication::translate("qCheckOutputVPage", "3) The output voltage  should be approximately the same as that measured last time. Otherwise, please check if correct ARB file is used and the BNC connections are good.", nullptr));
        label_4->setText(QCoreApplication::translate("qCheckOutputVPage", "Historical output voltage of the amplifier (date  and amplitude)", nullptr));
        label_5->setText(QCoreApplication::translate("qCheckOutputVPage", "Current output voltage (V):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qCheckOutputVPage: public Ui_qCheckOutputVPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCHECKOUTPUTVPAGE_H
