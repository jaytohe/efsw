
Python Bindings for [efsw](https://github.com/SpartanJ/efsw)


# How to build

1. `mkdir build && cd build`
2. `cmake ..`
3. `make pyefsw`

Go to the `build` directory and find the shared library file starting with `pyefsw`


# Basic Python Example

```python3
import pyefsw
from time import sleep

class UpdateListener(pyefsw.FileWatchListener):
    #Called whenever a change in the watched directory occurs
	def handleFileAction(self, watchID, directory, filename, action, oldFilename):
		if action == pyefsw.Action.Add:
			print(f"{filename} was added to {directory}")

fileWatcher = pyefsw.FileWatcher(useGenericFileWatcher=False)
listener = UpdateListener()
watchID1 = fileWatcher.addWatch("/tmp", listener, recursive=False) # Track only changes to top-level of /tmp.
fileWatcher.watch() # Watch for changes

# Keep python thread alive
try:
	while(True):
		sleep(2) # sleep for 2 seconds
except KeyboardInterrupt:
    fileWatcher.removeWatch(watchID1)
```

