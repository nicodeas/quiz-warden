import time
from config import QB_HOSTS


def qb_health_check(qb_handler):
    while True:
        for qb in QB_HOSTS:
            if qb_handler.health_check(qb) == False:
                print(f"QB at {qb[0]}:{qb[1]} is down")
                if qb in qb_handler.qbs:
                    qb_handler.qbs.remove(qb)
            else:
                if qb not in qb_handler.qbs:
                    print(f"Connected to QB at {qb[0]}:{qb[1]}")
                    qb_handler.qbs.append(qb)
        time.sleep(2)
