import logging.config
import inspect

def log_util():
    log.info("entering method "+__name__+"."+inspect.currentframe().f_back.f_code.co_name)
    log.info("exiting method "+__name__+"."+inspect.currentframe().f_back.f_code.co_name)


log = logging.getLogger(__name__)


def f3(a, b):
    log_util()
    return a * b


def f4(a, b):
    log_util()
    return a // b
