/*
 *  brief-kqueue-example.c
 *  FileNotification
 *
 *  This specific file created by Eric Gallager on 4/28/14.
 *  Taken from Apple's File System Events Programming Guide in the Apple
 *  Developer Documentation
 */

#include "brief-kqueue-example.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#define NUM_EVENT_SLOTS 1
#define NUM_EVENT_FDS 1

#ifndef __BRIEF_KQUEUE_EXAMPLE_H__
/* prototype for later function (in case compiling without header) */
char *flagstring(int flags);
#endif /* !__BRIEF_KQUEUE_EXAMPLE_H__ */

/* main */
int main(int argc, char *argv[])
{
    char *path = argv[1];
    int kq;
    int event_fd;
    struct kevent events_to_monitor[NUM_EVENT_FDS];
    struct kevent event_data[NUM_EVENT_SLOTS];
    void *user_data;
    struct timespec timeout;
    unsigned int vnode_events;
    int num_files; /* used lower down */
    int continue_loop; /* also used lower down */

    if (argc != 2) {
        fprintf(stderr, "Usage: monitor <file_path>\n");
        exit(-1);
    }

    /* Open a kernel queue. */
    if ((kq = kqueue()) < 0) {
        fprintf(stderr, "Could not open kernel queue.  Error was %s.\n",
				strerror(errno));
    }

    /*
     * Open a file descriptor for the file/directory that you
     * want to monitor.
     */
    event_fd = open(path, O_EVTONLY);
    if (event_fd <=0) {
        fprintf(stderr,
				"Failed to open file %s for monitoring. Error was %s.\n",
				path, strerror(errno));
        exit(-1);
    }

    /*
     * The address in user_data will be copied into a field in the
     * event. If you are monitoring multiple files, you could,
     * for example, pass in different data structure for each file.
     * For this example, the path string is used.
     */
    user_data = path;

    /* Set the timeout to wake us every half second. */
    timeout.tv_sec = 0; /* 0 seconds */
    timeout.tv_nsec = 500000000; /* 500 milliseconds */

    /* Set up a list of events to monitor. */
    vnode_events = (NOTE_DELETE |  NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE);
    EV_SET(&events_to_monitor[0], event_fd, EVFILT_VNODE, (EV_ADD | EV_CLEAR),
		   vnode_events, 0, user_data);

    /* Handle events. */
    num_files = 1;
    continue_loop = 40; /* Monitor for twenty seconds. */
    while (--continue_loop) {
        int event_count = kevent(kq, events_to_monitor, NUM_EVENT_SLOTS,
								 event_data, num_files, &timeout);
        if ((event_count < 0) || (event_data[0].flags == EV_ERROR)) {
            /* An error occurred. */
            fprintf(stderr,
					"An error occurred (event count %d). The error was %s.\n",
					event_count, strerror(errno));
            break;
        }
        if (event_count) {
            printf("Event %" PRIdPTR " occurred.  Filter %d, flags %d, filter flags %s, filter data %" PRIdPTR ", path %s\n",
				   event_data[0].ident,
				   event_data[0].filter,
				   event_data[0].flags,
				   flagstring((int)event_data[0].fflags),
				   event_data[0].data,
				   (char *)event_data[0].udata);
        } else {
            printf("No event.\n");
        }

        /* Reset the timeout. In case of a signal interrruption, the
         * values may change. */
        timeout.tv_sec = 0; /* 0 seconds */
        timeout.tv_nsec = 500000000; /* 500 milliseconds */
    }
    close(event_fd);
    return 0;
}

/* A simple routine to return a string for a set of flags. */
char *flagstring(int flags)
{
    static char ret[512];
    char *or = "";

    ret[0] = '\0'; /* clear the string. */
    if (flags & NOTE_DELETE) {
		strcat(ret, or);
		strcat(ret, "NOTE_DELETE");
		or = "|";
	}
    if (flags & NOTE_WRITE) {
		strcat(ret, or);
		strcat(ret, "NOTE_WRITE");
		or = "|";
	}
    if (flags & NOTE_EXTEND) {
		strcat(ret, or);
		strcat(ret, "NOTE_EXTEND");
		or = "|";
	}
    if (flags & NOTE_ATTRIB) {
		strcat(ret, or);
		strcat(ret, "NOTE_ATTRIB");
		or="|";
	}
    if (flags & NOTE_LINK) {
		strcat(ret, or);
		strcat(ret, "NOTE_LINK");
		or = "|";
	}
    if (flags & NOTE_RENAME) {
		strcat(ret, or);
		strcat(ret, "NOTE_RENAME");
		or = "|";
	}
    if (flags & NOTE_REVOKE) {
		strcat(ret, or);
		strcat(ret, "NOTE_REVOKE");
		or = "|";
	}

	/* dummy condition to use value stored to 'or': */
	if (or == NULL) {
		;
	}

    return ret;
}

/* EOF */
