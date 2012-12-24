#!/usr/bin/python3

import os

for name, dirs, files in os.walk('examples'):
    for sample in files:
        if sample.endswith('.pas'):
            print('===', 'executing', sample, '===')
            os.system('./pascal examples/' + sample)
            