import json

from flask import Flask, jsonify, request

from utils import run_benchmark, state

app = Flask(__name__)


@app.get('/database')
def database():
    with open('./data.json', 'r', encoding='utf-8') as f:
        data = json.load(f)

    return jsonify(data)


@app.post('/benchmark')
def benchmark():
    run_benchmark(request.json['name'], request.json['soft_start'])


@app.get('/benchmark')
def benchmark_state():
    return jsonify(state)


if __name__ == '__main__':
    app.run()
