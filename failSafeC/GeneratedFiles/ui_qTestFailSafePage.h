/********************************************************************************
** Form generated from reading UI file 'qTestFailSafePage.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTESTFAILSAFEPAGE_H
#define UI_QTESTFAILSAFEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_qTestFailSafePage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *bigSpikeBtn;
    QPushButton *highVBtn;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWizardPage *qTestFailSafePage)
    {
        if (qTestFailSafePage->objectName().isEmpty())
            qTestFailSafePage->setObjectName(QString::fromUtf8("qTestFailSafePage"));
        qTestFailSafePage->resize(468, 425);
        verticalLayout = new QVBoxLayout(qTestFailSafePage);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(qTestFailSafePage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        label = new QLabel(qTestFailSafePage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        label_3 = new QLabel(qTestFailSafePage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(qTestFailSafePage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        bigSpikeBtn = new QPushButton(qTestFailSafePage);
        bigSpikeBtn->setObjectName(QString::fromUtf8("bigSpikeBtn"));

        horizontalLayout->addWidget(bigSpikeBtn);

        highVBtn = new QPushButton(qTestFailSafePage);
        highVBtn->setObjectName(QString::fromUtf8("highVBtn"));

        horizontalLayout->addWidget(highVBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 231, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(qTestFailSafePage);

        QMetaObject::connectSlotsByName(qTestFailSafePage);
    } // setupUi

    void retranslateUi(QWizardPage *qTestFailSafePage)
    {
        qTestFailSafePage->setWindowTitle(QCoreApplication::translate("qTestFailSafePage", "qTestFailSafePage", nullptr));
        label_2->setText(QCoreApplication::translate("qTestFailSafePage", "1) Before using the transducer on human subjects, please perform two following sanity tests of the transducer in water", nullptr));
        label->setText(QCoreApplication::translate("qTestFailSafePage", "2) Select and run sanityTestBigSpike.ARB, which simulates the presence of big artifact beyond the safety limit. ", nullptr));
        label_3->setText(QCoreApplication::translate("qTestFailSafePage", "3) Select and run sanityTestAbnormalVoltage.ARB, which simulates the abrupt elevation of the output voltage from the amplifier, going beyond the pre-calibrated limit", nullptr));
        label_4->setText(QCoreApplication::translate("qTestFailSafePage", "4) The failsafe should work for both cases.", nullptr));
        bigSpikeBtn->setText(QCoreApplication::translate("qTestFailSafePage", "Test protection from big spike", nullptr));
        highVBtn->setText(QCoreApplication::translate("qTestFailSafePage", "Test protection from slow increasing voltage", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qTestFailSafePage: public Ui_qTestFailSafePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTESTFAILSAFEPAGE_H
