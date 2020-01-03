#include "qFailSafeWizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QFileInfo>

extern const QString FIELD_ZMAG = "zMag";
extern const QString FIELD_PHASE = "phase";
//
extern const QString JSON_DB_FILE_NAME = "./failsafedb.json";
extern const QString JSON_DB_RECORD = "records";
extern const QString JSON_DATE = "date";
extern const QString JSON_TIME = "time";
extern const QString JSON_XPMTR_ID = "experimenter";
extern const QString JSON_SUBJECT_ID = "subject ID";
extern const QString JSON_CHECK_V = "check V";
extern const QString JSON_ABNORMAL_V = "abnormal_V";
extern const QString JSON_ABNORMAL_ISPTA = "abnormal_ISPTA";
extern const QString JSON_ZMAG = "zMag";
extern const QString JSON_PHASE = "phase";

qRecord::qRecord()
{
}

qRecord::qRecord(QString _date, QString _time, QString _subjectID,
	QString _experimenter, double _checkV, bool _isAbnormalV,
	bool _isAbnormalIspta, double _zMag, double _phase) :
	date(_date), time(_time), subjectID(_subjectID), experimenter(_experimenter),
	checkV(_checkV), isAbnormalV(_isAbnormalV), isAbnormalIspta(_isAbnormalIspta),
	zMag(_zMag), phase(_phase)
{
}

void qRecord::write(QJsonObject &json) const
{
	json[JSON_DATE] = date;
	json[JSON_TIME] = time;
    json[JSON_SUBJECT_ID] = subjectID;
	json[JSON_XPMTR_ID] = experimenter;
	json[JSON_CHECK_V] = checkV;
	json[JSON_ABNORMAL_V] = isAbnormalV;
	json[JSON_ABNORMAL_ISPTA] = isAbnormalIspta;
	json[JSON_ZMAG] = zMag;
	json[JSON_PHASE] = phase;
}

void qRecord::read(const QJsonObject &json)
{
	if (json.contains(JSON_DATE) && json[JSON_DATE].isString())
        date = json[JSON_DATE].toString();

    if (json.contains(JSON_TIME) && json[JSON_TIME].isString())
        time = json[JSON_TIME].toString();

	if (json.contains(JSON_SUBJECT_ID) && json[JSON_SUBJECT_ID].isString())
        subjectID = json[JSON_SUBJECT_ID].toString();

	if (json.contains(JSON_XPMTR_ID) && json[JSON_XPMTR_ID].isString())
        experimenter = json[JSON_XPMTR_ID].toString();

	if (json.contains(JSON_CHECK_V) && json[JSON_CHECK_V].isDouble())
        checkV = json[JSON_CHECK_V].toDouble();

	if (json.contains(JSON_ABNORMAL_V) && json[JSON_ABNORMAL_V].isBool())
        isAbnormalV = json[JSON_ABNORMAL_V].toBool();

	if (json.contains(JSON_ABNORMAL_ISPTA) && json[JSON_ABNORMAL_ISPTA].isBool())
        isAbnormalIspta = json[JSON_ABNORMAL_ISPTA].toBool();
	
	if (json.contains(JSON_ZMAG) && json[JSON_ZMAG].isDouble())
        zMag = json[JSON_ZMAG].toDouble();

	if (json.contains(JSON_PHASE) && json[JSON_PHASE].isDouble())
        phase = json[JSON_PHASE].toDouble();
}

qFailSafeWizard::qFailSafeWizard(QWidget *parent)
	: QWizard(parent)
{
	loadJSONFile();
	//qRecord oneRecord("2019-10-22", "1:40:19", "subject A",
	//	"Experimenter A", 22.5, false, false, 51.6, 8);
	//m_records.append(oneRecord);

	//
	p_checkUSImpPage = new qCheckUSImpedancePage;
	p_checkOutputVPage = new qCheckOutputVPage;
	p_manualCheckPage = new qManualCheckPage;
	p_introPage = new qIntroPage;
	p_failSafeWin = new qFailSafeWin;
	
	//
	p_checkUSImpPage->updateRecords(m_records);
	p_checkOutputVPage->updateVRecords(m_records);

	addPage(p_introPage);
	addPage(p_checkUSImpPage);
	addPage(p_manualCheckPage);
	addPage(p_checkOutputVPage); //sanity test and check voltage

	//
	setWizardStyle(QWizard::ModernStyle);
	setPixmap(QWizard::BannerPixmap, QPixmap(":/failSafeC/Resources/banner2.png"));
	setPixmap(QWizard::LogoPixmap, QPixmap(":/failSafeC/Resources/unmLogo.png"));
	//setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));
	
	setWindowTitle("Failsafe Wizard");
	resize(700,467);
}

qFailSafeWizard::~qFailSafeWizard()
{
	delete p_checkUSImpPage;
	delete p_checkOutputVPage;
	delete p_failSafeWin;
}

QWizardPage *qFailSafeWizard::checkBNCPage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle("Check BNC cables and adaptors");
	page->setSubTitle("Check all the BNC cables and adaptors connections are stable. Tighten the cable and adaptors if loose.");

	return page;
}

