#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <visa.h>
#include "keysight33503A.h"
#include <QString>
#include <QThread>

/* Specify the default address */
//#define DEFAULT_LOGICAL_ADDRESS "GPIB0::10::INSTR"
#define DEFAULT_LOGICAL_ADDRESS "USB0::2391::11271::MY58001595::0::INSTR"

//
ViSession   viDefaultRM;
ViSession Instrument;
ViRsrc    TxtAddress = DEFAULT_LOGICAL_ADDRESS;
ViUInt32	actual = -1;
char SCPIcmd[10000];
char errorBuf[128];

#define	CHECK(func)	\
do {\
    ViStatus _s = (func);\
    if (_s < 0)\
    {\
	sprintf(errorBuf, "Error: %s returned %d\n", #func, _s);\
	emit errMsg(QString(errorBuf));\
	return;\
    }\
} while (0)


keysight33503A::keysight33503A(QObject *parent)
	:QThread(parent)
{
}

void keysight33503A::send(mode m)
{
	m_mode = m;
	start();
}

void keysight33503A::run()
{
	switch (m_mode)
	{
	case SMALLSINE:
		sendLowAmplitudeSine();
		break;
	case BIGSINE:
		sendHighAmplitudeSine();
		break;
	case NORMSINE:
		sendNormalAmplitudeSine();
		break;
	default:
		break;
	}
}

void keysight33503A::sendLowAmplitudeSine()
{
	//output a simple sin wave
	CHECK(viOpenDefaultRM(&viDefaultRM));
	CHECK(viOpen(viDefaultRM, TxtAddress, VI_NULL, 2500, &Instrument));
	strcpy(SCPIcmd, "*RST\n");  /* Reset the function generator */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	strcpy(SCPIcmd, "*CLS\n");  /* Clear errors and status registers */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	strcpy(SCPIcmd, "FUNCtion SINusoid\n");  /* Select waveshape */

	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	/* Other options are SQUare, RAMP, PULSe, NOISe, DC, and USER */
	strcpy(SCPIcmd, "OUTPut:LOAD 50\n"); /* Set the load impedance in Ohms (50 Ohms default) */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	/* May also be INFinity, as when using oscilloscope or DMM */
	strcpy(SCPIcmd, "VOLTage 0.05\n"); /*Set the amplitude in Vpp.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));

	strcpy(SCPIcmd, "FREQ 270000\n");  /*Set the freq in Hz.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));

	strcpy(SCPIcmd, "OUTPut ON\n");  /* Turn on the instrument output */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	//
	QThread::msleep(10000);

	strcpy(SCPIcmd, "OUTPut OFF\n");  /* Turn off the instrument output */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	CHECK(viClose(Instrument));
	CHECK(viClose(viDefaultRM));
}

void  keysight33503A::sendHighAmplitudeSine()
{
	//output a simple sin wave
	CHECK(viOpenDefaultRM(&viDefaultRM));
	CHECK(viOpen(viDefaultRM, TxtAddress, VI_NULL, 2500, &Instrument));
	strcpy(SCPIcmd, "*RST\n");  /* Reset the function generator */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	strcpy(SCPIcmd, "*CLS\n");  /* Clear errors and status registers */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	strcpy(SCPIcmd, "FUNCtion SINusoid\n");  /* Select waveshape */

	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	/* Other options are SQUare, RAMP, PULSe, NOISe, DC, and USER */
	strcpy(SCPIcmd, "OUTPut:LOAD 50\n"); /* Set the load impedance in Ohms (50 Ohms default) */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	/* May also be INFinity, as when using oscilloscope or DMM */
	strcpy(SCPIcmd, "VOLTage 0.02\n"); /*Set the amplitude in Vpp.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));

	strcpy(SCPIcmd, "FREQ 270000\n");  /*Set the freq in Hz.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));

	strcpy(SCPIcmd, "OUTPut ON\n");  /* Turn on the instrument output */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	//
	QThread::msleep(2000);
	strcpy(SCPIcmd, "VOLTage 0.12\n"); /*Set the amplitude in Vpp.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	QThread::msleep(3000);

	strcpy(SCPIcmd, "OUTPut OFF\n");  /* Turn off the instrument output */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	CHECK(viClose(Instrument));
	CHECK(viClose(viDefaultRM));
}

void  keysight33503A::sendNormalAmplitudeSine()
{
	//output a simple sin wave
	CHECK(viOpenDefaultRM(&viDefaultRM));
	CHECK(viOpen(viDefaultRM, TxtAddress, VI_NULL, 2500, &Instrument));
	strcpy(SCPIcmd, "*RST\n");  /* Reset the function generator */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	strcpy(SCPIcmd, "*CLS\n");  /* Clear errors and status registers */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	strcpy(SCPIcmd, "FUNCtion SINusoid\n");  /* Select waveshape */

	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	/* Other options are SQUare, RAMP, PULSe, NOISe, DC, and USER */
	strcpy(SCPIcmd, "OUTPut:LOAD 50\n"); /* Set the load impedance in Ohms (50 Ohms default) */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	/* May also be INFinity, as when using oscilloscope or DMM */
	strcpy(SCPIcmd, "VOLTage 0.05\n"); /*Set the amplitude in Vpp.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));

	strcpy(SCPIcmd, "FREQ 270000\n");  /*Set the freq in Hz.  Also see VOLTage:UNIT */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));

	strcpy(SCPIcmd, "OUTPut ON\n");  /* Turn on the instrument output */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	//
	QThread::msleep(4000);

	strcpy(SCPIcmd, "OUTPut OFF\n");  /* Turn off the instrument output */
	CHECK(viWrite(Instrument, (ViConstBuf)SCPIcmd, (ViUInt32)strlen(SCPIcmd), &actual));
	CHECK(viClose(Instrument));
	CHECK(viClose(viDefaultRM));
}

