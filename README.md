Python Bindings for ![efsw](https://web.ensoft.dev/efsw/efsw-logo.svg)


# How to build

1. `mkdir build && cd build`
2. `cmake ..`
3. `make`
Go to the `build directory` and find the shared library starting with `pyefsw`


# Basic Python Example

```python3
import pyefsw
from time import sleep
import signal

class UpdateListener(pyefsw.FileWatchListener):
    #handleFileAction will be called whenever a change in the watched directory occurs
	def handleFileAction(self, watchID, directory, filename, action, oldFilename):
		if directory and filename:
			print(directory, filename)

fileWatcher = pyefsw.FileWatcher(False) # Do not use generic File watcher
listener = UpdateListener()
watchID1 = fileWatcher.addWatch("/tmp", listener, False) # Track only changes to top-level of /tmp.
fileWatcher.watch() # Watch for changes

# Keep python thread alive
try:
	while(True):
		sleep(2) # sleep for 2 seconds
except KeyboardInterrupt:
    fileWatcher.removeWatch(watchID1)
```
