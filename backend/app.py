from flask import Flask, jsonify, request
from flask_cors import CORS
from sqlalchemy.orm import Session

from database import DBSession, Benchmark
from utils import run_benchmark, run_calibrate, states

app = Flask(__name__)
CORS(app)


@app.get('/database')
def database():
    db: Session = DBSession()
    data = db.query(Benchmark).all()

    return jsonify([item.to_dict() for item in data])


@app.get('/database/<id>')
def database_item(id: str):
    db: Session = DBSession()
    item = db.query(Benchmark).get(id)

    if item is None:
        return '', 404

    return jsonify(item.to_dict())


@app.post('/benchmark')
def benchmark():
    run_benchmark(request.json['name'], request.json['soft_start'])
    states.clear()

    return '', 200


@app.get('/benchmark')
def benchmark_state():
    return jsonify(states)


@app.post('/calibrate')
def calibrate():
    run_calibrate()

    return '', 200


if __name__ == '__main__':
    app.run()
