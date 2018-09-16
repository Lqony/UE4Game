// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Runtime/Core/Public/Templates/Function.h"
#include "PaperFlipbookComponent.h"
#include "Net/UnrealNetwork.h"
#include <cmath>
#include "SPHitBoxCPP.h"
#include "CreatureMeshComponent.h"
#include "SPPawnCPP.generated.h"

DECLARE_DYNAMIC_DELEGATE(FActionFunction);

class ASPPawnCPP;
typedef  void (ASPPawnCPP::*PawnActions)();
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

USTRUCT(BlueprintType)
struct FSPPanActions {
	GENERATED_BODY()

	float delay; 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction DelayAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction StopMove;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction StopJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction TouchGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction LeaveGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction ReleaseDefence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction Dash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction AirDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction SpotDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction LightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction UpperLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction DownLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction AirLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction AirUpperLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction AirDownLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction RealeaseStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FActionFunction StrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction SideStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction UpperStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction DownStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction AirStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction AirSideStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction AirUpperStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction AirDownStrongAttack;
};

USTRUCT(BlueprintType)
struct FSPKeyStates {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool LEFT_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool RIGHT_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool UP_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool DOWN_KEY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool LATTACK_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool SATTACK_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool JUMP_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	bool DEFENCE_KEY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool SUPER_LEFT_KEY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool  SUPER_RIGHT_KEY;
};

USTRUCT(BlueprintType)
struct FSPAnimationDetails {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//MeshRelativeLocation
		FVector2D FlipbookRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D HitBoxRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString AnimationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Loop;
};

USTRUCT(BlueprintType)
struct FSPPawnAttributes {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float MoveSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float JumpPower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		int AirJumpAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float Friction = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float AirFriction = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float Gravity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//How much percent are injures lowered
		float Tenacity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//How Much damage defence can get
		float Defence = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//Dash Speed
		float Dash = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//Dash Time 
		float DashTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//Spot Dodge Time 
		float SpotDodgeTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//Jump Time 
		float JumpTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float MaxGravity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float LaunchJumpTime = 0;
};

USTRUCT(BlueprintType)
struct FSPPawnStates {

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool MOVE_RIGHT = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool MOVE_LEFT = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool JUMP = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool JUMP_LEFT_WALL = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool JUMP_RIGHT_WALL = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool ON_GROUND = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//Character can not do anything from player commands while busy
		bool BUSY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//IF Can Move or no
		bool CAN_MOVE = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//IF Can jump or no
		bool CAN_JUMP = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool CAN_LIGHT_ATTACK = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool CAN_STRONG_ATTACK = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool CAN_DEFENCE = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool CAN_DASH = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool DEFENCE = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool STRONG_ATTACK = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool LIGHT_ATTACK = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool DASH = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool SIDE_DASH = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool UP_DASH = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool DOWN_DASH = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool SPOT_DODGE = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool AFFECTED_BY_GRAVITY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		bool SUPER_MOVE = false;
};

USTRUCT(BlueprintType)
struct FSPServerKeys {

	GENERATED_BODY()

	float KeyDelta[20];
	int KeyID[20];
};

USTRUCT(BlueprintType)
struct FSPWorkData {

	GENERATED_BODY()

		FVector PossitionError;
		float HitStun = 0.0f;
		//+1 every 0.1 second
		int StrongAttackMeter = 0;
		bool WasHit = false;
		FVector HitForce;
		bool AddingForce = false;
		FVector AddForce;
		int AirJumped;
		bool IsLocal;
		bool FacingRight;
		float CurrentDefence = 0.0f;
		//More injuried you are the further you fly after getting hit and how long hit stun last, 
		//(SOME CHAPMIONS MAY USED TO SOME ADDITIONAL THINGS)
		//You can not add to hit stun (you can only hit stun unstun enemy, if he is already hit stun then hit stun wont replenish
		int Injuries = 0;
		float DefenceDelta = 0.0f;

		bool CanJumpAgain = true;

		bool JumpTimer;
		float JumpTimerDelta;

		bool StrongAttackTimer;
		float StrongAttackTimerDelta;

		bool DashTimer;
		float DashTimerDelta;
		float DashTimerGoal;
		int DashTimerStage;

