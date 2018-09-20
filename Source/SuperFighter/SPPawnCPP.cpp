// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "SPPawnCPP.h"

// Sets default values
ASPPawnCPP::ASPPawnCPP()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collision_box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = collision_box;
	hit_box = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit_Box"));
	hit_box->SetupAttachment(RootComponent);
	hit_box->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	creature_mesh = CreateDefaultSubobject<UCreatureMeshComponent>(TEXT("Mesh"));
	creature_mesh->SetupAttachment(RootComponent);
	creature_mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Forces.X = 0.0f;
	Forces.Y = 0.0f;
	ClientPosition = GetActorLocation();
	ClientForces = Forces;

	StaticAttributes.MovementScale = 10.0f;
	StaticAttributes.AirMovementScale = 4.0f;
	StaticAttributes.WallJumpXModifier = 2.0f;
	StaticAttributes.WallJumpYModifier = 1.5f;

	WorkData.AirJumped = 0;
	WorkData.IsLocal = false;
	WorkData.FacingRight = true;
	WorkData.Injuries = 0;
	WorkData.StrongAttackMeter = 0;
	WorkData.PossitionError = FVector(0.0f, 0.0f, 0.0f);
	WorkData.HitStun = 0.0f;
	WorkData.CurrentDefence = 0.0f;
	WorkData.WasHit = false;
	WorkData.HitForce = FVector(0.0f, 0.0, 0.0f);
	WorkData.AddingForce = false;
	WorkData.AddForce = FVector(0.0f, 0.0, 0.0f);
	WorkData.DefenceDelta = 0.0f;
	WorkData.JumpTimer = false;
	WorkData.JumpTimerDelta = 0.0f;
	WorkData.StrongAttackTimer = false;
	WorkData.StrongAttackTimerDelta = 0.0f;
	WorkData.DashTimer = false;
	WorkData.DashTimerDelta = 0.0f;
	WorkData.DashTimerGoal = 0.0f;
	WorkData.DashTimerStage = 0;

	WorkData.ClientTimer = false;
	WorkData.ClientTimerDelta = 0.0f;
	WorkData.ClientTimerGoal = 0.0f;
	WorkData.ClientTimerStage = 0;
	
	WorkData.DelayTimer = false;
	WorkData.DelayTimerDelta = 0.0f;
	WorkData.DelayTimerGoal = 0.0f;

	WorkData.JumpStart = false;
	WorkData.HasJumped = false;
	WorkData.YUp = true;
	WorkData.UpB = true;

	WorkData.TimeTimer = false;
	WorkData.TimeChange = 1.0f;

	ClientCurrentDefence = 0.0f;
	ClientInjuries = 0.0f;

	DelayActionTime = 0.0f;

	
	States.MOVE_RIGHT = false;
	States.MOVE_LEFT = false;
	States.SUPER_MOVE = false;
	States.JUMP = false;
	States.JUMP_LEFT_WALL = false;
	States.JUMP_RIGHT_WALL = false;
	States.ON_GROUND = false;
	States.BUSY = false;
	States.CAN_MOVE = true;
	States.CAN_JUMP = true;
	States.CAN_LIGHT_ATTACK = true;
	States.CAN_STRONG_ATTACK = true;
	States.CAN_DEFENCE = true;
	States.CAN_DASH = true;
	States.DEFENCE = false;
	States.STRONG_ATTACK = false;
	States.LIGHT_ATTACK = false;
	States.DASH = false;
	States.SIDE_DASH = false;
	States.UP_DASH = false;
	States.DOWN_DASH = false;
	States.SPOT_DODGE = false;
	States.AFFECTED_BY_GRAVITY = true;

	KeyStates.LEFT_KEY = false;
	KeyStates.RIGHT_KEY = false;
	KeyStates.SUPER_LEFT_KEY = false;
	KeyStates.SUPER_RIGHT_KEY = false;
	KeyStates.UP_KEY = false;
	KeyStates.DOWN_KEY = false;

	KeyStates.LATTACK_KEY = false;
	KeyStates.SATTACK_KEY = false;
	KeyStates.JUMP_KEY = false;
	KeyStates.DEFENCE_KEY = false;

	LastKeyStates.LEFT_KEY = false;
	LastKeyStates.RIGHT_KEY = false;
	LastKeyStates.SUPER_LEFT_KEY = false;
	LastKeyStates.SUPER_RIGHT_KEY = false;
	LastKeyStates.UP_KEY = false;
	LastKeyStates.DOWN_KEY = false;

	LastKeyStates.LATTACK_KEY = false;
	LastKeyStates.SATTACK_KEY = false;
	LastKeyStates.JUMP_KEY = false;
	LastKeyStates.DEFENCE_KEY = false;

	for (int i = 0; i < 20; i++) {
		KeyTimers.KeyDelta[i] = 0.0f;
		KeyTimers.KeyID[i] = -1;
	}

	Attributes.MaxGravity = 0.0f;
}

// Called when the game starts or when spawned
void ASPPawnCPP::BeginPlay()
{
	Super::BeginPlay();
	SetUpIdle();
}

// Called every frame
void ASPPawnCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority()) {
		ApplyForces(DeltaTime/WorkData.TimeChange);
		Friction(DeltaTime / WorkData.TimeChange);
		Gravity(DeltaTime / WorkData.TimeChange);
		CalculateMovement(DeltaTime);
		UpdateTimers(DeltaTime);
		if(GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
		CheckKeyStates();
		CheckYDirection();

		if (States.DEFENCE) {
			DrawDefence();
		}
		if (Forces.X != 0.0f || Forces.Y != 0.0f) {
			ClientPosition = GetActorLocation();
			ClientForces = Forces;
		}
	}
	else {
		ApplyForces(DeltaTime / WorkData.TimeChange);
		Friction(DeltaTime / WorkData.TimeChange);
		Gravity(DeltaTime / WorkData.TimeChange);
		CalculateMovement(DeltaTime);
		UpdateTimers(DeltaTime);
		if (GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
		CheckKeyStates();
		CheckYDirection();

		if (States.DEFENCE) {
			DrawDefence();
		}
		

		//FixPossitionError();
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, WorkData.PossitionError.ToCompactString());
		/*AController *check = GetController();
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if(IsValid(check2))
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::SanitizeFloat(check->PlayerState->Ping));
		}*/
	}
}

// Called to bind functionality to input
void ASPPawnCPP::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void ASPPawnCPP::DrawStunMeter_Implementation(float Radius)
{
}


void ASPPawnCPP::PlayClientAnimation_Implementation(int index)
{
}

void ASPPawnCPP::StopDashForces()
{
		if (Forces.X > 0) {
			if ( Forces.X <= Attributes.Dash * 2.0f) {
				Forces.X = 0.0f;
			}
		}
		else {
			if (Forces.X >= -Attributes.Dash* 2.0f) {
				Forces.X = 0.0f;
			}
		}

		if (Forces.Y > 0) {
			if (Forces.Y <= Attributes.Dash* 2.0f) {
				Forces.Y = 0.0f;
			}
		}
		else {
			if (Forces.Y >= -Attributes.Dash* 2.0f) {
				Forces.Y = 0.0f;
			}
		}
}

void ASPPawnCPP::CallEndViewTarget()
{
	if (HasAuthority()) {
		if (IsLocal()) {
			APlayerController* LocalPlayer = UGameplayStatics::GetPlayerController(this, 0);
			LocalPlayer->SetViewTarget(this);		
		}
	}
}

void ASPPawnCPP::Local_Move(float AxisX)
{
	if (CanMove()) {
		if (AxisX == 0 && (States.MOVE_LEFT || States.MOVE_RIGHT)) {
			Server_Move(AxisX);
		}
		else if (AxisX > 0.0f && AxisX > 0.2f && !States.MOVE_RIGHT) {
			Server_Move(AxisX);
		}
		else if (AxisX < 0.0f && AxisX < -0.2f && !States.MOVE_LEFT) {
			Server_Move(AxisX);
		}
	}
}

void ASPPawnCPP::RepNot_UpdatePosition()
{
	if (!HasAuthority()) {
		if (!States.DASH ) {
		SetActorLocation(ClientPosition);
		}
	}
}

void ASPPawnCPP::RepNot_UpdateForces()
{
	if (!HasAuthority()) {
		if (!States.DASH) {
			Forces = ClientForces;
		}
	}
}




void ASPPawnCPP::RepNot_UpdateCurrentDefence()
{
	if (!HasAuthority()) {
		WorkData.CurrentDefence = ClientCurrentDefence;
	}
}

void ASPPawnCPP::RepNot_UpdateInjuries()
{
	if (!HasAuthority()) {
		WorkData.Injuries = ClientInjuries;
	}
}


void ASPPawnCPP::Server_MoveRight_Implementation()
{
	Move(true, false);
}

bool ASPPawnCPP::Server_MoveRight_Validate()
{
	return true;
}

void ASPPawnCPP::Server_MoveLeft_Implementation()
{
	Move(false, false);
}

bool ASPPawnCPP::Server_MoveLeft_Validate()
{
	return true;
}

void ASPPawnCPP::Server_SuperMoveRight_Implementation()
{
	Move(true, true);
}

bool ASPPawnCPP::Server_SuperMoveRight_Validate()
{
	return true;
}

void ASPPawnCPP::Server_SuperMoveLeft_Implementation()
{
	Move(false, true);
}

bool ASPPawnCPP::Server_SuperMoveLeft_Validate()
{
	return true;
}

void ASPPawnCPP::Server_StopMove_Implementation()
{
	StopMove();
}

bool ASPPawnCPP::Server_StopMove_Validate()
{
	return true;
}

void ASPPawnCPP::HitPunch(bool FromClient, FVector2D ClientAxisPosition)
{
	if (HasAuthority()) {

		/*FSPHitBoxDetails HBDetails;
		FVector self_position;
		FVector self_bounds;
		FVector temp;


		HBDetails.ExistTime = 0.1f;
		HBDetails.FriendlyFire = false;
		HBDetails.HitStun = 0.0f;
		HBDetails.MultiHit = false;
		HBDetails.Owner = this;

		if (FromClient) {
			HitPosition(ClientAxisPosition, HBDetails.Position, HBDetails.Force);
		}
		else {
			HitPosition(AxisPosition(), HBDetails.Position, HBDetails.Force);
		}
		
		SpawnHitBox(HBDetails);
		
	}
	else {
		Server_HitPunch(AxisPosition());
	*/}
}

void ASPPawnCPP::ManageStunState(float DeltaTime)
{
	
		WorkData.HitStun -= DeltaTime;
		if (WorkData.HitStun <= 0.0f) {
			WorkData.HitStun = 0.0f;
			
			EndStun();
			
		}
		else {
			float StunMeterRadius = (WorkData.HitStun * 50.0f) - 5;
			DrawStunMeter(StunMeterRadius);
		}
}

void ASPPawnCPP::ManageDefence(float DeltaTime)
{
	if (States.DEFENCE) {
		if (WorkData.CurrentDefence > 0.0f) {
			WorkData.DefenceDelta += DeltaTime;
			if (WorkData.DefenceDelta >= 0.2f) {
				WorkData.CurrentDefence -= (Attributes.Defence / 10.0f) * 0.2f;
					WorkData.DefenceDelta -= 0.2f;
					if (WorkData.CurrentDefence <= 0.0f) {
						ReleaseDefence();
						WorkData.CurrentDefence = 0.0f;
					}
			}
		}
	}
	else {
		if (WorkData.CurrentDefence < Attributes.Defence) {
			WorkData.DefenceDelta += DeltaTime;
			if (WorkData.DefenceDelta >= 0.2f) {
				WorkData.CurrentDefence += (Attributes.Defence / 5.0f) * 0.2f;
				WorkData.DefenceDelta -= 0.2f;
				if (WorkData.CurrentDefence > Attributes.Defence) {
					WorkData.CurrentDefence = Attributes.Defence;
				}
			}
		}

	}
}

