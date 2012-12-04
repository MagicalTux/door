snmpset -v 1 -c private 192.168.0.100 DenkoviMIB::p6state.1 i 0
snmpset -v 1 -c private 192.168.0.100 DenkoviMIB::p6state.2 i 0
sleep 1
snmpset -v 1 -c private 192.168.0.100 DenkoviMIB::p6state.1 i 1
snmpset -v 1 -c private 192.168.0.100 DenkoviMIB::p6state.2 i 1