		bool ClientTimer;
		float ClientTimerDelta;
		float ClientTimerGoal;
		int ClientTimerStage;

		bool DelayTimer;
		float DelayTimerDelta;
		float DelayTimerGoal;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//So we can run pre-jump animation
		bool JumpStart;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//If we jumped at least a bit
		bool HasJumped;
		//So we can check if Y direction was changed
		bool YUp;
		//Aerial recovery
		bool UpB;

		//So we can slow down or speed up time (only for pawn)
		bool TimeTimer;
		float TimeTimerDelta;
		float TimeTimerGoal;
		float TimeChange;
		float CurrentPlayRate;
};

USTRUCT(BlueprintType)
struct FSPStaticPawnAttributes {

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//How much movement speed per movespeed attribute
		float MovementScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//How much movement speed per movespeed attribute
		float AirMovementScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float WallJumpYModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float WallJumpXModifier;
};

UCLASS()
class SUPERFIGHTER_API ASPPawnCPP : public APawn
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FVector2D Forces;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FSPPawnAttributes Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FSPPawnStates States;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FSPStaticPawnAttributes StaticAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FSPWorkData WorkData;

	UPROPERTY(ReplicatedUsing = RepNot_UpdatePosition, EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FVector ClientPosition;
	UPROPERTY(ReplicatedUsing = RepNot_UpdateForces, EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FVector2D ClientForces;

	UPROPERTY(ReplicatedUsing = RepNot_UpdateCurrentDefence, EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		float ClientCurrentDefence;
	UPROPERTY(ReplicatedUsing = RepNot_UpdateInjuries, EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		int ClientInjuries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FSPKeyStates KeyStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
	FSPKeyStates LastKeyStates;
	FSPServerKeys KeyTimers;

	//Apply Forces working on player
	void ApplyForces(float DeltaTime);
	void CalculateMovement(float DeltaTime);
	void CheckKeyStates();

	//Return value of "value" giving its value is per second and we calc it per deltaTime
	float ValuePerSecond(float value, float deltaTime);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetAttributes(FSPPawnAttributes new_attributes);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void Move(bool right, bool super);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void StopMove();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ChangeAnimation(FSPAnimationDetails details);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void QuickChangeAnimation(UPaperFlipbook *Flipbook, FVector2D HitBox);
	
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ChangeAnimationWithLocation(UPaperFlipbook *Flipbook, FVector2D HitBox, FVector2D RelativeLocation);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ChangeMovementSpeed(float speed);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void AddForce(FVector force);

	void Friction(float DeltaTime);
	void Gravity(float DeltaTime);

	void FixPossitionError();

	void SetUpDefence();
	void ClearDefence();
	void UseDefence();
	void ReplenishDefence();

	void SetUpDash();
	void StopDash();
	void DashColdown();

	
	void StartLightAttack();
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
	void EndLightAttack();

	void SetUpStrongAttack();
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
	void ClearStrongAttack();
	void UpgradeStrongAttackMeter();

	void ClearStatesWhileHit();

	void ManageSyncFrames(float DeltaTime);

	void UpdateTimers(float DeltaTime);

	void ManageStunState(float DeltaTime);
	void ManageDefence(float DeltaTime);
	void ManageJump(float DeltaTime);
	void ManageLightAttack(float DeltaTime);
	void ManageStrongAttack(float DeltaTime);
	void ManageDash(float DeltaTime);
	void ManageClient(float DeltaTime);
	void ManageDelay(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		//Call on the beggining of every action, It block all possible moves so you need to unlock only those that are 
		//avaiable on this action and not worry about disabling ever other by hand
	void StartNewAction();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ResetRestrictions();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ResetForces(bool n_X, bool n_Y) { if (n_X) Forces.X = 0.0f; if (n_Y) Forces.Y = 0.0f;};

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASPHitBoxCPP> BPHitBoxClass;

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		//Will check if FacingLeft and will change xmodifier to -xmodifier and force.x tp - force.x if so, Loc Z is radius
		ASPHitBoxCPP* CreateHitBox(FVector LocationModifier, FVector Force, float ActivationTime, float DestroyTime,
			bool FriendlyFire, bool MultiHit, float Damage, float HitStun, bool followPlayer);
		ASPHitBoxCPP* SpawnHitBox(FSPHitBoxDetails Details);

	//ACTIONS---------------------------------------------------------
	float DelayActionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionStopMove;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionStopJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionTouchGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionLeaveGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionDefence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionReleaseDefence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionSpotDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionUpperLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionDownLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirUpperLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirDownLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionRealeaseStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionSideStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionUpperStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionDownStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirSideStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirUpperStrongAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		FActionFunction ActionAirDownStrongAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//When Y Forces Goes from + to - (or from 0 to + or from 0 to -)
		FActionFunction ActionYDirectionChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperFighter)
		//For Run When XAxis is >. 0.8
		FActionFunction ActionSuperMove;
	//END ACTIONS -----------------------------------------------------

public:

	// Sets default values for this pawn's properties
	ASPPawnCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SuperFighter)
		UBoxComponent *collision_box;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SuperFighter)
		UBoxComponent *hit_box;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SuperFighter)
		UCreatureMeshComponent *creature_mesh;

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void Jump();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void MakeBusy();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void UnBusy();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetCanJump(bool can) { if (States.CAN_JUMP != can) States.CAN_JUMP = can; };

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetCanMove(bool can) { if (States.CAN_MOVE != can) States.CAN_MOVE = can; };

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetCanLightAttack(bool can) { if (States.CAN_LIGHT_ATTACK != can) States.CAN_LIGHT_ATTACK = can; };

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetCanStrongAttack(bool can) { if(States.CAN_STRONG_ATTACK != can) States.CAN_STRONG_ATTACK = can; };

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetCanDash(bool can) { if (!WorkData.DashTimer && States.CAN_DASH != can) States.CAN_DASH = can; };

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetCanDefence(bool can) { if (States.CAN_DEFENCE != can) States.CAN_DEFENCE = can; };

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void StopJump();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void LightAttack(int index = 0/*0 - check, 1 - normal, 2 - upper, 3 - down*/);

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_LightAttack();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_UpperLightAttack();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_DownLightAttack();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void StrongAttack(int index = 0/*0 - check, 1 - strong attack, 2 - side , 3 - upper, 4 - down*/);

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_StrongAttack();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_SideStrongAttack();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_UpperStrongAttack();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_DownStrongAttack();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ReleaseStrongAttack();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_ReleaseStrongAttack();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void Defence(int index = 0/*0 - check, 1 - defend, 2 - sidedash, 3 - updash, 4 - downdash/spotdodge*/);

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_Defence();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_SideDash();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_UpDash();

	UFUNCTION(Server, unreliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_DownDash();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void ReleaseDefence();

	UFUNCTION(Server, reliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_ReleaseDefence();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void Hit(float forceX, float forcey);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		bool GroundUnderFeet() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		bool GroundNextToFeet(bool right) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void DrawStunMeter(float Radius);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void PlayClientAnimation(int index);

	void SetAsLocal() { WorkData.IsLocal = true; }

	void StopDashForces();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
	bool IsLocal() { return WorkData.IsLocal;  }

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
	void CallEndViewTarget();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void Local_Move(float AxisX);

	UFUNCTION()
	void RepNot_UpdatePosition();
	UFUNCTION()
		void RepNot_UpdateForces();
	UFUNCTION()
	void RepNot_UpdateCurrentDefence();
	UFUNCTION()
	void RepNot_UpdateInjuries();

	UFUNCTION(Server, unreliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void Server_Move(float AxisX);

	UFUNCTION(Server, unreliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void Server_MoveLeft();
		
	UFUNCTION(Server, unreliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void Server_MoveRight();

	UFUNCTION(Server, unreliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void Server_SuperMoveLeft();

	UFUNCTION(Server, unreliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void Server_SuperMoveRight();

	UFUNCTION(Server, reliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void Server_StopMove();

	UFUNCTION(Server, reliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_Jump();

	UFUNCTION(Server, reliable, WithValidation)
		//Server Will Only detect the jump that clients asks for
		void Server_StopJump();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void HitPunch(bool FromClient = false, FVector2D ClientAxisPosition = FVector2D(0, 0));

	UFUNCTION(Server, unreliable, WithValidation)
		void Server_HitPunch(FVector2D AxisPosition);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		FVector2D AxisPosition();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void DodgeBlink(bool start);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void DrawDefence();

	void HitPosition(FVector2D AxisPosition, FVector& Position, FVector& Force);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		//Called After Pawn Spawn as action to call idle state of character
		void SetUpIdle();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		//Must be called everytime we change anything here
		void ResetActions(float delay_delta);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void CallDelayAction();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void CallTouchGround();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void CallLeaveGround();

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_Dash(FVector2D n_Position, int index /*0 - side dash, 1 - up dash, 2 - down dash, 3 - spot dodge*/);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_Move(FVector2D n_Position, int index /*0 - right, 1 - left*/, bool super);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_StopMove(FVector2D n_Position);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_Jump(FVector2D n_Position, int index/*0 - jump, 1 - right wall, 2 -left wall*/);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_StopJump(FVector2D n_Position);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_Defence(FVector2D n_Position);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_StopDefence(FVector2D n_Position);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_LightAttack(FVector2D n_Position, int index /*0 - normal, 1 - up, 2 - down, 3 - normal air, 4 - up airm 5 - down air*/ );
		
	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_StrongAttack(FVector2D n_Position, int index /*0 - normal, 1 - side, 2 - up, 3 - down, 4 - air normal, 5 - air side, 6 - air up, 7 - air down */);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_StopStrongAttack(FVector2D n_Position, int StrongAttackMeter);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_CallDelayAction(FVector2D n_Position);

	UFUNCTION(NetMulticast, reliable, WithValidation, Category = SuperFighter)
		void Client_GetHit(FVector2D n_Position, FVector n_KnockBack, float n_HitStun);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void GetHit(float hitstun, float damage, FVector knockback);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void EndStun();

	void CallActionFunction(ASPPawnCPP& o, PawnActions p) {
		CALL_MEMBER_FN(o, p)();
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void ChangeAnimationRotation();

	UFUNCTION(NetMulticast, reliable, WithValidation, BlueprintCallable, Category = SuperFighter)
		void LooseStock();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = SuperFighter)
		void DashEnd();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
	bool FacingRight();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		float StrongAttackMeter();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		float CurrentDefence();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		float CurrentHitStun();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		//Called after pre-jump animation is done
		void StartJump() {
		WorkData.JumpStart = true;
		WorkData.HasJumped = false;
	};

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		int Injuries() {
		return WorkData.Injuries;
	}

	FVector2D GetSendPosition();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		//For Client we mainly use ClientStates because we Call events based on comparing States to Client states
		//Then in those Evenets we want to check most current states for server those are States
		//And for client those are ClientStates and after we call the events we finaly do states=clientstates
	FSPPawnStates CurrentStates() { return States; };

	void ChangeGravityAffection(bool new_grav) { if (States.AFFECTED_BY_GRAVITY != new_grav) States.AFFECTED_BY_GRAVITY = new_grav; }
	bool AffectedByGravity() { return States.AFFECTED_BY_GRAVITY;  }

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
	bool IsStun();
	bool CanMove();
	bool CanStopMove();
	bool CanDelayAction();
	bool CanJump();
	bool CanStopJump();
	bool CanLightAttack();
	bool CanStrongAttack();
	bool CanReleaseStrongAttack();
	bool CanDefence();
	bool CanReleaseDefence();
	bool CanDash();

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetSuperLeftKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetSuperRightKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetLeftKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetRightKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetUpKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetDownKey(bool state);

	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetLAttackKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetSAttackKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetDefenceKey(bool state);
	UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SetJumpKey(bool state);

	
		void ManageServerKeyStates(float DeltaTime);
		void ChangeServerKeyState(int ID);
		void SetNextServerKeyState(int ID, float DeltaTime);

		void CheckYDirection();

		UFUNCTION(BlueprintCallable, Category = SuperFighter)
			void SetTimeChange(float NewTime, float LastFor) { 
			WorkData.TimeChange = NewTime; WorkData.TimeTimer = true; 
			WorkData.TimeTimerDelta = 0.0f; WorkData.TimeTimerGoal = LastFor; 
			creature_mesh->animation_speed = WorkData.CurrentPlayRate / WorkData.TimeChange;
		}
		UFUNCTION(BlueprintCallable, Category = SuperFighter)
			float TimeChange() { return WorkData.TimeChange; }
		UFUNCTION(BlueprintCallable, Category = SuperFighter)
			void SetCurrentPlayRate(float rate);

		void ManageTimeChange(float DeltaTime);
		UFUNCTION(BlueprintCallable, Category = SuperFighter)
		void SlowTimeByForce(float force);
		void SlowTimeAfterHit(float force);
};
