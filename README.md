### Contents
- [Task](#task)
- [Research](#research)

# Task
Implement a small C++ program that takes an arbitrary number of paths
to directories as command-line arguments and monitors file system
events in them. The program should generate an alert to standard
output whenever a file is created or removed. The alert must include
the event type and the path to the associated file.

Example:
```
$ ./fsmonitor /tmp /var
- /tmp/I31fJWd
+ /var/vm/swapfile0
$
```

Optional: Show a simple UI alert for each event.
	
	
Provide these deliverables packaged in a gzipped tarball:
 * The program in one or multiple files.
 * README.txt containing build instructions.
 * Additional files if needed.

# Research
There are many ways to track changes in macOS file system:
- FSEvents: [link](https://developer.apple.com/documentation/coreservices/file_system_events?language=objc), [link](https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/FSEvents_ProgGuide/UsingtheFSEventsFramework/UsingtheFSEventsFramework.html)
- kqueue(2): [link](http://man.openbsd.org/kqueue.2), [link](https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/FSEvents_ProgGuide/KernelQueues/KernelQueues.html)
- Third-party libs:
  - [fswatch](https://github.com/emcrisostomo/fswatch): FSEvents + kqueue
  - [watchman](https://github.com/facebook/watchman): FSEvents on 10.7+, kqueue on earlier versions (from [docs](https://facebook.github.io/watchman/docs/install.html#system-requirements))
  - [kqwait](https://github.com/facebook/watchman): kqueue
  - [FSEventsParser](https://github.com/gurinderhans/SwiftFSWatcher): FSEvents
  - others: [SwiftFSWatcher](https://github.com/gurinderhans/SwiftFSWatcher) for Swift (FSEvents), [watchdog](https://github.com/gorakhargosh/watchdog) for Python (FSEvents, kqueue), [chokidar](https://github.com/paulmillr/chokidar) for Node.js (FSEvents)
- Workarounds (ideas to do at least smth):
  - repeatedly check for SHA of `ls -l` of each file ([example](https://gist.github.com/thiagoh/5d8f53bfb64985b94e5bc8b3844dba55))
  - `fs_usage -f filesys` not sure about it, I didn't get smth from lots of `fsgetpath` and `getattrlist` records (plus it's too verbose, plus it requires `sudo`)
  - `lsof`? `dtrace`?
  - [entr(1)](http://eradman.com/entrproject/) to run arbitrary commands when files change
  - load `plist` to `launchctl` ([example](https://tech.saigonist.com/b/code/how-watch-file-changes-and-trigger-shell-command-any-modification-os-x-and-linux.html)). How to `unload` in case of `SIGKILL`?
