import logging.config
import inspect


def log_util():
    log.info("entering method "+__name__+"."+inspect.currentframe().f_back.f_code.co_name)
    log.info("exiting method "+__name__+"."+inspect.currentframe().f_back.f_code.co_name)


log = logging.getLogger(__name__)


def f5(a, b):
    log_util()
    try:
        res = a/b
    except ZeroDivisionError:
        log.exception("Dividing by zero")
    else:
        return res
