### Contents
- [Task](#task)
- [Research](#research)
- [Build](#build)
- [Important](#important)
- [ToDo](#todo)

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

Although this was not mentioned in the assignment, I decided not to use third-party libraries.

Speaking about limitations, FSEvents is more preferable because it doesn't have such [scalability problems](https://github.com/emcrisostomo/fswatch#limitations) as kqueue:
> kqueue requires a file descriptor to be opened for every file being watched. As a result, this monitor scales badly with the number of files being observed, and may begin to misbehave as soon as a process runs out of file descriptors.
That's why I continued with FSEvents.

# Build
The project is made in Xcode, so it's simple to use with `fsmonitor.xcodeproj`.

But it's not a problem to build it differently somehow, e.g.:
```
$ cd macos-assignment/fsmonitor/fsmonitor
$ gcc main.cpp -framework CoreServices
$ ./a.out /some/path /some/another/path
```

# Important
File system events have a bit different meaning compared to user's expectations.
For example, moving a `/Users/admin/file` to Bin is not removing but renaming to `/Users/admin/.Trash/file`.
So to catch a removing event we need to actually delete smth:
```
touch file
rm file
```
The output will be:
```
+ /path/file
- /path/file
```

# ToDo
The code is pretty raw, there are many ways to improve it:
- exit if no args
- provide exit command
- handle moving to .Trash case
- etc.
