#pragma once

#include <QObject>
#include <QColor>
#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
class qOneLED : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(bool on READ getOn WRITE setOn)

public:
	qOneLED(QWidget *parent = 0)
		: QWidget(parent), m_on(false) {}

	bool getOn() const
	{
		return m_on;
	}
	void setOn(bool on)
	{
		if (on == m_on)
			return;
		else
			emit valueChanged();

		m_on = on;
		update();
	}

public slots:
	void turnOff() { setOn(false); }
	void turnOn() { setOn(true); }
signals:
	void valueChanged();
protected:
	void paintEvent(QPaintEvent *) override
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		if (!m_on)
			painter.setBrush(QColor(0, 100, 0)); //dark green
		else
			painter.setBrush(Qt::green); //bright green
		painter.drawEllipse(0, 0, 25, 25);
	}

private:
	bool m_on;
};


//#define LED_TEST_ISPPA 0
//#define LED_TEST_ISPTA 1
//#define LED_CHECK_V 2

class qNewLEDs :public QWidget
{
public:
	typedef enum _LED_ID {
		LED_ISPTA_READY = 0,
		LED_ISPTA_DONE = 1,
		LED_ISPPA_READY = 2,
		LED_ISPPA_DONE = 3,
		LED_CHECK_V_READY = 4,
		LED_CHECK_V_DONE = 5
	} LED_ID;


	QPushButton *p_isptaStartBtn;
	QPushButton *p_isptaStopBtn;
	//
	QPushButton *p_isppaStartBtn;
	QPushButton *p_isppaStopBtn;
	//
	QPushButton *p_checkVStartBtn;
	QPushButton *p_checkVStopBtn;
	//
	QPushButton *p_resetBtn;

	qNewLEDs(QWidget *parent = 0)
		:QWidget(parent)
	{
		p_isptaStartBtn = new QPushButton(QIcon(":/failSafeC/Resources/start.png"), "Start");
		p_isppaStartBtn = new QPushButton(QIcon(":/failSafeC/Resources/start.png"), "Start");
		p_checkVStartBtn = new QPushButton(QIcon(":/failSafeC/Resources/start.png"), "Start");
		//
		p_isptaStopBtn = new QPushButton(QIcon(":/failSafeC/Resources/stop.png"), "Stop");
		p_isppaStopBtn = new QPushButton(QIcon(":/failSafeC/Resources/stop.png"), "Stop");
		p_checkVStopBtn = new QPushButton(QIcon(":/failSafeC/Resources/stop.png"), "Stop");
		p_resetBtn = new QPushButton(QIcon(":/failSafeC/Resources/stop.png"), "Reset");
		for (int i=0; i<= LED_CHECK_V_DONE;i++)
			pLEDs.append(new qOneLED(this));

		QGridLayout  *gly = new QGridLayout(this);
		//
		gly->addWidget(new QLabel("", this), 0, 0);
		gly->addWidget(new QLabel("Ready", this), 0, 1);
		gly->addWidget(new QLabel("Success", this), 0, 2);
		gly->addWidget(p_resetBtn, 0, 3);
		//
		gly->addWidget(new QLabel("Sanity test of I_spta", this), 1, 0);
		gly->addWidget(new QLabel("Sanity test of I_sppa", this), 2, 0);
		gly->addWidget(new QLabel("Test the output voltage when the \n"
			"tip of the trasnducer\n"
			"is immersed in a water beake", this), 3, 0);
		//
		gly->addWidget(pLEDs[LED_ISPTA_READY], 1, 1);
		gly->addWidget(pLEDs[LED_ISPTA_DONE], 1, 2);
		gly->addWidget(p_isptaStartBtn, 1, 3);
		//gly->addWidget(p_isptaStopBtn, 1, 4);
		//
		gly->addWidget(pLEDs[LED_ISPPA_READY], 2, 1);
		gly->addWidget(pLEDs[LED_ISPPA_DONE], 2, 2);
		gly->addWidget(p_isppaStartBtn, 2, 3);
		//gly->addWidget(p_isppaStopBtn, 2, 4);
		//
		gly->addWidget(pLEDs[LED_CHECK_V_READY], 3, 1);
		gly->addWidget(pLEDs[LED_CHECK_V_DONE], 3, 2);
		gly->addWidget(p_checkVStartBtn, 3, 3);
		//gly->addWidget(p_checkVStopBtn, 3, 4);
		//
		for(int k=0; k<4; k++)
			gly->setRowMinimumHeight(k, 30);

		turnAll(false);
		setLayout(gly);
		//
		QPalette pal = palette();
		pal.setColor(QPalette::Background, Qt::white);
		setPalette(pal);
		setAutoFillBackground(true);
	}

