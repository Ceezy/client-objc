//
//  MingKeMing.h
//  MingKeMing
//
//  Created by Albert Moky on 2018/9/30.
//  Copyright © 2018 DIM Group. All rights reserved.
//

// Extends
//#import "NSObject+Singleton.h"
//#import "NSObject+JsON.h"
//#import "NSData+Crypto.h"
//#import "NSString+Crypto.h"
//#import "NSArray+Merkle.h"

// Types
//#import "MKMString.h"
//#import "MKMArray.h"
//#import "MKMDictionary.h"

// Cryptography
#import "MKMCryptographyKey.h"
#import "MKMSymmetricKey.h"
#import "MKMAsymmetricKey.h"
#import "MKMPublicKey.h"
#import "MKMPrivateKey.h"
//#import "MKMAESKey.h"
//#import "MKMRSAPublicKey.h"
//#import "MKMRSAPrivateKey.h"
//#import "MKMECCPublicKey.h"
//#import "MKMECCPrivateKey.h"

// Entity
#import "MKMID.h"
#import "MKMAddress.h"
#import "MKMMeta.h"
#import "MKMEntity.h"

// History
#import "MKMHistoryOperation.h"
#import "MKMHistoryTransaction.h"
#import "MKMHistoryBlock.h"
#import "MKMHistory.h"
#import "MKMEntityHistoryDelegate.h"
#import "MKMAccountHistoryDelegate.h"
#import "MKMSocialEntityHistoryDelegate.h"
#import "MKMGroupHistoryDelegate.h"
#import "MKMConsensus.h"

// group
#import "MKMSocialEntity.h"
#import "MKMGroup.h"
#import "MKMMember.h"

// user
#import "MKMAccount.h"
#import "MKMUser.h"
#import "MKMContact.h"

// facebook
#import "MKMProfile.h"
#import "MKMBarrack.h"

NS_ASSUME_NONNULL_BEGIN

#define MKM_VERSION 0x00010100

// free functions
NSString * mkmVersion(void);

NS_ASSUME_NONNULL_END