void ASPPawnCPP::ManageJump(float DeltaTime)
{
	WorkData.JumpTimerDelta += DeltaTime;
	if (!WorkData.JumpStart && WorkData.JumpTimerDelta >= Attributes.LaunchJumpTime) {
		WorkData.JumpTimerDelta = 0.0f;
		StartJump();
	}
	else if (WorkData.JumpTimerDelta >= Attributes.JumpTime) {
		WorkData.JumpTimerDelta = 0.0f;
		WorkData.JumpTimer = false;
		StopJump();
	}
}

void ASPPawnCPP::ManageStrongAttack(float DeltaTime)
{
	WorkData.StrongAttackTimerDelta += DeltaTime;
	while(WorkData.StrongAttackTimerDelta >= 0.05f) {
		WorkData.StrongAttackTimerDelta -= 0.05f;
		UpgradeStrongAttackMeter();
	}
}

void ASPPawnCPP::ManageDash(float DeltaTime)
{
	WorkData.DashTimerDelta += DeltaTime;
	if (WorkData.DashTimerStage == 0) {
		if (WorkData.DashTimerDelta >= WorkData.DashTimerGoal) {
			WorkData.DashTimerDelta = 0.0f;
			WorkData.DashTimerGoal = 0.0f;
			WorkData.DashTimerStage = 0;
			WorkData.DashTimer = false;

			StopDash();
		}
	}
	else {
		if (WorkData.DashTimerDelta >= WorkData.DashTimerGoal) {
			WorkData.DashTimerDelta = 0.0f;
			WorkData.DashTimerGoal = 0.0f;
			WorkData.DashTimerStage = 0;
			WorkData.DashTimer = false;

			DashColdown();
		}
	}
}

void ASPPawnCPP::ManageClient(float DeltaTime)
{
	if (!HasAuthority()) {
		WorkData.ClientTimerDelta += DeltaTime; 
		if (WorkData.ClientTimerDelta >= WorkData.ClientTimerGoal) {
			WorkData.ClientTimer = false;
			WorkData.ClientTimerDelta = 0.0f;
			WorkData.ClientTimerGoal = 0.0f;

			if (WorkData.ClientTimerStage == 0) {
				ReleaseDefence();
			}
			else if (WorkData.ClientTimerStage == 1) {
				StopJump();
			}
			else if (WorkData.ClientTimerStage == 2) {
				StopMove();
			}
		}
	}
}

void ASPPawnCPP::ManageDelay(float DeltaTime)
{
	if (HasAuthority()) {
		WorkData.DelayTimerDelta += DeltaTime;
		if (WorkData.DelayTimerDelta >= WorkData.DelayTimerGoal) {
			WorkData.DelayTimer = false;
			WorkData.DelayTimerDelta = 0.0f;
			CallDelayAction();
		}
	}
}

void ASPPawnCPP::StartNewAction()
{
	States.CAN_DASH = false;
	States.CAN_DEFENCE = false;
	States.CAN_JUMP = false;
	States.CAN_LIGHT_ATTACK = false;
	States.CAN_MOVE = false;
	States.CAN_STRONG_ATTACK = false;
	WorkData.DelayTimer = false;
}

void ASPPawnCPP::ResetRestrictions()
{
	UnBusy();
	States.CAN_DASH = true;
	States.CAN_DEFENCE = true;
	States.CAN_JUMP = true;
	States.CAN_LIGHT_ATTACK = true;
	States.CAN_MOVE = true;
	States.CAN_STRONG_ATTACK = true;
}

ASPHitBoxCPP* ASPPawnCPP::CreateHitBox(FVector LocationModifier, FVector Force, float ActivationTime, float DestroyTime, bool
	FriendlyFire, bool MultiHit, float Damage, float HitStun, bool followPlayer)
{
	FSPHitBoxDetails Details;
	FVector CurrentPosition;
	FVector2D FolowDistance;
	if (!FacingRight()) {
		LocationModifier.X = -LocationModifier.X;
		Force.X = -Force.X;
	}

	if (Force.X != 0.0f && Forces.X != 0.0f) {
		if ((Force.X > 0.0f && Forces.X > 0.0f) || (Force.X < 0.0f && Forces.X < 0.0f)) {
			Force.X += Forces.X / Force.Z;
		}
	}
	if (Force.Y != 0.0f && Forces.Y != 0.0f) {
		if ((Force.Y > 0.0f && Forces.Y > 0.0f) || (Force.Y < 0.0f && Forces.Y < 0.0f)) {
			Force.Y += Forces.Y / Force.Z;
		}
	}

	CurrentPosition = GetActorLocation();
	FolowDistance.X = LocationModifier.X;
	FolowDistance.Y = LocationModifier.Y;
	LocationModifier.X += CurrentPosition.X;
	LocationModifier.Y += CurrentPosition.Z;

	Details.Position = LocationModifier;
	Details.Force = Force;
	Details.ActivationTime = ActivationTime;
	Details.DestroyTime = DestroyTime;
	Details.FriendlyFire = FriendlyFire;
	Details.MultiHit = MultiHit;
	Details.Owner = this;
	Details.Damage = Damage;
	Details.HitStun = HitStun;
	Details.FollowPlayer = followPlayer;
	Details.FollowDistance = FolowDistance;

	return SpawnHitBox(Details);
}

ASPHitBoxCPP * ASPPawnCPP::SpawnHitBox(FSPHitBoxDetails Details)
{	
	ASPHitBoxCPP *HitBox;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	HitBox = GetWorld()->SpawnActor<ASPHitBoxCPP>(BPHitBoxClass, Location, Rotation, SpawnInfo);
	HitBox->SetHitbox(Details);
	return HitBox;
}

void ASPPawnCPP::DodgeBlink_Implementation(bool start)
{
	if (start) {
		//animation->SetSpriteColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "FIX DODGE BLINK");
		
	}
	else {
		//animation->SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void ASPPawnCPP::DrawDefence_Implementation()
{
}

void ASPPawnCPP::HitPosition(FVector2D AxisPosition, FVector& Position, FVector& Force)
{
	FVector2D axis_position = AxisPosition;
	FVector self_position;
	self_position = GetActorLocation();

	Position.Y = 35.0f; //Spehere radius
	Force.Y = 0.0f;
	if (axis_position.X > 1 || axis_position.X < -1) axis_position.X = 0.0f;
	if (axis_position.Y > 1 || axis_position.X < -1) axis_position.Y = 0.0f;

	if (axis_position.X == 0 && axis_position.Y == 0) {

		Position.X = self_position.X;
		Position.Z = self_position.Z;
		Force.Z = 0.0f;
		if (WorkData.FacingRight) {
			Position.X += 35.0f;
			Force.X = 1000.0f;
		}
		else {
			Position.X -= 35.0f;
			Force.X = -1000.0f;
		}

		
	}
	else{
		FVector2D temp_axis = axis_position;
		if (temp_axis.X < 0) temp_axis.X *= -1.0f;
		if (temp_axis.Y < 0) temp_axis.Y *= -1.0f;
		Force.X = 1000.0f * axis_position.X;
		Force.Z = 1000.0f * axis_position.Y;

		if (temp_axis.X > temp_axis.Y) {

			Position.X = self_position.X;
			Position.Z = self_position.Z;
			axis_position.X > 0 ? Position.X += 35.0f : Position.X -= 35.0f;
		}
		else {
			Position.X = self_position.X;
			Position.Z = self_position.Z;
			axis_position.Y > 0 ? Position.Z += 35.0f : Position.Z -= 35.0f;
		}
	}
}

void ASPPawnCPP::ChangeAnimationRotation_Implementation()
{
	/*FRotator rotation(0.0f, 0.0f, 0.0f);
	if(WorkData.FacingRight)
	{
		animation->SetRelativeRotation(rotation, false);
	}
	else {
		rotation = FRotator(0.0f, 180.0f,0.0f);
		animation->SetRelativeRotation(rotation, false);
	}*/
}

void ASPPawnCPP::SetUpIdle_Implementation()
{

}


FVector2D ASPPawnCPP::AxisPosition_Implementation()
{
	return FVector2D();
}

void ASPPawnCPP::Server_HitPunch_Implementation(FVector2D AxisPosition)
{
	HitPunch(true, AxisPosition);
}

bool ASPPawnCPP::Server_HitPunch_Validate(FVector2D AxisPosition)
{
	return true;
}



void ASPPawnCPP::Server_StopJump_Implementation()
{
	StopJump();
}

bool ASPPawnCPP::Server_StopJump_Validate()
{
	return true;
}

void ASPPawnCPP::Server_Jump_Implementation()
{	
	Jump();
}

bool ASPPawnCPP::Server_Jump_Validate()
{
	return true;
}

void ASPPawnCPP::Server_Move_Implementation(float AxisX)
{	
	if (CanMove())
	{
		if (AxisX >= -1 && AxisX <= 1) {
			if (AxisX == 0 && (States.MOVE_LEFT || States.MOVE_RIGHT)) {
				StopMove();
			}
			else if (AxisX > 0.2f && !States.MOVE_RIGHT) {
				Move(true, false);
			}
			else if (AxisX < -0.2f && !States.MOVE_LEFT) {
				Move(false, false);
			}
		}
	}
}

bool ASPPawnCPP::Server_Move_Validate(float AxisX)
{
	return true;
}

void ASPPawnCPP::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ASPPawnCPP, ClientPosition);
	DOREPLIFETIME(ASPPawnCPP, ClientForces);
	DOREPLIFETIME(ASPPawnCPP, Attributes);
	DOREPLIFETIME(ASPPawnCPP, StaticAttributes);
	DOREPLIFETIME(ASPPawnCPP, ClientCurrentDefence);
	DOREPLIFETIME(ASPPawnCPP, ClientInjuries);
}


