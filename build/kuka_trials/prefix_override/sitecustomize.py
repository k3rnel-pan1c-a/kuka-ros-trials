import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/cortex/kuka_ws/src/kuka-ros-trials/install/kuka_trials'
