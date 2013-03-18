//
//  U13Log.h
//  U13Log
//
//  Created by Brane on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <mach/mach_time.h>

typedef void(^UI13LogWillLogHandler)();

typedef uint64_t U13LOG_T_UNITS;

typedef enum {
    LOG_LEVEL_VERBOSE = 0,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_PERFORMANCE,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
    LOG_LEVEL_COUNT
} U13LogLevel;

#ifdef UNIT_TEST
#define LOG_LEVEL_DEFAULT LOG_LEVEL_DEBUG
#else // ! UNIT_TEST
#ifdef DEBUG
#define LOG_LEVEL_DEFAULT LOG_LEVEL_DEBUG
#else // ! DEBUG
#define LOG_LEVEL_DEFAULT LOG_LEVEL_INFO
#endif // DEBUG
#endif // UNIT_TEST

/** Log extension for iOS
 
 This is not a replacement for NSLog, it is an extension that calls NSLog.  Replacing NSLog is not advisable due to its unstable use of variadic macros (see A note on Macros below).
 
 ## Log levels
 
 Log levels loosely mirror those on Android:
 
 - Error - A condition that should never occur, has.
 - Warning - A condition that ought not to occur, has.
 - Info - Information about the program's operation, that we want to preserve in release builds.
 - Debug - Information about the program's operation, that will be stripped from release builds.
 - Verbose - a level for more detailed logging than Debug, otherwise behaves the same as Debug
 
 Levels are specified using macros containing the first letter of the level, e.g. `LOG_E(@"The foo should have had a bar!");`
 
 
 ## Break On Error
 
 When logging an Error-levelled entry in debug mode, the system will stop the debugger on the offending line.  Code should not produce errors, so when it does it's worth our immediate attention, even if we simply press Run to resume.  If you don't agree, please let me know so I can avoid your code.
 
 
 ## A better assert than NSAssert
 
 Following from being able to break on an error usefully, we can now assert and actually stop where the assertion occurred, rather than crashing and forcing the developer to decipher a stack trace (imagining what the variables might have been).
 
 LOG_A is used for this.
 
 
 ## Measurement
 
 LOG_T provides logging of process execution times.  For instance, it can be configured to write a log entry whenever a query takes more than an expected time.  This is not a profiler, Apple provides that already.
 
 
 ## Code Expiry
 
 A source of pain on many projects is code that is kept to preserve backward compatibility, that should be removed at some point.  By adding a statement of when the code should be removed, it is possible to let the debugger tell us when that time has been reached.  This is superior to deprecation warnings, which tend only to produce noise.
 
 
 ### A note on Macros
 
 U13Log uses two macro variants for logging, LOG_? and LOG_?F.  The former is for single-parameter log statements, the latter for printf-style statements.
 
 Why not rely entirely on a single variadic macro as NSLog does?
 
 The problem is the system can't distinguish between "My string has %@ in it" and "I want to replace this %@", and will always parse the first parameter as if it is a printf string format, then crash when trying to look up %@.  You can see this for yourself in [UI13LogTests testNSLogCrashOnInvalidFormatString].
 
 So we use the non-*F methods for single-parameter log statements, to ensure those parameters are never parsed as if they were printf strings.
 
 A little more pain for peace of mind.
 
 */
@interface U13Log : NSObject

+ (U13LogLevel)level;
+ (void)setLevel:(U13LogLevel)level;

+ (NSString *)nameForLevel:(U13LogLevel)level;

/** Log an arbitrary log level.
 
 Exposed for testing, should not normally be called directly.
 
 */
+ (void)log:(int)level fn:(const char *)fn line:(int)line msg:(NSString *)msg;

/** Log an error.
 
 Should not normally be called directly, use LOG_E or LOG_EF.
 
 */
+ (void)e:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

/** Log a warning.
 
 Should not normally be called directly, use LOG_W or LOG_WF.
 
 */
+ (void)w:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

/** Log information.
 
 Should not normally be called directly, use LOG_I or LOG_IF.
 
 */
+ (void)i:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

/** Log debug information.
 
 Should not normally be called directly, use LOG_D or LOG_DF.
 
 */
+ (void)d:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