void ASPPawnCPP::Jump()
{	
	if (HasAuthority()) {
			if (CanJump()) {
				if (GroundUnderFeet()) {
					if(!States.JUMP) States.JUMP = true;

					WorkData.JumpTimer = true;
					WorkData.JumpTimerDelta = 0.0f;
					
					ActionJump.ExecuteIfBound();
					Client_Jump(GetSendPosition(), 0);

					WorkData.JumpStart = false;
					WorkData.HasJumped = false;
				}
				else if (GroundNextToFeet(true)) {
					if (!States.JUMP_RIGHT_WALL) States.JUMP_RIGHT_WALL = true;
					
					WorkData.JumpTimer = true;
					WorkData.JumpTimerDelta = 0.0f;

					ActionJump.ExecuteIfBound();
					Client_Jump(GetSendPosition(), 1);

					WorkData.JumpStart = false;
					WorkData.HasJumped = false;
				}
				else if (GroundNextToFeet(false)) {
					if(!States.JUMP_LEFT_WALL) States.JUMP_LEFT_WALL = true;
					
					WorkData.JumpTimer = true;
					WorkData.JumpTimerDelta = 0.0f;

					ActionJump.ExecuteIfBound();
					Client_Jump(GetSendPosition(), 2);

					WorkData.JumpStart = false;
					WorkData.HasJumped = false;
				}
				else if (WorkData.AirJumped < Attributes.AirJumpAmount) {
					WorkData.AirJumped++;
					if (States.MOVE_LEFT) {
						if(!States.JUMP_RIGHT_WALL) States.JUMP_RIGHT_WALL = true;
						
						WorkData.JumpTimer = true;
						WorkData.JumpTimerDelta = 0.0f;

						ActionJump.ExecuteIfBound();
						Client_Jump(GetSendPosition(), 1);

						WorkData.JumpStart = false;
						WorkData.HasJumped = false;
					}
					else if (States.MOVE_RIGHT) {
						if(!States.JUMP_LEFT_WALL) States.JUMP_LEFT_WALL = true;
						
						WorkData.JumpTimer = true;
						WorkData.JumpTimerDelta = 0.0f;

						ActionJump.ExecuteIfBound();
						Client_Jump(GetSendPosition(), 2);

						WorkData.JumpStart = false;
						WorkData.HasJumped = false;
					}
					else {
						if(!States.JUMP) States.JUMP = true;
						
						WorkData.JumpTimer = true;
						WorkData.JumpTimerDelta = 0.0f;

						ActionJump.ExecuteIfBound();
						Client_Jump(GetSendPosition(), 0);

						WorkData.JumpStart = false;
						WorkData.HasJumped = false;
					}
			}
		}
	}
	else {
		WorkData.JumpStart = false;
		WorkData.HasJumped = false;

		WorkData.JumpTimer = true;
		WorkData.JumpTimerDelta = 0.0f;

		ActionJump.ExecuteIfBound();
	}
}

void ASPPawnCPP::MakeBusy()
{
	if (States.BUSY != true) States.BUSY = true;
}

void ASPPawnCPP::UnBusy()
{
	if (States.BUSY != false) States.BUSY = false;
}

void ASPPawnCPP::StopJump()
{
		if (CanStopJump()) 
		{

			if (!HasAuthority()) {
				WorkData.ClientTimer = false;
			}

			if (HasAuthority()) {
				Client_StopJump(GetSendPosition());
			}

			if (States.JUMP) {
				States.JUMP = false;
				ActionStopJump.ExecuteIfBound();
			}
			if (States.JUMP_LEFT_WALL) {
				States.JUMP_LEFT_WALL = false;
				ActionStopJump.ExecuteIfBound();
			}
			if (States.JUMP_RIGHT_WALL) {
				States.JUMP_RIGHT_WALL = false;
				ActionStopJump.ExecuteIfBound();
			}
			
			WorkData.JumpTimer = false;
			WorkData.JumpStart = false;
			WorkData.HasJumped = false;
	}
}

void ASPPawnCPP::LightAttack(int index)
{
	if (HasAuthority()) {
		if (CanLightAttack()) {
			StartLightAttack();
			if (index == 0) {
				FVector2D CurrentAxis = AxisPosition();
				FVector2D AbsCurrentAxis;
				if (CurrentAxis.X < 0) AbsCurrentAxis.X = CurrentAxis.X * -1; else AbsCurrentAxis.X = CurrentAxis.X;
				if (CurrentAxis.Y < 0) AbsCurrentAxis.Y = CurrentAxis.Y * -1; else AbsCurrentAxis.Y = CurrentAxis.Y;

				if ((CurrentAxis.X == 0.0f && CurrentAxis.Y == 0.0f) || (AbsCurrentAxis.X > AbsCurrentAxis.Y)) {
					if (States.ON_GROUND) {
						ActionLightAttack.ExecuteIfBound();
						Client_LightAttack(GetSendPosition(), 0);
					}
					else {
						ActionAirLightAttack.ExecuteIfBound();
						Client_LightAttack(GetSendPosition(), 3);
					}
				}
				else {
					if (CurrentAxis.Y > 0.0f) {
						if (States.ON_GROUND) {
							ActionUpperLightAttack.ExecuteIfBound();
							Client_LightAttack(GetSendPosition(), 1);
						}
						else {
							ActionAirUpperLightAttack.ExecuteIfBound();
							Client_LightAttack(GetSendPosition(), 4);
						}
					}
					else {
						if (States.ON_GROUND) {
							ActionDownLightAttack.ExecuteIfBound();
							Client_LightAttack(GetSendPosition(), 2);
						}
						else {
							ActionAirDownLightAttack.ExecuteIfBound();
							Client_LightAttack(GetSendPosition(), 5);
						}
					}
				}
			}
			else if(index == 1) {
				if (States.ON_GROUND) {
					ActionLightAttack.ExecuteIfBound();
					Client_LightAttack(GetSendPosition(), 0);
				}
				else {
					ActionAirLightAttack.ExecuteIfBound();
					Client_LightAttack(GetSendPosition(), 3);
				}
			}
			else if (index == 2) {
				if (States.ON_GROUND) {
					ActionUpperLightAttack.ExecuteIfBound();
					Client_LightAttack(GetSendPosition(), 1);
				}
				else {
					ActionAirUpperLightAttack.ExecuteIfBound();
					Client_LightAttack(GetSendPosition(), 4);
				}
			}
			else if (index == 3) {
				if (States.ON_GROUND) {
					ActionDownLightAttack.ExecuteIfBound();
					Client_LightAttack(GetSendPosition(), 2);
				}
				else {
					ActionAirDownLightAttack.ExecuteIfBound();
					Client_LightAttack(GetSendPosition(), 5);
				}
			}
		}
	}
	else {
		StartLightAttack();
		switch (index) {
		case 0:
			ActionLightAttack.ExecuteIfBound();
			break;

		case 1:
			ActionUpperLightAttack.ExecuteIfBound();
			break;

		case 2:
			ActionDownLightAttack.ExecuteIfBound();
			break;

		case 3:
			ActionAirLightAttack.ExecuteIfBound();
			break;

		case 4:
			ActionAirUpperLightAttack.ExecuteIfBound();
			break;

		case 5:
			ActionAirDownLightAttack.ExecuteIfBound();
			break;
		}
	}
}

void ASPPawnCPP::StrongAttack(int index)
{
	if (HasAuthority()) {
		if (CanStrongAttack() && ((index != 3) || (States.ON_GROUND) || (WorkData.UpB) ) ){

			SetUpStrongAttack();

			if (index == 0) {
				FVector2D CurrentAxis = AxisPosition();
				FVector2D AbsCurrentAxis;
				if (CurrentAxis.X < 0) AbsCurrentAxis.X = CurrentAxis.X * -1; else AbsCurrentAxis.X = CurrentAxis.X;
				if (CurrentAxis.Y < 0) AbsCurrentAxis.Y = CurrentAxis.Y * -1; else AbsCurrentAxis.Y = CurrentAxis.Y;

				if (CurrentAxis.X == 0.0f && CurrentAxis.Y == 0.0f) {
					if (States.ON_GROUND)
						ActionStrongAttack.ExecuteIfBound();
					else
						ActionAirStrongAttack.ExecuteIfBound();
				}
				else if (AbsCurrentAxis.X > AbsCurrentAxis.Y) {
					if (States.ON_GROUND)
						ActionSideStrongAttack.ExecuteIfBound();
					else
						ActionAirSideStrongAttack.ExecuteIfBound();
				}
				else {
					if (CurrentAxis.Y > 0.0f) {
						if (States.ON_GROUND)
							ActionUpperStrongAttack.ExecuteIfBound();
						else {
							ActionAirUpperStrongAttack.ExecuteIfBound();
							WorkData.UpB = false;
						}
							
					}
					else {
						if (States.ON_GROUND)
							ActionDownStrongAttack.ExecuteIfBound();
						else
							ActionAirDownStrongAttack.ExecuteIfBound();
					}
				}
			}
			else if (index == 1) {
				if (States.ON_GROUND) {
					ActionStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 0);
				}
				else {
					ActionAirStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 4);
				}
			}
			else if (index == 2) {
				if (States.ON_GROUND) {
					ActionSideStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 1);
				}
				else {
					ActionAirSideStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 5);
				}
			}
			else if (index == 3) {
				if (States.ON_GROUND) {
					ActionUpperStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 2);
				}
				else {
					ActionAirUpperStrongAttack.ExecuteIfBound();
					WorkData.UpB = false;
					Client_StrongAttack(GetSendPosition(), 6);
				}
			}
			else if (index == 4) {
				if (States.ON_GROUND) {
					ActionDownStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 3);
				}
				else {
					ActionAirDownStrongAttack.ExecuteIfBound();
					Client_StrongAttack(GetSendPosition(), 7);
				}
			}
		}
	}
	else {
		SetUpStrongAttack();

		switch (index) {
		case 0:
			ActionStrongAttack.ExecuteIfBound();
			break;

		case 1:
			ActionSideStrongAttack.ExecuteIfBound();
			break;

		case 2:
			ActionUpperStrongAttack.ExecuteIfBound();
			break;

		case 3:
			ActionDownStrongAttack.ExecuteIfBound();
			break;

		case 4:
			ActionAirStrongAttack.ExecuteIfBound();
			break;

		case 5:
			ActionAirSideStrongAttack.ExecuteIfBound();
			break;

		case 6:
			ActionAirUpperStrongAttack.ExecuteIfBound();
			break;

		case 7:
			ActionAirDownStrongAttack.ExecuteIfBound();
			break;
		}
	}	
}


void ASPPawnCPP::Server_StrongAttack_Implementation()
{
	StrongAttack(1);
}

bool ASPPawnCPP::Server_StrongAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Server_SideStrongAttack_Implementation()
{
	StrongAttack(2);
}

bool ASPPawnCPP::Server_SideStrongAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Server_UpperStrongAttack_Implementation()
{
	StrongAttack(3);
}

bool ASPPawnCPP::Server_UpperStrongAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Server_DownStrongAttack_Implementation()
{
	StrongAttack(4);
}

bool ASPPawnCPP::Server_DownStrongAttack_Validate()
{
	return true;
}

void ASPPawnCPP::ReleaseStrongAttack()
{
	if (HasAuthority()) {
		if (CanReleaseStrongAttack()) {
			ActionRealeaseStrongAttack.ExecuteIfBound();
			Client_StopStrongAttack(GetSendPosition(), WorkData.StrongAttackMeter);
		}
	}
	else {
		if (CanReleaseStrongAttack()) {
			ActionRealeaseStrongAttack.ExecuteIfBound();
			WorkData.ClientTimer = false;
		}
	}
}

void ASPPawnCPP::Server_ReleaseStrongAttack_Implementation()
{
	ReleaseStrongAttack();
}

bool ASPPawnCPP::Server_ReleaseStrongAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Server_LightAttack_Implementation()
{
	LightAttack(1);
}

bool ASPPawnCPP::Server_LightAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Server_UpperLightAttack_Implementation()
{
	LightAttack(2);
}

bool ASPPawnCPP::Server_UpperLightAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Server_DownLightAttack_Implementation()
{
	LightAttack(3);
}

bool ASPPawnCPP::Server_DownLightAttack_Validate()
{
	return true;
}

