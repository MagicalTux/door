#include <QObject>
#include <QVariant>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

class QSnmp: public QObject {
	Q_OBJECT;
public:
	QSnmp(const QString &peername, const QByteArray &community); // v1
	~QSnmp();
	QVariant get(const QString &oid);
	bool set(const QString &oid, int val);

private:
	struct snmp_session session, *ss;
};


