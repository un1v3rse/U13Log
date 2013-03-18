//
//  U13DebugBreak.h
//  U13Log
//
//  Created by Brane on 13-02-12.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#include "TargetConditionals.h"

// code based on this reference:
// http://cocoawithlove.com/2008/03/break-into-debugger.html


void U13DebugBreakSetEnabled(int enabled);

#ifdef DEBUG

int AmIBeingDebugged (void);

#if __ppc64__ || __ppc__

#if (TARGET_IPHONE_SIMULATOR)
#define U13DebugBreak() \
if(AmIBeingDebugged()) \
{ \
__asm__("li r0, 20\nsc\nnop\nli r0, 37\nli r4, 2\nsc\nnop\n" \
: : : "memory","r0","r3","r4" ); \
}
#else // ! TARGET_IPHONE_SIMULATOR, no break
#define U13DebugBreak()
#endif // TARGET_IPHONE_SIMULATOR

#else // ! __ppc64__ || __ppc__, i.e. OS X

#define U13DebugBreak() if(AmIBeingDebugged()) {__asm__("int $3\n" : : );}

#endif // __ppc64__ || __ppc__

#else // ! DEBUG

#define U13DebugBreak()

#endif // DEBUG
