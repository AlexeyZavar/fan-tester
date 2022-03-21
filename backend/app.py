from flask import Flask, jsonify, request
from flask_cors import CORS
from sqlalchemy.orm import Session

from database import DBSession, Benchmark
from utils import run_benchmark, state

app = Flask(__name__)
CORS(app)


@app.get('/database')
def database():
    db: Session = DBSession()
    data = db.query(Benchmark).all()

    return jsonify([item.to_dict() for item in data])


@app.get('/database/<name>')
def database_item(name: str):
    db: Session = DBSession()
    item = db.query(Benchmark).filter_by(name=name).one()

    return jsonify(item.to_dict())


@app.post('/benchmark')
def benchmark():
    run_benchmark(request.json['name'], request.json['soft_start'])

    return '', 200


@app.get('/benchmark')
def benchmark_state():
    return jsonify(state)


if __name__ == '__main__':
    app.run()
