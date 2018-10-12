//
//  DIMKeyStore.h
//  DIM
//
//  Created by Albert Moky on 2018/10/12.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import "MingKeMing.h"

NS_ASSUME_NONNULL_BEGIN

@interface DIMKeyStore : NSObject

+ (instancetype)sharedInstance;

/**
 Get PK for contact to encrypt or verify message
 
 @param contact - contact with ID
 @return PK
 */
- (MKMPublicKey *)publicKeyForContact:(const MKMContact *)contact;

/**
 Get SK for user to decrypt or sign message
 
 @param user - user with ID
 @return SK
 */
- (MKMPrivateKey *)privateKeyForUser:(const MKMUser *)user;

- (void)setPrivateKey:(MKMPrivateKey *)SK
              forUser:(const MKMUser *)user;

/**
 Get PW for contact or group to encrypt or decrypt message
 
 @param entity - entity with ID
 @return PW
 */
- (MKMSymmetricKey *)passphraseForEntity:(const MKMEntity *)entity;

- (void)setPassphrase:(MKMSymmetricKey *)PW
            forEntity:(const MKMEntity *)entity;

/**
 Get encrypted SK for user to store elsewhere
 
 @param user - user with ID
 @param scKey - password to encrypt the SK
 @return KS
 */
- (NSData *)privateKeyStoredForUser:(const MKMUser *)user
                         passphrase:(const MKMSymmetricKey *)scKey;

@end

NS_ASSUME_NONNULL_END
