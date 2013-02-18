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

#if (TARGET_IPHONE_SIMULATOR) && (defined DEBUG)

bool AmIBeingDebugged (void);

#if __ppc64__ || __ppc__
#define U13DebugBreak() \
if(AmIBeingDebugged()) \
{ \
__asm__("li r0, 20\nsc\nnop\nli r0, 37\nli r4, 2\nsc\nnop\n" \
: : : "memory","r0","r3","r4" ); \
}
#else
#define U13DebugBreak() if(AmIBeingDebugged()) {__asm__("int $3\n" : : );}
#endif

#else
#define U13DebugBreak()
#endif
