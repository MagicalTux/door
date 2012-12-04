#include <QObject>
#include <QMap>
#include <QUdpSocket>

class Door;

class DoorServer: public QObject {
	Q_OBJECT;
public:
	DoorServer();

	void addDoor(const QString &name, Door *door);

public slots:
	void readSocket();
	
protected:
	QByteArray handle(const QByteArray &req);

private:
	QMap<QString, Door*> doors;
	QUdpSocket s;
};

