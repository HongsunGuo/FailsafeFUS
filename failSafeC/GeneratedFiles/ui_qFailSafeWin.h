/********************************************************************************
** Form generated from reading UI file 'qFailSafeWin.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QFAILSAFEWIN_H
#define UI_QFAILSAFEWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qFailSafeWin
{
public:
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionQuit;
    QAction *actionCalibration_curve;
    QAction *actionInterStimulus_interval;
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *qFailSafeWin)
    {
        if (qFailSafeWin->objectName().isEmpty())
            qFailSafeWin->setObjectName(QString::fromUtf8("qFailSafeWin"));
        qFailSafeWin->resize(979, 753);
        actionStart = new QAction(qFailSafeWin);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        actionStart->setMenuRole(QAction::TextHeuristicRole);
        actionStart->setShortcutVisibleInContextMenu(false);
        actionStop = new QAction(qFailSafeWin);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        actionQuit = new QAction(qFailSafeWin);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCalibration_curve = new QAction(qFailSafeWin);
        actionCalibration_curve->setObjectName(QString::fromUtf8("actionCalibration_curve"));
        actionInterStimulus_interval = new QAction(qFailSafeWin);
        actionInterStimulus_interval->setObjectName(QString::fromUtf8("actionInterStimulus_interval"));
        centralWidget = new QWidget(qFailSafeWin);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        qFailSafeWin->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(qFailSafeWin);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setIconSize(QSize(48, 48));
        qFailSafeWin->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(qFailSafeWin);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        qFailSafeWin->setStatusBar(statusBar);

        retranslateUi(qFailSafeWin);

        QMetaObject::connectSlotsByName(qFailSafeWin);
    } // setupUi

    void retranslateUi(QMainWindow *qFailSafeWin)
    {
        qFailSafeWin->setWindowTitle(QCoreApplication::translate("qFailSafeWin", "qFailSafeWin", nullptr));
        actionStart->setText(QCoreApplication::translate("qFailSafeWin", "Start", nullptr));
        actionStop->setText(QCoreApplication::translate("qFailSafeWin", "Stop", nullptr));
        actionQuit->setText(QCoreApplication::translate("qFailSafeWin", "Quit", nullptr));
        actionCalibration_curve->setText(QCoreApplication::translate("qFailSafeWin", "Calibration curve", nullptr));
        actionInterStimulus_interval->setText(QCoreApplication::translate("qFailSafeWin", "InterStimulus interval", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qFailSafeWin: public Ui_qFailSafeWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QFAILSAFEWIN_H
