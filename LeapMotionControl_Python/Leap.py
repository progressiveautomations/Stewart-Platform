import platform

osname = platform.system()

if osname == 'Windows':
	from win64 import *
	
elif osname == 'Darwin':
	from darwin64 import *
	
else:
	raise ImportError('Unsupported platform')