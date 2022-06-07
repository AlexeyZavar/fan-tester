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

        self.tensometer1 = 10.221
        self.amperes = 10.821
        self.voltage = 14.652
        self.iteration = 0

        self.in_waiting = 1

    def write(self, *args):
        pass

    def readline(self):
        time.sleep(1)
        self.pwm += 50

        if self.pwm > 2000 and self.iteration > 10:
            self.running = False
            self.iteration = 0
            self.tensometer1 = 10.221
            self.amperes = 10.821
            self.voltage = 9.731
        else:
            if self.iteration <= 10:
                self.iteration += 1

            self.amperes += random.randint(10, 27) * 0.1
            self.voltage += random.randint(-200, 200) * 0.0001
            self.tensometer1 += random.randint(1000, 2000) * 0.1


        res = (json.dumps({
            'pwm': self.pwm,
            'current_rpm': 1000,
            'current_amperes': self.amperes,
            'current_voltage': self.voltage,
            'tensometer1': self.tensometer1,
            'tensometer2': 0,
            'tensometer3': 0,
            'running': self.running,
        }) + '\n').encode('utf-8')

        if self.pwm >= 2000:
            self.running = True
            self.pwm = 1000

        return res


try:
    arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
except:
    print('--------------------------')
    print(' Arduino is not connected ')
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
        if state['pwm'] < 2000:
            states.append(state)
        print(time.time(), state)

    data = Benchmark()
    data.name = name
    data.soft_start = soft_start
    data.performed_on = performed_on

    data.warmup_time = warmed - performed_on

    data.data = states

    db: Session = DBSession()
    db.add(data)
    db.commit()

    states.clear()


def run_calibrate():
    data = {
        'action': 'calibrate'
    }
    s = json.dumps(data)

    arduino.write(s.encode('utf-8'))
