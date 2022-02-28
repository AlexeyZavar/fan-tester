import json
import time
from threading import Thread

import serial
from sqlalchemy.orm import Session

from database import DBSession, Benchmark

try:
    arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)
except:
    print('Arduino is not connected')
    exit(-1)

state = None


def run_benchmark(name: str, soft_start: bool):
    t = Thread(target=benchmark, args=(name, soft_start))
    t.start()


def benchmark(name: str, soft_start: bool):
    global state

    performed_on = time.time()

    data = {
        'action': 'start_benchmark',
        'params': {
            'soft_start': soft_start
        }
    }
    s = json.dumps(data).encode('utf-8')

    arduino.write(s)

    states = []

    state = json.loads(arduino.read_all().decode('utf-8'))
    states.append(state)
    warmed = time.time()

    while state['running']:
        state = json.loads(arduino.read_all().decode('utf-8'))
        states.append(state)

    thrust_amperes = [(state['tensometer1'] / state['current_amperes']) for state in states]
    thrust_power = [state['tensometer1'] + state['tensometer2'] + state['tensometer3'] for state in states]
    efficiency = [(20 / state['current_amperes']) for state in states]

    max_amperes = max(states, key=lambda x: x['current_amperes'])['current_amperes']
    max_voltage = max(states, key=lambda x: x['current_voltage'])['current_voltage']
    min_voltage = min(states, key=lambda x: x['current_voltage'])['current_voltage']
    max_power = max(states, key=lambda x: x['current_voltage'])['current_voltage']
    max_thrust = max(states, key=lambda x: x['tensometer1'])['tensometer1']

    data = Benchmark()
    data.name = name
    data.soft_start = soft_start
    data.performed_on = performed_on

    data.thrust_amperes = thrust_amperes
    data.thrust_power = thrust_power
    data.efficiency = efficiency
    data.warmup_time = warmed - performed_on

    data.max_amperes = max_amperes
    data.max_voltage = max_voltage
    data.min_voltage = min_voltage
    data.max_power = max_power
    data.max_thrust = max_thrust

    db: Session = DBSession()
    db.add(data)
    db.commit()

    state = None
