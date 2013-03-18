//
//  U13DebugBreak.m
//  U13Log
//
//  Created by Brane on 13-02-12.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import "U13DebugBreak.h"

#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>

static int U13_DEBUG_BREAK_ENABLED = true;


void U13DebugBreakSetEnabled(int enabled) {
    U13_DEBUG_BREAK_ENABLED = enabled;
}


#ifdef DEBUG


// code from
// http://developer.apple.com/library/mac/#qa/qa2004/qa1361.html

int AmIBeingDebugged(void)
// Returns true if the current process is being debugged (either
// running under the debugger or has a debugger attached post facto).
{
    int                 junk;
    int                 mib[4];
    struct kinfo_proc   info;
    size_t              size;

	if (!U13_DEBUG_BREAK_ENABLED)
        return false;
    
    // Initialize the flags so that, if sysctl fails for some bizarre
    // reason, we get a predictable result.
	
    info.kp_proc.p_flag = 0;
	
    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.
	
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();
	
    // Call sysctl.
	
    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    assert(junk == 0);
	
    // We're being debugged if the P_TRACED flag is set.
    return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
}

#endif