void ASPPawnCPP::Defence(int index)
{
	if (HasAuthority()) {
		FVector CurrentPosition = GetActorLocation();
		FVector2D SendPosition;
		SendPosition.X = CurrentPosition.X;
		SendPosition.Y = CurrentPosition.Z;

		if (index == 0) {
			FVector2D CurrentAxis = AxisPosition();
			FVector2D AbsCurrentAxis;
			if (CurrentAxis.X < 0) AbsCurrentAxis.X = CurrentAxis.X * -1; else AbsCurrentAxis.X = CurrentAxis.X;
			if (CurrentAxis.Y < 0) AbsCurrentAxis.Y = CurrentAxis.Y * -1; else AbsCurrentAxis.Y = CurrentAxis.Y;

			if ((CurrentAxis.X == 0.0f && CurrentAxis.Y == 0.0f) && !States.MOVE_RIGHT && !States.MOVE_LEFT) {
				if (CanDefence()) 
				{	
					SetUpDefence();
					ActionDefence.ExecuteIfBound();
				}
			}
			else if (AbsCurrentAxis.X >= AbsCurrentAxis.Y) {
				if (CanDash()) {
					
					States.SIDE_DASH = true;
					SetUpDash();
				}
			}
			else {
				if (CurrentAxis.Y > 0.0f) {
					if (CanDash()) {
					
						States.UP_DASH = true;
						SetUpDash();
					}
				}
				else {
					if (CanDash()) {
						if (States.ON_GROUND) {
							
							States.SPOT_DODGE = true;
							SetUpDash();
						}
							
						else {
							
							States.DOWN_DASH = true;
							SetUpDash();
						}
					}
				}
			}
		}
		else if (index == 1) {
			if (CanDefence()) {
				SetUpDefence();
				Client_Defence(GetSendPosition());
			}
		}
		else if (index == 2 || index == 3) {
			if (CanDash()) {
				if (index == 2) {
					States.SIDE_DASH = true;
					Client_Dash(SendPosition, 0);
				}
				else {
					States.UP_DASH = true;
					Client_Dash(SendPosition, 1);
				}

				SetUpDash();
			}
		}
		else if (index == 4) {
			if (CanDash()) {
				if (States.ON_GROUND) {
					States.SPOT_DODGE = true;
					Client_Dash(SendPosition, 3);
					SetUpDash();
				}
				else {
					States.DOWN_DASH = true;
					Client_Dash(SendPosition, 2);
					SetUpDash();
				}
					
			}
		}
	}
}


void ASPPawnCPP::Server_Defence_Implementation()
{
	Defence(1);
}

bool ASPPawnCPP::Server_Defence_Validate()
{
	return true;
}

void ASPPawnCPP::Server_SideDash_Implementation()
{
	Defence(2);
}

bool ASPPawnCPP::Server_SideDash_Validate()
{
	return true;
}

void ASPPawnCPP::Server_UpDash_Implementation()
{
	Defence(3);
}

bool ASPPawnCPP::Server_UpDash_Validate()
{
	return true;
}

void ASPPawnCPP::Server_DownDash_Implementation()
{
	Defence(4);
}

bool ASPPawnCPP::Server_DownDash_Validate()
{
	return true;
}

void ASPPawnCPP::ReleaseDefence()
{
		if (CanReleaseDefence())
		{
			if (!HasAuthority()) {
				WorkData.ClientTimer = false;
			}
			ClearDefence();
			ActionReleaseDefence.ExecuteIfBound();
			if (HasAuthority()) {
				Client_StopDefence(GetSendPosition());
			}
		}
}

void ASPPawnCPP::Server_ReleaseDefence_Implementation()
{
	ReleaseDefence();
}

bool ASPPawnCPP::Server_ReleaseDefence_Validate()
{
	return true;
}

void ASPPawnCPP::Hit(float forceX, float forceY)
{
	if (HasAuthority()) {
		Forces.X += forceX;
		Forces.Y += forceY;
	}
}

bool ASPPawnCPP::GroundNextToFeet_Implementation(bool right) const
{
	//We dont use c++ version for now only BP for ray casy
	return false;
}

bool ASPPawnCPP::GroundUnderFeet_Implementation() const
{
	//We dont realy use the c++ version
	return false;
}

void ASPPawnCPP::ChangeAnimation(FSPAnimationDetails details)
{
	collision_box->SetBoxExtent(FVector(details.CollisionBox.X, 10, details.CollisionBox.Y), true);
	hit_box->SetBoxExtent(FVector(details.HitBox.X, 10, details.HitBox.Y), true);
	hit_box->SetRelativeLocation(FVector(details.HitBoxRelativeLocation.X, 0.0f, details.HitBoxRelativeLocation.Y));

	SetCurrentPlayRate(details.AnimationSpeed);
	creature_mesh->SetBluePrintAnimationLoop(details.Loop);
	creature_mesh->SetBluePrintBlendActiveAnimation(details.AnimationName, 0.5f);
	
	
	
	if (WorkData.FacingRight) {
		creature_mesh->SetRelativeLocation(FVector(details.FlipbookRelativeLocation.X, 0.0f, details.FlipbookRelativeLocation.Y));
	}
	else {
		creature_mesh->SetRelativeLocation(FVector(details.FlipbookRelativeLocation.X*-1.0f, 0.0f, details.FlipbookRelativeLocation.Y));
	}
}

void ASPPawnCPP::QuickChangeAnimation(UPaperFlipbook * Flipbook, FVector2D HitBox)
{
	hit_box->SetBoxExtent(FVector(HitBox.X, 10, HitBox.Y), true);
	//animation->SetFlipbook(Flipbook);
	//animation->PlayFromStart();
}

void ASPPawnCPP::ChangeAnimationWithLocation(UPaperFlipbook * Flipbook, FVector2D HitBox, FVector2D RelativeLocation)
{
	hit_box->SetBoxExtent(FVector(HitBox.X, 10, HitBox.Y), true);
	//animation->SetFlipbook(Flipbook);
	//animation->SetRelativeLocation(FVector(RelativeLocation.X, 0.0f, RelativeLocation.Y));
	//animation->PlayFromStart();
}

void ASPPawnCPP::ChangeMovementSpeed(float speed)
{
	Attributes.MoveSpeed = speed;
}

void ASPPawnCPP::AddForce(FVector force)
{
	if (HasAuthority()) {
		WorkData.AddingForce = true;
		WorkData.AddForce = force;
	}
}

void ASPPawnCPP::Friction(float DeltaTime)
{
		if( !States.DASH && 
			( (!States.MOVE_LEFT && !States.MOVE_RIGHT) || 
			(Attributes.MoveSpeed > 0.0f && Forces.X > Attributes.MoveSpeed) ||
				(States.MOVE_LEFT && Attributes.MoveSpeed*-1.0f < 0.0f &&  Forces.X < Attributes.MoveSpeed*-1.0f) ) )
		{
			if (Forces.X > 0.0f) {
				if (GroundUnderFeet()) {
					Forces.X -= ValuePerSecond(Attributes.Friction, DeltaTime);
				}
				else {
					Forces.X -= ValuePerSecond(Attributes.AirFriction, DeltaTime);
				}
				if (Forces.X < 0.0f) {
					Forces.X = 0.0f;
				}
			}
			else if (Forces.X < 0.0f) {
				if (GroundUnderFeet()) {
					Forces.X += ValuePerSecond(Attributes.Friction, DeltaTime);
				}
				else {
					Forces.X += ValuePerSecond(Attributes.AirFriction, DeltaTime);
				}
				if (Forces.X > 0.0f) {
					Forces.X = 0.0f;
				}
			}
		}
}

void ASPPawnCPP::Gravity(float DeltaTime)
{
	if (States.AFFECTED_BY_GRAVITY && Forces.Y > Attributes.MaxGravity && !GroundUnderFeet() && !States.DASH && !States.JUMP 
		&& !States.JUMP_LEFT_WALL && !States.JUMP_RIGHT_WALL) {
		if ((States.LIGHT_ATTACK || States.STRONG_ATTACK) && Forces.Y < 0.0f)
			Forces.Y -= ValuePerSecond(Attributes.Gravity, DeltaTime) / 10.0f;
		else
			Forces.Y -= ValuePerSecond(Attributes.Gravity, DeltaTime);
		}
}

void ASPPawnCPP::FixPossitionError()
{
	if (!HasAuthority()) {
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::SanitizeFloat(WorkData.PossitionError.X));
		FVector current_position = this->GetActorLocation();
		if (WorkData.PossitionError.X > 0.0f) {
			if (WorkData.PossitionError.X <= 10.0f || WorkData.PossitionError.X > 50.0f) {
				WorkData.PossitionError.X = 0.0f;
				current_position.X += WorkData.PossitionError.X;
			}
			else {
				WorkData.PossitionError.X -= 10.0f;
				current_position.X += 10.0f;
			}
		}
		else if (WorkData.PossitionError.X < 0.0f) {
			if (WorkData.PossitionError.X >= -10.0f || WorkData.PossitionError.X < -50.0f) {
				WorkData.PossitionError.X = 0.0f;
				current_position.X += WorkData.PossitionError.X;
			}
			else {
				WorkData.PossitionError.X += 10.0f;
				current_position.X += -10.0f;
			}
		}

		if (WorkData.PossitionError.Z > 0.0f) {
			if (WorkData.PossitionError.Z <= 10.0f || WorkData.PossitionError.X > 50.0f) {
				WorkData.PossitionError.Z = 0.0f;
				current_position.Z += WorkData.PossitionError.Z;
			}
			else {
				WorkData.PossitionError.Z -= 10.0f;
				current_position.Z += 10.0f;
			}
		}
		else if (WorkData.PossitionError.Z < 0.0f) {
			if (WorkData.PossitionError.Z >= -10.0f || WorkData.PossitionError.X < -50.0f) {
				WorkData.PossitionError.Z = 0.0f;
				current_position.X += WorkData.PossitionError.Z;
			}
			else {
				WorkData.PossitionError.Z += 10.0f;
				current_position.Z += -10.0f;
			}
		}

		this->SetActorLocation(current_position);
	}
}

void ASPPawnCPP::UpgradeStrongAttackMeter()
{
	WorkData.StrongAttackMeter++;
}

void ASPPawnCPP::ClearStatesWhileHit()
{
		if(!HasAuthority()){
			WorkData.ClientTimer = false;
		}

		if(States.MOVE_RIGHT)States.MOVE_RIGHT = false;
		if (States.MOVE_LEFT)States.MOVE_LEFT = false;
		if(States.SUPER_MOVE)States.SUPER_MOVE = false;
		if (States.JUMP) {
			States.JUMP = false;
		}
		if (States.JUMP_LEFT_WALL) {
			States.JUMP_LEFT_WALL = false;
		}
		if (States.JUMP_RIGHT_WALL) {
			States.JUMP_RIGHT_WALL = false;
		}
		if (States.DEFENCE) {
			States.DEFENCE = false;
		}
		if (States.STRONG_ATTACK) {
			States.STRONG_ATTACK = false;
		}
		if (States.LIGHT_ATTACK) {
			States.LIGHT_ATTACK = false;
		}

		WorkData.DelayTimer = false;
		WorkData.StrongAttackTimer = false;
		WorkData.JumpTimer = false;
}

void ASPPawnCPP::UpdateTimers(float DeltaTime)
{
	ManageDefence(DeltaTime);
	if (IsStun()) {
		ManageStunState(DeltaTime / WorkData.TimeChange);
	}
	if (WorkData.JumpTimer) {
		ManageJump(DeltaTime / WorkData.TimeChange);
	}
	if (WorkData.StrongAttackTimer) {
		ManageStrongAttack(DeltaTime / WorkData.TimeChange);
	}
	if (WorkData.DashTimer) {
		ManageDash(DeltaTime / WorkData.TimeChange);
	}
	if (!HasAuthority() && WorkData.ClientTimer) {
		ManageClient(DeltaTime);
	}
	if (HasAuthority() && WorkData.DelayTimer) {
		ManageDelay(DeltaTime / WorkData.TimeChange);
	}
	if (HasAuthority()) {
		ManageServerKeyStates(DeltaTime);
	}
	if (WorkData.TimeTimer) {
		ManageTimeChange(DeltaTime);
	}
}

