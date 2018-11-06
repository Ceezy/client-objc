//
//  MKMContact+Message.h
//  DIMCore
//
//  Created by Albert Moky on 2018/9/30.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import "MingKeMing.h"

NS_ASSUME_NONNULL_BEGIN

@class DIMInstantMessage;
@class DIMSecureMessage;
@class DIMCertifiedMessage;

@interface MKMContact (Message)

- (DIMSecureMessage *)encryptMessage:(const DIMInstantMessage *)msg;

- (DIMSecureMessage *)verifyMessage:(const DIMCertifiedMessage *)msg;

// passphrase
- (MKMSymmetricKey *)keyForEncryptMessage:(const DIMInstantMessage *)msg;

@end

NS_ASSUME_NONNULL_END
