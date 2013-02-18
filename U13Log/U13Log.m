//
//  U13Log.m
//  U13Log
//
//  Created by Brane on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import "U13Log.h"

#include <mach/mach_time.h>
#import "U13DebugBreak.h"

static const NSTimeInterval XPIRE_FROM_INTERVAL = 365 * 24 * 60 * 60;

static int LOG_LEVEL = U13LOG_LEVEL_DEFAULT;

static NSString *LOG_LEVEL_ABBR[U13LOG_LEVEL_COUNT];
static NSString *LOG_LEVEL_NAMES[U13LOG_LEVEL_COUNT];

static UI13LogWillLogHandler WILL_LOG_HANDLER = nil;


#ifdef UNIT_TEST
static NSString *TEST_LAST_LOGGED_MESSAGE = nil;
#endif // UNIT_TEST

/** Singleton log
 
 This does not implement the standard iOS singleton pattern, on purpose.  That pattern changes between ARC and non-ARC, and there is nothing in this class would be affected by late init.
 
 */
@implementation U13Log


+ (void)initialize {
    LOG_LEVEL_ABBR[U13LOG_LEVEL_VERBOSE] = @"V";
    LOG_LEVEL_ABBR[U13LOG_LEVEL_DEBUG] = @"D";
    LOG_LEVEL_ABBR[U13LOG_LEVEL_INFO] = @"I";
    LOG_LEVEL_ABBR[U13LOG_LEVEL_PERFORMANCE] = @"P";
    LOG_LEVEL_ABBR[U13LOG_LEVEL_WARNING] = @"W";
    LOG_LEVEL_ABBR[U13LOG_LEVEL_ERROR] = @"E";
    
    
}

+ (U13LogLevel)level { return LOG_LEVEL; }
+ (void)setLevel:(U13LogLevel)level {
    LOG_LEVEL = level;
}

+ (NSString *)nameForLevel:(U13LogLevel)level {
    
    if (!LOG_LEVEL_NAMES[0]) {
        LOG_LEVEL_NAMES[U13LOG_LEVEL_VERBOSE] = NSLocalizedString( @"Verbose", @"Verbose" );
        LOG_LEVEL_NAMES[U13LOG_LEVEL_DEBUG] = NSLocalizedString( @"Debug", @"Debug" );
        LOG_LEVEL_NAMES[U13LOG_LEVEL_INFO] = NSLocalizedString( @"Info", @"Info" );
        LOG_LEVEL_NAMES[U13LOG_LEVEL_PERFORMANCE] = NSLocalizedString( @"Performance", @"Performance" );
        LOG_LEVEL_NAMES[U13LOG_LEVEL_WARNING] = NSLocalizedString( @"Warning", @"Warning" );
        LOG_LEVEL_NAMES[U13LOG_LEVEL_ERROR] = NSLocalizedString( @"Error", @"Error" );
    }
    
    return LOG_LEVEL_NAMES[level];
}


+ (void)log:(int)level fn:(const char *)fn line:(int)line msg:(NSString *)msg {
	
	if (level >= LOG_LEVEL) {
        
        if (WILL_LOG_HANDLER)
            WILL_LOG_HANDLER();
		
		NSLog( @"%@ %s:%d: %@", LOG_LEVEL_ABBR[level], fn, line, msg );
        
#ifdef UNIT_TEST
        TEST_LAST_LOGGED_MESSAGE = msg;
#endif // UNIT_TEST
	}
	
#ifdef DEBUG
	if (level == U13LOG_LEVEL_ERROR) {
#ifndef UNIT_TEST
		U13DebugBreak();
        // You are here because something is wrong.  Check your call stack.  You are welcome :)
#endif // ! UNIT_TEST
	}
#endif // DEBUG
}

+ (void)e:(const char *)fn line:(int)line msg:(NSString *)msg {
	[self log:U13LOG_LEVEL_ERROR fn:fn line:line msg:msg];
}

+ (void)w:(const char *)fn line:(int)line msg:(NSString *)msg {
	[self log:U13LOG_LEVEL_WARNING fn:fn line:line msg:msg];
}

+ (void)i:(const char *)fn line:(int)line msg:(NSString *)msg {
	[self log:U13LOG_LEVEL_INFO fn:fn line:line msg:msg];
}

+ (void)t:(const char *)fn line:(int)line msg:(NSString *)msg start:(U13LOG_T_UNITS)start cutoff:(U13LOG_T_UNITS)cutoff {
	U13LOG_T_UNITS duration = LOG_T_TIME() - start;
    if (duration > cutoff)
        [self log:U13LOG_LEVEL_PERFORMANCE fn:fn line:line msg:[NSString stringWithFormat:@"%@: %llu", msg, duration]];
}

+ (void)d:(const char *)fn line:(int)line msg:(NSString *)msg {
	[self log:U13LOG_LEVEL_DEBUG fn:fn line:line msg:msg];
}

+ (void)v:(const char *)fn line:(int)line msg:(NSString *)msg {
	[self log:U13LOG_LEVEL_VERBOSE fn:fn line:line msg:msg];
}

+ (U13LOG_T_UNITS)tUnitsFromInterval:(NSTimeInterval)interval {
    
    static mach_timebase_info_data_t sTimebaseInfo;
    
    // If this is the first time we've run, get the timebase.
    // We can use denom == 0 to indicate that sTimebaseInfo is
    // uninitialised because it makes no sense to have a zero
    // denominator is a fraction.
    
    if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
    
    // Do the maths. We hope that the multiplication doesn't
    // overflow; the price you pay for working in fixed point.
    
    U13LOG_T_UNITS result = (interval * 1000000000) * sTimebaseInfo.denom / sTimebaseInfo.numer;
    
    return result;
}

+ (NSDateFormatter *)expiryFormatter {
    static NSDateFormatter *fmt = nil;
    if (!fmt) {
        fmt = [[NSDateFormatter alloc] init];
        [fmt setDateFormat:@"yyyy-mm-dd"];
    }
    return fmt;
}

+ (BOOL)xpiredFrom:(NSString *)from {
    NSDate *test = [[self expiryFormatter] dateFromString:from];
    NSTimeInterval interval = [[NSDate date] timeIntervalSinceDate:test];
    return interval > XPIRE_FROM_INTERVAL;
}

+ (BOOL)xpiredOn:(NSString *)on {
    NSDate *test = [[self expiryFormatter] dateFromString:on];
    NSTimeInterval interval = [[NSDate date] timeIntervalSinceDate:test];
    return interval > 0;
}


+ (UI13LogWillLogHandler)willLogHandler {
    return WILL_LOG_HANDLER;
}

+ (void)setWillLogHandler:(UI13LogWillLogHandler)handler {
    WILL_LOG_HANDLER = [handler copy];
}

#ifdef UNIT_TEST
+ (NSString *)testLastLoggedMessage {
    return TEST_LAST_LOGGED_MESSAGE;
}
#endif // UNIT_TEST


@end

