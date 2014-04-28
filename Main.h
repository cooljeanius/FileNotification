/*
 *	File:		Main.h
 *
 *	Contains:	Typedefs and interfaces
 *
 *	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple
 *				Computer, Inc. ("Apple") in consideration of your agreement to
 *				the following terms, and your use, installation, modification
 *				or redistribution of this Apple software constitutes acceptance
 *				of these terms.  If you do not agree with these terms, please
 *				do not use, install, modify or redistribute this Apple software.
 *
 *				In consideration of your agreement to abide by the following
 *				terms, and subject to these terms, Apple grants you a personal,
 *				non-exclusive license, under Apple’s copyrights in this original
 *				Apple software (the "Apple Software"), to use, reproduce, modify
 *				and redistribute the Apple Software, with or without
 *				modifications, in source and/or binary forms; provided that if
 *				you redistribute the Apple Software in its entirety and without
 *				modifications, you must retain this notice and the following
 *				text and disclaimers in all such redistributions of the
 *				Apple Software.  Neither the name, trademarks, service marks or
 *				logos of Apple Computer, Inc. may be used to endorse or promote
 *				products derived from the Apple Software without specific prior
 *				written permission from Apple.  Except as expressly stated in
 *				this notice, no other rights or licenses, express or implied,
 *				are granted by Apple herein, including but not limited to any
 *				patent rights that may be infringed by your derivative works or
 *				by other works in which the Apple Software may be incorporated.
 *
 *				The Apple Software is provided by Apple on an "AS IS" basis.
 *				APPLE MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *				LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 *				MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING
 *				THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 *				COMBINATION WITH YOUR PRODUCTS.
 *
 *				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT,
 *				INCIDENTAL OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *				TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *				DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY
 *				OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
 *				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY
 *				OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR
 *				OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF
 *				SUCH DAMAGE.
 *
 *	Copyright © 2003 Apple Computer, Inc., All Rights Reserved
 */

#ifndef __MAIN_H__
#define __MAIN_H__ 1

#if (defined(__x86_64) || defined(__x86_64__)) && !defined(TARGET_CPU_X86)
# define TARGET_CPU_X86 64
#endif /* (__x86_64 || __x86_64__) && !TARGET_CPU_X86 */
#if (defined(__ppc64) || defined(__ppc64__)) && !defined(TARGET_CPU_PPC)
# define TARGET_CPU_PPC 64
#endif /* (__ppc64 || __ppc64__) && !TARGET_CPU_PPC */
#ifndef SIGDIGLEN
# define SIGDIGLEN 36 /* what it is for TARGET_CPU_PPC || TARGET_CPU_X86 */
#endif /* !SIGDIGLEN */
#if defined(__LP64__) && !defined(__LP64_SAVED__)
# define __LP64_SAVED__ 1
# undef __LP64__
# ifdef __MACHINEEXCEPTIONS__
#  undef __MACHINEEXCEPTIONS__
# endif /* __MACHINEEXCEPTIONS__ */
# define __MACHINEEXCEPTIONS__ 1
# ifndef AreaID
/* Some basic declarations used throughout the kernel */
typedef struct OpaqueAreaID* AreaID;
# endif /* !AreaID */
#endif /* __LP64__ && !__LP64_SAVED__ */
#include <Carbon/Carbon.h>
#if defined(__LP64_SAVED__) && !defined(__LP64__)
# define __LP64__ 1
#endif /* __LP64_SAVED__ && !__LP64__ */
#ifdef __LP64__
# include <CoreFoundation/CoreFoundation.h>
#endif /* __LP64__ */
#include <sys/param.h>

#ifndef __LP64__
# pragma options align=mac68k
#endif /* !__LP64__ */

#define kMaxFoldersToWatch	10

enum {
	kEventClassMP = '_MP_',
	kEventKQueue = 'KQEv',
  	typeKEvent = 'KEvt'
};

struct GlobalAppInfo
{
	CFBundleRef mainBundle;
	IBNibRef mainNib;
};
typedef struct GlobalAppInfo GlobalAppInfo;

/*  We use this structure to pass information between the threads throug
 * CarbonEvents: */
struct MPControlInfo
{
	EventTargetRef eventTarget;
	ControlRef dateControl;
};
typedef struct MPControlInfo MPControlInfo;

struct MyMPTaskInfo
{
	char path[kMaxFoldersToWatch][MAXPATHLEN];
	MPControlInfo mpControlInfo[kMaxFoldersToWatch];
	int count;
	Boolean done;
};
typedef struct MyMPTaskInfo MyMPTaskInfo;


/*	prototypes for functions from SampleUtils.c: */
OSStatus GetControlBySigAndID(WindowRef window, OSType signature, SInt32 id,
							  ControlRef *control);
CFStringRef	GetControlCFString(WindowRef window, OSType signature, SInt32 id);
void SetControlCFString(WindowRef window, OSType signature, SInt32 id,
						CFStringRef cfString);
char *GetControlCString(WindowRef window, OSType signature, SInt32 id,
						char *my_cString);
void SetControlCString(WindowRef window, OSType signature, SInt32 id,
					   char *my_cString);
void GetControlPString(WindowRef window, OSType signature, SInt32 id,
					   StringPtr pString);
void SetControlPString(WindowRef window, OSType signature, SInt32 id,
					   StringPtr pString);
SInt16 GetControlValueByID(WindowRef window, OSType signature, SInt32 id);
void SetControlValueByID(WindowRef window, OSType signature, SInt32 id,
						 SInt32 value);
void CFStringToStr255(CFStringRef input, StringPtr output);
void SendWindowCloseEvent(WindowRef window);
void SendCommandProcessEvent(UInt32 commandID);

#ifndef __LP64__
# pragma options align=reset
#endif /* !__LP64__ */
#endif /* !__MAIN_H__ */

/* EOF */