	void turnAll(bool on)
	{
		for(int i=0; i<pLEDs.size(); i++)
			pLEDs[i]->setOn(on);
	}

	void turn(int id, bool on = false)
	{
		if (id<0 || id>LED_CHECK_V_DONE)
			return;
		pLEDs[id]->setOn(on);
	}

	bool getLEDStatus(int id)
	{
		if (id<0 || id>LED_CHECK_V_DONE)
			return false;

		return pLEDs[id]->getOn();
	}

private:
	QVector<qOneLED *> pLEDs;
};

class qLEDs: public QWidget
{
public:
	qLEDs(QWidget *parent = 0)
		: QWidget(parent)
	{
		QGridLayout  *gly= new QGridLayout(this);
		//
		p1_initLabel = new QLabel("1) Initialization all hardware", this);
		p2_genXWaveLabel = new QLabel("2) Start test", this);
		p1_breakCircuitLabel = new QLabel("3) Activate failsafe", this);
		p1_resetRealyLabel = new QLabel("4) Reset relay and clear", this);

		//
		p1_initLED = new qOneLED(this);
		p2_genXWaveLED = new qOneLED(this);
		p3_breakCircuitLED = new qOneLED(this);
		p4_resetRealyLED = new qOneLED(this);
		//
		gly->addWidget(p1_initLabel, 0, 0);
		gly->addWidget(p2_genXWaveLabel, 1, 0);
		gly->addWidget(p1_breakCircuitLabel, 2, 0);
		gly->addWidget(p1_resetRealyLabel, 3, 0);
		//
		gly->addWidget(p1_initLED, 0, 1);
		gly->addWidget(p2_genXWaveLED, 1, 1);
		gly->addWidget(p3_breakCircuitLED, 2, 1);
		gly->addWidget(p4_resetRealyLED, 3, 1);
		//
		gly->setRowMinimumHeight(0, 100);
		gly->setRowMinimumHeight(1, 100);
		gly->setRowMinimumHeight(2, 100);
		gly->setRowMinimumHeight(3, 100);
		//
		setLayout(gly);
		turnAll(false);
		//
		QPalette pal = palette();
		pal.setColor(QPalette::Background, Qt::white);
		setPalette(pal);
		setAutoFillBackground(true);
	}

	enum ID { INIT, GEN, BREAK, RESET};

	void turnAll(bool on)
	{
		p1_initLED->setOn(on);
		p2_genXWaveLED->setOn(on);
		p3_breakCircuitLED->setOn(on);
		p4_resetRealyLED->setOn(on);
	}

	void turn(qLEDs::ID id, bool on = false)
	{
		switch (id)
		{
		case qLEDs::INIT:
			p1_initLED->setOn(on);
			break;
		case qLEDs::GEN:
			p2_genXWaveLED->setOn(on);
			break;
		case qLEDs::BREAK:
			p3_breakCircuitLED->setOn(on);
			break;
		case qLEDs::RESET:
			p4_resetRealyLED->setOn(on);
			break;
		default:
			break;
		}
	}

private:
	qOneLED *p1_initLED;
	qOneLED *p2_genXWaveLED;
	qOneLED *p3_breakCircuitLED;
	qOneLED *p4_resetRealyLED;
	//
	QLabel *p1_initLabel;
	QLabel *p2_genXWaveLabel;
	QLabel *p1_breakCircuitLabel;
	QLabel *p1_resetRealyLabel;
};
