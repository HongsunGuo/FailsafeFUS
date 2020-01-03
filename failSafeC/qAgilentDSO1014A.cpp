#include "qAgilentDSO1014A.h"
#include <memory>

#define VISA_ADDRESS "USB0::0x0957::0x0588::CN51215823::0::INSTR"
#define IEEEBLOCK_SPACE 5000000
#define QuitChk(functionCall) functionCall; if( m_quit ) goto QUIT_NOW; else

qAgilentDSO1014A::qAgilentDSO1014A(tOScopeData *pOScopeData, QObject *parent)
	: QThread(parent)
{
	m_pOScopeData = pOScopeData;
	ieeeblock_data = new unsigned char[IEEEBLOCK_SPACE];
}

qAgilentDSO1014A::~qAgilentDSO1014A()
{
	delete[]ieeeblock_data;
}

void qAgilentDSO1014A::acquireDataOnTrigger()
{
	const QMutexLocker locker(&m_mutex);
	//
	if (!isRunning())
		start();
	else
		m_cond.wakeOne();
}

void qAgilentDSO1014A::run()
{
	err = viOpenDefaultRM(&defaultRM);
	if (err != VI_SUCCESS) error_handler();

	/* Open the session using the oscilloscope's VISA address. */
	err = viOpen(defaultRM, VISA_ADDRESS, VI_NULL, VI_NULL, &vi);
	if (err != VI_SUCCESS) error_handler();
	/* Set the I/O timeout*/
	err = viSetAttribute(vi, VI_ATTR_TMO_VALUE, 2000);
	if (err != VI_SUCCESS) error_handler();

	initialize();
//	analyze();
	bool isTimeout = false;

	emit threadStarted();

	while (!m_quit)
	{
		m_mutex.lock();
		if(m_cond.wait(&m_mutex, 1000)) //release m_mutex and waiting for the wake of the condition...
			analyze();
		m_mutex.unlock();
	}
}

void qAgilentDSO1014A::initialize(void)
{
	/* Clear the interface. */
	err = viClear(vi);
	if (err != VI_SUCCESS) error_handler();
	/* Get and display the device's *IDN? string. */
	do_query_string("*IDN?");
	emit response("Oscilloscope *IDN? string: " + QString(str_result));
	/* Clear status and load the default setup. */
	do_command("*CLS");
	do_command("*RST");

	/* Set probe attenuation factor (0.001X to 1000X). */
	do_command(":CHANnel1:PROBe 1X");
	do_query_string(":CHANnel1:PROBe?");
	response_handler("Channel 1 probe attenuation factor: ");
	/* Use auto-scale to automatically configure oscilloscope. */
	do_command(":AUToscale");
	do_command(":KEY:MNUOFF");

	/* Change settings with individual commands:
	/* Set vertical scale and offset. */
	do_command(":CHANnel1:SCALe 2");
	do_query_string(":CHANnel1:SCALe?");
	response_handler("Channel 1 vertical scale: ");

	do_command(":CHANnel1:OFFSet 0");
	do_query_string(":CHANnel1:OFFSet?");
	response_handler("Channel 1 offset: ");

	/* Set horizontal scale and offset. */
	do_command(":TIMebase:MAIN:SCALe 0.1"); //in sec; 100 us = 0.1 ms = 0.0001 s
	do_query_string(":TIMebase:MAIN:SCALe?");
	response_handler("Timebase main scale: ");
	do_command(":TIMebase:MAIN:OFFSet 0.0000");
	do_query_string(":TIMebase:MAIN:OFFSet?");

	/* Set trigger mode (EDGE, PULSe, PATTern, VIDEO, or
	* ALTernation) and input source. */
	do_command(":TRIGger:MODE EDGE");
	do_query_string(":TRIGger:MODE?");
	response_handler("Trigger mode: ");

	/* Set EDGE trigger parameters. */
	//do_command(":TRIGger:EDGE:SOURCe CHANnel1");
	do_command(":TRIGger:EDGE:SOURCe CHANnel2");
	do_query_string(":TRIGger:EDGE:SOURce?");
	response_handler("Trigger edge source: ");

	do_command(":TRIGger:EDGE:LEVel 2");
	do_query_string(":TRIGger:EDGE:LEVel?");
	response_handler("Trigger edge level: ");

	do_command(":TRIGger:EDGE:SLOPe POSitive");
	do_query_string(":TRIGger:EDGE:SLOPe?");
	response_handler("Trigger edge slope: ");
	//added by Hongsun
	do_command(":TRIGger:EDGE:SWEep SINGle");
	do_query_string(":TRIGger:EDGE:SWEep?");
	response_handler("Trigger SWEep mode: ");

	///* Set the acquisition type (NORMal, PEAK, or AVERage). */
	do_command(":ACQuire:TYPE NORMal");
	do_query_string(":ACQuire:TYPE?");
	response_handler("Acquire type: ");
	//
	/* Set the waveform points mode. */
	do_command(":WAVeform:POINts:MODE RAW");
	do_query_string(":WAVeform:POINts:MODE?");

	/* Set the desired number of waveform points. */
	do_command(":WAVeform:POINts 10240");
	do_query_string(":WAVeform:POINts?");

	/* Set the waveform source. */
	do_command(":WAVeform:SOURce CHANnel1");
	do_query_string(":WAVeform:SOURce?");

	/* Choose the format of the data returned (WORD, BYTE, ASCII): */
	do_command(":WAVeform:FORMat BYTE");
	do_query_string(":WAVeform:FORMat?");
}


