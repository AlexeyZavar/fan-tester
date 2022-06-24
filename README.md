# Мини-стенд тестирования ВМГ

## Setup

```bash
$ git clone https://github.com/AlexeyZavar/fan-tester.git
```

### backend

```bash
$ cd backend
$ python -m venv venv

# linux
$ source venv/bin/activate
# windows
$ venv/bin/activate

$ pip install -r requirements.txt
$ python main.py
```

### frontend

NodeJS LTS required.

```bash
$ cd frontend
$ npm install --global yarn
$ yarn install
$ yarn dev
```

Open http://localhost:3000

### firmware

PlatformIO required.

Open in VS Code or CLion and build.
