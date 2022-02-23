from threading import Thread

state = None


def run_benchmark(name: str, soft_start: bool):
    t = Thread(target=benchmark, args=(name, soft_start))
    t.start()


def benchmark(name: str, soft_start: bool):
    # tba
    return
