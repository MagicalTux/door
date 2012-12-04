#include <QCoreApplication>
#include "QSnmp.hpp"
#include "Door.hpp"
#include "DoorServer.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	// invoke SNMP
	QSnmp snmp("192.168.0.100", "private");

	// create doors
	Door *maindoor = new Door("DenkoviMIB::p6state.1", "DenkoviMIB::p6state.2", "DenkoviMIB::p6state.3", "DenkoviMIB::p6state.4", "DenkoviMIB::JP2Value.1", "DenkoviMIB::JP2Value.2", "DenkoviMIB::JP2Value.3", snmp);
	Door *meetingdoor = new Door("DenkoviMIB::p6state.5", "DenkoviMIB::p6state.6", "", "", "DenkoviMIB::JP2Value.5", "DenkoviMIB::JP2Value.6", "DenkoviMIB::JP2Value.7", snmp, true);

	DoorServer *serv = new DoorServer();
	serv->addDoor("main", maindoor);
	serv->addDoor("mtg", meetingdoor);

	return app.exec();
}

