//
//  DIMContact.h
//  DIM
//
//  Created by Albert Moky on 2018/9/30.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import "MingKeMing.h"

NS_ASSUME_NONNULL_BEGIN

#define DIMContactWithID(ID) (DIMContact *)MKMContactWithID(ID)

@class DIMInstantMessage;
@class DIMSecureMessage;
@class DIMCertifiedMessage;

@protocol DIMContact <MKMPublicKey>

- (DIMSecureMessage *)encryptMessage:(const DIMInstantMessage *)msg;

- (DIMSecureMessage *)verifyMessage:(const DIMCertifiedMessage *)msg;

@end

@interface DIMContact : MKMContact <DIMContact>

@end

@interface DIMContact (Passphrase)

- (MKMSymmetricKey *)cipherKeyForEncrypt:(const DIMInstantMessage *)msg;

@end

NS_ASSUME_NONNULL_END
