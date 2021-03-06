//
//  DIMTerminal+Group.m
//  DIMClient
//
//  Created by Albert Moky on 2019/3/9.
//  Copyright © 2019 DIM Group. All rights reserved.
//

#import "NSObject+JsON.h"
#import "NSData+Crypto.h"

#import "DIMTerminal+Group.h"

@implementation DIMTerminal (GroupManage)

- (BOOL)sendOutGroupID:(const DIMID *)groupID
                  meta:(const DIMMeta *)meta
               profile:(nullable const DIMProfile *)profile
               members:(const NSArray<const DIMID *> *)newMembers {
    NSAssert([meta matchID:groupID], @"meta not match group ID: %@, %@", groupID, meta);
    NSAssert(!profile || [profile.ID isEqual:groupID], @"profile not match group ID: %@, %@", groupID, profile);
    
    DIMGroup *group = DIMGroupWithID(groupID);
    DIMUser *user = self.currentUser;
    
    if (group.ID.type != MKMNetwork_Polylogue) {
        NSAssert(false, @"unsupported group type: %@", group.ID);
        return NO;
    }
    
    // checking founder
    const DIMID *founder = group.founder;
    if (![founder isValid] || !MKMNetwork_IsPerson(founder.type)) {
        NSAssert(false, @"invalid founder: %@", founder);
        return NO;
    }
    NSUInteger index = [newMembers indexOfObject:founder];
    if (index == NSNotFound) {
        NSAssert(false, @"the founder not found in the member list");
        // add the founder to the front of group members list
        NSMutableArray *mArray = [newMembers mutableCopy];
        [mArray insertObject:founder atIndex:0];
        newMembers = mArray;
    } else if (index != 0) {
        //NSAssert(false, @"the founder must be the first member");
        // move the founder to the front
        NSMutableArray *mArray = [newMembers mutableCopy];
        [mArray exchangeObjectAtIndex:index withObjectAtIndex:0];
        newMembers = mArray;
    } else {
        newMembers = [newMembers copy];
    }
    
    // checking expeled list with old members
    NSArray<const DIMID *> *members = group.members;
    NSMutableArray<const DIMID *> *expels;
    expels = [[NSMutableArray alloc] initWithCapacity:members.count];
    for (const DIMID *ID in members) {
        // if old member not in the new list, expel it
        if (![newMembers containsObject:ID]) {
            [expels addObject:ID];
        }
    }
    if (expels.count > 0) {
        // only the founder can expel members
        if (![group.founder isEqual:user.ID]) {
            NSLog(@"user (%@) not the founder of group: %@", user, group);
            return NO;
        }
        if ([expels containsObject:founder]) {
            NSLog(@"cannot expel founder (%@) of group: %@", group.founder, group);
            return NO;
        }
    }
    
    // check membership
    if (![group.founder isEqual:user.ID] && ![members containsObject:user.ID]) {
    //if (![group existsMember:user.ID]) {
        NSLog(@"user (%@) not a member of group: %@", user, group);
        return NO;
    }
    
    DIMMessageContent *cmd;
    
    // 1. send out meta & profile
    if (profile) {
        NSString *string = [profile jsonString];
        NSData *CT = [user.privateKey sign:[string data]];
        NSString *signature = [CT base64Encode];
        cmd = [[DIMProfileCommand alloc] initWithID:groupID
                                               meta:meta
                                            profile:string
                                          signature:signature];
    } else {
        cmd = [[DIMMetaCommand alloc] initWithID:groupID
                                            meta:meta];
    }
    
    // 1.1. share to station
    [self sendCommand:(DIMCommand *)cmd];
    
    // 1.2. send to each new member
    for (const DIMID *ID in newMembers) {
        [self sendContent:cmd to:ID];
    }
    
    // 2. send expel command to all old members
    if (members.count > 0) {
        if (expels.count > 0) {
            cmd = [[DIMExpelCommand alloc] initWithGroup:groupID members:expels];
            [cmd setObject:user.ID forKey:@"owner"];
            
            for (const DIMID *ID in members) {
                [self sendContent:cmd to:ID];
            }
        }
    }
    
    // 3. send invite command to all new members
    cmd = [[DIMInviteCommand alloc] initWithGroup:groupID members:newMembers];
    [cmd setObject:user.ID forKey:@"owner"];
    for (const DIMID *ID in newMembers) {
        [self sendContent:cmd to:ID];
    }
    
    return YES;
}

