//
//  DIMUser.h
//  DIM
//
//  Created by Albert Moky on 2018/9/30.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import "MingKeMing.h"

NS_ASSUME_NONNULL_BEGIN

@class DIMInstantMessage;
@class DIMSecureMessage;
@class DIMCertifiedMessage;

@protocol DIMUser <MKMPrivateKey>

- (DIMInstantMessage *)decryptMessage:(const DIMSecureMessage *)msg;

- (DIMCertifiedMessage *)signMessage:(const DIMSecureMessage *)msg;

@end

@interface DIMUser : MKMUser <DIMUser>

+ (instancetype)userWithID:(const MKMID *)ID;

@end

@interface DIMUser (Passphrase)

- (MKMSymmetricKey *)cipherKeyForDecrpyt:(const DIMSecureMessage *)msg;

@end

NS_ASSUME_NONNULL_END
