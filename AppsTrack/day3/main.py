import inspect
import logging.config
import p1.m1
import p1.m3
import p1.p2.m2
import p1.p3.m4
'''
MAIN_LOGGING= {
    'version': 1,
    'formatters': {
        'common': {
            'class': 'logging.Formatter',
            'format': '%(asctime)s %(levelname)s %(message)s',
        },
    },
    'handlers': {
        'console': {
            'class': 'logging.StreamHandler',
            'level': 'WARN',
        },
        'file': {
            'class': 'logging.FileHandler',
            'level': 'DEBUG',
            'filename': 'main.log',
            'formatter': 'common',
            'mode': 'a',
        },
        'P1':{
            'class': 'logging.FileHandler',
            'level': 'DEBUG',
            'filename': 'P1.log',
            'formatter': 'common',
            'mode': 'a',
        },
        'P2': {
            'class': 'logging.FileHandler',
            'level': 'DEBUG',
            'filename': 'P2.log',
            'formatter': 'common',
            'mode': 'a',
        },
    },
    'loggers': {
        __name__: {
            'handlers': ['file', 'console'],
            'level': 'DEBUG',
        },
        'p1': {
            'handlers': ['P1', 'console'],
            'level': 'DEBUG',
        },
        'p1.p2': {
            'handlers': ['P2', 'console'],
            'propagate': False,
            'level': 'DEBUG',
        },
    },
}

'''

CUSTOM_LOGGING = {
    'version': 1,
    'disable_existing_loggers': False,
    'loggers': {
        '': {
            'handlers': ['console', 'mainlog'],
            'level': "DEBUG",
        },
        'p1.p2': {
            'handlers': ['p2log', 'console'],
            'level': "DEBUG",
            'propagate': False
        },
        'p1': {
            'handlers': ['p1log', 'console'],
            'level': "DEBUG",
            'propagate': False
        },
    },
    'handlers': {
        'console': {
            'level': 'WARN',
            'class': 'logging.StreamHandler',
            'stream': 'ext://sys.stdout',
        },
        'p2log': {
            'class': 'logging.FileHandler',
            'filename': 'p2.log'
        },

        'p1log': {
            'class': 'logging.FileHandler',
            'filename': 'p1.log'
        },

        'mainlog': {
            'class': 'logging.FileHandler',
            'filename': 'main.log'
        }
    }
}


def log_util():
    log.info("entering method "+__name__+"."+inspect.currentframe().f_back.f_code.co_name)
    log.info("exiting method "+__name__+"."+inspect.currentframe().f_back.f_code.co_name)


logging.config.dictConfig(CUSTOM_LOGGING)
log = logging.getLogger(__name__)

if __name__ == '__main__':
    log_util()
    print(p1.m1.f1(1, 2))
    print(p1.m1.f2(2, 1))
    print(p1.m3.f5(2, 0))
    print(p1.p2.m2.f3(3, 4))
    print(p1.p2.m2.f4(10, 5))
    p1.p3.m4.f6()
    log.warning("exiting method " + __name__+" at warning level")