void ASPPawnCPP::SetUpDefence()
{
		if (!States.DEFENCE) States.DEFENCE = true;

		if (States.JUMP) {
			States.JUMP = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_LEFT_WALL) {
			States.JUMP_LEFT_WALL = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_RIGHT_WALL) {
			States.JUMP_RIGHT_WALL = false;
			WorkData.JumpTimer = false;
		}

		ActionDefence.ExecuteIfBound();
}

void ASPPawnCPP::ClearDefence()
{
	
		if (States.DEFENCE) States.DEFENCE = false;
}

void ASPPawnCPP::UseDefence()
{
	//Not in use replaced by ManageDefence
	if (HasAuthority()) {
		WorkData.CurrentDefence -= Attributes.Defence / 20.0f;
		if (WorkData.CurrentDefence <= 0.0f) {
			WorkData.CurrentDefence = 0.0f;
		//	GetWorldTimerManager().ClearTimer(WorkData.DefenceTimer);
			ReleaseDefence();
		}
	}
}

void ASPPawnCPP::ReplenishDefence()
{
	//Not in use replaced by ManagDefence
	if (HasAuthority()) {
		if (WorkData.CurrentDefence < Attributes.Defence) {
			WorkData.CurrentDefence += Attributes.Defence / 10.0f;
			if (WorkData.CurrentDefence >= Attributes.Defence) {
				WorkData.CurrentDefence = Attributes.Defence;
				//GetWorldTimerManager().ClearTimer(WorkData.DefenceTimer);
			}
		}
		//else {
			//GetWorldTimerManager().ClearTimer(WorkData.DefenceTimer);
		//}
	}
}

void ASPPawnCPP::SetUpDash()
{
		
		if (!States.DASH) States.DASH = true;

		if (States.MOVE_RIGHT) States.MOVE_RIGHT = false;
		if (States.MOVE_LEFT)States.MOVE_LEFT = false;
		if(States.SUPER_MOVE)States.SUPER_MOVE = false;
		if (States.JUMP) {
			States.JUMP = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_LEFT_WALL) {
			States.JUMP_LEFT_WALL = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_RIGHT_WALL) {
			States.JUMP_RIGHT_WALL = false;
			WorkData.JumpTimer = false;
		}	
		
		if (States.SPOT_DODGE) {
			WorkData.DashTimer = true;
			WorkData.DashTimerDelta = 0.0f;
			WorkData.DashTimerGoal = Attributes.SpotDodgeTime;
			WorkData.DashTimerStage = 0;
			
			ActionSpotDodge.ExecuteIfBound();
		}
		else {
			WorkData.DashTimer = true;
			WorkData.DashTimerDelta = 0.0f;
			WorkData.DashTimerGoal = Attributes.DashTime;
			WorkData.DashTimerStage = 0;
			if (HasAuthority()) {
				if(States.ON_GROUND)
					ActionDash.ExecuteIfBound();
				else
					ActionAirDash.ExecuteIfBound();
			}
			else {
				if (GroundUnderFeet())
					ActionDash.ExecuteIfBound();
				else
					ActionAirDash.ExecuteIfBound();
			}
		}

		DodgeBlink(true);
}

void ASPPawnCPP::StopDash()
{
	
		if (States.DASH) States.DASH = false;

		if (States.CAN_DASH) States.CAN_DASH = false;
		
		WorkData.DashTimer = true;
		WorkData.DashTimerDelta = 0.0f;
		WorkData.DashTimerStage = 1;
		if(HasAuthority())
			WorkData.DashTimerGoal = 1.0f;
		else {
			float PingDelta = GetWorld()->GetFirstPlayerController()->PlayerState->Ping * 2.0f;
			PingDelta /= 1000.0f;
			WorkData.DashTimerGoal = 1.0f - PingDelta;
		}

		DodgeBlink(false);
		StopDashForces();
		DashEnd();
}

void ASPPawnCPP::DashColdown()
{
	
		if (!States.CAN_DASH) States.CAN_DASH = true;
		WorkData.DashTimer = false;

}

void ASPPawnCPP::StartLightAttack()
{
		if (!States.LIGHT_ATTACK) States.LIGHT_ATTACK = true;

		if (States.MOVE_RIGHT) States.MOVE_RIGHT = false;
		if (States.MOVE_LEFT)States.MOVE_LEFT = false;
		if(States.SUPER_MOVE)States.SUPER_MOVE = false;
		if (States.JUMP) {
			States.JUMP = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_LEFT_WALL) {
			States.JUMP_LEFT_WALL = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_RIGHT_WALL) {
			States.JUMP_RIGHT_WALL = false;
			WorkData.JumpTimer = false;
		}		
}

void ASPPawnCPP::EndLightAttack()
{
		if (States.LIGHT_ATTACK) States.LIGHT_ATTACK = false;
}

void ASPPawnCPP::SetUpStrongAttack()
{
		if (!States.STRONG_ATTACK) States.STRONG_ATTACK = true;

		if(States.MOVE_RIGHT) States.MOVE_RIGHT = false;
		if(States.MOVE_LEFT )States.MOVE_LEFT = false;
		if(States.SUPER_MOVE)States.SUPER_MOVE = false;
		if (States.JUMP) {
			States.JUMP = false;
			WorkData.JumpTimer = false;
		} 
		if (States.JUMP_LEFT_WALL) {
			States.JUMP_LEFT_WALL = false;
			WorkData.JumpTimer = false;
		}
		if (States.JUMP_RIGHT_WALL) {
			States.JUMP_RIGHT_WALL = false;
			WorkData.JumpTimer = false;
		}
		if (States.LIGHT_ATTACK) {
			States.LIGHT_ATTACK = false;
		}
		
		WorkData.StrongAttackMeter = 0;
		WorkData.StrongAttackTimer = true;
		WorkData.StrongAttackTimerDelta = 0.0f;
}

void ASPPawnCPP::ClearStrongAttack()
{
	
		if (States.STRONG_ATTACK) States.STRONG_ATTACK = false;

		WorkData.StrongAttackTimer = false;

}



void ASPPawnCPP::ApplyForces(float DeltaTime)
{
	FVector current_location;
	float l_BounceForce = 0.0f;
	
		if (Forces.X != 0.0f) {
			current_location = GetActorLocation();
			current_location.X += Forces.X * (DeltaTime / 1.0f);
			if (!SetActorLocation(current_location, true, nullptr)) {
				if (IsStun() && (Forces.X > 200.0f || Forces.X < -200.0f)) {
					Forces.X = -Forces.X;
					Forces.X >= 0.0f ? l_BounceForce = Forces.X : l_BounceForce = -Forces.X;
				}
				else {
					Forces.X = 0.0f;
				}
				
			}
		}
		if (Forces.Y != 0.0f) {
			current_location = GetActorLocation();
			current_location.Z += Forces.Y * (DeltaTime / 1.0f);
			if (!SetActorLocation(current_location, true, nullptr)) {
				if (Forces.Y < 0) { 
					WorkData.AirJumped = 0;
					WorkData.UpB = true;
				}
				if (IsStun() && (Forces.Y > 200.0f || Forces.Y < -200.0f)) {
					Forces.Y = -Forces.Y;
					float l_BounceForce2;
					Forces.Y >= 0.0f ? l_BounceForce2 = Forces.Y : l_BounceForce2 = -Forces.Y;
					if (l_BounceForce < l_BounceForce2) l_BounceForce = l_BounceForce2;
				}
				else {
					Forces.Y = 0.0f;
				}
			}
		}	

		if (Forces.Y == 0.0f && !States.ON_GROUND) {
			if (GroundUnderFeet()) {
				States.ON_GROUND = true;
					CallTouchGround();
			}
		}else if (Forces.Y != 0.0f && States.ON_GROUND) {
			if (!GroundUnderFeet()) {
				States.ON_GROUND = false;
					CallLeaveGround();
			}
		}

		if (l_BounceForce > 0.0f) SlowTimeByForce(l_BounceForce);

}

void ASPPawnCPP::CalculateMovement(float DeltaTime)
{	
	float CalculatedMovementSpeed;
		if (States.MOVE_LEFT) {
			if (GroundUnderFeet()) {
				CalculatedMovementSpeed = Attributes.MoveSpeed * StaticAttributes.MovementScale;
				if (Forces.X < CalculatedMovementSpeed * 2 && Forces.X > -CalculatedMovementSpeed) {
					Forces.X = -CalculatedMovementSpeed;
				}
			}
			else {
					CalculatedMovementSpeed = Attributes.MoveSpeed * StaticAttributes.AirMovementScale;
				if (Forces.X < CalculatedMovementSpeed * 2 && Forces.X > -CalculatedMovementSpeed) {
					Forces.X = -CalculatedMovementSpeed;
				}
			}
		}
		else if (States.MOVE_RIGHT) {
			if (GroundUnderFeet()) {
				CalculatedMovementSpeed = Attributes.MoveSpeed * StaticAttributes.MovementScale;
				if (Forces.X > -CalculatedMovementSpeed * 2 && Forces.X < CalculatedMovementSpeed) {
					Forces.X = CalculatedMovementSpeed;
				}
			}
			else {
					CalculatedMovementSpeed = Attributes.MoveSpeed * StaticAttributes.AirMovementScale;
				if (Forces.X > -CalculatedMovementSpeed * 2 && Forces.X < CalculatedMovementSpeed) {
					Forces.X = CalculatedMovementSpeed;
				}
			}
		}

		if (States.JUMP && WorkData.JumpStart) {
			Forces.Y = Attributes.JumpPower;
			WorkData.HasJumped = true;
		}
		else if (States.JUMP_LEFT_WALL && WorkData.JumpStart) {
			WorkData.HasJumped = true;
			Forces.Y = Attributes.JumpPower / StaticAttributes.WallJumpYModifier;
			if (Forces.X < Attributes.JumpPower / StaticAttributes.WallJumpXModifier) {
				Forces.X = Attributes.JumpPower / StaticAttributes.WallJumpXModifier;
			}
		}
		else if (States.JUMP_RIGHT_WALL && WorkData.JumpStart) {
			WorkData.HasJumped = true;
			Forces.Y = Attributes.JumpPower / StaticAttributes.WallJumpYModifier;
			if (Forces.X > -Attributes.JumpPower / StaticAttributes.WallJumpXModifier) {
				Forces.X = -Attributes.JumpPower / StaticAttributes.WallJumpXModifier;
			}
		}

		if (States.SIDE_DASH) {
			States.SIDE_DASH = false;
			if (WorkData.FacingRight) {
				if (Forces.X <= 0.0f && Forces.X >= -Attributes.Dash) {
					Forces.X = Attributes.Dash;
				}
				else if (Forces.X > 0) {
					Forces.X += Attributes.Dash;
				}
			}
			else {
				if (Forces.X >= 0.0f && Forces.X <= Attributes.Dash) {
					Forces.X = -Attributes.Dash;
				}
				else if (Forces.X < 0) {
					Forces.X -= Attributes.Dash;
				}
			}
		
			if (Forces.Y <= Attributes.Dash && Forces.Y >= -Attributes.Dash)
			{
				Forces.Y = 0.0f;
			}
		}
		else if (States.UP_DASH) {
			States.UP_DASH = false;
			if (Forces.X <= Attributes.Dash && Forces.X >= -Attributes.Dash) 
			{
				Forces.X = 0.0f;
			}

			if (Forces.Y <= 0.0f && Forces.Y >= -Attributes.Dash) 
			{
				Forces.Y = Attributes.Dash;
			}
			else if (Forces.Y > 0.0f) 
			{
				Forces.Y += Attributes.Dash;
			}
		}
		if (States.DOWN_DASH) {
			States.DOWN_DASH = false;
			if (Forces.X <= Attributes.Dash && Forces.X >= -Attributes.Dash)
			{
				Forces.X = 0.0f;
			}

			if (Forces.Y >= 0.0f && Forces.Y <= Attributes.Dash)
			{
				Forces.Y = -Attributes.Dash;
			}
			else if (Forces.Y < 0.0f)
			{
				Forces.Y -= Attributes.Dash;
			}
		}
		else if (States.SPOT_DODGE) {
			States.SPOT_DODGE = false;
		}

		if (WorkData.WasHit) {
			WorkData.WasHit = false;

			float temp_force = WorkData.HitForce.X * WorkData.HitForce.Z;
			if (temp_force > 0.0f) {
				if (Forces.X <= 0.0f) {
					Forces.X = temp_force;
				}
				else {
					Forces.X += temp_force;
				}
			}
			else {
				if (Forces.X >= 0.0f) {
					Forces.X = temp_force;
				}
				else {
					Forces.X += temp_force;
				}
			}

			temp_force = WorkData.HitForce.Y * WorkData.HitForce.Z;
			if (temp_force > 0.0f) {
				if (Forces.Y <= 0.0f) {
					Forces.Y = temp_force;
				}
				else {
					Forces.Y += temp_force;
				}
			}
			else {
				if (Forces.Y >= 0.0f) {
					Forces.Y = temp_force;
				}
				else {
					Forces.Y += temp_force;
				}
			}
		}

		if (WorkData.AddingForce) {
			WorkData.AddingForce = false;
			Forces.X += WorkData.AddForce.X * WorkData.AddForce.Z;
			Forces.Y += WorkData.AddForce.Y * WorkData.AddForce.Z;
		}
}

