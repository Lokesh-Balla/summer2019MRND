import time
import threading

counter = 0


def watcher():
    while True:
        global counter
        print('counter is ', counter)
        time.sleep(1)


def update():
    while True:
        global counter
        counter += 1
        time.sleep(0.3)
        # time.sleep(0.1)
        # print(counter)


def activity(tag):
    for i in range(5):
        thread_id = threading.current_thread().ident
        print("hi from " + tag + " and thread-id is " + str(thread_id))
        time.sleep(1)


def worker1():
    print('worker1 thread-id is ', threading.current_thread().ident)
    # activity("worker1")
    update()


def worker2():
    print('worker2 thread-id is ', threading.current_thread().ident)
    # activity("worker2")
    update()


def manyThreads():
    threads=[]
    wtch=threading.Thread(target=watcher)
    wtch.start()
    for i in range(10):
        thread=threading.Thread(target=worker1)
        thread.start()
        thread.join()
        threads.append(thread)

def main():
    print('main thread-id is ', threading.current_thread().ident)
    thread1 = threading.Thread(target=worker1)
    thread2 = threading.Thread(target=worker2)
    thread1.start()
    thread2.start()



if __name__ == "__main__":
    manyThreads()
