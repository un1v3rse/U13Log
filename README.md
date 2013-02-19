# U13Log

A simple logging extension for iOS.

Features:

- log levels
- break on error
- better assertion than NSAssert (stops on the assert rather than crashing the app)
- simple process timing
- simple code expiry
- log EVERYTHING (including normal NSLog calls) to a rotating set of files (optional)

More information in the source-level documentation.  See Documentation below to generate AppleDocs if you prefer.


## Why Yet ANOTHER Logger?

Mainly, I wanted to play around with GitHub before I post something more substantial... stay tuned.

But also, there were drawbacks to all of the frameworks I've looked at:

1.  Fricking huge ([NSLogger](https://github.com/fpillet/NSLogger),[Lumberjack](https://github.com/robbiehanson/CocoaLumberjack)).  All I wanted was something like [this](http://brenwill.com/2010/flexible-ios-logging/).  Does a logger really have to be so complicated?!!

2.  Asynchronous execution (NSLogger, [TestFlight](https://testflightapp.com/sdk/), optional in Lumberjack) under pretext of performance.  Async logging produces the all too often encountered possibility that the log entry that was right before a hard crash did not get written.  If you're doing so much logging that you have a performance problem, learn how to use the debugger and stop that.

3.  Use a replacement to NSLog which preserves the crashy behaviour of `NSLog(@"%@")` (Lumberjack,TestFlight).  I want simple, easy to verify rules for my team, in this case "if your code contains `NSLog`, it is wrong."

4.  File-level logging requires a redefine of all NSLog calls to use the new library's calls (NSLogger,Lumberjack,TestFlight) .  Useless if you want log statements from iOS or third party libraries. 


My goals for this library are these:


### Compatibility  

Third party code will not need to change to use the features of the library.  

NSLog can be freely intermixed with the library's log calls.  NSLog calls will still get logged to a file.


### Simplicity

Problems that can be solved without multi-threading, should be.  I've never needed a high performance logger (even on 68K Palm OS!), and I believe that if you need one, you're logging too much.


### Enforced code quality

Forcing non-format strings to a separate method prevents crashes.  

Forcing the debugger to stop on error means errors get looked at rather than lost in a stream of 1000 log messages.

Breaking assertions instead of crashing increases the available information about the assertion state, reducing the time to fix.

Code expiry means files get maintained and complexity is reduced.


## Installation

Copy the project into your source tree.

Include these files in your project by dragging them in:

- U13DebugBreak.m/h
- U13Log.m/h

In your prefix file, add

    #import "U13Log.h"


If you want file-based logging:

- U13Log2File.m/h

and modify your main.m file:

    int main(int argc, char *argv[])
    {
		// this is what you add for file-based logging
        @autoreleasepool {
            [U13Log2File setup];
        }

        @autoreleasepool {
            return UIApplicationMain(argc, argv, nil, NSStringFromClass([U13AppDelegate class]));
        }
    }

Happy coding!


## Examples

Log an error (which will break to the debugger when encountered):

    LOG_E(@"Should not have hit this");


Log a warning with a format string (the separation of the format string from regular reporting avoids a bug in NSLog):

    LOG_WF(@"I don't think %@ was supposed to %@", @"this", @"happen");


Assert a state (which will break to the debugger when failed, not crash like NSAssert does):

    LOG_A( handle, @"Handle should never be nil" );


Time a process:

    LOG_T_UNITS start = LOG_T_TIME();
    
    // do some time-intensive process
    
    LOG_T(start, @"Meme catalogue complete");


Mark some code for expiry (the debugger will break here when the date is past):

    LOG_XPIRE_ON( @"2014-02-14", @"Happy Valentine's day, clean out this code it's obsolete now" );


## Documentation

Documentation requires installation of [appledoc](https://github.com/tomaz/appledoc)

On your command line, from the root of the project, run:

    ./docs.sh

to generate the docs to a folder named docs.


