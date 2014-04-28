/*
 *  brief-kqueue-example.h
 *  FileNotification
 *
 *  Created by Eric Gallager on 4/28/14.
 *  Header for code derived from Apple sample code.
 */

#ifndef __BRIEF_KQUEUE_EXAMPLE_H__
#define __BRIEF_KQUEUE_EXAMPLE_H__ 1

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#ifndef flagstring
extern char *flagstring(int flags);
#endif /* !flagstring */

#endif /* !__BRIEF_KQUEUE_EXAMPLE_H__ */

/* EOF */
