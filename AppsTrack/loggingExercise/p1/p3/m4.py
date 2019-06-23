import logging.config

log = logging.getLogger(__name__)


def f6():
    log.info("entering method " + __name__ + "." + f6.__name__)
    print('funtion 6')
    log.info("exiting method " + __name__ + "." + f6.__name__)
