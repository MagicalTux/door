#include <stdio.h>
#include "QSnmp.hpp"

QSnmp::QSnmp(const QString &peername, const QByteArray &community) {
	init_snmp("door_control");
	snmp_sess_init(&session);
	session.peername = strdup(peername.toUtf8().constData()); //"192.168.0.100";
	session.version = SNMP_VERSION_1;
	session.community = (unsigned char*)strdup(community.constData()); //(unsigned char*)"private";
	session.community_len = strlen((char*)session.community);

	ss = snmp_open(&session);
	if (!ss) {
		snmp_perror("ack");
		snmp_log(LOG_ERR, "something horrible happened!!!\n");
		return;
	}

	qDebug("QSnmp: session established");
}

QSnmp::~QSnmp() {
	snmp_close(ss);
}

QVariant QSnmp::get(const QString &oid) {
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
//	oid anOID[MAX_OID_LEN];
	u_long anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
	int status;
	struct variable_list *vars;

	pdu = snmp_pdu_create(SNMP_MSG_GET);
	read_objid(oid.toUtf8().constData(), anOID, &anOID_len);
	snmp_add_null_var(pdu, anOID, anOID_len);
	status = snmp_synch_response(ss, pdu, &response);

	if (status != STAT_SUCCESS || response->errstat != SNMP_ERR_NOERROR) {
		qDebug("QSnmp: failed to get an oid");
		if (response) snmp_free_pdu(response);
		return QVariant();
	}

	for(vars = response->variables; vars; vars = vars->next_variable) {
		if (vars->type == ASN_OCTET_STR) {
			// string
			QByteArray res(vars->val_len, '\0');
			memcpy(res.data(), vars->val.string, vars->val_len);
			snmp_free_pdu(response);
			return res;
		}
		if (vars->type == ASN_INTEGER) {
			int res = (int)*vars->val.integer;
			snmp_free_pdu(response);
			return res;
		}
		print_variable(vars->name, vars->name_length, vars);
		qDebug("QSnmp: unknown var type %d", vars->type);
	}

	snmp_free_pdu(response);
	return QVariant();
}

bool QSnmp::set(const QString &oid, int val) {
	struct snmp_pdu *pdu;
	struct snmp_pdu *response = NULL;
//	oid anOID[MAX_OID_LEN];
	u_long anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
	int status;

	pdu = snmp_pdu_create(SNMP_MSG_SET);
	read_objid(oid.toUtf8().constData(), anOID, &anOID_len);
	snmp_add_var(pdu, anOID, anOID_len, 'i', QByteArray::number(val).constData());
	//snmp_add_var(pdu, anOID, anOID_len, ASN_INTEGER, QByteArray::number(val).constData());

	status = snmp_synch_response(ss, pdu, &response);

	if (status != STAT_SUCCESS || response->errstat != SNMP_ERR_NOERROR) {
		qDebug("QSnmp: failed to set an oid");
		if (response) snmp_free_pdu(response);
		return false;
	}

	if (response) snmp_free_pdu(response);
	return true;
}

