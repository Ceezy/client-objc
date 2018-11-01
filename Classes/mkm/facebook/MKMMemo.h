//
//  MKMMemo.h
//  MingKeMing
//
//  Created by Albert Moky on 2018/9/30.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import "MKMDictionary.h"

NS_ASSUME_NONNULL_BEGIN

@class MKMID;

@interface MKMMemo : MKMDictionary

@property (readonly, strong, nonatomic) MKMID *ID;

- (instancetype)initWithID:(const MKMID *)ID;
- (instancetype)initWithDictionary:(NSDictionary *)dict;

/**
 Verify signature of this profile with public key of ID
 
 @param ID - Account ID
 @return YES/NO
 */
- (BOOL)matchID:(const MKMID *)ID;

@end

@interface MKMContactMemo : MKMMemo

@end

NS_ASSUME_NONNULL_END
