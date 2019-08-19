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
    'timeout': '--timeout=(.+)',
    'command': '--command=(.+)'
}
parseResults = {
    'port': '/dev/ttyUSB0',
    'baudrate': '9600',
    'timeout': 0,
    'command': None
}
for arg in argv:
    for key, regex in regexps.items():
        m = re.search(regex, arg)
        if m:
            parseResults[key] = m.group(1)


port, baudrate, timeout = parseResults['port'], parseResults['baudrate'], parseResults['timeout']
s = None
try:
    s = Serial(port = port, baudrate = baudrate, timeout = timeout)
except Exception as e:
    print("Couldn't create serial object. Trying again...")
    print(e)
    exit(-1)

s.close()

allCommands = Commands.getCommands()
command = parseResults["command"]
commandExists = command != None
commandWasSent = False
while (not commandExists or (commandExists and not commandWasSent)):
    # Get command
    if not commandExists:
        command = input('>>> ')
    # Check some important commands
    if command == 'quit':
        print("Goodbye")
        break
    # Check if command exists
    # If not exists, show all commands
    if command not in allCommands:
        print('Inputted command doesn\'t exists')
        print('There is a list of existing commands: ')
        print('-'*50)
        print('quit')
        for c in allCommands:
            print(c)
        print('-'*50)
        commandWasSent = True
        continue
    msg = f"{allCommands[command]}"
    # msg = f"{allCommands[command]}|{hashlib.md5(str.encode(allCommands[command])).hexdigest()}"
    try:
        # Open serial
        s.open()
        # Send command
        s.write(str.encode(msg + "\n"))
        # Close serial
        s.close()
    except Exception as e:
        print("Couldn't open serial")
        continue
    # Show send result
    print(f"Sended {command} ({allCommands[command]}) as {msg}")
    commandWasSent = True