void qAgilentDSO1014A::sleep(clock_t wait)
{
	clock_t goal;
	goal = wait + clock();
	while (goal > clock())
		;
}

void qAgilentDSO1014A::do_query_string(const char * query)
{
	if (m_quit)
		return;

	char message[80];
	strcpy(message, query);
	strcat(message, "\n");
	err = viPrintf(vi, message);
	if (err != VI_SUCCESS) error_handler();
	err = viScanf(vi, "%t", str_result);
	if (err != VI_SUCCESS) error_handler();
	wait_operation_complete();
	check_instrument_errors();
}

void qAgilentDSO1014A::do_command(const char * command)
{
	if (m_quit)
		return;
	char message[80];
	strcpy(message, command);
	strcat(message, "\n");
	err = viPrintf(vi, message);
	if (err != VI_SUCCESS) error_handler();
	wait_operation_complete();
	check_instrument_errors();
}

void qAgilentDSO1014A::wait_operation_complete()
{
	char str_opc_result[256] = "0";
	while (strncmp(str_opc_result, "1", 1) != 0)
	{
		/* Small wait to prevent excessive queries. */
		sleep((clock_t)CLOCKS_PER_SEC / 10);
		err = viQueryf(vi, "*OPC?\n", "%t", str_opc_result);
		if (err != VI_SUCCESS) error_handler();
	}
}

void qAgilentDSO1014A::wait_read_waveform()
{
	char str_opc_result[256] = "0";
	while (strncmp(str_opc_result, "1", 1) != 0)
	{
		/* Small wait to prevent excessive queries. */
		sleep((clock_t)CLOCKS_PER_SEC / 10);
		err = viQueryf(vi, "*OPC?\n", "%t", str_opc_result);
		//if (err != VI_SUCCESS) error_handler();  comment by Hongsun. Don't do error_handler()
	}
}

void qAgilentDSO1014A::check_instrument_errors()
{
	char str_err_val[256] = { 0 };
	char str_out[800] = "";
	err = viQueryf(vi, ":SYSTem:ERRor?\n", "%t", str_err_val);
	if (err != VI_SUCCESS) error_handler();
	while (strncmp(str_err_val, "0,No error", 2) != 0)
	{
		strcat(str_out, ", ");
		strcat(str_out, str_err_val);
		err = viQueryf(vi, ":SYSTem:ERRor?\n", "%t", str_err_val);
		if (err != VI_SUCCESS) error_handler();
	}
	if (strcmp(str_out, "") != 0)
	{
		printf("INST Error%s\n", str_out);
		err = viFlush(vi, VI_READ_BUF);
		if (err != VI_SUCCESS) error_handler();
		err = viFlush(vi, VI_WRITE_BUF);
		if (err != VI_SUCCESS) error_handler();
	}
}

int qAgilentDSO1014A::do_command_ieeeblock(const char * command, int num_bytes)
{
	if (m_quit)
		return -1;
	char message[80];
	int data_length;
	strcpy(message, command);
	strcat(message, " #8%08d");
	err = viPrintf(vi, message, num_bytes);
	if (err != VI_SUCCESS) error_handler();
	err = viBufWrite(vi, (ViConstBuf)ieeeblock_data, num_bytes, (ViPUInt32)&data_length);
	if (err != VI_SUCCESS) error_handler();
	wait_operation_complete();
	check_instrument_errors();
	return(data_length);
}

int qAgilentDSO1014A::do_query_ieeeblock(const char * query)
{
	if (m_quit)
		return -1;

	char message[80];
	int data_length;
	strcpy(message, query);
	strcat(message, "\n");
	err = viPrintf(vi, message);
	if (err != VI_SUCCESS)
	{
		return -1;
	}

	/* Delay before reading data. */
	sleep((clock_t)CLOCKS_PER_SEC * 2);
	data_length = IEEEBLOCK_SPACE;
	err = viScanf(vi, "%#b\n", &data_length, ieeeblock_data);
	if (err != VI_SUCCESS)
	{
		return -1;
		//error_handler();
	}
	if (data_length == IEEEBLOCK_SPACE)
	{
		emit response("IEEE block buffer full: May not have received all data.");
	}

	wait_operation_complete();
	//check_instrument_errors();
	return(data_length);
}

