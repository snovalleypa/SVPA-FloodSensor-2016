<?php
header('Content-Type: application/json');
$payload = file_get_contents("php://input");
$report = json_decode($payload);

$deviceId = $report->{'deviceId'};

$outBytes = file_put_contents(($deviceId . ".json"), $payload);

print '{';
print "deviceId: ";
print $deviceId;
print ', bytes written: ';
print $outBytes;
print '}';
?>
