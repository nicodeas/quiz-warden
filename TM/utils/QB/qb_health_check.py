import time
from config import QB_HOSTS, HEALTHCHECK_INTERVAL

def do_health_check(qb_handler):
    for qb in QB_HOSTS:
        res = qb_handler.health_check(qb)
        if  res == False:
            print(f"QB at {qb[0]}:{qb[1]} is down")
            if qb in qb_handler.qbs:
                qb_handler.qbs.pop(qb)
        else:
            if qb not in qb_handler.qbs:
                print(f"Connected to QB at {qb[0]}:{qb[1]}")
                qb_handler.qbs[qb] = res

def qb_health_check(qb_handler):
    while True:
        do_health_check(qb_handler)
        time.sleep(HEALTHCHECK_INTERVAL)
