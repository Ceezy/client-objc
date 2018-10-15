//
//  MKMAccount.h
//  MingKeMing
//
//  Created by Albert Moky on 2018/9/23.
//  Copyright © 2018 DIM Group. All rights reserved.
//

#import "MKMEntity.h"
#import "MKMProfile.h"

NS_ASSUME_NONNULL_BEGIN

#define MKM_IMMORTAL_HULK_ID @"hulk@4bejC3UratNYGoRagiw8Lj9xJrx8bq6nnN"
#define MKM_MONKEY_KING_ID   @"moki@4LrJHfGgDD6Ui3rWbPtftFabmN8damzRsi"

@class MKMPublicKey;

typedef NS_ENUM(SInt32, MKMAccountStatus) {
    MKMAccountStatusInitialized = 0,
    MKMAccountStatusRegistered = 1,
    MKMAccountStatusDead = -1,
};

@interface MKMAccount : MKMEntity {
    
    // profiles
    MKMAccountProfile *_profile;
    
    // parse the history to update status
    MKMAccountStatus _status;
    
    MKMPublicKey *_publicKey;
}

@property (readonly, strong, nonatomic) MKMAccountProfile *profile;
@property (readonly, nonatomic) MKMAccountStatus status;

@property (readonly, strong, nonatomic) MKMPublicKey *publicKey;

@end

@interface MKMAccount (Profile)

// special fields in profile
@property (readonly, strong, nonatomic) NSString *name;
@property (readonly, nonatomic) MKMGender gender;
@property (readonly, strong, nonatomic) NSString *avatar; // URL

@end

NS_ASSUME_NONNULL_END