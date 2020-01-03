#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <visa.h>
#include "failSafeGlobal.h" 

class qAgilentDSO1014A : public QThread
{
	Q_OBJECT

public:
	qAgilentDSO1014A(tOScopeData *pOScopeData, QObject *parent = Q_NULLPTR);
	~qAgilentDSO1014A();
	void acquireDataOnTrigger();
	void quitNow()
	{
		m_quit = true;
	}
	void saveDataToCSV();
private:
	void run() override;
	bool m_quit = false;
	QMutex m_mutex;
	QWaitCondition m_cond;
	tOScopeData *m_pOScopeData;
	//
	ViSession defaultRM; /* Device session ID. */
	ViSession vi;
	ViStatus err; /* VISA function return value. */
	char str_result[256] = { 0 }; /* Result from do_query_string(). */
	double num_result; /* Result from do_query_number(). */
	unsigned char *ieeeblock_data; /* Result from do_query_ieeeblock(). */
	//unsigned short ieeeblock_data[IEEEBLOCK_SPACE]; /* Result from do_query_ieeeblock(). */
	double dbl_results[10]; /* Result from do_query_numbers(). */
	//
	void response_handler(const char *rsp);
	//
	void error_handler();
	void initialize(void);
	void sleep(clock_t wait);
	void do_query_string(const char *query);
	void do_command(const char *command);
	void wait_operation_complete();
	void wait_read_waveform();
	void check_instrument_errors();
	int do_command_ieeeblock(const char *command, int num_bytes);
	int do_query_ieeeblock(const char *query);
	void do_query_numbers(const char *query);
	void do_query_number(const char *query);
	void analyze(void);
signals:
	void threadStarted();
	void DSO1014DataAcquired();
	void newDataAcquired();
	void readyForTrigger();
	void response(const QString &s);
	void errorMsg(const QString &s);
	void timeoutMsg(const QString &s);

};