void ASPPawnCPP::CheckKeyStates()
{	
	float PingDelta = 0.0f;

	if (!HasAuthority()) {
		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->Ping * 2.0f;
				PingDelta /= 1000.0f;
			}
		}
		
	}

	
		if (LastKeyStates.DEFENCE_KEY && !KeyStates.DEFENCE_KEY) {
			if (CanReleaseDefence()) {
					Server_ReleaseDefence();

					if (!HasAuthority()) {
						WorkData.ClientTimer = true;
						WorkData.ClientTimerDelta = 0.0f;
						WorkData.ClientTimerGoal = PingDelta;
						WorkData.ClientTimerStage = 0;
					}	
			}
			
		}
		else if (!LastKeyStates.DEFENCE_KEY && States.DEFENCE) {
			if (CanReleaseDefence())
				Server_ReleaseDefence();
		}
		else if (LastKeyStates.JUMP_KEY && !KeyStates.JUMP_KEY) {
			WorkData.CanJumpAgain = true;
			if (CanStopJump())
			Server_StopJump();

			if (!HasAuthority()) {
				WorkData.ClientTimer = true;
				WorkData.ClientTimerDelta = 0.0f;
				WorkData.ClientTimerGoal = PingDelta;
				WorkData.ClientTimerStage = 1;
			}
		}
		else if (!KeyStates.JUMP_KEY && (States.JUMP || States.JUMP_LEFT_WALL || States.JUMP_RIGHT_WALL)) {
			if (CanStopJump())
				Server_StopJump();

			if (!HasAuthority()) {
				WorkData.ClientTimer = true;
				WorkData.ClientTimerDelta = 0.0f;
				WorkData.ClientTimerGoal = PingDelta;
				WorkData.ClientTimerStage = 1;
			}
		}
		else if (LastKeyStates.SATTACK_KEY && !KeyStates.SATTACK_KEY) {
			if (CanReleaseStrongAttack()) {
				Server_ReleaseStrongAttack();
			}
		}
		else if (!LastKeyStates.SATTACK_KEY && States.STRONG_ATTACK) {
			if (CanReleaseStrongAttack())
			Server_ReleaseStrongAttack();
		}
		else if (LastKeyStates.LEFT_KEY && !KeyStates.LEFT_KEY) {
			if (States.MOVE_LEFT && CanStopMove()) {
				Server_StopMove();
				
				if (!HasAuthority()) {
					WorkData.ClientTimer = true;
					WorkData.ClientTimerDelta = 0.0f;
					WorkData.ClientTimerGoal = PingDelta;
					WorkData.ClientTimerStage = 2;
				}
			}
		}
		else if (!LastKeyStates.LEFT_KEY && States.MOVE_LEFT) {
			if (CanStopMove()) {
				Server_StopMove();
			}	
		}
		else if (LastKeyStates.RIGHT_KEY && !KeyStates.RIGHT_KEY) {
			if (States.MOVE_RIGHT && CanStopMove()) {
				Server_StopMove();
			
				if (!HasAuthority()) {
					WorkData.ClientTimer = true;
					WorkData.ClientTimerDelta = 0.0f;
					WorkData.ClientTimerGoal = PingDelta;
					WorkData.ClientTimerStage = 2;
				}
			}
		}
		else if (!LastKeyStates.RIGHT_KEY && States.MOVE_RIGHT ) {
			if (CanStopMove()) {
				Server_StopMove();
			}
		}
		else if (KeyStates.DEFENCE_KEY) {
			if (KeyStates.RIGHT_KEY || KeyStates.LEFT_KEY) {
				if(CanDash())
				Server_SideDash();
			}
			else if (KeyStates.UP_KEY) {
				if (CanDash())
				Server_UpDash();
			}
			else if (KeyStates.DOWN_KEY) {
				if (CanDash())
				Server_DownDash();
			}
			else {
				if (CanDefence())
				Server_Defence();
			}
		}
		else if (KeyStates.JUMP_KEY) {
			if (CanJump() && WorkData.CanJumpAgain) {
				Server_Jump();
				WorkData.CanJumpAgain = false;
			}
			if (KeyStates.RIGHT_KEY) {
				if (!States.MOVE_RIGHT && CanMove()) {
					Server_MoveRight();
				}
			}
			else if (KeyStates.LEFT_KEY) {
				if (!States.MOVE_LEFT && CanMove())
					Server_MoveLeft();
			}
		}
		else if (KeyStates.LATTACK_KEY)
		{
			if (CanLightAttack())
			{
				if (KeyStates.UP_KEY) {
					Server_UpperLightAttack();
				}
				else if (KeyStates.DOWN_KEY) {
					Server_DownLightAttack();
				}
				else {
					Server_LightAttack();
				}
			}
		}
		else if (KeyStates.SATTACK_KEY)
		{
			if (CanStrongAttack()) {

				if (KeyStates.RIGHT_KEY || KeyStates.LEFT_KEY) {
					Server_SideStrongAttack();
				}
				else if (KeyStates.UP_KEY) {
					Server_UpperStrongAttack();
				}
				else if (KeyStates.DOWN_KEY) {
					Server_DownStrongAttack();
				}
				else {
					Server_StrongAttack();
				}
			}
		}
		else if (KeyStates.RIGHT_KEY) {
			if (CanMove()) {
				if (KeyStates.SUPER_RIGHT_KEY && !States.SUPER_MOVE) {
					Server_SuperMoveRight();
				}
				else if(!States.MOVE_RIGHT || (!KeyStates.SUPER_RIGHT_KEY && States.SUPER_MOVE)) {
					Server_MoveRight();
				}
			}
			
		}
		else if (KeyStates.LEFT_KEY) {
			if ( CanMove()) {
				if (KeyStates.SUPER_LEFT_KEY && !States.SUPER_MOVE) {
					Server_SuperMoveLeft();
				}
				else if(!States.MOVE_LEFT || (!KeyStates.SUPER_LEFT_KEY && States.SUPER_MOVE)){
					Server_MoveLeft();
				}
			}
		}

		LastKeyStates = KeyStates;
}

float ASPPawnCPP::ValuePerSecond(float value, float deltaTime)
{
	return value * (deltaTime / 1.0f);
}

void ASPPawnCPP::SetAttributes(FSPPawnAttributes new_attributes)
{
	Attributes = new_attributes;
	WorkData.CurrentDefence = Attributes.Defence;
	ClientCurrentDefence = WorkData.CurrentDefence;
}

void ASPPawnCPP::Move(bool right, bool super)
{
		if (CanMove()) {
			if (right) {
				if(States.MOVE_LEFT) States.MOVE_LEFT = false;
				if(!States.MOVE_RIGHT) States.MOVE_RIGHT  = true;
				if (States.SUPER_MOVE != super) {
					States.SUPER_MOVE = super;
				}
				if (!WorkData.FacingRight) {
					FRotator rotation(0.0f, 0.0f, 0.0f);
					WorkData.FacingRight = true;
					
					//creature_mesh->SetRelativeRotation(rotation, false);
					FVector RelScale = creature_mesh->RelativeScale3D;
					if (RelScale.X < 0) RelScale.X *= -1;
					creature_mesh->SetRelativeScale3D(RelScale);
					creature_mesh->SetRelativeLocation(FVector(creature_mesh->RelativeLocation.X*-1.0f, 0.0f, creature_mesh->RelativeLocation.Z));
				}
				if (HasAuthority()) {
					Client_Move(GetSendPosition(), 0, super);
				}
			}
			else {
				if (!States.MOVE_LEFT) States.MOVE_LEFT = true;
				if (States.MOVE_RIGHT) States.MOVE_RIGHT = false;
				if (States.SUPER_MOVE != super) States.SUPER_MOVE = super;
				if (WorkData.FacingRight) {
					FRotator rotation(0.0f, 180.0f, 0.0f);
					WorkData.FacingRight = false;
					
					//creature_mesh->SetRelativeRotation(rotation, false);
					FVector RelScale = creature_mesh->RelativeScale3D;
					if (RelScale.X > 0) RelScale.X *= -1;
					creature_mesh->SetRelativeScale3D(RelScale);
					creature_mesh->SetRelativeLocation(FVector(creature_mesh->RelativeLocation.X*-1.0f, 0.0f, creature_mesh->RelativeLocation.Z));
				}
				if (HasAuthority()) {
					Client_Move(GetSendPosition(), 1, super);
				}
			}
			
			if (super) {
				ActionSuperMove.ExecuteIfBound();
			}
			else {
				ActionMove.ExecuteIfBound();
			}
		}
}

void ASPPawnCPP::StopMove()
{
	if (HasAuthority()) {
		if (CanStopMove()) {
			if (States.MOVE_LEFT) States.MOVE_LEFT = false;
			if (States.MOVE_RIGHT) States.MOVE_RIGHT = false;
			if (States.SUPER_MOVE) States.SUPER_MOVE = false;
			ActionStopMove.ExecuteIfBound();
			Client_StopMove(GetSendPosition());
		}
	}
	else {
		if (States.MOVE_LEFT || States.MOVE_RIGHT) {
			if (States.MOVE_LEFT) States.MOVE_LEFT = false;
			if (States.MOVE_RIGHT) States.MOVE_RIGHT = false;
			if (States.SUPER_MOVE) States.SUPER_MOVE = false;
			ActionStopMove.ExecuteIfBound();
		}
	}
}

void ASPPawnCPP::ResetActions(float delay_delta)
{
	if (HasAuthority()) {
		DelayActionTime = delay_delta;
		WorkData.DelayTimer = false;
		if (DelayActionTime > 0.000f) {
			WorkData.DelayTimer = true;
			WorkData.DelayTimerDelta = 0.0f;
			WorkData.DelayTimerGoal = DelayActionTime;
		}
	}
}