void qAgilentDSO1014A::do_query_numbers(const char * query)
{
	if (m_quit)
		return;

	char message[80];
	strcpy(message, query);
	strcat(message, "\n");
	err = viPrintf(vi, message);
	if (err != VI_SUCCESS) error_handler();
	/* Small delay before read. */
	sleep((clock_t)CLOCKS_PER_SEC / 10);
	err = viScanf(vi, "%,10lf\n", dbl_results);
	if (err != VI_SUCCESS) error_handler();
	wait_operation_complete();
	check_instrument_errors();
}

void qAgilentDSO1014A::do_query_number(const char * query)
{
	if (m_quit)
		return;

	char message[80];
	strcpy(message, query);
	strcat(message, "\n");
	err = viPrintf(vi, message);
	if (err != VI_SUCCESS) error_handler();
	err = viScanf(vi, "%lf", &num_result);
	if (err != VI_SUCCESS) error_handler();
	wait_operation_complete();
	check_instrument_errors();
}

void qAgilentDSO1014A::analyze(void)
{
	double wav_format;
	double acq_type;
	double wav_points;
	double avg_count;
	double x_increment;
	double x_origin;
	double x_reference;
	double y_increment;
	double y_origin;
	double y_reference;

	int num_bytes; /* Number of bytes returned from instrument. */
	int i;
	clock_t readWaveTimeout = 10000;
	/* Download waveform data.


	/* Display the waveform settings: */
	do_query_numbers(":WAVeform:PREamble?");
	wav_format = dbl_results[0]; // BYTE 0, WORD 1, ASCii 2
	acq_type = dbl_results[1]; // NORMAL 0, PEAK 1, AVERAGE 2
	wav_points = dbl_results[2]; //Waveform points desired
	avg_count = dbl_results[3]; //Waveform average count
	x_increment = dbl_results[4]; //Waveform X increment
	x_origin = dbl_results[5]; //Waveform X origin
	x_reference = dbl_results[6]; //Waveform X reference
	y_increment = dbl_results[7]; //Waveform Y increment
	y_origin = dbl_results[8]; //Waveform Y origin
	y_reference = dbl_results[9]; //Waveform Y reference
	emit response("Get all the dbl_results");
	/* Read waveform data. */
	clock_t goal = readWaveTimeout + clock();
	//do_query_ieeeblock(":WAVeform:DATA?"); //this one will be timeout. Delay at least 2 sec
	emit readyForTrigger();
	while (clock() < goal)
	{
		if (m_quit)
			return;
		num_bytes = do_query_ieeeblock(":WAVeform:DATA?");
		if (num_bytes > 0 && num_bytes != IEEEBLOCK_SPACE)
		{
			//save waveform data
			m_pOScopeData->mutex.lock();
			m_pOScopeData->size = num_bytes;
			for (i = 0; i < num_bytes - 1; i++)
			{
				m_pOScopeData->xData[i] = x_origin + ((double)i * x_increment);
				m_pOScopeData->yData[i] = ((y_reference - (double)ieeeblock_data[i]) * y_increment)
					- y_origin;
			}
			m_pOScopeData->mutex.unlock();
			emit response(QString::number(num_bytes) + " points acquired!");
			emit DSO1014DataAcquired();
			break;
		}
	}
}

void qAgilentDSO1014A::saveDataToCSV()
{
	FILE *fp;
	/* Open file for output. */
	fp = fopen(".\\waveform_data.csv", "wb");
	/* Output waveform data in CSV format. */
	m_pOScopeData->mutex.lock();
	int num_bytes = m_pOScopeData->size;
	for (int i = 0; i < num_bytes - 1; i++)
	{
		/* Write time value, voltage value. */
		fprintf(fp, "%9f, %6f\n", m_pOScopeData->xData[i], m_pOScopeData->yData[i]);
	}
	m_pOScopeData->mutex.unlock();
	//emit response("Waveform format BYTE data written to .\\waveform_data.csv");
	/* Close output file. */
	fclose(fp);
}

void qAgilentDSO1014A::response_handler(const char * rsp)
{
	if (m_quit)
		return;
	emit response(QString(rsp) + QString(str_result));
}

void qAgilentDSO1014A::error_handler()
{
	char err_msg[1024] = { 0 };
	viStatusDesc(vi, err, err_msg);
	if (err < VI_SUCCESS)
		emit errorMsg(QString(err_msg));
}
