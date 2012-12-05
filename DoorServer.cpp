#include "DoorServer.hpp"
#include "Door.hpp"
#include <QStringList>

DoorServer::DoorServer() {
	s.bind(7415);
	connect(&s, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void DoorServer::addDoor(const QString &name, Door *door) {
	doors.insert(name, door);
}

void DoorServer::readSocket() {
	while(s.hasPendingDatagrams()) {
		QByteArray req(s.pendingDatagramSize(), '\0');
		QHostAddress addr;
		quint16 port;
		s.readDatagram(req.data(), req.length(), &addr, &port);

		QByteArray res = handle(req);

		s.writeDatagram(res.constData(), res.length(), addr, port);
	}
}

QByteArray DoorServer::handle(const QByteArray &req) {
	QList<QByteArray> l = req.split('|');
	if (l.at(0) == "list") {
		return QStringList(doors.keys()).join("|").toUtf8();
	}
	if (l.at(0) == "open_tmp") {
		QString door = QString::fromUtf8(l.at(1));
		if (!doors.contains(door)) return "error";
		doors.value(door)->open_tmp();
		return "ok";
	}
	if (l.at(0) == "open") {
		QString door = QString::fromUtf8(l.at(1));
		if (!doors.contains(door)) return "error";
		doors.value(door)->setOpen(true);
		return "ok";
	}
	if (l.at(0) == "close") {
		QString door = QString::fromUtf8(l.at(1));
		if (!doors.contains(door)) return "error";
		doors.value(door)->setOpen(false);
		return "ok";
	}
	return "error";
}

