# door control

This uses this controller:

http://www.denkovi.com/productv/44/snmp-and-web-based-data-acquisition-i-o-module.html

## Trap handling

Trap handling requires running snmptrapd.

To set the trap server IP, run:

	snmpset -v1 -c private 192.168.0.100 DenkoviMIB::SNMPtrap1 a 192.168.0.88

You'll need to modify the snmptrapd commandline to add `-m DenkoviMIB:DISMAN-EVENT-MIB:SNMPv2-MIB`

