/********************************************************************************
** Form generated from reading UI file 'qSanityChkDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSANITYCHKDLG_H
#define UI_QSANITYCHKDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_qSanityChkDlg
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *qSanityChkDlg)
    {
        if (qSanityChkDlg->objectName().isEmpty())
            qSanityChkDlg->setObjectName(QString::fromUtf8("qSanityChkDlg"));
        qSanityChkDlg->resize(459, 518);
        verticalLayout = new QVBoxLayout(qSanityChkDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        verticalLayout->addLayout(verticalLayout_2);

        verticalSpacer = new QSpacerItem(20, 458, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(378, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(qSanityChkDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(qSanityChkDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), qSanityChkDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), qSanityChkDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(qSanityChkDlg);
    } // setupUi

    void retranslateUi(QDialog *qSanityChkDlg)
    {
        qSanityChkDlg->setWindowTitle(QCoreApplication::translate("qSanityChkDlg", "qSanityChkDlg", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qSanityChkDlg: public Ui_qSanityChkDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSANITYCHKDLG_H
