import time

def exec():
    print("start")
    print(task.value("a"))

    for i in range(5):
        time.sleep(1)
        if task.isInterruptionRequested():
            print("canceled\n")
            return

    print("finished\n")

exec()
