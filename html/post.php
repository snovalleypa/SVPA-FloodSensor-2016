<?php
header('Content-Type: application/json');

$payload = file_get_contents("php://input");

file_put_contents("LastPayload.txt", $payload);

$report = json_decode($payload);

$deviceId = $report->{'deviceId'};

$outBytes = file_put_contents(($deviceId . ".json"), $payload);


print '{';
print "bytesReceived: ";
print strlen($payload);
print ', ';
print "deviceId: ";
print $deviceId;
print ', ';
print 'bytes written: ';
print $outBytes;
print '}';
?>
