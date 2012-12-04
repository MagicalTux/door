<?php

$sock = stream_socket_client("udp://127.0.0.1:7415");

var_dump(stream_socket_sendto($sock, 'list'));
var_dump(stream_socket_recvfrom($sock, 8192));

var_dump(stream_socket_sendto($sock, 'open_tmp|main'));
var_dump(stream_socket_recvfrom($sock, 8192));