bool ASPPawnCPP::Client_Dash_Validate(FVector2D n_Position, int index) {
	return true;
}

void ASPPawnCPP::Client_Dash_Implementation(FVector2D n_Position, int index)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		
		switch (index) {
		case 0:
			States.SIDE_DASH = true;
			break;
			
		case 1:
			States.UP_DASH = true;
			break;

		case 2:
			States.DOWN_DASH = true;
			break;

		case 3:
			States.SPOT_DODGE = true;
			break;
		}

		SetUpDash();
	}
}

bool ASPPawnCPP::CallDelayAction_Validate() {
	return true;
}

void ASPPawnCPP::CallDelayAction_Implementation()
{	
	if (HasAuthority()) {
		if (CanDelayAction()) {
			ActionDelay.ExecuteIfBound();
		}
	}
	else {
		ActionDelay.ExecuteIfBound();
	}
}


void ASPPawnCPP::CallTouchGround()
{
	ActionTouchGround.ExecuteIfBound();
}

void ASPPawnCPP::CallLeaveGround()
{
	ActionLeaveGround.ExecuteIfBound();
}

void ASPPawnCPP::EndStun_Implementation()
{

}

void ASPPawnCPP::GetHit_Implementation(float hitstun, float damage, FVector knockback/*x/y are directions, z is force*/)
{
	if (HasAuthority()) {

		if (States.DEFENCE) {
			if (WorkData.CurrentDefence > damage) {
				WorkData.CurrentDefence -= damage;
				ClientCurrentDefence = WorkData.CurrentDefence;
				damage = damage / 10.0f;
			}
			else {
				WorkData.CurrentDefence = 0.0f;
				ClientCurrentDefence = WorkData.CurrentDefence;
				hitstun = hitstun * 1.25f;
			}
		}

		WorkData.Injuries += damage - (damage / (100.0f / ((float)Attributes.Tenacity * 0.5f)));
		ClientInjuries = WorkData.Injuries;

		if (!States.DEFENCE || WorkData.CurrentDefence == 0.0f) {
			WorkData.WasHit = true;
			ClearStatesWhileHit();

			if (WorkData.HitStun < 0.06f) {
				WorkData.HitStun = hitstun
					+ ((hitstun * (float)WorkData.Injuries) / 200.0f); //Injuries adds to hitstun;
				WorkData.HitStun -= (WorkData.HitStun *(float)Attributes.Tenacity) / 100.0f; //Tenacity lowers hitstun

			}

			knockback.Z += knockback.Z / (100.0f / (WorkData.Injuries * 0.4f));
			WorkData.HitForce = knockback;
			SlowTimeAfterHit(knockback.Z);
			Client_GetHit(GetSendPosition(), knockback, WorkData.HitStun);
		}
	}
}

void ASPPawnCPP::DashEnd_Implementation()
{
}

bool ASPPawnCPP::LooseStock_Validate() {
	return true;
}

void ASPPawnCPP::LooseStock_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, "STOCK LOST");
	Forces.X = 0.0f;
	Forces.Y = 0.0f;

	
	WorkData.AirJumped = 0;

	WorkData.FacingRight = true;
	FRotator rotation(0.0f, 0.0f, 0.0f);

	//creature_mesh->SetRelativeRotation(rotation, false);
	FVector RelScale = creature_mesh->RelativeScale3D;
	if (RelScale.X < 0) RelScale.X *= -1;
	creature_mesh->SetRelativeScale3D(RelScale);
	creature_mesh->SetRelativeLocation(FVector(creature_mesh->RelativeLocation.X*-1.0f, 0.0f, creature_mesh->RelativeLocation.Z));

	WorkData.Injuries = 0;
	WorkData.StrongAttackMeter = 0;
	WorkData.HitStun = 0.0f;
	WorkData.CurrentDefence = 0.0f;
	WorkData.WasHit = false;
	WorkData.HitForce = FVector(0.0f, 0.0, 0.0f);
	WorkData.AddingForce = false;
	WorkData.AddForce = FVector(0.0f, 0.0, 0.0f);
	WorkData.DefenceDelta = 0.0f;
	WorkData.JumpTimer = false;
	WorkData.JumpTimerDelta = 0.0f;
	WorkData.StrongAttackTimer = false;
	WorkData.StrongAttackTimerDelta = 0.0f;
	WorkData.DashTimer = false;
	WorkData.DashTimerDelta = 0.0f;
	WorkData.DashTimerGoal = 0.0f;
	WorkData.DashTimerStage = 0;

	WorkData.ClientTimer = false;
	WorkData.ClientTimerDelta = 0.0f;
	WorkData.ClientTimerGoal = 0.0f;
	WorkData.ClientTimerStage = 0;

	WorkData.DelayTimer = false;
	WorkData.DelayTimerDelta = 0.0f;
	WorkData.DelayTimerGoal = 0.0f;

	ClientCurrentDefence = 0.0f;
	ClientInjuries = 0.0f;

	DelayActionTime = 0.0f;

	States.MOVE_RIGHT = false;
	States.MOVE_LEFT = false;
	States.SUPER_MOVE = false;
	States.JUMP = false;
	States.JUMP_LEFT_WALL = false;
	States.JUMP_RIGHT_WALL = false;
	States.ON_GROUND = false;
	States.BUSY = false;
	States.DEFENCE = false;
	States.STRONG_ATTACK = false;
	States.LIGHT_ATTACK = false;
	States.DASH = false;
	States.SIDE_DASH = false;
	States.UP_DASH = false;
	States.DOWN_DASH = false;
	States.SPOT_DODGE = false;
	States.AFFECTED_BY_GRAVITY = true;

	KeyStates.LEFT_KEY = false;
	KeyStates.RIGHT_KEY = false;
	KeyStates.SUPER_LEFT_KEY = false;
	KeyStates.SUPER_RIGHT_KEY = false;
	KeyStates.UP_KEY = false;
	KeyStates.DOWN_KEY = false;

	KeyStates.LATTACK_KEY = false;
	KeyStates.SATTACK_KEY = false;
	KeyStates.JUMP_KEY = false;
	KeyStates.DEFENCE_KEY = false;

	LastKeyStates.LEFT_KEY = false;
	LastKeyStates.RIGHT_KEY = false;
	LastKeyStates.SUPER_LEFT_KEY = false;
	LastKeyStates.SUPER_RIGHT_KEY = false;
	LastKeyStates.UP_KEY = false;
	LastKeyStates.DOWN_KEY = false;

	LastKeyStates.LATTACK_KEY = false;
	LastKeyStates.SATTACK_KEY = false;
	LastKeyStates.JUMP_KEY = false;
	LastKeyStates.DEFENCE_KEY = false;

	SetUpIdle();
}

bool ASPPawnCPP::FacingRight()
{
	if (WorkData.FacingRight)
		return true;
	else
	return false;
}

float ASPPawnCPP::StrongAttackMeter()
{
	return WorkData.StrongAttackMeter;
}

float ASPPawnCPP::CurrentDefence()
{
	return WorkData.CurrentDefence;
}

float ASPPawnCPP::CurrentHitStun()
{
	return WorkData.HitStun;
}

FVector2D ASPPawnCPP::GetSendPosition()
{
	FVector CurrentPosition = GetActorLocation();
	FVector2D SendPosition;
	SendPosition.X = CurrentPosition.X;
	SendPosition.Y = CurrentPosition.Z;

	return SendPosition;
}

bool ASPPawnCPP::IsStun()
{
	if (WorkData.HitStun > 0.0f) {
		return true;
	}
	return false;
}

bool ASPPawnCPP::CanMove() {
	if (HasAuthority()) {
		if (!IsStun() && !States.BUSY && States.CAN_MOVE)
			return true;
		else 
			return false;
	}
	else {
		return true;
	}
	
}

