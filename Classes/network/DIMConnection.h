//
//  DIMConnection.h
//  DIM
//
//  Created by Albert Moky on 2018/10/18.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class DIMStation;

@protocol DIMConnectionDelegate;

/**
 Connection to process connect/send/receive...
 */
@protocol DIMConnection <NSObject>

@property (readonly, strong, nonatomic) DIMStation *target;
@property (readonly, nonatomic, getter=isConnected) BOOL connected;

/**
 Connect to a station

 @param station - station will replace the target
 @return YES on success
 */
- (BOOL)connectTo:(DIMStation *)station;

/**
 (Re)connect to the target station

 @return YES on success
 */
- (BOOL)connect;

/**
 Close this connectiion
 */
- (void)close;

/**
 Send data to the target station
 
 @param jsonData - json data pack
 @return YES on success
 */
- (BOOL)sendData:(const NSData *)jsonData;

@end

#pragma mark - Base Connection

@interface DIMConnection : NSObject <DIMConnection> {
    
    DIMStation * _target;
    BOOL _connected;
}

@property (weak, nonatomic) id<DIMConnectionDelegate> delegate;

- (instancetype)initWithTargetStation:(DIMStation *)station
NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
