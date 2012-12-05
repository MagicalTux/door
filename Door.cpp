#include "Door.hpp"
#include "QSnmp.hpp"

Door::Door(const QString &_open1, const QString &_open2, const QString &_close1, const QString &_close2, const QString &_closed_sensor, const QString &_unlocked_sensor, const QString &_locked_sensor, QSnmp &_snmp, bool _autolock): snmp(_snmp) {
	open1 = _open1;
	open2 = _open2;
	close1 = _close1;
	close2 = _close2;
	closed_sensor = _closed_sensor;
	unlocked_sensor = _unlocked_sensor;
	locked_sensor = _locked_sensor;
	autolock = _autolock;
	in_action = false;
	tmp_open = false;

	snmp.set(open1, 1);
	snmp.set(open2, 1);
	if (!autolock) {
		snmp.set(close1, 1);
		snmp.set(close2, 1);
	}

	connect(&timer_check, SIGNAL(timeout()), this, SLOT(doCheck()));
	timer_check.start(100);
}

void Door::doCheck() {
	if (in_action) return;

	if (!snmp.get(closed_sensor).toInt()) {
		// check if door is not closed
		if (!autolock) {
			if (snmp.get(locked_sensor).toInt())
				open();
		}
		return; // skip tests if door is open
	}

	if ((!autolock) && (!tmp_open) && (!force_open)) {
		if (snmp.get(unlocked_sensor).toInt())
			close();
	}
	if (force_open) {
		if (snmp.get(locked_sensor).toInt())
			open();
	}
}

void Door::close() {
	tmp_open = false;
	if (!snmp.get(closed_sensor).toInt()) return; // can't close
	snmp.set(close1, 0);
	snmp.set(close2, 0);
	QTimer::singleShot(1000, this, SLOT(close_end()));
	in_action = true;
}

void Door::close_end() {
	if (autolock) return;
	in_action = false;
	snmp.set(close1, 1);
	snmp.set(close2, 1);
}

void Door::open() {
	if (autolock) {
		snmp.set(open1, 0);
		snmp.set(open2, 0);
		in_action = true;
		QTimer::singleShot(15000, this, SLOT(open_end()));
		return;
	}
	snmp.set(open1, 0);
	snmp.set(open2, 0);
	QTimer::singleShot(1000, this, SLOT(open_end()));
	in_action = true;
}

void Door::open_tmp() {
	open();
	if (autolock) return;
	tmp_open = true;
	QTimer::singleShot(10000, this, SLOT(close()));
}

void Door::open_end() {
	in_action = false;
	snmp.set(open1, 1);
	snmp.set(open2, 1);
}

void Door::setOpen(bool op) {
	force_open = op;
	if (op)
		open();
}

