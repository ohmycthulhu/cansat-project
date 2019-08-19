
# Ids are strings so we can compare without casting
class Statuses:
    OK = '0'
    FAILED = '1'
    HASH_FAILED = '2'
    NO_COMMAND = '3'
    def getStatus (id):
        if id == Statuses.OK:
            return 'OK'
        if id == Statuses.FAILED:
            return 'Failed'
        if id == Statuses.HASH_FAILED:
            return 'Hash failed'
        if id == Statuses.NO_COMMAND:
            return 'No Command'

# Ids are strings so we can compare without casting
class Commands:
    RESET = '1' # 1
    START_CAMERA =  '2' # 2
    START_RECORDING = '3' # 3
    STOP_RECORDING = '4' # 4
    FORCE_START_CAMERA = '5' # 5
    UNDEFINED = '0'
    def getCommand (id):
        if id == Commands.RESET:
            return 'Reset'
        if id == Commands.START_CAMERA:
            return 'Start Camera'
        if id == Commands.START_RECORDING:
            return 'Start Recording'
        if id == Commands.STOP_RECORDING:
            return 'Stop Recording'
        if id == Commands.FORCE_START_CAMERA:
            return 'Force Camera Start'
        if id == Commands.UNDEFINED:
            return 'Undefined'

    def getCommands():
        return {
            'reset': '1',
            'start_camera': '2',
            'start_recording': '3',
            'stop_recording': '4',
            'force_start_camera': '5'
        }