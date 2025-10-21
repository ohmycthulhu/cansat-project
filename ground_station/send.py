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
    'command': '--command=(.+)',
    'times': '--times=(\d+)',
    'step': '--step=([0-9.]+)'
}
parseResults = {
    'port': '/dev/ttyUSB0',
    'baudrate': '9600',
    'timeout': '0',
    'command': None,
    'times': '1',
    'step': '.5'
}
for arg in argv:
    for key, regex in regexps.items():
        m = re.search(regex, arg)
        if m:
            parseResults[key] = m.group(1)


port, baudrate, timeout = parseResults['port'], parseResults['baudrate'], int(parseResults['timeout'])
s = None
try:
    s = Serial(port = port, baudrate = baudrate, timeout = timeout)
except Exception as e:
    print("Couldn't create serial object. Trying again...")
    print(e)
    exit(-1)

s.close()

def sendCommand (command):
    msg = f"{allCommands[command]}||\n"
    try:
        s.open()
        s.write(str.encode(msg))
        s.close()
    except Exception as e:
        print("Couldn't open serial")
        return False

    print(f"Sended {command} ({allCommands[command]}) as {msg}")
    return True


def interpretCommand(command, times=1, step=0.5):
    if command not in allCommands:
        print('Inputted command doesn\'t exists')
        print('There is a list of existing commands: ')
        print('-'*50)
        print('quit')
        for c in allCommands:
            print(c)
        print('-'*50)
        return False
    else:
        for i in range(times):
            print(f"Try #{i}")
            while not sendCommand(command):
                pass
            sleep(step)
        return True


allCommands = Commands.getCommands()

prompt_command = parseResults["command"]
if prompt_command is not None:
    res = interpretCommand(
        command=prompt_command,
        times=int(parseResults['times']),
        step=float(parseResults['step'])
    )

    exit(0 if res else 1)

while True:
    command = input('>>> ')

    if command == 'quit':
        print("Goodbye")
        break

    interpretCommand(command)