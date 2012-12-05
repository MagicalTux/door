#include <QObject>
#include <QTimer>

class QSnmp;

class Door: public QObject {
	Q_OBJECT;
public:
	Door(const QString &open1, const QString &open2, const QString &close1, const QString &close2, const QString &closed_sensor, const QString &unlocked_sensor, const QString &locked_sensor, QSnmp &snmp, bool autolock = false);

public slots:
	void doCheck(); // check sensors status
	void close(); // close door
	void close_end(); // end closing (automatically called by close())
	void open(); // open door
	void open_tmp(); // open door for 10 secs
	void open_end(); // end opening (automatically called by open())
	void setOpen(bool);
	bool getOpen();

private:
	QString open1, open2, close1, close2;
	QString closed_sensor, unlocked_sensor, locked_sensor;
	bool autolock;
	bool in_action, tmp_open;
	bool force_open;

	QTimer timer_check;

	QSnmp &snmp;
};