bool ASPPawnCPP::CanStopMove() { 
	if (HasAuthority()) {
		if (States.MOVE_LEFT || States.MOVE_RIGHT)
			return true;
		else
			return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanDelayAction() {
	if (HasAuthority()) {
		if (!IsStun() && !States.BUSY) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanJump() { 
	if (HasAuthority()) {
		if (!IsStun() && !States.BUSY && States.CAN_JUMP) {
			if (WorkData.AirJumped < Attributes.AirJumpAmount || GroundUnderFeet()
				|| GroundNextToFeet(true) || GroundNextToFeet(false)) {
				return true;
			}
			else return false;
		}
		else
			return false;
	}
	else {
		return true;
	}
	
}

bool ASPPawnCPP::CanStopJump() { 
	if (HasAuthority()) {
		if ((States.JUMP || States.JUMP_LEFT_WALL || States.JUMP_RIGHT_WALL) && WorkData.HasJumped) {
			return true;
		}
		return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanLightAttack() { 
	if (HasAuthority()) {
		if (!IsStun() && !States.BUSY && States.CAN_LIGHT_ATTACK)
			return true; 
		else 
			return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanStrongAttack() { 
	if (HasAuthority()) {
		if (!IsStun() && !States.BUSY && States.CAN_STRONG_ATTACK)
			return true;
		else
			return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanReleaseStrongAttack() { 
	if (HasAuthority()) {
		if (States.STRONG_ATTACK) {
			return true;
		}
		return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanDefence() { 
	if (HasAuthority()) {
		//You can not defend in air
		if (!IsStun() && !States.BUSY && States.CAN_DEFENCE && WorkData.CurrentDefence > 0.0f && States.ON_GROUND ) {
			return true;
		}
		else 
			return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanReleaseDefence() {
	if (HasAuthority()){
		if (States.DEFENCE) {
			return true;
		}
		return false;
	}
	else {
		return true;
	}
}

bool ASPPawnCPP::CanDash()
{
	if (HasAuthority()) {
		if (!IsStun() && !States.BUSY && States.CAN_DASH) {
			return true;
		}
		else
			return false;
	}
	else {
		return true;
	}
}

void ASPPawnCPP::SetSuperRightKey(bool state)
{

	if (!HasAuthority()) {
		if (KeyStates.SUPER_RIGHT_KEY != state)
			KeyStates.SUPER_RIGHT_KEY = state;
	}
	else if (KeyStates.SUPER_RIGHT_KEY != state) {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(18, PingDelta);

		}
		else {
			SetNextServerKeyState(19, PingDelta);
		}
	}
}

void ASPPawnCPP::SetSuperLeftKey(bool state)
{

	if (!HasAuthority()) {
		if (KeyStates.SUPER_LEFT_KEY != state)
			KeyStates.SUPER_LEFT_KEY = state;
	}
	else if (KeyStates.SUPER_LEFT_KEY != state) {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(16, PingDelta);

		}
		else {
			SetNextServerKeyState(17, PingDelta);
		}
	}
}

void ASPPawnCPP::SetLeftKey(bool state)
{

	if (!HasAuthority()) {
		if (KeyStates.LEFT_KEY != state)
		KeyStates.LEFT_KEY = state;
	}
	else if (KeyStates.LEFT_KEY != state) {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(14, PingDelta);
			
		}
		else {
			SetNextServerKeyState(15, PingDelta);
		}
	}
}
void ASPPawnCPP::SetRightKey(bool state)
{
	{

		if (!HasAuthority()) {
			if(KeyStates.RIGHT_KEY != state)
			KeyStates.RIGHT_KEY = state;
		}
		else  if (KeyStates.RIGHT_KEY != state) {

			float PingDelta = 0.01f;

			AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
			if (IsValid(check)) {
				APlayerState *check2 = check->PlayerState;
				if (IsValid(check2))
				{
					PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
					PingDelta /= 1000.0f;
				}
			}

			if (state) {
				SetNextServerKeyState(12, PingDelta);
			}
			else {
				SetNextServerKeyState(13, PingDelta);
			}
		}
	};
}

void ASPPawnCPP::SetUpKey(bool state)
{

	if (!HasAuthority()) {
		if (KeyStates.UP_KEY != state)
		KeyStates.UP_KEY = state;
	}
	else if (KeyStates.UP_KEY != state) {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(10, PingDelta);
		}
		else {
			SetNextServerKeyState(11, PingDelta);
		}
	}
}
void ASPPawnCPP::SetDownKey(bool state)
{

	if (!HasAuthority()) {
		if (KeyStates.DOWN_KEY != state)
		KeyStates.DOWN_KEY = state;
	}
	else if (KeyStates.DOWN_KEY != state) {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(8, PingDelta);

		}
		else {
			SetNextServerKeyState(9, PingDelta);
		}
	}
}

void ASPPawnCPP::SetLAttackKey(bool state)
{

	if (!HasAuthority()) {
		KeyStates.LATTACK_KEY = state;
	}
	else {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(6, PingDelta);
		}
		else {
			SetNextServerKeyState(7, PingDelta);
		}
	}
}

void ASPPawnCPP::SetSAttackKey(bool state)
{


	if (!HasAuthority()) {
		KeyStates.SATTACK_KEY = state;
	}
	else  {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(4, PingDelta);
		}
		else {
			SetNextServerKeyState(5, PingDelta);
		}
	}
}

void ASPPawnCPP::SetDefenceKey(bool state)
{
	if (!HasAuthority()) {
		KeyStates.DEFENCE_KEY = state;
	}
	else  {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}
		if (state) {
			SetNextServerKeyState(0, PingDelta);
		}
		else {
			SetNextServerKeyState(1, PingDelta);
		}
	}
}

void ASPPawnCPP::SetJumpKey(bool state)
{

	if (!HasAuthority()) {
		KeyStates.JUMP_KEY = state;
	}
	else {

		float PingDelta = 0.01f;

		AController *check = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (IsValid(check)) {
			APlayerState *check2 = check->PlayerState;
			if (IsValid(check2))
			{
				PingDelta = check->PlayerState->ExactPing / 2.0f + 0.01f;
				PingDelta /= 1000.0f;
			}
		}

		if (state) {
			SetNextServerKeyState(2, PingDelta);
		}
		else {
			SetNextServerKeyState(3, PingDelta);
		}
	}
}

void ASPPawnCPP::ManageServerKeyStates(float DeltaTime)
{
	if (HasAuthority()) {
		for (int i = 0; i < 20; i++) {
			if (KeyTimers.KeyDelta[i] > 0.0f) {
				KeyTimers.KeyDelta[i] -= DeltaTime;
				if (KeyTimers.KeyDelta[i] <= 0.0f) {
					KeyTimers.KeyDelta[i] = 0.0f;
					ChangeServerKeyState(KeyTimers.KeyID[i]);
				}
			}
		}
	}
}

void ASPPawnCPP::ChangeServerKeyState(int ID)
{

	switch (ID) {
		case 0:
			KeyStates.DEFENCE_KEY = true;
		break;
		case 1:
			KeyStates.DEFENCE_KEY = false;
		break;
		case 2:
			KeyStates.JUMP_KEY = true;
			break;
		case 3:
			KeyStates.JUMP_KEY = false;
			break;
		case 4:
			KeyStates.SATTACK_KEY = true;
			break;
		case 5:
			KeyStates.SATTACK_KEY = false;
			break;
		case 6:
			KeyStates.LATTACK_KEY = true;
			break;
		case 7:
			KeyStates.LATTACK_KEY = false;
			break;
		case 8:
			KeyStates.DOWN_KEY = true;
			break;
		case 9:
			KeyStates.DOWN_KEY = false;
			break;
		case 10:
			KeyStates.UP_KEY = true;
			break;
		case 11:
			KeyStates.UP_KEY = false;
			break;
		case 12:
			KeyStates.RIGHT_KEY = true;
			break;
		case 13:
			KeyStates.RIGHT_KEY = false;
			break;
		case 14:
			KeyStates.LEFT_KEY = true;
			break;
		case 15:
			KeyStates.LEFT_KEY = false;
			break;
		case 16:
			KeyStates.SUPER_LEFT_KEY = true;
			break;
		case 17:
			KeyStates.SUPER_LEFT_KEY = false;
			break;
		case 18:
			KeyStates.SUPER_RIGHT_KEY = true;
			break;
		case 19:
			KeyStates.SUPER_RIGHT_KEY = false;
			break;
	}
}

void ASPPawnCPP::SetNextServerKeyState(int ID, float DeltaTime)
{
	for (int i = 0; i < 20; i++) {
		if (KeyTimers.KeyDelta[i] == 0.0f) {
			KeyTimers.KeyDelta[i] = DeltaTime;
			KeyTimers.KeyID[i] = ID;
			i = 20;
			break;
		}
		if (i == 19) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "SERVER KEY COMMAND QUE FULL. COMMAND SKIPPED");
		}
	}
	
}

void ASPPawnCPP::CheckYDirection()
{
	if (Forces.Y > 0.0f) {
		if (!WorkData.YUp) {
			WorkData.YUp = true;
			ActionYDirectionChange.ExecuteIfBound();
		}
	}
	else if(Forces.Y < 0.0f){
		if (WorkData.YUp) {
			WorkData.YUp = false;
			ActionYDirectionChange.ExecuteIfBound();
		}
	}
}

void ASPPawnCPP::SetCurrentPlayRate(float rate)
{
	WorkData.CurrentPlayRate = rate;
	creature_mesh->animation_speed = WorkData.CurrentPlayRate / WorkData.TimeChange;
}

void ASPPawnCPP::ManageTimeChange(float DeltaTime)
{
	WorkData.TimeTimerDelta += DeltaTime;
	if (WorkData.TimeTimerDelta >= WorkData.TimeTimerGoal) {
		WorkData.TimeTimer = false;
		WorkData.TimeChange = 1.0f;
		creature_mesh->animation_speed = WorkData.CurrentPlayRate;
	}
}

void ASPPawnCPP::SlowTimeByForce(float force)
{	
	if (force < 0.0f) force = -force;

	float l_TimeChange = force / 10.0f;
	float l_LastFor = 0.1f + ((force / 1000.0f) * 0.1);

	if (l_TimeChange <= 1.0f) {
		l_TimeChange = 1.0f;
	}
	else {
		SetTimeChange(l_TimeChange, l_LastFor);
	}
}

void ASPPawnCPP::SlowTimeAfterHit(float force)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), this->StaticClass(), FoundActors);

	int i = 0;
	while (FoundActors.IsValidIndex(i)) {
		ASPPawnCPP *LocalPawn = Cast<ASPPawnCPP>(FoundActors[i]);
		LocalPawn->SlowTimeByForce(force);
		i++;
	}
}

void ASPPawnCPP::Client_Move_Implementation(FVector2D n_Position, int index, bool super)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);

		switch (index) {
		case 0:
			States.MOVE_LEFT = false;
			States.MOVE_RIGHT = true;
			States.SUPER_MOVE = super;
			Move(true, super);
			break;

		case 1:
			States.MOVE_RIGHT = false;
			States.MOVE_LEFT = true;
			States.SUPER_MOVE = super;
			Move(false, super);
			break;
		}

	}
}

bool ASPPawnCPP::Client_Move_Validate(FVector2D n_Position, int index, bool super)
{
	return true;
}

bool ASPPawnCPP::Client_StopMove_Validate(FVector2D n_Position) {
	return true;
}

void ASPPawnCPP::Client_StopMove_Implementation(FVector2D n_Position)
{
	if (!HasAuthority()) {
		if (States.MOVE_LEFT || States.MOVE_RIGHT) {

			FVector CurrentPosition;
			CurrentPosition.X = n_Position.X;
			CurrentPosition.Z = n_Position.Y;
			CurrentPosition.Y = 0.0f;
			SetActorLocation(CurrentPosition, false);
		
			WorkData.ClientTimer = false;
			StopMove();
		}
	}
}

void ASPPawnCPP::Client_Jump_Implementation(FVector2D n_Position, int index)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);

		switch (index) {
		case 0:
			States.JUMP = true;
			break;

		case 1:
			States.JUMP_RIGHT_WALL = true;
			break;

		case 2:
			States.JUMP_LEFT_WALL = true;
			break;
		}

		Jump();
	}
}

bool ASPPawnCPP::Client_Jump_Validate(FVector2D n_Position, int index)
{
	return true;
}

void ASPPawnCPP::Client_StopJump_Implementation(FVector2D n_Position)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		StopJump();
	}
}

bool ASPPawnCPP::Client_StopJump_Validate(FVector2D n_Position)
{
	return true;
}

void ASPPawnCPP::Client_Defence_Implementation(FVector2D n_Position)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		SetUpDefence();
	}
}

bool ASPPawnCPP::Client_Defence_Validate(FVector2D n_Position) {
	return true;
}

void ASPPawnCPP::Client_StopDefence_Implementation(FVector2D n_Position)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		ReleaseDefence();
	}
}

bool ASPPawnCPP::Client_StopDefence_Validate(FVector2D n_Position)
{
	return true;
}

void ASPPawnCPP::Client_LightAttack_Implementation(FVector2D n_Position, int index)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		LightAttack(index);
	}
}

bool ASPPawnCPP::Client_LightAttack_Validate(FVector2D n_Position, int index)
{
	return true;
}

void ASPPawnCPP::Client_CallDelayAction_Implementation(FVector2D n_Position)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		CallDelayAction();
	}
}

bool ASPPawnCPP::Client_CallDelayAction_Validate(FVector2D n_Position)
{
	return true;
}

void ASPPawnCPP::Client_StrongAttack_Implementation(FVector2D n_Position, int index)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);

		StrongAttack(index);
	}
}

bool ASPPawnCPP::Client_StrongAttack_Validate(FVector2D n_Position, int index)
{
	return true;
}

void ASPPawnCPP::Client_StopStrongAttack_Implementation(FVector2D n_Position, int StrongAttackMeter)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		WorkData.StrongAttackMeter = StrongAttackMeter;
		ReleaseStrongAttack();
	}
}

bool ASPPawnCPP::Client_StopStrongAttack_Validate(FVector2D n_Position, int StrongAttackMeter)
{
	return true;
}

void ASPPawnCPP::Client_GetHit_Implementation(FVector2D n_Position, FVector n_KnockBack, float n_HitStun)
{
	if (!HasAuthority()) {
		FVector CurrentPosition;
		CurrentPosition.X = n_Position.X;
		CurrentPosition.Z = n_Position.Y;
		CurrentPosition.Y = 0.0f;
		SetActorLocation(CurrentPosition, false);
		WorkData.HitForce = n_KnockBack;
		WorkData.WasHit = true;

		float PingDelta = GetWorld()->GetFirstPlayerController()->PlayerState->Ping * 2.0f;
		PingDelta /= 1000.0f;
		WorkData.HitStun = n_HitStun - PingDelta;
		ClearStatesWhileHit();

		SlowTimeAfterHit(n_KnockBack.Z);
	}
}

bool ASPPawnCPP::Client_GetHit_Validate(FVector2D n_Position, FVector n_KnockBack, float n_HitStun)
{
	return true;
}