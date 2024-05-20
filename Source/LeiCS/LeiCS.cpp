// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeiCS.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, LeiCS, "LeiCS");

/** Singles */
UE_DEFINE_GAMEPLAY_TAG(TAG_CanProcessInput, "CanProcessInput")
UE_DEFINE_GAMEPLAY_TAG(TAG_CanProcessDirectionalInput, "CanProcessDirectionalInput")
UE_DEFINE_GAMEPLAY_TAG(TAG_CanMove, "CanMove")
UE_DEFINE_GAMEPLAY_TAG(TAG_IsInParryWindow, "IsInParryWindow")
UE_DEFINE_GAMEPLAY_TAG(TAG_IsInHitWindow, "IsInHitWindow")
UE_DEFINE_GAMEPLAY_TAG(TAG_IsRecoveringAfterParry, "IsRecoveringAfterParry")

/** Actions */
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Attack, "Action.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_AttackState, "Action.AttackState")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_CombatState, "Action.CombatState")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Defense, "Action.Defense")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_DefenseState, "Action.DefenseState")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_LockTarget, "Action.LockTarget")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_None, "Action.None")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_ReceiveHit, "Action.ReceiveHit")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_ParryMovement, "Action.ParryMovement")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Rune, "Action.Rune")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_RuneState, "Action.RuneState")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Sprint, "Action.Sprint")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Dodge, "Action.Dodge")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_ReceiveStagger, "Action.ReceiveStagger")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_RecoverFromParry, "Action.RecoverFromParry")
UE_DEFINE_GAMEPLAY_TAG(TAG_Action_FireCleave, "Action.FireCleave")

/** Attributes */
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Attack, "Attribute.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Defense, "Attribute.Defense")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health, "Attribute.Health")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_MaxWalkSpeed, "Attribute.MaxWalkSpeed")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Poise, "Attribute.Poise")

/** Directions */
UE_DEFINE_GAMEPLAY_TAG(TAG_Direction_None, "Direction.None")
UE_DEFINE_GAMEPLAY_TAG(TAG_Direction_Down, "Direction.Down")
UE_DEFINE_GAMEPLAY_TAG(TAG_Direction_Left, "Direction.Left")
UE_DEFINE_GAMEPLAY_TAG(TAG_Direction_Right, "Direction.Right")
UE_DEFINE_GAMEPLAY_TAG(TAG_Direction_Up, "Direction.Up")

/** Gameplay States */
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayState_Default, "GameplayState.Default")
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayState_Attack, "GameplayState.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayState_Defense, "GameplayState.Defense")
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayState_Rune, "GameplayState.Rune")
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayState_Combat, "GameplayState.Combat")

/** Status */
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Sprint, "Status.Sprint")
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Airborne, "Status.Airborne")
UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Staggered, "Status.Staggered")