QWizardPage *qFailSafeWizard::groundInstrumentsPage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle("Ground instruments");
	page->setSubTitle("Ground the equipment as well as the cart and/or shelf that houses the instruments.");
	return page;
}

QWizardPage *qFailSafeWizard::connectPowerPage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle("Connect power");
	page->setSubTitle("Verify and make sure proper AC voltages are used to power the instruments. Power on all instruments.");
	return page;
}

QWizardPage *qFailSafeWizard::checkErrorCodesPage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle("No error codes");
	page->setSubTitle("Once powered on, the LED screen of the amplifier and the screen of function generator "
		"should indicate no errors. Otherwise, please troubleshoot and fix of the errors"
		"before using these two instruments.");
	return page;
}

QWizardPage *qFailSafeWizard::testFailSafePage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle("Test of the failsafe");
	page->setSubTitle(" ");
	/*page->setSubTitle("1) Before using the transducer on human subjects, please perform two following sanity tests of the transducer in water.\n"
		"2) Select and run sanityTestBigArtifactsanityTestBigSpike.ARB, which simulates the presence of big artifact beyond the safety limit.\n"
		"3) Select and run sanityTestAbnormalVoltage.ARB, which simulates the abrupt elevation of the output "
		"voltage from the amplifier, going beyond the pre-calibrated limit.\n"
		"4) The failsafe should work for both cases.");*/
	QLabel *label = new QLabel("1) Before using the transducer on human subjects, please perform two following sanity tests of the transducer in water.\n\n"
		"2) Select and run sanityTestBigArtifactsanityTestBigSpike.ARB, which simulates the presence of big artifact beyond the safety limit.\n\n"
		"3) Select and run sanityTestAbnormalVoltage.ARB, which simulates the abrupt elevation of the output "
		"voltage from the amplifier, going beyond the pre-calibrated limit.\n\n"
		"4) The failsafe should work for both cases.");
	label->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	page->setLayout(layout);

	return page;
}

void qFailSafeWizard::showDAQDialog()
{
}

void qFailSafeWizard::done(int result)
{
	if(result != QDialog::Accepted)
	{
		QWizard::done(result);

		return;
	}
	else
	{
		if(validateCurrentPage())
		{
			/*
			save to json file
			*/
			QDateTime dateTime = field(FIELD_DATE_TIME).toDateTime();
			qRecord record;
			record.date = dateTime.date().toString("yyyy-MM-dd");
			record.time = dateTime.time().toString("HH_mm_ss");
			record.experimenter = field(FIELD_XPMTR_iD).toString();
			record.subjectID = field(FIELD_SUBJECT_iD).toString();
			record.zMag = field(FIELD_ZMAG).toDouble();
			record.phase = field(FIELD_PHASE).toDouble();
			record.checkV = field(FIELD_CHECK_V).toDouble();
			//
			/*
			Creat a folder for storing data
			*/
			QDir dir(QCoreApplication::applicationDirPath());
			QString subDir("Data\\"+ record.date + "_" + record.time);
			if (!dir.exists(subDir))
				dir.mkpath(subDir);

			dir.cd(subDir);
			p_failSafeWin->setDataDir(dir);
			//
			p_failSafeWin->setWindowModality(Qt::ApplicationModal);
			p_failSafeWin->show();
			record.isAbnormalIspta = p_failSafeWin->isAbnormalISPTA();
			record.isAbnormalV = p_failSafeWin->isAbnormalV();
			writeJSONFile(record);
			QWizard::done(result);
			return;
		}
		else
		{
			/* code */
			QMessageBox::warning(this, "Warning", "Please complete all the sanity test!");
		}
	}
}

bool qFailSafeWizard::loadJSONFile()
{
	QFileInfo fi(QDir(QCoreApplication::applicationDirPath()), JSON_DB_FILE_NAME);
	QFile loadFile(fi.absoluteFilePath());
	if (!loadFile.open(QIODevice::ReadOnly))
        return false;

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
	QJsonObject json = loadDoc.object();

    if (json.contains(JSON_DB_RECORD) && json[JSON_DB_RECORD].isArray())
	{
        QJsonArray recordArray = json[JSON_DB_RECORD].toArray();
		m_records.clear();
        m_records.reserve(recordArray.size());
        for (int recordIndex = 0; recordIndex < recordArray.size(); ++recordIndex) {
            QJsonObject levelObject = recordArray[recordIndex].toObject();
			qRecord record;
			record.read(levelObject);
            m_records.append(record);
        }
    }
    return true;
}

bool qFailSafeWizard::writeJSONFile(const qRecord &newRecord)
{
	QJsonObject json;
	m_records.append(newRecord);
	//
	QJsonArray recordArray;
    for (const qRecord &record : m_records) {
        QJsonObject recordObject;
		record.write(recordObject);
        recordArray.append(recordObject);
    }
    json[JSON_DB_RECORD] = recordArray;
	//
	QFileInfo fi(QDir(QCoreApplication::applicationDirPath()), JSON_DB_FILE_NAME);
	QFile saveFile(fi.absoluteFilePath());
	if (!saveFile.open(QIODevice::WriteOnly))
		return false;

	QJsonDocument saveDoc(json);
	if (saveFile.write(saveDoc.toJson()) < 0)
		return false;
	else
		return true;
}
