import json

import serial

arduino = serial.Serial(port='COM9', baudrate=115200, timeout=.1)
cmd = {
    'action': 'start_benchmark',
    'params': {'soft_start': False}
}
arduino.write(json.dumps(cmd).encode('utf-8'))
