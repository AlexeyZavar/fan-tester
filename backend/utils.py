import json
import random
import time
from threading import Thread

import serial
from sqlalchemy.orm import Session

from database import DBSession, Benchmark


class FakeArduino:
    def __init__(self):
        self.pwm = 1000
        self.running = True

        self.in_waiting = 1

    def write(self, *args):
        pass

    def readline(self):
        time.sleep(1)
        self.pwm += 50

        if self.pwm > 2000:
            self.running = False

        return (json.dumps({
            'pwm': self.pwm,
            'current_rpm': random.randint(5, 20),
            'current_amperes': random.randint(5, 20),
            'current_voltage': random.randint(5, 20),
            'tensometer1': random.randint(5, 20),
            'tensometer2': random.randint(5, 20),
            'tensometer3': random.randint(5, 20),
            'running': self.running,
        }) + '\n').encode('utf-8')


try:
    arduino = serial.Serial(port='COM9', baudrate=9600, timeout=.1)
except:
    print('--------------------------')
    print(' Arduino is not connected')
    print('--------------------------')

    arduino = FakeArduino()

states = []


def run_benchmark(name: str, soft_start: bool):
    t = Thread(target=benchmark, args=(name, soft_start))
    t.start()


def benchmark(name: str, soft_start: bool):
    global states

    if states:
        raise Exception('Benchmark already running')

    performed_on = time.time()

    data = {
        'action': 'start_benchmark',
        'params': {
            'soft_start': soft_start
        }
    }
    s = json.dumps(data).encode('utf-8')

    arduino.write(s)

    while not arduino.in_waiting: pass
    state = json.loads(arduino.readline().decode('utf-8'))
    states.append(state)
    warmed = time.time()
    print(time.time(), state)

    while state['running']:
        while not arduino.in_waiting: pass
        state = json.loads(arduino.readline().decode('utf-8'))
        states.append(state)
        print(time.time(), state)

    data = Benchmark()
    data.name = name
    data.soft_start = soft_start
    data.performed_on = performed_on

    data.data = states

    db: Session = DBSession()
    db.add(data)
    db.commit()

    states = []


def run_calibrate():
    data = {
        'action': 'calibrate'
    }
    s = json.dumps(data)

    arduino.write(s.encode('utf-8'))
