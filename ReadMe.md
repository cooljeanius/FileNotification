**FileNotification Read Me**

Kqueue notifications were introduced in Mac OS X 10.3 to provide a
file notification API. The general recommended usage is
to create a separate thread, open a file descriptor to the file system object,
and then block on the call to kevent.

Looking at the sample source code:
Within `DisplaySimpleWindow()` we first initialize our `MyMPTaskInfo` structure
used to pass information into our Carbon MultiProcessing thread
(or \"MP thread\" for short, which will be the term used for it hereafter),
which may not be accessible or safe to initialize from our thread.
We then create the MP thread passing in the initialized `MyMPTaskInfo`
structure.

Most of the interesting work is done in the small `MyMPTask()` routine on
an MP thread. Here we define the events we are interested in watching,
open file descriptors to the directories we are watching,
and block on the call to `kevent()`. If any of the defined events occurs
to one of the file descriptors that we have opened,
the call to `kevent()` then unblocks, and we then post the
information back to the main event queue for display.
The routine `PostKQueueEvent()` is responsible for setting up our
custom CarbonEvent and posting it to the main event queue.
After the event is posted, we block on our call to `kevent()` again until
another event is received.

**When should the kqueue mechanism be used?**

This solution is ideal for watching drop boxes,
or maintaining up-to-date status of a number of directories.
For instance, if your application presents views similar to the Finder views,
rather than polling for changes to each viewed directory,
you should consider a kqueue notification mechanism instead.

**There are limitations to be aware of:**

Because a file descriptor must be opened for each file and folder
that is being watched it is not efficient or possible to use this mechanism
to watch all changes occurring to an entire disk.

If the folder that you are watching is on an NFS server,
you will NOT be notified of changes by other network users.
However, SMB and AFP do support a \"push\" model of notifications.

kqueues may not be supported by all file systems.

The fsevents API is generally preferred on newer versions of OS X,
although kqueues may still be useful for portability, or in cases where finer
granularity is needed.

**More kqueue information:**
Typing \"` man 2 kqueue `" in the Terminal will display the kqueue man page.

Either click the \"?\" button from the FileNotification sample application
to read the enclosed \"kqueue.pdf\" file, or open the file manually,
for more in-depth information on kqueues.
