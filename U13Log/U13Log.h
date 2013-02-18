//
//  U13Log.h
//  U13Log
//
//  Created by Brane on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^UI13LogWillLogHandler)();

typedef uint64_t U13LOG_T_UNITS;

typedef enum {
	U13LOG_LEVEL_VERBOSE = 0,
	U13LOG_LEVEL_DEBUG,
	U13LOG_LEVEL_INFO,
	U13LOG_LEVEL_PERFORMANCE,
	U13LOG_LEVEL_WARNING,
	U13LOG_LEVEL_ERROR,
    U13LOG_LEVEL_COUNT
} U13LogLevel;

#ifdef UNIT_TEST
#define U13LOG_LEVEL_DEFAULT U13LOG_LEVEL_DEBUG
#else // ! UNIT_TEST
#ifdef DEBUG
#define U13LOG_LEVEL_DEFAULT U13LOG_LEVEL_DEBUG
#else // ! DEBUG
#define U13LOG_LEVEL_DEFAULT U13LOG_LEVEL_INFO
#endif // DEBUG
#endif // UNIT_TEST

/** Log extension for iOS
 
 Impement log levelling similar to Android.
 
 Adds simple measurement methods.
 
 Adds code expiry.
 
 Use macros defined in Log.h instead of calling these class methods directly.
 
 */
@interface U13Log : NSObject

+ (U13LogLevel)level;
+ (void)setLevel:(U13LogLevel)level;
+ (NSString *)nameForLevel:(U13LogLevel)level;

+ (void)log:(int)level fn:(const char *)fn line:(int)line msg:(NSString *)msg;

+ (void)e:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

+ (void)w:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

+ (void)i:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

+ (void)t:(const char *)fn line:(NSInteger)line msg:(NSString *)msg start:(U13LOG_T_UNITS)start cutoff:(U13LOG_T_UNITS)cutoff;

+ (void)d:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

+ (void)v:(const char *)fn line:(NSInteger)line msg:(NSString *)msg;

+ (U13LOG_T_UNITS)tUnitsFromInterval:(NSTimeInterval)interval;


+ (BOOL)xpiredFrom:(NSString *)from;
+ (BOOL)xpiredOn:(NSString *)on;

+ (UI13LogWillLogHandler)willLogHandler;
+ (void)setWillLogHandler:(UI13LogWillLogHandler)handler;

#ifdef UNIT_TEST
+ (NSString *)lastLoggedMessage;
#endif // UNIT_TEST


@end

/** Macro definitions for UI13Log
 
 Why not rely entirely on variadic macros as NSLog does?
 
 The problem is the system can't distinguish between "My string has %@ in it" and "I want to replace this %@", and will always parse the first parameter as if it is a printf string format, then crash when trying to look up %@.
 
 So we use the non-*F methods for single-parameter log statements, to ensure those parameters are never parsed as if they were printf strings.
 
 A little more pain for peace of mind.
 */

#define LOG_E( msg_ ) [U13Log e:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_EF( fmt_, ... ) [U13Log e:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]

#define LOG_W( msg_ ) [U13Log w:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_WF( fmt_, ... ) [U13Log w:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]

#define LOG_I( msg_ ) [U13Log i:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_]
#define LOG_IF( fmt_, ... ) [U13Log i:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__]]

#define LOG_T_TIME() mach_absolute_time()
#define LOG_T( start_, msg_ ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_ start:start_ cutoff:0]
#define LOG_TF( start_, fmt_, ... ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__] start:start_ cutoff:0]
#define LOG_T_CUTOFF( cutoff_, start_, msg_ ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:msg_ start:start_ cutoff:cutoff_]
#define LOG_TF_CUTOFF( cutoff_, start_, fmt_, ... ) [U13Log t:__PRETTY_FUNCTION__ line:__LINE__ msg:[NSString stringWithFormat:fmt_, __VA_ARGS__] start:start_ cutoff:cutoff_]

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


