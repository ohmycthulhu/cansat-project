#!/usr/bin/python3.6
from serial import Serial
from sys import argv
import re
import hashlib
from commands import Commands, Statuses
from time import sleep

regexps = {
    'port': '--port=(.+)',
    'baudrate': '--baudrate=(.+)',
    'timeout': '--timeout=(.+)'
}
parseResults = {
    'port': '/dev/ttyUSB0',
    'baudrate': '9600',
    'timeout': 0
}
for arg in argv:
    for key, regex in regexps.items():
        m = re.search(regex, arg)
        if m:
            parseResults[key] = m.group(1)


port, baudrate, timeout = parseResults['port'], parseResults['baudrate'], parseResults['timeout']
print(port, baudrate)
s = None
print("Creating serial")
try:
    s = Serial(port = port, baudrate = baudrate, timeout = timeout)
except Exception as e:
    print("Couldn't create serial object. Trying again...", flush = True)
    print(e)
    exit(-1)

def parse(s):
    # Message consists of: type/payload|hash
    parts = s.split('|')
    if len(parts) < 2:
        return None
    # Check hash of message
    if hashlib.md5(str.encode(parts[0])).hexdigest() != parts[1].strip():
        print('Hash failed')
        return None

    message_parts = parts[0].split('/')
    if len(message_parts) < 2:
        print('Message parts are too small')
        return None
    if message_parts[0] == '0':
        telemetry_parts = message_parts[1].split(',')
        
        # Telemetry format:
        # {Command ID 0}, {Running time 1 }, {Packets count 2}, {Temperature 3}, {Pressure 4}, ...
        # {Humidity 5}, {Height 6}, {speed 7}, {Latitude 8}, {Longitude 9}, {Voltage 10}

        return {
            'type': 'Telemetry',
            'type_id': 0,
            'Running Time': telemetry_parts[1],
            'Packet #': telemetry_parts[2],
            'Temperature': telemetry_parts[3],
            'Pressure': telemetry_parts[4],
            'Humidity': telemetry_parts[5],
            'Height': telemetry_parts[6],
            'Speed': telemetry_parts[7],
            'Latitude': telemetry_parts[8],
            'Longitude': telemetry_parts[9],
            'Voltage': telemetry_parts[10]
        }
    if message_parts[0] == '1':
        report_parts = message_parts[1].split(',')
        command = Commands.getCommand(report_parts[0])
        status = Statuses.getStatus(report_parts[1])
        return { 'type': 'Command report', 'type_id': 1, 'command': command, 'status': status }

def convertDataToLog(data):
    # type, {all other columns}\n
    return f"{data['type']},{','.join([data[key] for key in data if key != 'type' and key != 'type_id'])}\n"
    

log = open('log.txt', 'a')
rawLog = open('raw_log.txt', 'a')

savedBuffer = ""
while True:
    try:
        # Read all buffer
        serialBuffer = ''.join([chr(x) for x in s.read(1024)])
        rawLog.write(serialBuffer)
        jointBuffer = savedBuffer + serialBuffer
    except Exception as e:
        print("Couldn't open read bufer. Trying again...")
        continue
    if jointBuffer and "\n" in jointBuffer:
    # Parse buffer and show results
        messages = jointBuffer.split('\n')
        for message in messages:
            data = parse(message)
            if data:
                print("*"*50, "", "", "", sep='\n')
                print(f"Type: {data['type']}")
                if data['type_id'] == 0:
                    for name, value in data.items():
                        if name != 'type' and name != 'type_id':
                            print(f"{name}: {value}")
                if data['type_id'] == 1:
                    print(f"Command: {data['command']}", f"Status: {data['status']}", sep = '\n')
                    sleep(2)
                print("*"*50, "", "", "", sep='\n')
                log.write(convertDataToLog(data))
        savedBuffer = '' if jointBuffer[-1] == '\n' else messages[-1]
    # Sleep
    sleep(1)