- (DIMGroup *)createGroupWithSeed:(const NSString *)seed
                          members:(const NSArray<const MKMID *> *)list
                          profile:(const NSDictionary *)dict {
    DIMUser *user = self.currentUser;
    
    // generate group meta with current user's private key
    DIMMeta *meta = [[DIMMeta alloc] initWithVersion:MKMMetaDefaultVersion
                                                seed:seed
                                          privateKey:user.privateKey
                                           publicKey:nil];
    // generate group ID
    const DIMID *ID = [meta buildIDWithNetworkID:MKMNetwork_Polylogue];
    // save meta for group ID
    DIMBarrack *barrack = [DIMBarrack sharedInstance];
    [barrack saveMeta:meta forEntityID:ID];
    
    // end out meta+profile command
    DIMProfile *profile;
    profile = [[DIMProfile alloc] initWithID:ID];
    //dict = [dict copy];
    for (NSString *key in dict) {
        if ([key isEqualToString:@"ID"]) {
            continue;
        }
        [profile setObject:[dict objectForKey:key] forKey:key];
    }
    NSLog(@"new group: %@, meta: %@, profile: %@", ID, meta, profile);
    BOOL sent = [self sendOutGroupID:ID meta:meta profile:profile members:list];
    if (!sent) {
        NSLog(@"failed to send out group: %@, %@, %@, %@", ID, meta, profile, list);
        return nil;
    }
    
    // create group
    return DIMGroupWithID(ID);
}

- (BOOL)updateGroupWithID:(const MKMID *)ID
                  members:(const NSArray<const MKMID *> *)list
                  profile:(const MKMProfile *)profile {
    DIMGroup *group = DIMGroupWithID(ID);
    const DIMMeta *meta = group.meta;
    NSLog(@"update group: %@, meta: %@, profile: %@", ID, meta, profile);
    BOOL sent = [self sendOutGroupID:ID meta:meta profile:profile members:list];
    if (!sent) {
        NSLog(@"failed to send out group: %@, %@, %@, %@", ID, meta, profile, list);
        return NO;
    }
    
    return YES;
}

@end

@implementation DIMTerminal (GroupHistory)

- (BOOL)checkPolylogueCommand:(DKDMessageContent *)content
                    commander:(const MKMID *)sender {
    const DIMID *groupID = [DIMID IDWithID:content.group];
    DIMGroup *group = DIMGroupWithID(groupID);
    NSString *command = content.command;
    
    // check founder
    BOOL isFounder = [group isFounder:sender];
    
    // check membership
    BOOL isMember = [group existsMember:sender];
    
    if ([command isEqualToString:@"invite"]) {
        // add member(s)
        if (isFounder || isMember) {
            return YES;
        } else {
            NSLog(@"!!! only the founder or member can invite other members");
            return NO;
        }
    } else if ([command isEqualToString:@"expel"]) {
        // remove member(s)
        if (isFounder) {
            return YES;
        } else {
            NSLog(@"!!! only the founder(owner) can expel members");
            return NO;
        }
    } else if ([command isEqualToString:@"quit"]) {
        // remove member
        if (isFounder) {
            NSLog(@"founder can not quit from polylogue: %@", group);
            return NO;
        } else if (isMember) {
            return YES;
        } else {
            NSLog(@"!!! you are not a member yet");
            return NO;
        }
    } else {
        NSAssert(false, @"unknown command: %@", command);
    }
    
    return NO;
}

- (BOOL)checkGroupCommand:(DKDMessageContent *)content
                commander:(const MKMID *)sender {
    const DIMID *groupID = [DIMID IDWithID:content.group];
    
    if (groupID.type == MKMNetwork_Polylogue) {
        return [self checkPolylogueCommand:content commander:sender];
    } else if (groupID.type == MKMNetwork_Chatroom) {
        // TODO: check by group history consensus
    }
    
    NSAssert(false, @"unsupport group type: %@", groupID);
    return NO;
}

@end