/** Log verbose debug information.
 
 Should not normally be called directly, use LOG_V or LOG_VF.
 
 */
+ (void)v:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

/** Log a timing.
 
 Should not normally be called directly, use LOG_T, LOG_TF, LOG_T_CUTOFF, or LOG_TF_CUTOFF.
 
 @param start The time the process started.  This method should be called at the time the process ends, so no need to supply the end.
 @param cutoff Do not make a log entry if the time is below this threshold.
 
 */
+ (void)t:(const char *)fn line:(NSInteger)line msg:(NSString *)msg start:(U13LOG_T_UNITS)start cutoff:(U13LOG_T_UNITS)cutoff;

/** Convert a time interval to our time units.
 
 */
+ (U13LOG_T_UNITS)tUnitsFromInterval:(NSTimeInterval)interval;


/** Calculate if the provided date FROM indicates expiry.
 
 Should not normally be called directly, used by LOG_XPIRE_FROM
 
 */
+ (BOOL)xpiredFrom:(NSString *)from;

/** Calculate if the provided date ON indicates expiry.
 
 Should not normally be called directly, used by LOG_XPIRE_ON
 
 */
+ (BOOL)xpiredOn:(NSString *)on;


/** Get the current willLogHandler
 
 Intended to be used to see if a handler is already present.
 
 */
+ (UI13LogWillLogHandler)willLogHandler;


/** Set the current willLogHandler
 
 Used to fire events when logging happens.  Used by U13Log2File to control rotating the log.
 
 */
+ (void)setWillLogHandler:(UI13LogWillLogHandler)handler;


#ifdef UNIT_TEST
/** Get the text of the last logged message.
 
 Used for testing the system.
 
 */
+ (NSString *)testLastLoggedMessage;
#endif // UNIT_TEST


@end

#define LOG_LEVEL [U13Log level]
#define LOG_SET_LEVEL(level) [U13Log setLevel:level]


#define LOG_E( msg_ ) [U13Log e:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_EF( fmt_, ... ) [U13Log e:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]

#define LOG_W( msg_ ) [U13Log w:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_WF( fmt_, ... ) [U13Log w:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]

#define LOG_I( msg_ ) [U13Log i:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_IF( fmt_, ... ) [U13Log i:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]

#ifdef DEBUG
#define LOG_D( msg_ ) [U13Log d:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_DF( fmt_, ... ) [U13Log d:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]
#else
#define LOG_D( msg_ )
#define LOG_DF( fmt_, ... )
#endif

#ifdef DEBUG
#define LOG_V( msg_ ) [U13Log v:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_VF( fmt_, ... ) [U13Log v:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]
#else
#define LOG_V( msg_ )
#define LOG_VF( fmt_, ... )
#endif


// Assertions
#ifdef DEBUG
#define LOG_A( test_, msg_ ) if (!(test_)) { LOG_E( msg_ ); }
#define LOG_AF( test_, fmt_, ... ) if (!(test_)) { LOG_EF( fmt_, __VA_ARGS__ ); }
#else
#define LOG_A( test_, msg_ )
#define LOG_AF( test_, fmt_, ... )
#endif


// Timing
#define LOG_T_UNITS U13LOG_T_UNITS
#define LOG_T_TIME() mach_absolute_time()
#define LOG_T( start_, msg_ ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_ start:start_ cutoff:0]
#define LOG_TF( start_, fmt_, ... ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__] start:start_ cutoff:0]
#define LOG_T_CUTOFF( cutoff_, start_, msg_ ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_ start:start_ cutoff:cutoff_]
#define LOG_TF_CUTOFF( cutoff_, start_, fmt_, ... ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__] start:start_ cutoff:cutoff_]


// Code expiration policy
#ifdef DEBUG
#define LOG_XPIRE_FROM( from_, msg_ ) \
if ([U13Log xpired_from:from_]) \
LOG_E(msg_);
#define LOG_XPIRE_ON( on_, msg_ ) \
if ([U13Log xpired_on:on_]) \
LOG_E(msg_);
#else
#define LOG_XPIRE_FROM( from_, msg_ )
#define LOG_XPIRE_ON( on_, msg_ )
#endif


// Override NSAssert
//#undef NSAssert
//#define NSAssert LOG_A


