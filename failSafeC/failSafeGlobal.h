#pragma once
#include <QMutex>

#define PLOT_BUF_SIZE 80000
#define OSCOPE_BUFF 20000
#define PASSWORD "unm#1"
#define SAMPLE_RATE 1000000.0
#define V_MAX 5.0
#define NI_ATTEN_FACTOR 27.7 //attentuation to NI acq card

typedef struct _tOScopeData {
	double *xData;
	double *yData;
	int size;
	QMutex mutex;
}tOScopeData;

extern const QString FIELD_DATE_TIME;
extern const QString FIELD_XPMTR_iD;
extern const QString FIELD_SUBJECT_iD;
extern const QString FIELD_ZMAG;
extern const QString FIELD_PHASE;
extern const QString FIELD_CHECK_V;

extern const QString JSON_DB_FILE_NAME;
extern const QString JSON_DATE;
extern const QString JSON_TIME;
extern const QString JSON_XPMTR_iD;
extern const QString JSON_SUBJECT_iD;
extern const QString JSON_ABNORMAL_V;
extern const QString JSON_ABNORMAL_ISPTA;