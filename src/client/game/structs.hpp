
#pragma once

struct gentity_s;

#undef vector
struct vec2_t
{
    union
    {
        struct { float x, y; };
        float v[2];
    };

    vec2_t() : x(0), y(0) { }
    vec2_t(float value) : x(value), y(value) { }
    vec2_t(float* value) : x(value[0]), y(value[1]) { }
    vec2_t(float x, float y) : x(x), y(y) { }

    float& operator[](const int i)
    {
        return v[i];
    }
};

struct vec3_t
{
    union
    {
        struct { float x, y, z; };
        struct { float pitch, yaw, roll; };
        float v[3];
    };

    vec3_t() : x(0), y(0), z(0) { }
    vec3_t(float value) : x(value), y(value), z(value) { }
    vec3_t(float* value) : x(value[0]), y(value[1]), z(value[2]) { }
    vec3_t(float x, float y, float z) : x(x), y(y), z(z) { }

    float& operator[](const int i)
    {
        return v[i];
    }
};

struct vec4_t
{
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float v[4];
    };

    vec4_t() : x(0), y(0), z(0), w(0) { }
    vec4_t(float value) : x(value), y(value), z(value), w(value) { }
    vec4_t(float* value) : x(value[0]), y(value[1]), z(value[2]), w(value[3]) { }
    vec4_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

    float& operator[](const int i)
    {
        return v[i];
    }
};

struct GfxImage
{
    char __padding_0x0[0x1C];
    int size;
    short width;
    short height;
    char __padding_0x24[0x4];
    void* pixels;
    char __padding_0x2C[0x38];
    const char* name;
};

struct GfxDrawSurfFields
{
    uint64_t objectId : 15;
    uint64_t reflectionProbeIndex : 8;
    uint64_t hasGfxEntIndex : 1;
    uint64_t customIndex : 5;
    uint64_t materialSortedIndex : 12;
    uint64_t prepass : 2;
    uint64_t useHeroLighting : 1;
    uint64_t sceneLightIndex : 8;
    uint64_t viewModelRender : 1;
    uint64_t surfType : 4;
    uint64_t primarySortKey : 6;
    uint64_t unused : 1;
};

union GfxDrawSurf
{
    GfxDrawSurfFields fields;
    uint64_t packed;
};

struct MaterialInfo
{
    const char* name;
    unsigned char gameFlags;
    unsigned char sortKey;
    unsigned char textureAtlasRowCount;
    unsigned char textureAtlasColumnCount;
    GfxDrawSurf drawSurf;
    unsigned int surfaceTypeBits;
    int padding;
};


struct MaterialTechniqueSet
{
    const char* name;
    unsigned char worldVertFormat;
    unsigned char unused[2];
    MaterialTechniqueSet* remappedTechniqueSet;
    void* techniques[37];
};

struct MaterialTextureDef
{
    unsigned int nameHash;
    char nameStart;
    char nameEnd;
    unsigned char samplerState;
    unsigned char semantic;
    GfxImage* image;
};

struct MaterialConstantDef
{
    unsigned int nameHash;
    char name[12];
    float literal[4];
};

struct GfxStateBits
{
    unsigned int loadBits[2];
};

struct Material
{
    MaterialInfo info;
    unsigned char stateBitsEntry[37];
    unsigned char textureCount;
    unsigned char constantCount;
    unsigned char stateBitsCount;
    unsigned char stateFlags;
    unsigned char cameraRegion;
    unsigned char materialType;
    unsigned char layerCount;
    MaterialTechniqueSet* techniqueSet;
    MaterialTextureDef* textureTable;
    MaterialConstantDef* constantTable;
    GfxStateBits* stateBitsTable;
    const char** subMaterials;
};

union XAssetHeader
{
    Material* material;
    GfxImage* image;
};


union hudelem_color_t
{
    struct 
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    int rgba;
};

struct hudelem_s
{
    he_type_t type;
    float x;
    float y;
    float z;
    int targetEntNum;
    float fontScale;
    float fromFontScale;
    int fontScaleStartTime;
    int fontScaleTime;
    int font;
    int alignOrg;
    int alignScreen;
    hudelem_color_t color;
    hudelem_color_t fromColor;
    int fadeStartTime;
    int fadeTime;
    int label;
    int width;
    int height;
    int materialIndex;
    int fromWidth;
    int fromHeight;
    int scaleStartTime;
    int scaleTime;
    float fromX;
    float fromY;
    int fromAlignOrg;
    int fromAlignScreen;
    int moveStartTime;
    int moveTime;
    int time;
    int duration;
    float value;
    int text;
    float sort;
    hudelem_color_t glowColor;
    int fxBirthTime;
    int fxLetterTime;
    int fxDecayStartTime;
    int fxDecayDuration;
    int soundID;
    int flags;
};

struct game_hudelem_s
{ 
    hudelem_s elem; 
    int clientNum; 
};

struct DObjAnimMat
{
    float quat[4];
    float trans[3];
    float transWeight;
};

struct XSurface
{

};

struct XModelSurfs
{
    const char* name;
    XSurface* surfs;
    unsigned short numsurfs;
    int partBits[6];
};

struct XModelLodInfo
{
    float dist;
    unsigned short numsurfs;
    unsigned short surfIndex;
    XModelSurfs* modelSurfs;
    int partBits[6];
    XSurface* surfs;
};

struct Bounds
{
    float midPoint[3];
    float halfSize[3];
};

struct XModelCollSurf_s
{
    Bounds bounds;
    int boneIdx;
    int contents;
    int surfFlags;
};

struct XBoneInfo
{
    Bounds bounds;
    union
    {
        float radiusSquared;
        unsigned int radiusSquaredAsInt;
    };
};

struct PhysPreset
{
    const char* name;
    int type;
    float mass;
    float bounce;
    float friction;
    float bulletForceScale;
    float explosiveForceScale;
    const char* sndAliasPrefix;
    float piecesSpreadFraction;
    float piecesUpwardVelocity;
    float minMomentum;
    float maxMomentum;
    float minPitch;
    float maxPitch;
    PhysPresetScaling volumeType;
    PhysPresetScaling pitchType;
    bool tempDefaultToCylinder;
    bool perSurfaceSndAlias;
};

struct PhysGeomInfo
{
    void* brushWrapper;
    int type;
    float orientation[3][3];
    Bounds bounds;
};

struct PhysMass
{
    float centerOfMass[3];
    float momentsOfInertia[3];
    float productsOfInertia[3];
};

struct PhysCollmap
{
    const char* name;
    unsigned int count;
    PhysGeomInfo* geoms;
    PhysMass mass;
    Bounds bounds;
};

struct XModel
{
    const char* name;
    unsigned char numBones;
    unsigned char numRootBones;
    unsigned char numsurfs;
    float scale;
    unsigned int noScalePartBits[6];
    unsigned short* boneNames;
    unsigned char* parentList;
    short* quats;
    float* trans;
    unsigned char* partClassification;
    DObjAnimMat* baseMat;
    Material** materialHandles;
    XModelLodInfo lodInfo[4];
    char maxLoadedLod;
    char numLods;
    char collLod;
    unsigned char flags;
    XModelCollSurf_s* collSurfs;
    int numCollSurfs;
    int contents;
    XBoneInfo* boneInfo;
    float radius;
    Bounds bounds;
    unsigned short* invHighMipRadius;
    int memUsage;
    PhysPreset* physPreset;
    PhysCollmap* physCollmap;
    float quantization;
};

union Weapon
{
    struct
    {
        unsigned int weaponIdx : 8;
        unsigned int weaponVariation : 4;
        unsigned int weaponScopes : 3;
        unsigned int weaponUnderBarrels : 2;
        unsigned int weaponOthers : 4;
        unsigned int scopeVariation : 3;
        unsigned int padding : 8;
    };
    unsigned int data;
};

struct playerEvents_t
{
    int eventSequence;
    int events[4];
    unsigned int eventParms[4];
    int oldEventSequence;
    int timeADSCameUp;
};

struct SprintState
{
    int sprintButtonUpRequired;
    int sprintDelay;
    int lastSprintStart;
    int lastSprintEnd;
    int sprintStartMaxLength;
};

struct MantleState
{
    float yaw;
    int timer;
    int transIndex;
    int flags;
};

struct PlayerActiveWeaponState
{
    int weapAnim;
    int weaponTime;
    int weaponDelay;
    int weaponRestrictKickTime;
    int weaponState;
    int weapHandFlags;
    unsigned int weaponShotCount;
};

struct PlayerEquippedWeaponState
{
    bool usedBefore;
    bool dualWielding;
    bool inAltMode;
    bool needsRechamber[2];
    int zoomLevelIndex;
};

struct GlobalAmmo
{
    int ammoType;
    int ammoCount;
};

struct ClipAmmo
{
    int clipIndex;
    int ammoCount[2];
};

struct PlayerWeaponCommonState
{
    Weapon offHand;
    OffhandClass offhandPrimary;
    OffhandClass offhandSecondary;
    Weapon weapon;
    int weapFlags;
    float fWeaponPosFrac;
    float aimSpreadScale;
    int adsDelayTime;
    int spreadOverride;
    int spreadOverrideState;
    float fAimSpreadMovementScale;
    PlayerHandIndex lastWeaponHand;
    GlobalAmmo ammoNotInClip[15];
    ClipAmmo ammoInClip[15];
    int weapLockFlags;
    int weapLockedEntnum;
    float weapLockedPos[3];
    int weaponIdleTime;
    Weapon forcedViewAnimWeapon;
    int forcedViewAnimWeaponState;
    Weapon forcedViewAnimOriginalWeapon;
};

struct ActionSlotParam_SpecifyWeapon
{
    Weapon weapon;
};

struct ActionSlotParam
{
    ActionSlotParam_SpecifyWeapon specifyWeapon;
};

struct playerState_s
{
    int commandTime;
    int pm_type;
    int pm_time;
    int pm_flags;
    int otherFlags;
    int linkFlags;
    int bobCycle;
    float origin[3];
    float velocity[3];
    int grenadeTimeLeft;
    int throwbackGrenadeOwner;
    int throwbackGrenadeTimeLeft;
    Weapon throwbackWeapon;
    int remoteEyesEnt;
    int remoteEyesTagname;
    int remoteControlEnt;
    int vehicleViewCalcEnt;
    int foliageSoundTime;
    int gravity;
    int speed;
    float delta_angles[3];
    int groundEntityNum;
    float vLadderVec[3];
    int jumpTime;
    float jumpOriginZ;
    int legsTimer;
    int legsAnim;
    int torsoTimer;
    int torsoAnim;
    int legsAnimDuration;
    int torsoAnimDuration;
    int damageTimer;
    int damageDuration;
    int flinchYawAnim;
    int movementDir;
    int eFlags;
    playerEvents_t pe;
    int unpredictableEventSequence;
    int unpredictableEventSequenceOld;
    int unpredictableEvents[4];
    unsigned int unpredictableEventParms[4];
    int clientNum;
    int viewmodelIndex;
    float viewangles[3];
    int viewHeightTarget;
    float viewHeightCurrent;
    int viewHeightLerpTime;
    int viewHeightLerpTarget;
    int viewHeightLerpDown;
    float viewAngleClampBase[2];
    float viewAngleClampRange[2];
    float viewAngleResistMax[2];
    float viewAngleResistMin[2];
    int damageEvent;
    int damageYaw;
    int damagePitch;
    int damageCount;
    int damageFlags;
    int stats[4];
    float proneDirection;
    float proneDirectionPitch;
    float proneTorsoPitch;
    ViewLockTypes viewlocked;
    int viewlocked_entNum;
    float linkAngles[3];
    float linkWeaponAngles[3];
    int linkWeaponEnt;
    int loopSound;
    int vehicleType;
    float groundTiltAngles[3];
    int cursorHint;
    int cursorHintString;
    int cursorHintEntIndex;
    int cursorHintDualWield;
    int motionTrackerEnabled;
    int locationSelectionInfo;
    SprintState sprintState;
    float holdBreathScale;
    int holdBreathTimer;
    float moveSpeedScaleMultiplier;
    MantleState mantleState;
    PlayerActiveWeaponState weapState[2];
    Weapon weaponsEquipped[15];
    PlayerEquippedWeaponState weapEquippedData[15];
    PlayerWeaponCommonState weapCommon;
    int meleeChargeDist;
    int meleeChargeTime;
    float meleeChargeYaw;
    unsigned int perks[2];
    unsigned int perkSlots[9];
    ActionSlotType actionSlotType[4];
    ActionSlotParam actionSlotParam[4];
    int weaponHudIconOverrides[6];
    int animScriptedType;
    int shellshockIndex;
    int shellshockTime;
    int shellshockDuration;
    float viewKickScale;
    float dofNearStart;
    float dofNearEnd;
    float dofFarStart;
    float dofFarEnd;
    float dofNearBlur;
    float dofFarBlur;
    float dofViewmodelStart;
    float dofViewmodelEnd;
    struct 
    {
        hudelem_s elem[256];
    } hud;
    int recoilScale;
    int diveDirection;
    int stunTime;
};

struct usercmd_s
{
    int serverTime;
    int buttons;
    int angles[3];
    Weapon weapon;
    Weapon offHand;
    char forwardmove;
    char rightmove;
    unsigned char upmove;
    unsigned char downmove;
    char pitchmove;
    char yawmove;
    float gunPitch;
    float gunYaw;
    float gunXOfs;
    float gunYOfs;
    float gunZOfs;
    short meleeChargeYaw;
    unsigned char meleeChargeDist;
    char selectedLoc[2];
    unsigned char selectedLocAngle;
    char remoteControlAngles[2];
};

struct clientPersistent_t
{
    clientConnected_t connected;
    usercmd_s cmd;
    usercmd_s oldcmd;
    int maxHealth;
    float moveSpeedScaleMultiplier;
    int motionTrackerEnabled;
    char playerName[32];
};

struct EntHandle
{
    unsigned short number;
    unsigned short infoIndex;
};

struct viewClamp
{
    float start[2];
    float current[2];
    float goal[2];
};

struct viewClampState
{
    viewClamp min;
    viewClamp max;
    viewClamp resistMax;
    viewClamp resistMin;
    float accelTime;
    float decelTime;
    float totalTime;
    float startTime;
};

struct gclient_s
{
    playerState_s ps;
    clientPersistent_t pers;
    int flags;
    int buttons;
    int oldbuttons;
    int latched_buttons;
    int buttonsSinceLastFrame;
    float fGunPitch;
    float fGunYaw;
    float fGunXOfs;
    float fGunYOfs;
    float fGunZOfs;
    int damage_blood;
    int damage_stun;
    float damage_from[3];
    int damage_fromWorld;
    float currentAimSpreadScale;
    gentity_s* pHitHitEnt;
    EntHandle pLookatEnt;
    float prevLinkedInvQuat[4];
    bool prevLinkAnglesSet;
    bool link_rotationMovesEyePos;
    bool link_doCollision;
    bool link_useTagAnglesForViewAngles;
    bool link_useBaseAnglesForViewClamp;
    float linkAnglesFrac;
    viewClampState link_viewClamp;
    int inControlTime;
    int lastTouchTime;
    EntHandle useHoldEntity;
    int useHoldTime;
    int useButtonDone;
    int bDisableAutoPickup;
    float criticalBulletDamageDist;
    int invulnerableDuration;
    int invulnerableExpireTime;
    bool invulnerableActivated;
    bool invulnerableEnabled;
    bool deathShieldEnabled;
    unsigned char attackerCount;
    float attackerAccuracyMultiplier;
    float accuracyDistMultiplier;
    float damageMultiplier;
    bool playerMoved;
    float playerLOSCheckPos[2];
    float playerLOSCheckDir[2];
    int playerLOSPosTime;
    int playerADSTargetTime;
    float futurePos[3];
    int futurePosUpdateTime;
    Weapon lastWeapon;
    bool lastWeaponAltStatus;
    Weapon lastStowedWeapon;
    bool previouslyFiring;
    bool previouslyFiringLeftHand;
    bool previouslyUsingNightVision;
    bool groundEntChanged;
    int groundTiltEntNum;
    int hudElemLastAssignedSoundID;
    int visionDuration[5];
    char visionName[5][64];
    XModel* weaponModel;
    XModel* knifeModel;
    int lastStand;
    unsigned short attachShieldTagName;
    int shieldHealth;
    hintType_t hintForcedType;
    int hintForcedString;
    Weapon streamWeapon;
};

struct trajectory_t
{
    trType_t trType;
    int trTime;
    int trDuration;
    float trBase[3];
    float trDelta[3];
};

struct LerpEntityStateTurret
{
    float gunAngles[3];
    int lastBarrelRotChangeTime;
    unsigned short lastBarrelRotChangeRate;
    unsigned short lastHeatChangeLevel;
    int lastHeatChangeTime;
    bool isBarrelRotating;
    bool isOverheat;
    bool isHeatingUp;
    bool isOnCompass;
};

struct LerpEntityStateLoopFx
{
    float cullDist;
    int period;
};

struct LerpEntityStateActor
{
    unsigned char species;
    unsigned char stepMove;
    int lookAtEntityNum;
    int hitLocation;
    float impactVector[3];
    unsigned char headIcon;
    unsigned char headIconTeam;
    short padding;
};

struct LerpEntityStatePrimaryLight
{
    unsigned char colorAndExp[4];
    float intensity;
    float radius;
    float cosHalfFovOuter;
    float cosHalfFovInner;
};

struct LerpEntityStatePlayer
{
    int movementDir;
};

struct LerpEntityStateVehicle
{
    unsigned int indices;
    unsigned int flags;
    float bodyPitch;
    float bodyRoll;
    float steerYaw;
    float gunPitch;
    float gunYaw;
};

struct LerpEntityStateMissile
{
    int launchTime;
    MissileFlightMode flightMode;
};

struct LerpEntityStateSoundBlend
{
    float lerp;
    float volumeScale;
};

struct LerpEntityStateBulletTracer
{
    float end[3];
};

struct LerpEntityStateBulletHit
{
    float start[3];
};

struct LerpEntityStateEarthquake
{
    float scale;
    float radius;
    int duration;
};

struct LerpEntityStateCustomExplode
{
    int startTime;
};

struct LerpEntityStateExplosion
{
    float innerRadius;
    float outerRadius;
    float magnitude;
};

struct LerpEntityStateExplosionJolt
{
    float innerRadius;
    float outerRadius;
    float impulse[3];
};

struct LerpEntityStatePhysicsJitter
{
    float innerRadius;
    float outerRadius;
    float minDisplacement;
    float maxDisplacement;
};

struct LerpEntityStateRadiusDamage
{
    float range;
    int damageMin;
    int damageMax;
};

struct LerpEntityStateScriptMover
{
    int entToTakeMarksFrom;
    int xModelIndex;
};

union LerpEntityStateTypeUnion
{
    LerpEntityStateTurret turret;
    LerpEntityStateLoopFx loopFx;
    LerpEntityStateActor actor;
    LerpEntityStatePrimaryLight primaryLight;
    LerpEntityStatePlayer player;
    LerpEntityStateVehicle vehicle;
    LerpEntityStateMissile missile;
    LerpEntityStateSoundBlend soundBlend;
    LerpEntityStateBulletTracer bulletTracer;
    LerpEntityStateBulletHit bulletHit;
    LerpEntityStateEarthquake earthquake;
    LerpEntityStateCustomExplode customExplode;
    LerpEntityStateExplosion explosion;
    LerpEntityStateExplosionJolt explosionJolt;
    LerpEntityStatePhysicsJitter physicsJitter;
    LerpEntityStateRadiusDamage radiusDamage;
    LerpEntityStateScriptMover scriptMover;
};

struct LerpEntityState
{
    int eFlags;
    trajectory_t pos;
    trajectory_t apos;
    LerpEntityStateTypeUnion u;
    int useCount;
};

struct StaticEntityStateVehicle
{
    int team;
};

struct StaticEntityStateTurret
{
    unsigned short carrierEntNum;
};

union StaticEntityStateTypeUnion
{
    StaticEntityStateVehicle vehicle;
    StaticEntityStateTurret turret;
};

struct entityState_s
{
    unsigned char eType;
    unsigned char surfType;
    union
    {
        unsigned char scale;
        unsigned char eventParm2;
        unsigned char hintString;
    };
    unsigned short loopSound;
    unsigned int eventParm;
    LerpEntityState lerp;
    StaticEntityStateTypeUnion staticState;
    Weapon weapon;
    int inAltWeaponMode;
    unsigned short number;
    unsigned short otherEntityNum;
    unsigned short groundEntityNum;
    union
    {
        unsigned short brushModel;
        unsigned short triggerModel;
        unsigned short primaryLight;
    };
    int time2;
    int solid;
    int eventSequence;
    unsigned char events[4];
    unsigned int eventParms[4];
    union
    {
        int hintType;
        int perClientFxBits;
        int actorFlags;
    };
};

struct entityShared_t
{
    unsigned char isLinked;
    unsigned char modelType;
    unsigned char svFlags;
    unsigned char eventType;
    unsigned char isInUse;
    Bounds box;
    int contents;
    Bounds absBox;
    float currentOrigin[3];
    float currentAngles[3];
    EntHandle ownerNum;
    int eventTime;
};

struct pathlink_s
{
    float fDist;
    unsigned short nodeNum;
    unsigned char disconnectCount;
    unsigned char negotiationLink;
    unsigned char flags;
    unsigned char ubBadPlaceCount[3];
};

struct pathnode_constant_t
{
    nodeType type;
    unsigned short spawnflags;
    unsigned short targetname;
    unsigned short script_linkName;
    unsigned short script_noteworthy;
    unsigned short target;
    unsigned short animscript;
    int animscriptfunc;
    float vOrigin[3];
    float fAngle;
    float forward[2];
    float fRadius;
    union
    {
        float minUseDistSq;
        PathNodeErrorCode error;
    };
    short wOverlapNode[2];
    unsigned short totalLinkCount;
    pathlink_s* Links;
};

struct SentientHandle
{
    unsigned short number;
    unsigned short infoIndex;
};

struct pathnode_dynamic_t
{
    SentientHandle pOwner;
    int iFreeTime;
    int iValidTime[3];
    int dangerousNodeTime[3];
    int inPlayerLOSTime;
    short wLinkCount;
    short wOverlapCount;
    short turretEntNumber;
    unsigned char userCount;
    bool hasBadPlaceLink;
};

struct pathnode_t;
struct pathnode_transient_t
{
    int iSearchFrame;
    pathnode_t* pNextOpen;
    pathnode_t* pPrevOpen;
    pathnode_t* pParent;
    float fCost;
    float fHeuristic;
    union
    {
        float nodeCost;
        int linkIndex;
    };
};

/* 5523 */
struct pathnode_t
{
    pathnode_constant_t constant;
    pathnode_dynamic_t dynamic;
    pathnode_transient_t transient;
};

struct sentient_s
{
    gentity_s* ent;
    team_t eTeam;
    int iThreatBias;
    int iThreatBiasGroupIndex;
    bool bIgnoreMe;
    bool bIgnoreAll;
    bool originChanged;
    float oldOrigin[3];
    float maxVisibleDist;
    float surprisedByMeDistSq;
    int iEnemyNotifyTime;
    int attackerCount;
    gentity_s* lastAttacker;
    EntHandle syncedMeleeEnt;
    EntHandle targetEnt;
    EntHandle scriptTargetEnt;
    float entityTargetThreat;
    int meleeAttackerSpot[4];
    float attackerAccuracy;
    bool ignoreRandomBulletDamage;
    bool doDamageToAll;
    bool turretInvulnerability;
    bool scriptTargetHasTagEye;
    pathnode_t* pClaimedNode;
    pathnode_t* pPrevClaimedNode;
    pathnode_t* pNearestNode;
    unsigned char bNearestNodeValid;
    unsigned char bNearestNodeBad;
    bool bOnlyGoodNearestNodes;
    bool inuse;
    bool expendable;
    int banNodeTime;
};

struct ai_transition_cmd_t
{
    ai_state_transition_t eTransition;
    ai_state_t eState;
};

struct ActorShoot
{
    float accuracy;
    float playerSightAccuracy;
    unsigned int missCount;
    unsigned int hitCount;
    float debugLastAccuracy;
    int lastShotTime;
    bool lastCanShootEnemyResult;
    int lastCanShootEnemyTime;
};

struct ActorOrientation
{
    float fDesiredBodyYaw;
    float fLookPitch;
    float fLookYaw;
    float maxFaceEnemyDistSq;
    unsigned char relativeDir;
    unsigned char prevRelativeDir;
    bool faceMotion;
    bool gunBlockedByWall;
    bool lockScriptOrient;
    float vLookForward[3];
    float vLookRight[3];
    float vLookUp[3];
};

struct ai_orient_t
{
    ai_orient_mode_t eMode;
    float fDesiredLookPitch;
    float fDesiredLookYaw;
    float fDesiredBodyYaw;
};

/* 6252 */
struct ActorPainDeath
{
    int iPainTime;
    int minPainDamage;
    bool allowPain;
    bool allowDeath;
    bool delayedDeath;
    bool dieQuietly;
    bool forceRagdollImmediate;
    int iDamageTaken;
    int iDamageYaw;
    float damageDir[3];
    unsigned short damageHitLoc;
    unsigned short damageWeapon;
    int damageMOD;
};

/* 6253 */
struct ActorProne
{
    unsigned short animProneLow;
    unsigned short animProneLevel;
    unsigned short animProneHigh;
    bool bProneOK;
    unsigned char feetDirection;
    float fInvProneAnimLowPitch;
    float fInvProneAnimHighPitch;
    float fProneLastDiff;
};

/* 6257 */
struct ActorCachedInfo
{
    int time;
    float pos[3];
    float dir[3];
};

/* 6258 */
struct ActorLookAtInfo
{
    float vLookAtPos[3];
    float fLookAtTurnAngle;
    float fLookAtTurnSpeed;
    float fLookAtTurnAccel;
    float fLookAtAnimYawLimit;
    float fLookAtYawLimit;
    unsigned short animLookAtStraight;
    unsigned short animLookAtLeft;
    unsigned short animLookAtRight;
    bool bDoLookAt;
    bool bLookAtSetup;
    int iLookAtBlendEndTime;
    float fLookAtAnimBlendRate;
    float fLookAtLimitBlendRate;
};

struct actor_prone_info_s
{
    bool bCorpseOrientation;
    bool orientPitch;
    bool prone;
    int iProneTime;
    int iProneTrans;
    float fBodyHeight;
    union
    {
        float fTorsoPitch;
        float fBodyPitch;
    };
    union
    {
        float fWaistPitch;
        float fBodyRoll;
    };
};

struct ActorDelayedWeaponDrop
{
    float matrix[4][3];
    int time;
    unsigned short tagName;
    DelayedWeapDropState state;
    Weapon weapon;
};

struct ActorCoverArrivalInfo
{
    int arrivalNotifyRequested;
    int animscriptOverrideRunTo;
    float animscriptOverrideRunToPos[3];
    float offsetIncrement[2];
    int offsetAdjustCount;
    float arrivalYaw;
    EntHandle scriptedArrivalEnt;
};

struct ActorNodeSelect
{
    int numCoverNodesInGoal;
    int iPotentialCoverNodeCount;
    int iPotentialAmbushNodeCount;
    int nextFindBestCoverTime;
    bool requestDifferentCover;
    bool keepClaimedNode;
    bool keepClaimedNodeIfValid;
    bool keepNodeDuringScriptedAnim;
    bool doDangerReact;
    int dangerReactGoalTime;
    int dangerReactDuration;
    float engageMinDist;
    float engageMinFalloffDist;
    float engageMaxDist;
    float engageMaxFalloffDist;
};

struct ActorSight
{
    float fovDot;
    float fovDotBusy;
    float fMaxSightDistSqrd;
    int latency;
    bool ignoreCloseFoliage;
    bool lastEnemySightPosValid;
    float lastEnemySightPos[3];
    float anglesToLikelyEnemyPath[3];
    int faceLikelyEnemyPathNeedCheckTime;
    int faceLikelyEnemyPathNeedRecalculateTime;
    const pathnode_t* faceLikelyEnemyPathNode;
    float upAimLimit;
    float downAimLimit;
    float rightAimLimit;
    float leftAimLimit;
};

struct vis_cache_t
{
    bool bVisible;
    int iLastUpdateTime;
    int iLastVisTime;
};

struct sentient_info_t
{
    vis_cache_t VisCache;
    int iLastAttackMeTime;
    int lastKnownPosTime;
    int attackTime;
    bool surprised;
    float vLastKnownPos[3];
    pathnode_t* pLastKnownNode;
};

struct ai_suppression_t
{
    int iTime;
    sentient_s* pSuppressor;
    float clipPlane[3];
    int movementOnly;
};

struct ActorSuppression
{
    int ignoreSuppression;
    int suppressionWait;
    int suppressionDuration;
    int suppressionStartTime;
    float suppressionMeter;
};

struct ActorCombat
{
    int ambushStartTime;
    int exposedStartTime;
    int exposedDuration;
    bool provideCoveringFire;
    bool doingAmbush;
    bool currentAmbushNodeInvalid;
    bool allAmbushNodesFailed;
    CombatMode combatMode;
    AlertLevel alertLevel;
    bool damageShield;
    bool noGrenadeReturnThrow;
    bool noAttackerAccuracyMod;
    float frontShieldAngleCos;
};

struct potential_threat_t
{
    bool isEnabled;
    float direction[2];
};

struct ActorSecondaryTarget
{
    EntHandle entity;
    float dirToEnt[2];
    float distToEnt;
    pathnode_t* node;
};

struct ActorThreat
{
    bool hasThreateningEnemy;
    bool allEnemiesInSimilarDir;
    bool newEnemyReaction;
    bool ignoreExplosionEvents;
    bool bPacifist;
    int iPacifistWait;
    potential_threat_t potentialThreat;
    int threatUpdateTime;
    float footstepDetectDistSq;
    float footstepDetectDistWalkSq;
    float footstepDetectDistSprintSq;
    float reactionTargetPos[3];
    float newEnemyReactionDistSq;
    float highlyAwareRadius;
    SentientHandle pFavoriteEnemy;
    int numSecondaryTarget;
    ActorSecondaryTarget secondaryTargets[2];
};

struct ActorGrenade
{
    float grenadeAwareness;
    int grenadeExistTime;
    EntHandle pGrenade;
    Weapon iGrenadeWeapon;
    unsigned short GrenadeTossMethod;
    bool bGrenadeTossValid;
    bool bGrenadeTargetValid;
    bool grenadeTossWithBounce;
    int iGrenadeAmmo;
    float vGrenadeTossPos[3];
    float vGrenadeTargetPos[3];
    float vGrenadeTossVel[3];
    float pickupPos[3];
};

struct ActorTurret
{
    gentity_s* pTurret;
    unsigned short turretAnim;
    unsigned char turretAnimSet;
    char padding;
};

struct scr_animscript_t
{
    int func;
    int endFunc;
    unsigned short name;
    short padding;
};

struct actor_physics_t
{
    float vOrigin[3];
    float vVelocity[3];
    unsigned short groundEntNum;
    int iFootstepTimer;
    int bHasGroundPlane;
    float groundplaneSlope;
    int iSurfaceType;
    float vWishDelta[3];
    int bIsAlive;
    int iEntNum;
    aiphys_t ePhysicsType;
    float fGravity;
    int iMsec;
    Bounds bounds;
    bool prone;
    bool stepMove;
    int iTraceMask;
    int foliageSoundTime;
    int iNumTouch;
    int iTouchEnts[32];
    int iHitEntnum;
    float vHitOrigin[2];
    float vHitNormal[2];
    unsigned char bStuck;
    unsigned char bDeflected;
    bool pathGoingDown;
    AI_STAIRS_STATE stairsState;
};

struct ActorNavigation
{
    float fWalkDist;
    float fWalkDistFacingMotion;
    bool isInBadPlace;
    char badplaceRecheckPathLen;
    float badPlaceAwareness;
};

struct pathpoint_t
{
    float vOrigPoint[3];
    float fDir2D[2];
    float fOrigLength;
    int iNodeNum;
};

struct path_t
{
    pathpoint_t pts[32];
    char wPathLen;
    char wOrigPathLen;
    char wDodgeCount;
    char wNegotiationStartNode;
    char lookaheadNextNode;
    char pathChangeNotifyNode;
    short wDodgeEntity;
    float vFinalGoal[3];
    float vStartPos[3];
    float lookaheadDir[3];
    float lookaheadPos[3];
    float fLookaheadDist;
    float fLookaheadAmount;
    float fLookaheadDistToNextNode;
    int minLookAheadNodes;
    int flags;
    int iPathTime;
    int iPathClearedTime;
    team_t eTeam;
    float fCurrLength;
    float vCurrPoint[3];
    int iPathEndTime;
    float pathEndAnimDistSq;
    bool pathEndAnimNotified;
    bool lookaheadHitsStairs;
    bool useChokePoints;
    float pathChangeTracePos[2];
    int randomPercent;
};

struct path_trim_t
{
    int iIndex;
    int iDelta;
};

struct actor_goal_s
{
    float pos[3];
    float radius;
    float height;
    pathnode_t* node;
    gentity_s* volume;
};

struct actor_s
{
    gentity_s* ent;
    sentient_s* sentient;
    AISpecies species;
    int talkToSpecies;
    ai_state_t eState[6];
    ai_substate_t eSubState[6];
    unsigned int stateLevel;
    int iStateTime;
    int preThinkTime;
    ai_transition_cmd_t StateTransitions[13];
    unsigned int transitionCount;
    ai_state_t eSimulatedState[6];
    unsigned int simulatedStateLevel;
    unsigned short properName;
    unsigned short weaponName;
    ActorShoot shoot;
    ActorOrientation orientation;
    ai_orient_t CodeOrient;
    ai_orient_t ScriptOrient;
    ActorPainDeath painDeath;
    ActorProne prone;
    actor_prone_info_s ProneInfo;
    ActorCachedInfo eyeInfo;
    ActorCachedInfo muzzleInfo;
    ActorLookAtInfo lookAtInfo;
    ActorDelayedWeaponDrop weapDrops[2];
    ActorCoverArrivalInfo arrivalInfo;
    ActorNodeSelect nodeSelect;
    ActorSight sight;
    sentient_info_t sentientInfo[50];
    ai_suppression_t Suppressant[4];
    ActorSuppression suppression;
    ActorCombat combat;
    ActorThreat threat;
    ActorGrenade grenade;
    ActorTurret turret;
    ai_stance_e eAllowedStances;
    unsigned short AnimScriptHandle;
    scr_animscript_t* pAnimScriptFunc;
    scr_animscript_t* pPrevAnimScriptFunc;
    scr_animscript_t* pAttackScriptFunc;
    scr_animscript_t* pRequestedScript;
    scr_animscript_t AnimScriptSpecific;
    ai_traverse_mode_t eTraverseMode;
    unsigned char moveMode;
    bool useCombatScriptAtCover;
    bool prevAnimScriptTerminated;
    bool safeToChangeScript;
    bool bUseGoalWeight;
    bool bCanClimbLadders;
    ai_animmode_t eAnimMode;
    ai_animmode_t eScriptSetAnimMode;
    unsigned short anim_pose;
    actor_physics_t Physics;
    ActorNavigation navigation;
    path_t Path;
    path_trim_t TrimInfo;
    float fInterval;
    int pathWaitTime;
    int iTeamMoveWaitTime;
    int iTeamMoveDodgeTime;
    int stoppedWaitStartTime;
    EntHandle stoppedWaitEnt;
    actor_s* pPileUpActor;
    gentity_s* pPileUpEnt;
    int bDontAvoidPlayer;
    float sideMove;
    bool noDodgeMove;
    int mayMoveTime;
    float nodeOffsetDist;
    float nodeOffsetPos[3];
    float prevMoveDir[2];
    float leanAmount;
    float turnRate;
    EntHandle pCloseEnt;
    int moveHistoryIndex;
    bool moveHistoryConsistent;
    float moveHistory[10][2];
    actor_goal_s codeGoal;
    aiGoalSources codeGoalSrc;
    actor_goal_s scriptGoal;
    EntHandle scriptGoalEnt;
    int scriptGoalEntLastUpdate;
    int scriptGoalEntUpdateInterval;
    float pathEnemyLookahead;
    float pathEnemyFightDist;
    float meleeAttackDist;
    bool useEnemyGoal;
    bool useMeleeAttackSpot;
    bool goalPosChanged;
    bool commitToFixedNode;
    bool ignoreForFixedNodeSafeCheck;
    bool fixedNode;
    float fixedNodeSafeRadius;
    float fixedNodeSafeVolumeRadiusSq;
    EntHandle fixedNodeSafeVolume;
    int bDropWeapon;
    int bDrawOnCompass;
    int iTraceCount;
    int iUseHintString;
    unsigned char useable;
    bool ignoreTriggers;
    bool pushable;
    bool script_pushable;
    bool inuse;
    int shieldHealth;
    unsigned short shieldName;
    unsigned short potentialCoverNode[10];
    unsigned short potentialAmbushNode[32];
    const char* pszDebugInfo;
};

struct VehiclePathNode
{
    unsigned short name;
    unsigned short target;
    unsigned short script_linkname;
    unsigned short script_noteworthy;
    short index;
    short flags;
    float speed;
    float lookAhead;
    float origin[3];
    float dir[3];
    float angles[3];
    float length;
    short nextIdx;
    short prevIdx;
    short notifyIdx;
    short padding;
};

struct VehiclePathPos
{
    short nodeIdx;
    short flags;
    float frac;
    float speed;
    float lookAhead;
    float origin[3];
    float angles[3];
    float lookPos[3];
    VehiclePathNode switchNode[2];
};

struct VehicleTrackObstacle
{
    float origin[2];
    float radius;
};

struct VehicleTrackSector
{
    float startEdgeDir[2];
    float startEdgeDist;
    float leftEdgeDir[2];
    float leftEdgeDist;
    float rightEdgeDir[2];
    float rightEdgeDist;
    float sectorLength;
    float sectorWidth;
    float totalPriorLength;
    float totalFollowingLength;
    VehicleTrackObstacle* obstacles;
    unsigned int obstacleCount;
};

struct VehicleTrackSegment
{
    const char* targetName;
    VehicleTrackSector* sectors;
    unsigned int sectorCount;
    const VehicleTrackSegment** nextBranches;
    unsigned int nextBranchesCount;
    const VehicleTrackSegment** prevBranches;
    unsigned int prevBranchesCount;
    float endEdgeDir[2];
    float endEdgeDist;
    float totalLength;
};

struct VehicleTrackpos
{
    const VehicleTrackSegment* segment;
    unsigned int sectorIndex;
};

/* 6284 */
struct vehicle_physic_t
{
    float origin[3];
    float prevOrigin[3];
    float angles[3];
    float prevAngles[3];
    Bounds bounds;
    float vel[3];
    float bodyVel[3];
    float accel[3];
    float bodyAccel[3];
    float rotVel[3];
    float bodyPitchVel;
    float bodyRollVel;
    float wheelZVel[6];
    float wheelZPos[6];
    int wheelSurfType[6];
    float wheelCompression[6];
    float wheelPrevCompression[6];
    float worldTilt[3];
    float worldTiltVel[3];
};

struct PhysVehicleWheel
{
    float originLocal[3];
    unsigned int flags;
    float forward[3];
    float speedRatio;
    float accelRatio;
    float sideFriction;
    bool slipping;
};

struct dBase
{
};

struct dObject : dBase
{
    dObject* next;
    dObject** tome;
    void* userdata;
    int tag;
    void* world;
};

struct dxJoint;
struct dxJointNode
{
    dxJoint* joint;
    void* body;
    int bodyTag;
    dxJointNode* next;
};

struct dxJoint : dObject
{
    JointType type;
    int flags;
    dxJointNode node[2];
};

struct JointOrientation : dxJoint
{
    unsigned int constraintRowCount;
    float rollResistance;
    float pitchResistance;
    float yawResistance;
    float uprightStrengthRoll;
    float uprightStrengthPitch;
    bool uprightCorrection;
    float targetPitch;
    float maxRoll;
    float maxPitch;
};

struct JointPath : dxJoint
{
    int substeps;
    float pos[2];
    float vel[2];
    float yaw;
    float yawVel;
    float cfm;
    float erp;
    bool enabled;
};

struct VehiclePhysDef
{
    int physicsEnabled;
    const char* physPresetName;
    const PhysPreset* physPreset;
    const char* accelGraphName;
    VehicleAxleType steeringAxle;
    VehicleAxleType powerAxle;
    VehicleAxleType brakingAxle;
    float topSpeed;
    float reverseSpeed;
    float maxVelocity;
    float maxPitch;
    float maxRoll;
    float suspensionTravelFront;
    float suspensionTravelRear;
    float suspensionStrengthFront;
    float suspensionDampingFront;
    float suspensionStrengthRear;
    float suspensionDampingRear;
    float frictionBraking;
    float frictionCoasting;
    float frictionTopSpeed;
    float frictionSide;
    float frictionSideRear;
    float velocityDependentSlip;
    float rollStability;
    float rollResistance;
    float pitchResistance;
    float yawResistance;
    float uprightStrengthPitch;
    float uprightStrengthRoll;
    float targetAirPitch;
    float airYawTorque;
    float airPitchTorque;
    float minimumMomentumForCollision;
    float collisionLaunchForceScale;
    float wreckedMassScale;
    float wreckedBodyFriction;
    float minimumJoltForNotify;
    float slipThresholdFront;
    float slipThresholdRear;
    float slipFricScaleFront;
    float slipFricScaleRear;
    float slipFricRateFront;
    float slipFricRateRear;
    float slipYawTorque;
};

struct GraphFloat
{
    char name[64];
    float knots[32][2];
    unsigned short knotCount;
    float scale;
};

struct PhysVehicle
{
    int body;
    PhysVehicleWheel wheels[6];
    JointOrientation orientationConstraint;
    JointPath pathConstraint;
    unsigned int wheelsOnGroundCount;
    float invWheelsWithPower;
    float desiredAccel;
    float desiredSpeed;
    float brakingFactor;
    float steeringAngle;
    float pitchControl;
    float velocity[3];
    float acceleration[3];
    float prevAcceleration[3];
    float localVelocity[3];
    float localAcceleration[3];
    float jolt[3];
    float frontWheelCenter[3];
    float rearWheelCenter[3];
    float steeringPoint[3];
    float steeringPointDist;
    float steeringHalfWidth;
    unsigned int flags;
    const VehiclePhysDef* def;
    const GraphFloat* accelGraph;
    float topSpeed;
    int passEntityNum;
    int modelIndex;
    float tankMaxYawSpeed;
    float tankYawAccel;
    float conveyorVelocity[2];
};

struct PhysVehicleControl
{
    float steering;
    float desiredSpeed;
    float desiredSpeedReverse;
    float accelOverride;
    float brake;
    float pitch;
};

struct HelicopterHover
{
    float hoverRadius;
    float hoverSpeed;
    float hoverAccel;
    float hoverGoalPos[3];
    int useHoverAccelForAngles;
};

struct HeliPathPos
{
    HelicopterMoveState moveState;
    float speed;
    float origin[3];
    float angles[3];
    float vel[3];
    float rotVel[3];
    float accel[3];
    HelicopterHover hover;
    int stopping;
    int yawSlowDown;
    float prevGoalYaw;
    gentity_s* ent;
    float goalPosition[3];
    int stopAtGoal;
    float manualSpeed;
    float manualAccel;
    float manualDecel;
    float maxDragSpeed;
    float nearGoalNotifyDist;
    float turningAbility;
    float lookAtEntOrigin[3];
    bool hasLookAtEnt;
    int hasGoalYaw;
    int hasTargetYaw;
    float goalYaw;
    float targetYaw;
    float yawOverShoot;
    float maxAngleVel[3];
    float yawAccel;
    float yawDecel;
    float maxPitchAngle;
    float maxRollAngle;
    float vehDefAccel;
};

struct VehicleTurret
{
    int fireTime;
    int fireBarrel;
    float barrelOffset;
    int barrelBlocked;
    bool turretMoving;
};

struct VehicleJitter
{
    int jitterPeriodMin;
    int jitterPeriodMax;
    int jitterEndTime;
    float jitterOffsetRange[3];
    float jitterDeltaAccel[3];
    float jitterAccel[3];
    float jitterPos[3];
};

struct VehicleBoat
{
    float rockingDirYaw;
    float bounceCurrentForce;
    float bouncePeakForce;
};

struct VehicleTags
{
    int player;
    int detach;
    int popout;
    int body;
    int turret;
    int turret_base;
    int barrel;
    int flash[5];
    int wheel[6];
};

struct Vehicle
{
    gentity_s* ent;
    VehicleDrivingState drivingState;
    VehicleTransmission transMode;
    VehiclePathPos pathPos;
    VehiclePathDir pathDir;
    VehicleTrackpos trackPos;
    vehicle_physic_t phys;
    PhysVehicle* physVeh;
    PhysVehicleControl physVehCtrl;
    int boostTime;
    int defIndex;
    int team;
    int useHintStringIndex;
    HeliPathPos heliPathPos;
    float speed;
    float pathSpeed;
    short waitNode;
    float waitSpeed;
    VehicleTurret turret;
    VehicleJitter jitter;
    VehicleBoat boat;
    VehicleCameraState cameraState;
    unsigned short lookAtText0;
    unsigned short lookAtText1;
    VehicleManualMode manualMode;
    float manualSpeed;
    float manualAccel;
    float manualDecel;
    float manualTime;
    VehicleRotateToMode rotateToYawMode;
    float rotateToYaw;
    float rotateToYawAccel;
    int hasTarget;
    int targetEnt;
    EntHandle lookAtEnt;
    float targetOrigin[3];
    float targetOffset[3];
    float goalPosition[3];
    float joltDir[2];
    float joltTime;
    float joltWave;
    float joltSpeed;
    float joltDecel;
    float throttle;
    float brake;
    float impactSpeed;
    VehicleTags boneIndex;
    int turretHitNum;
    unsigned int trophyAmmo;
    int trophyReloadEndTime;
    VehiclePathType pathType;
};

struct Turret
{
    bool inuse;
    int flags;
    int fireTime;
    float arcmin[2];
    float arcmax[2];
    float dropPitch;
    int stance;
    int prevStance;
    int fireSndDelay;
    float userOrigin[3];
    float playerSpread;
    int state;
    EntHandle target;
    float targetOffset[3];
    EntHandle manualTarget;
    float manualTargetOffset[3];
    int targetTime;
    int stateChangeTime;
    int modeChangeTime;
    float maxRangeSquared;
    int prevTargetIndex;
    team_t eTeam;
    int convergenceTime[2];
    float convergenceHeightPercent;
    float targetPos[3];
    float missOffsetNormalized[3];
    float scanSpeed;
    float scanDecelYaw;
    int scanPauseTime;
    bool triggerDown;
    float heatLevel;
    int heatPenaltyEndTime;
    float barrelRollRate;
    int autoRotationStopDelay;
    int lastAutoRotationRequestTime;
    float initialYawmin;
    float initialYawmax;
    float forwardAngleDot;
    int suppressTime;
    SentientHandle detachSentient;
    float aiSpread;
    float pitchCap;
    float originError[3];
    float anglesError[3];
    unsigned short fireSnd;
    unsigned short fireSndPlayer;
    unsigned short stopSnd;
    unsigned short stopSndPlayer;
    unsigned short scanSnd;
};

struct item_ent_t
{
    int ammoCount;
    int clipAmmoCount[2];
    Weapon weapon;
    bool dualWieldItem;
    int shieldHealth;
};

struct spawner_ent_t
{
    int team;
    int timestamp;
    int index;
    int count;
};

struct trigger_ent_t
{
    int threshold;
    int accumulate;
    int timestamp;
    bool requireLookAt;
};

struct mover_positions_t
{
    float decelTime;
    float speed;
    float midTime;
    float pos1[3];
    float pos2[3];
    float pos3[3];
};

struct mover_slidedata_t
{
    Bounds bounds;
    float velocity[3];
};

struct mover_ent_t
{
    union
    {
        mover_positions_t pos;
        mover_slidedata_t slide;
    };
    mover_positions_t angle;
};

struct corpse_ent_t
{
    int removeTime;
};

struct actor_ent_t
{
    int spawnTime;
};

struct missile_fields_grenade
{
    float predictLandPos[3];
    int predictLandTime;
    float wobbleCycle;
    float curve;
};

struct missile_fields_nonGrenade
{
    float curvature[3];
    float targetEntOffset[3];
    float targetPos[3];
    float launchOrigin[3];
    MissileStage stage;
};

struct missile_ent_t
{
    int timestamp;
    float time;
    int timeOfBirth;
    float travelDist;
    float surfaceNormal[3];
    team_t team;
    int flags;
    union
    {
        missile_fields_grenade grenade;
        missile_fields_nonGrenade nonGrenade;
    };
};

struct blend_ent_t
{
    float pos[3];
    float vel[3];
    float viewQuat[4];
    bool changed;
    float accelTime;
    float decelTime;
    float startTime;
    float totalTime;
};

struct tagInfo_s
{
    gentity_s* parent;
    gentity_s* next;
    unsigned short name;
    bool blendToParent;
    bool blendOnlyYaw;
    bool collisionPhysics;
    int index;
    float axis[4][3];
    float parentInvAxis[4][3];
};

struct animscripted_s
{
    float axis[4][3];
    float originError[3];
    float anglesError[3];
    unsigned short anim;
    unsigned short root;
    unsigned char bStarted;
    unsigned char mode;
    float fHeightOfs;
    float fEndPitch;
    float fEndRoll;
    float fOrientLerp;
};

union XAnimIndices
{
    unsigned char* _1;
    unsigned short* _2;
    void* data;
};

struct XAnimNotifyInfo
{
    unsigned short name;
    float time;
};

union XAnimDynamicFrames
{
    unsigned char(*_1)[3];
    unsigned short(*_2)[3];
};

union XAnimDynamicIndices
{
    unsigned char _1[1];
    unsigned short _2[1];
};

struct XAnimPartTransFrames
{
    float mins[3];
    float size[3];
    XAnimDynamicFrames frames;
    XAnimDynamicIndices indices;
};

union XAnimPartTransData
{
    XAnimPartTransFrames frames;
    float frame0[3];
};

struct XAnimPartTrans
{
    unsigned short size;
    unsigned char smallTrans;
    XAnimPartTransData u;
};

struct XAnimDeltaPartQuatDataFrames2
{
    short(*frames)[2];
    XAnimDynamicIndices indices;
};

union XAnimDeltaPartQuatData2
{
    XAnimDeltaPartQuatDataFrames2 frames;
    short frame0[2];
};

struct XAnimDeltaPartQuat2
{
    unsigned short size;
    XAnimDeltaPartQuatData2 u;
};

struct XAnimDeltaPartQuatDataFrames
{
    short(*frames)[4];
    XAnimDynamicIndices indices;
};

union XAnimDeltaPartQuatData
{
    XAnimDeltaPartQuatDataFrames frames;
    short frame0[4];
};

struct XAnimDeltaPartQuat
{
    unsigned short size;
    XAnimDeltaPartQuatData u;
};

struct XAnimDeltaPart
{
    XAnimPartTrans* trans;
    XAnimDeltaPartQuat2* quat2;
    XAnimDeltaPartQuat* quat;
};

struct XAnimParts
{
    const char* name;
    unsigned short dataByteCount;
    unsigned short dataShortCount;
    unsigned short dataIntCount;
    unsigned short randomDataByteCount;
    unsigned short randomDataIntCount;
    unsigned short numframes;
    unsigned char flags;
    unsigned char boneCount[12];
    unsigned char notifyCount;
    unsigned char assetType;
    bool pad;
    unsigned int randomDataShortCount;
    unsigned int indexCount;
    float framerate;
    float frequency;
    unsigned short* names;
    unsigned char* dataByte;
    short* dataShort;
    int* dataInt;
    short* randomDataShort;
    unsigned char* randomDataByte;
    int* randomDataInt;
    XAnimIndices indices;
    XAnimNotifyInfo* notify;
    XAnimDeltaPart* deltaPart;
};

struct XAnimParent
{
    unsigned short flags;
    unsigned short children;
};

struct XAnimEntry
{
    unsigned short numAnims;
    unsigned short parent;
    union
    {
        XAnimParts* parts;
        XAnimParent animParent;
    };
};

struct XAnim_s
{
    unsigned int size;
    XAnimEntry entries[1];
};

struct XAnimTree
{
    XAnim_s* anims;
    unsigned short children;
    short padding;
};

struct gentity_s
{
    entityState_s s;
    entityShared_t r;
    gclient_s* client;
    actor_s* actor;
    sentient_s* sentient;
    Vehicle* vehicle;
    Turret* turret;
    int physObjId;
    unsigned char physicsObject;
    unsigned char takedamage;
    unsigned char active;
    unsigned char handler;
    unsigned short classname;
    unsigned short script_classname;
    unsigned short script_linkName;
    unsigned short script_noteworthy;
    unsigned short target;
    unsigned short targetname;
    unsigned int attachIgnoreCollision;
    int spawnflags;
    int flags;
    int clipmask;
    int processedFrame;
    EntHandle parent;
    int nextthink;
    int health;
    int maxHealth;
    int damage;
    EntHandle grenadeActivator;
    union
    {
        item_ent_t item[2];
        spawner_ent_t spawner;
        trigger_ent_t trigger;
        mover_ent_t mover;
        corpse_ent_t corpse;
        actor_ent_t actorInfo;
        missile_ent_t missile;
        blend_ent_t blend;
    };
    EntHandle missileTargetEnt;
    EntHandle remoteControlledOwner;
    unsigned short lookAtText0;
    unsigned short lookAtText1;
    struct
    {
        unsigned short notifyString;
        unsigned short index;
        unsigned char stoppable;
    } snd_wait;
    tagInfo_s* tagInfo;
    gentity_s* tagChildren;
    animscripted_s* scripted;
    unsigned short attachModelNames[31];
    unsigned short attachTagNames[31];
    unsigned short model;
    unsigned short disconnectedLinks;
    int iDisconnectTime;
    float angleLerpRate;
    XAnimTree* pAnimTree;
    gentity_s* nextFree;
};

struct SpawnVar
{
    bool spawnVarsValid;
    int numSpawnVars;
    unsigned int spawnVarsKey[64];
    char* spawnVarsValue[64];
    int numSpawnVarChars;
    char spawnVarChars[2048];
};

struct cached_tag_mat_t
{
    int time;
    int entnum;
    unsigned short name;
    float tagMat[4][3];
};

struct trigger_info_t
{
    unsigned short entnum;
    unsigned short otherEntnum;
    int useCount;
    int otherUseCount;
};

struct actor_prone_info_lerp_t
{
    int iProneTime;
    float fBodyHeight;
    float fBodyPitch;
    float fBodyRoll;
};

struct cgameData_t
{
    int snapTime;
    unsigned char specialIndex[2048];
    actor_prone_info_s actorProneInfo[48];
    actor_prone_info_lerp_t prevActorProneInfo[48];
    unsigned char actorOnCompass[32];
    unsigned char actorTeam[32];
};

struct actorAntilagFrame_t
{
    float position[32][3];
    float angles[32][3];
    unsigned char useCount[32];
    unsigned char inUse[32];
};

struct level_locals_t
{
    gclient_s* clients;
    gentity_s* gentities;
    int num_entities;
    gentity_s* firstFreeEnt;
    gentity_s* lastFreeEnt;
    sentient_s* sentients;
    actor_s* actors;
    Vehicle* vehicles;
    Turret* turrets;
    int initializing;
    int clientIsSpawning;
    int maxclients;
    int framenum;
    int time;
    int previousTime;
    int timeAtLastPause;
    int respawnTime;
    int newSession;
    int actorCorpseCount;
    int actorCount;
    SpawnVar spawnVar;
    int reloadDelayTime;
    int absoluteReloadDelayTime;
    EntHandle droppedWeaponCue[32];
    int changelevel;
    int bMissionSuccess;
    int bMissionFailed;
    int exitTime;
    int savepersist;
    char cinematic[64];
    float fFogOpaqueDist;
    float fFogOpaqueDistSqrd;
    unsigned int grenadeHintCount;
    int remapCount;
    int iSearchFrame;
    loading_t loading;
    int bDrawCompassFriendlies;
    int bPlayerIgnoreRadiusDamage;
    int bPlayerIgnoreRadiusDamageLatched;
    unsigned char triggerIndex;
    int currentEntityThink;
    int currentIndex;
    bool checkAnimChange;
    int bRegisterItems;
    int framePos;
    cached_tag_mat_t cachedTagMat;
    cached_tag_mat_t cachedEntTargetTagMat;
    unsigned short soundAliasFirst;
    unsigned short soundAliasLast;
    trigger_info_t pendingTriggerList[256];
    trigger_info_t currentTriggerList[256];
    int pendingTriggerListSize;
    int currentTriggerListSize;
    unsigned char entTriggerIndex[2048];
    cgameData_t cgameData;
    unsigned short modelMap[1024];
    float priorityNodeBias;
    int scriptPrintChannel;
    float compassMapUpperLeft[2];
    float compassMapWorldSize[2];
    float compassNorth[2];
    float mapSunColor[3];
    float mapSunDirection[3];
    int hudElemLastAssignedSoundID;
    int checkSoundsFinish;
    actorAntilagFrame_t actorAntilagFrames[20];
    int nextActorAntilagFrame;
    unsigned char localizedStringRefCount[1200];
};

struct VariableStackBuffer
{
    const char* pos;
    unsigned short size;
    unsigned short bufLen;
    unsigned short localId;
    unsigned char time;
    char buf[1];
};

union VariableUnion
{
    int intValue;
    unsigned int uintValue;
    float floatValue;
    unsigned int stringValue;
    const float* vectorValue;
    const char* codePosValue;
    unsigned int pointerValue;
    VariableStackBuffer* stackValue;
    unsigned int entityOffset;
};

struct VariableValue
{
    VariableUnion u;
    int type;
};

struct function_stack_t
{
    const char* pos;
    unsigned int localId;
    unsigned int localVarCount;
    VariableValue* top;
    VariableValue* startTop;
};

struct function_frame_t
{
    function_stack_t fs;
    int topType;
};

struct scrVmPub_t
{
    unsigned int* localVars;
    VariableValue* maxstack;
    int function_count;
    function_frame_t* function_frame;
    VariableValue* top;
    unsigned int inparamcount;
    unsigned int outparamcount;
    function_frame_t function_frame_start[32];
    VariableValue stack[2048];
};

struct trace_t
{ 
    float fraction; 
    float normal[3]; 
    int surfaceFlags; 
    int contents; 
    const char* material; 
    TraceHitType hitType; 
    unsigned short hitId; 
    unsigned short modelIndex;
    unsigned short partName;
    unsigned short partGroup;
    bool allsolid; 
    bool startsolid; 
    bool walkable; 
};

struct scr_entref_t
{ 
    unsigned short entnum; 
    unsigned short classnum;
};

struct StateTimers
{
    int iFireDelay;
    int iMeleeDelay;
    int meleeChargeDelay;
    int iDetonateDelay;
    int iRechamberTime;
    int rechamberTimeOneHanded;
    int iRechamberBoltTime;
    int iHoldFireTime;
    int iDetonateTime;
    int iMeleeTime;
    int meleeChargeTime;
    int iReloadTime;
    int reloadShowRocketTime;
    int iReloadEmptyTime;
    int iReloadAddTime;
    int iReloadStartTime;
    int iReloadStartAddTime;
    int iReloadEndTime;
    int iDropTime;
    int iRaiseTime;
    int iAltDropTime;
    int quickDropTime;
    int quickRaiseTime;
    int iBreachRaiseTime;
    int iEmptyRaiseTime;
    int iEmptyDropTime;
    int sprintInTime;
    int sprintLoopTime;
    int sprintOutTime;
    int stunnedTimeBegin;
    int stunnedTimeLoop;
    int stunnedTimeEnd;
    int nightVisionWearTime;
    int nightVisionWearTimeFadeOutEnd;
    int nightVisionWearTimePowerUp;
    int nightVisionRemoveTime;
    int nightVisionRemoveTimePowerDown;
    int nightVisionRemoveTimeFadeInStart;
    int fuseTime;
    int aiFuseTime;
    int blastFrontTime;
    int blastRightTime;
    int blastBackTime;
    int blastLeftTime;
    int raiseInterruptableTime;
    int firstRaiseInterruptableTime;
    int reloadInterruptableTime;
    int reloadEmptyInterruptableTime;
    int fireInterruptableTime;
};

struct ADSOverlay
{
    Material* shader;
    Material* shaderLowRes;
    Material* shaderEMP;
    Material* shaderEMPLowRes;
    weapOverlayReticle_t reticle;
    float width;
    float height;
    float widthSplitscreen;
    float heightSplitscreen;
};

struct FxFloatRange
{
    float base;
    float amplitude;
};

struct FxIntRange
{
    int base;
    int amplitude;
};

struct FxElemVec3Range
{
    float base[3];
    float amplitude[3];
};

struct FxSpawnDefLooping
{
    int intervalMsec;
    int count;
};

struct FxSpawnDefOneShot
{
    FxIntRange count;
};

union FxSpawnDef
{
    FxSpawnDefLooping looping;
    FxSpawnDefOneShot oneShot;
};

struct FxElemAtlas
{
    unsigned char behavior;
    unsigned char index;
    unsigned char fps;
    unsigned char loopCount;
    unsigned char colIndexBits;
    unsigned char rowIndexBits;
    short entryCount;
};

struct FxElemVelStateInFrame
{
    FxElemVec3Range velocity;
    FxElemVec3Range totalDelta;
};

struct FxElemVelStateSample
{
    FxElemVelStateInFrame local;
    FxElemVelStateInFrame world;
};

struct FxElemVisualState
{
    unsigned char color[4];
    float rotationDelta;
    float rotationTotal;
    float size[2];
    float scale;
};

struct FxElemVisStateSample
{
    FxElemVisualState base;
    FxElemVisualState amplitude;
};

struct FxElemMarkVisuals
{
    Material* materials[2];
};

struct FxEffectDef;
union FxEffectDefRef
{
    const FxEffectDef* handle;
    const char* name;
};

struct GfxLightImage
{
    GfxImage* image;
    unsigned char samplerState;
};

struct GfxLightDef
{
    const char* name;
    GfxLightImage attenuation;
    GfxLightImage cucoloris;
    int lmapLookupStart;
};

union FxElemVisuals
{
    const void* anonymous;
    Material* material;
    XModel* model;
    FxEffectDefRef effectDef;
    const char* soundName;
    GfxLightDef* lightDef;
};

union FxElemDefVisuals
{
    FxElemMarkVisuals* markArray;
    FxElemVisuals* array;
    FxElemVisuals instance;
};

struct FxTrailVertex
{
    float pos[2];
    float normal[2];
    float texCoord;
};

struct FxTrailDef
{
    int scrollTimeMsec;
    int repeatDist;
    float invSplitDist;
    float invSplitArcDist;
    float invSplitTime;
    int vertCount;
    FxTrailVertex* verts;
    int indCount;
    unsigned short* inds;
};

struct FxSparkFountainDef
{
    float gravity;
    float bounceFrac;
    float bounceRand;
    float sparkSpacing;
    float sparkLength;
    int sparkCount;
    float loopTime;
    float velMin;
    float velMax;
    float velConeFrac;
    float restSpeed;
    float boostTime;
    float boostFactor;
};

struct FxSpotLightDef
{
    float fovInnerFraction;
    float startRadius;
    float endRadius;
    float brightness;
    float maxLength;
    int exponent;
};

union FxElemExtendedDefPtr
{
    FxTrailDef* trailDef;
    FxSparkFountainDef* sparkFountainDef;
    FxSpotLightDef* spotLightDef;
    void* unknownDef;
};

struct FxElemDef
{
    int flags;
    FxSpawnDef spawn;
    FxFloatRange spawnRange;
    FxFloatRange fadeInRange;
    FxFloatRange fadeOutRange;
    float spawnFrustumCullRadius;
    FxIntRange spawnDelayMsec;
    FxIntRange lifeSpanMsec;
    FxFloatRange spawnOrigin[3];
    FxFloatRange spawnOffsetRadius;
    FxFloatRange spawnOffsetHeight;
    FxFloatRange spawnAngles[3];
    FxFloatRange angularVelocity[3];
    FxFloatRange initialRotation;
    FxFloatRange gravity;
    FxFloatRange reflectionFactor;
    FxElemAtlas atlas;
    unsigned char elemType;
    unsigned char visualCount;
    unsigned char velIntervalCount;
    unsigned char visStateIntervalCount;
    const FxElemVelStateSample* velSamples;
    const FxElemVisStateSample* visSamples;
    FxElemDefVisuals visuals;
    Bounds collBounds;
    FxEffectDefRef effectOnImpact;
    FxEffectDefRef effectOnDeath;
    FxEffectDefRef effectEmitted;
    FxFloatRange emitDist;
    FxFloatRange emitDistVariance;
    FxElemExtendedDefPtr extended;
    unsigned char sortOrder;
    unsigned char lightingFrac;
    unsigned char useItemClip;
    unsigned char fadeInfo;
    int randomSeed;
};

struct FxEffectDef
{
    const char* name;
    int flags;
    int totalSize;
    int msecLoopingLife;
    int elemDefCountLooping;
    int elemDefCountOneShot;
    int elemDefCountEmission;
    float occlusionQueryDepthBias;
    int occlusionQueryFadeIn;
    int occlusionQueryFadeOut;
    FxFloatRange occlusionQueryScaleRange;
    const FxElemDef* elemDefs;
};

struct LoadedSound
{
    const char* name;
    //XaSound sound;
};

struct StreamFileNameRaw
{
    const char* dir;
    const char* name;
};

struct StreamFileNamePacked
{
    unsigned int offset;
    unsigned int length;
};

union StreamFileInfo
{
    StreamFileNameRaw raw;
    StreamFileNamePacked packed;
};

struct StreamFileName
{
    unsigned int fileIndex;
    StreamFileInfo info;
};

struct StreamedSound
{
    StreamFileName filename;
};

union SoundFileRef
{
    LoadedSound* loadSnd;
    StreamedSound streamSnd;
};

struct SoundFile
{
    unsigned char type;
    unsigned char exists;
    SoundFileRef u;
};

struct SndCurve
{
    const char* filename;
    unsigned char knotCount;
    float knots[16][2];
};

struct SpeakerMap
{
    bool isDefault;
    const char* name;
    //XAUDIOCHANNELMAP channelMaps[2][2];
};

struct snd_alias_t
{
    const char* aliasName;
    const char* subtitle;
    const char* secondaryAliasName;
    const char* chainAliasName;
    const char* mixerGroup;
    SoundFile* soundFile;
    int sequence;
    float volMin;
    float volMax;
    int volModIndex;
    float pitchMin;
    float pitchMax;
    float distMin;
    float distMax;
    float velocityMin;
    int flags;
    unsigned char masterPriority;
    float masterPercentage;
    float slavePercentage;
    float probability;
    float lfePercentage;
    float centerPercentage;
    int startDelay;
    SndCurve* volumeFalloffCurve;
    float envelopMin;
    float envelopMax;
    float envelopPercentage;
    SpeakerMap* speakerMap;
};

struct snd_alias_list_t
{
    const char* aliasName;
    snd_alias_t* head;
    int count;
};

struct TracerDef
{
    const char* name;
    Material* material;
    unsigned int drawInterval;
    float speed;
    float beamLength;
    float beamWidth;
    float screwRadius;
    float screwDist;
    float colors[5][4];
};

struct WeaponDef
{
    const char* szOverlayName;
    XModel** gunXModel;
    XModel* handXModel;
    const char** szXAnimsRightHanded;

    const char** szXAnimsLeftHanded;
    const char* szModeName;
    unsigned short* notetrackSoundMapKeys;
    unsigned short* notetrackSoundMapValues;

    unsigned short* notetrackRumbleMapKeys;
    unsigned short* notetrackRumbleMapValues;
    int playerAnimType;
    weapType_t weapType;

    weapClass_t weapClass;
    PenetrateType penetrateType;
    weapInventoryType_t inventoryType;
    weapFireType_t fireType;
    OffhandClass offhandClass;
    weapStance_t stance;
    const FxEffectDef* viewFlashEffect;
    const FxEffectDef* worldFlashEffect;
    snd_alias_list_t* pickupSound;
    snd_alias_list_t* pickupSoundPlayer;
    snd_alias_list_t* ammoPickupSound;
    snd_alias_list_t* ammoPickupSoundPlayer;
    snd_alias_list_t* projectileSound;
    snd_alias_list_t* pullbackSound;
    snd_alias_list_t* pullbackSoundPlayer;
    snd_alias_list_t* fireSound;
    snd_alias_list_t* fireSoundPlayer;
    snd_alias_list_t* fireSoundPlayerAkimbo;
    snd_alias_list_t* fireLoopSound;
    snd_alias_list_t* fireLoopSoundPlayer;
    snd_alias_list_t* fireStopSound;
    snd_alias_list_t* fireStopSoundPlayer;
    snd_alias_list_t* fireLastSound;
    snd_alias_list_t* fireLastSoundPlayer;
    snd_alias_list_t* emptyFireSound;
    snd_alias_list_t* emptyFireSoundPlayer;
    snd_alias_list_t* meleeSwipeSound;
    snd_alias_list_t* meleeSwipeSoundPlayer;
    snd_alias_list_t* meleeHitSound;
    snd_alias_list_t* meleeMissSound;
    snd_alias_list_t* rechamberSound;
    snd_alias_list_t* rechamberSoundPlayer;
    snd_alias_list_t* reloadSound;
    snd_alias_list_t* reloadSoundPlayer;
    snd_alias_list_t* reloadEmptySound;
    snd_alias_list_t* reloadEmptySoundPlayer;
    snd_alias_list_t* reloadStartSound;
    snd_alias_list_t* reloadStartSoundPlayer;
    snd_alias_list_t* reloadEndSound;
    snd_alias_list_t* reloadEndSoundPlayer;
    snd_alias_list_t* detonateSound;
    snd_alias_list_t* detonateSoundPlayer;
    snd_alias_list_t* nightVisionWearSound;
    snd_alias_list_t* nightVisionWearSoundPlayer;
    snd_alias_list_t* nightVisionRemoveSound;
    snd_alias_list_t* nightVisionRemoveSoundPlayer;
    snd_alias_list_t* altSwitchSound;
    snd_alias_list_t* altSwitchSoundPlayer;
    snd_alias_list_t* raiseSound;
    snd_alias_list_t* raiseSoundPlayer;
    snd_alias_list_t* firstRaiseSound;
    snd_alias_list_t* firstRaiseSoundPlayer;
    snd_alias_list_t* putawaySound;
    snd_alias_list_t* putawaySoundPlayer;
    snd_alias_list_t* scanSound;
    snd_alias_list_t* changeVariableZoomSound;
    snd_alias_list_t** bounceSound;
    snd_alias_list_t** rollingSound;
    const FxEffectDef* viewShellEjectEffect;
    const FxEffectDef* worldShellEjectEffect;
    const FxEffectDef* viewLastShotEjectEffect;
    const FxEffectDef* worldLastShotEjectEffect;
    Material* reticleCenter;
    Material* reticleSide;
    int iReticleCenterSize;
    int iReticleSideSize;
    int iReticleMinOfs;
    activeReticleType_t activeReticleType;
    float vStandMove[3];
    float vStandRot[3];
    float strafeMove[3];
    float strafeRot[3];
    float vDuckedOfs[3];
    float vDuckedMove[3];
    float vDuckedRot[3];
    float vProneOfs[3];
    float vProneMove[3];
    float vProneRot[3];
    float fPosMoveRate;
    float fPosProneMoveRate;
    float fStandMoveMinSpeed;
    float fDuckedMoveMinSpeed;
    float fProneMoveMinSpeed;
    float fPosRotRate;
    float fPosProneRotRate;
    float fStandRotMinSpeed;
    float fDuckedRotMinSpeed;
    float fProneRotMinSpeed;
    XModel** worldModel;
    XModel* worldClipModel;
    XModel* rocketModel;
    XModel* knifeModel;
    XModel* worldKnifeModel;
    Material* hudIcon;
    weaponIconRatioType_t hudIconRatio;
    Material* pickupIcon;
    weaponIconRatioType_t pickupIconRatio;
    Material* ammoCounterIcon;
    weaponIconRatioType_t ammoCounterIconRatio;
    ammoCounterClipType_t ammoCounterClip;
    int iStartAmmo;
    const char* szAmmoName;
    int iAmmoIndex;
    const char* szClipName;
    int iClipIndex;
    int iMaxAmmo;
    int shotCount;
    const char* szSharedAmmoCapName;
    int iSharedAmmoCapIndex;
    int iSharedAmmoCap;
    int damage;
    int playerDamage;
    int iMeleeDamage;
    int iDamageType;
    StateTimers stateTimers;
    StateTimers akimboStateTimers;
    float autoAimRange;
    float aimAssistRange;
    float aimAssistRangeAds;
    float aimPadding;
    float enemyCrosshairRange;
    float moveSpeedScale;
    float adsMoveSpeedScale;
    float sprintDurationScale;
    float fAdsZoomInFrac;
    float fAdsZoomOutFrac;
    ADSOverlay overlay;
    WeapOverlayInteface_t overlayInterface;
    float fAdsBobFactor;
    float fAdsViewBobMult;
    float fHipSpreadStandMin;
    float fHipSpreadDuckedMin;
    float fHipSpreadProneMin;
    float hipSpreadStandMax;
    float hipSpreadDuckedMax;
    float hipSpreadProneMax;
    float fHipSpreadDecayRate;
    float fHipSpreadFireAdd;
    float fHipSpreadTurnAdd;
    float fHipSpreadMoveAdd;
    float fHipSpreadDuckedDecay;
    float fHipSpreadProneDecay;
    float fHipReticleSidePos;
    float fAdsIdleAmount;
    float fHipIdleAmount;
    float adsIdleSpeed;
    float hipIdleSpeed;
    float fIdleCrouchFactor;
    float fIdleProneFactor;
    float fGunMaxPitch;
    float fGunMaxYaw;
    float adsIdleLerpStartTime;
    float adsIdleLerpTime;
    float swayMaxAngle;
    float swayLerpSpeed;
    float swayPitchScale;
    float swayYawScale;
    float swayHorizScale;
    float swayVertScale;
    float swayShellShockScale;
    float adsSwayMaxAngle;
    float adsSwayLerpSpeed;
    float adsSwayPitchScale;
    float adsSwayYawScale;
    float adsSwayHorizScale;
    float adsSwayVertScale;
    float adsViewErrorMin;
    float adsViewErrorMax;
    PhysCollmap* physCollmap;
    float dualWieldViewModelOffset;
    weaponIconRatioType_t killIconRatio;
    int iReloadAmmoAdd;
    int iReloadStartAdd;
    int ammoDropStockMin;
    int ammoDropClipPercentMin;
    int ammoDropClipPercentMax;
    int iExplosionRadius;
    int iExplosionRadiusMin;
    int iExplosionInnerDamage;
    int iExplosionOuterDamage;
    float damageConeAngle;
    float bulletExplDmgMult;
    float bulletExplRadiusMult;
    int iProjectileSpeed;
    int iProjectileSpeedUp;
    int iProjectileSpeedForward;
    int iProjectileActivateDist;
    float projLifetime;
    float timeToAccelerate;
    float projectileCurvature;
    XModel* projectileModel;
    weapProjExposion_t projExplosion;
    const FxEffectDef* projExplosionEffect;
    const FxEffectDef* projDudEffect;
    snd_alias_list_t* projExplosionSound;
    snd_alias_list_t* projDudSound;
    WeapStickinessType stickiness;
    float lowAmmoWarningThreshold;
    float ricochetChance;
    bool riotShieldEnableDamage;
    int riotShieldHealth;
    float riotShieldDamageMult;
    float* parallelBounce;
    float* perpendicularBounce;
    const FxEffectDef* projTrailEffect;
    const FxEffectDef* projBeaconEffect;
    float vProjectileColor[3];
    guidedMissileType_t guidedMissileType;
    float maxSteeringAccel;
    int projIgnitionDelay;
    const FxEffectDef* projIgnitionEffect;
    snd_alias_list_t* projIgnitionSound;
    float fAdsAimPitch;
    float fAdsCrosshairInFrac;
    float fAdsCrosshairOutFrac;
    int adsGunKickReducedKickBullets;
    float adsGunKickReducedKickPercent;
    float fAdsGunKickPitchMin;
    float fAdsGunKickPitchMax;
    float fAdsGunKickYawMin;
    float fAdsGunKickYawMax;
    float fAdsGunKickAccel;
    float fAdsGunKickSpeedMax;
    float fAdsGunKickSpeedDecay;
    float fAdsGunKickStaticDecay;
    float fAdsViewKickPitchMin;
    float fAdsViewKickPitchMax;
    float fAdsViewKickYawMin;
    float fAdsViewKickYawMax;
    float fAdsViewScatterMin;
    float fAdsViewScatterMax;
    float fAdsSpread;
    int hipGunKickReducedKickBullets;
    float hipGunKickReducedKickPercent;
    float fHipGunKickPitchMin;
    float fHipGunKickPitchMax;
    float fHipGunKickYawMin;
    float fHipGunKickYawMax;
    float fHipGunKickAccel;
    float fHipGunKickSpeedMax;
    float fHipGunKickSpeedDecay;
    float fHipGunKickStaticDecay;
    float fHipViewKickPitchMin;
    float fHipViewKickPitchMax;
    float fHipViewKickYawMin;
    float fHipViewKickYawMax;
    float fHipViewScatterMin;
    float fHipViewScatterMax;
    float fightDist;
    float maxDist;
    const char* accuracyGraphName[2];
    float(*originalAccuracyGraphKnots[2])[2];
    unsigned short originalAccuracyGraphKnotCount[2];
    int iPositionReloadTransTime;
    float leftArc;
    float rightArc;
    float topArc;
    float bottomArc;
    float accuracy;
    float aiSpread;
    float playerSpread;
    float minTurnSpeed[2];
    float maxTurnSpeed[2];
    float pitchConvergenceTime;
    float yawConvergenceTime;
    float suppressTime;
    float maxRange;
    float fAnimHorRotateInc;
    float fPlayerPositionDist;
    const char* szUseHintString;
    const char* dropHintString;
    int iUseHintStringIndex;
    int dropHintStringIndex;
    float horizViewJitter;
    float vertViewJitter;
    float scanSpeed;
    float scanAccel;
    int scanPauseTime;
    const char* szScript;
    float fOOPosAnimLength[2];
    int minDamage;
    int minPlayerDamage;
    float fMaxDamageRange;
    float fMinDamageRange;
    float destabilizationRateTime;
    float destabilizationCurvatureMax;
    int destabilizeDistance;
    float* locationDamageMultipliers;
    const char* fireRumble;
    const char* meleeImpactRumble;
    TracerDef* tracerType;
    bool turretADSEnabled;
    float turretADSTime;
    float turretFov;
    float turretFovADS;
    float turretScopeZoomRate;
    float turretScopeZoomMin;
    float turretScopeZoomMax;
    float turretOverheatUpRate;
    float turretOverheatDownRate;
    float turretOverheatPenalty;
    snd_alias_list_t* turretOverheatSound;
    const FxEffectDef* turretOverheatEffect;
    const char* turretBarrelSpinRumble;
    float turretBarrelSpinSpeed;
    float turretBarrelSpinUpTime;
    float turretBarrelSpinDownTime;
    snd_alias_list_t* turretBarrelSpinMaxSnd;
    snd_alias_list_t* turretBarrelSpinUpSnd[4];
    snd_alias_list_t* turretBarrelSpinDownSnd[4];
    snd_alias_list_t* missileConeSoundAlias;
    snd_alias_list_t* missileConeSoundAliasAtBase;
    float missileConeSoundRadiusAtTop;
    float missileConeSoundRadiusAtBase;
    float missileConeSoundHeight;
    float missileConeSoundOriginOffset;
    float missileConeSoundVolumescaleAtCore;
    float missileConeSoundVolumescaleAtEdge;
    float missileConeSoundVolumescaleCoreSize;
    float missileConeSoundPitchAtTop;
    float missileConeSoundPitchAtBottom;
    float missileConeSoundPitchTopSize;
    float missileConeSoundPitchBottomSize;
    float missileConeSoundCrossfadeTopSize;
    float missileConeSoundCrossfadeBottomSize;
    bool sharedAmmo;
    bool lockonSupported;
    bool requireLockonToFire;
    bool isAirburstWeapon;
    bool bigExplosion;
    bool noAdsWhenMagEmpty;
    bool avoidDropCleanup;
    bool inheritsPerks;
    bool crosshairColorChange;
    bool bRifleBullet;
    bool armorPiercing;
    bool bBoltAction;
    bool aimDownSight;
    bool canHoldBreath;
    bool canVariableZoom;
    bool bRechamberWhileAds;
    bool bBulletExplosiveDamage;
    bool bCookOffHold;
    bool bClipOnly;
    bool noAmmoPickup;
    bool adsFireOnly;
    bool cancelAutoHolsterWhenEmpty;
    bool disableSwitchToWhenEmpty;
    bool suppressAmmoReserveDisplay;
    bool laserSightDuringNightvision;
    bool markableViewmodel;
    bool noDualWield;
    bool flipKillIcon;
    bool bNoPartialReload;
    bool bSegmentedReload;
    bool blocksProne;
    bool silenced;
    bool isRollingGrenade;
    bool projExplosionEffectForceNormalUp;
    bool bProjImpactExplode;
    bool stickToPlayers;
    bool stickToVehicles;
    bool stickToTurrets;
    bool hasDetonator;
    bool disableFiring;
    bool timedDetonation;
    bool rotate;
    bool holdButtonToThrow;
    bool freezeMovementWhenFiring;
    bool thermalScope;
    bool altModeSameWeapon;
    bool turretBarrelSpinEnabled;
    bool missileConeSoundEnabled;
    bool missileConeSoundPitchshiftEnabled;
    bool missileConeSoundCrossfadeEnabled;
    bool offhandHoldIsCancelable;
    bool doNotAllowAttachmentsToOverrideSpread;
    unsigned short stowTag;
    XModel* stowOffsetModel;
};

struct AttAmmoGeneral
{
    PenetrateType penetrateType;
    float penetrateMultiplier;
    ImpactType impactType;
    weapFireType_t fireType;
    TracerDef* tracerType;
    bool rifleBullet;
    bool armorPiercing;
};

struct AttSight
{
    bool aimDownSight;
    bool adsFire;
    bool rechamberWhileAds;
    bool noAdsWhenMagEmpty;
    bool canHoldBreath;
    bool canVariableZoom;
    bool hideRailWithThisScope;
};

struct AttReload
{
    bool noPartialReload;
    bool segmentedReload;
};

struct AttAddOns
{
    bool motionTracker;
    bool silenced;
};

struct AttGeneral
{
    bool boltAction;
    bool inheritsPerks;
    float enemyCrosshairRange;
    Material* reticleCenter;
    Material* reticleSide;
    int reticleCenterSize;
    int reticleSideSize;
    float moveSpeedScale;
    float adsMoveSpeedScale;
};

struct AttAimAssist
{
    float autoAimRange;
    float aimAssistRange;
    float aimAssistRangeAds;
};

struct AttAmmunition
{
    int maxAmmo;
    int startAmmo;
    int clipSize;
    int shotCount;
    int reloadAmmoAdd;
    int reloadStartAdd;
};

struct AttDamage
{
    int damage;
    int minDamage;
    int meleeDamage;
    float maxDamageRange;
    float minDamageRange;
    int playerDamage;
    int minPlayerDamage;
};

struct AttLocationDamage
{
    float locNone;
    float locHelmet;
    float locHead;
    float locNeck;
    float locTorsoUpper;
    float locTorsoLower;
    float locRightArmUpper;
    float locRightArmLower;
    float locRightHand;
    float locLeftArmUpper;
    float locLeftArmLower;
    float locLeftHand;
    float locRightLegUpper;
    float locRightLegLower;
    float locRightFoot;
    float locLeftLegUpper;
    float locLeftLegLower;
    float locLeftFoot;
    float locGun;
};

struct AttIdleSettings
{
    float hipIdleAmount;
    float hipIdleSpeed;
    float idleCrouchFactor;
    float idleProneFactor;
    float adsIdleLerpStartTime;
    float adsIdleLerpTime;
};

struct AttADSSettings
{
    float adsSpread;
    float adsAimPitch;
    float adsTransInTime;
    float adsTransOutTime;
    int adsReloadTransTime;
    float adsCrosshairInFrac;
    float adsCrosshairOutFrac;
    float adsZoomFov;
    float adsZoomInFrac;
    float adsZoomOutFrac;
    float adsBobFactor;
    float adsViewBobMult;
    float adsViewErrorMin;
    float adsViewErrorMax;
};

struct AttHipSpread
{
    float hipSpreadStandMin;
    float hipSpreadDuckedMin;
    float hipSpreadProneMin;
    float hipSpreadMax;
    float hipSpreadDuckedMax;
    float hipSpreadProneMax;
    float hipSpreadFireAdd;
    float hipSpreadTurnAdd;
    float hipSpreadMoveAdd;
    float hipSpreadDecayRate;
    float hipSpreadDuckedDecay;
    float hipSpreadProneDecay;
};

struct AttGunKick
{
    int hipGunKickReducedKickBullets;
    float hipGunKickReducedKickPercent;
    float hipGunKickPitchMin;
    float hipGunKickPitchMax;
    float hipGunKickYawMin;
    float hipGunKickYawMax;
    float hipGunKickAccel;
    float hipGunKickSpeedMax;
    float hipGunKickSpeedDecay;
    float hipGunKickStaticDecay;
    int adsGunKickReducedKickBullets;
    float adsGunKickReducedKickPercent;
    float adsGunKickPitchMin;
    float adsGunKickPitchMax;
    float adsGunKickYawMin;
    float adsGunKickYawMax;
    float adsGunKickAccel;
    float adsGunKickSpeedMax;
    float adsGunKickSpeedDecay;
    float adsGunKickStaticDecay;
};

struct AttViewKick
{
    float hipViewKickPitchMin;
    float hipViewKickPitchMax;
    float hipViewKickYawMin;
    float hipViewKickYawMax;
    float hipViewKickCenterSpeed;
    float adsViewKickPitchMin;
    float adsViewKickPitchMax;
    float adsViewKickYawMin;
    float adsViewKickYawMax;
    float adsViewKickCenterSpeed;
};

struct AttADSOverlay
{
    ADSOverlay overlay;
    bool thermalScope;
};

struct AttUI
{
    Material* dpadIcon;
    Material* ammoCounterIcon;
    weaponIconRatioType_t dpadIconRatio;
    weaponIconRatioType_t ammoCounterIconRatio;
    ammoCounterClipType_t ammoCounterClip;
};

struct AttRumbles
{
    const char* fireRumble;
    const char* meleeImpactRumble;
};

struct AttProjectile
{
    int explosionRadius;
    int explosionInnerDamage;
    int explosionOuterDamage;
    float damageConeAngle;
    int projectileSpeed;
    int projectileSpeedUp;
    int projectileActivateDist;
    float projectileLifetime;
    XModel* projectileModel;
    weapProjExposion_t projExplosionType;
    const FxEffectDef* projExplosionEffect;
    bool projExplosionEffectForceNormalUp;
    snd_alias_list_t* projExplosionSound;
    const FxEffectDef* projDudEffect;
    snd_alias_list_t* projDudSound;
    bool projImpactExplode;
    float destabilizationRateTime;
    float destabilizationCurvatureMax;
    int destabilizeDistance;
    const FxEffectDef* projTrailEffect;
    int projIgnitionDelay;
    const FxEffectDef* projIgnitionEffect;
    snd_alias_list_t* projIgnitionSound;
};

struct WeaponAttachment
{
    const char* szInternalName;
    const char* szDisplayName;
    AttachmentType type;
    weapType_t weaponType;
    weapClass_t weapClass;
    XModel** worldModels;
    XModel** viewModels;
    XModel** reticleViewModels;
    AttAmmoGeneral* ammogeneral;
    AttSight* sight;
    AttReload* reload;
    AttAddOns* addOns;
    AttGeneral* general;
    AttAimAssist* aimAssist;
    AttAmmunition* ammunition;
    AttDamage* damage;
    AttLocationDamage* locationDamage;
    AttIdleSettings* idleSettings;
    AttADSSettings* adsSettings;
    AttADSSettings* adsSettingsMain;
    AttHipSpread* hipSpread;
    AttGunKick* gunKick;
    AttViewKick* viewKick;
    AttADSOverlay* adsOverlay;
    AttUI* ui;
    AttRumbles* rumbles;
    AttProjectile* projectile;
    float ammunitionScale;
    float damageScale;
    float damageScaleMin;
    float stateTimersScale;
    float fireTimersScale;
    float idleSettingsScale;
    float adsSettingsScale;
    float adsSettingsScaleMain;
    float hipSpreadScale;
    float gunKickScale;
    float viewKickScale;
    float viewCenterScale;
    int loadIndex;
    bool hideIronSightsWithThisAttachment;
    bool shareAmmoWithAlt;
};

struct AnimOverrideEntry
{
    unsigned short attachment1;
    unsigned short attachment2;
    const char* overrideAnim;
    const char* altmodeAnim;
    unsigned int animTreeType;
    int animTime;
    int altTime;
};

struct SoundOverrideEntry
{
    unsigned short attachment1;
    unsigned short attachment2;
    snd_alias_list_t* overrideSound;
    snd_alias_list_t* altmodeSound;
    unsigned int soundType;
};

struct FXOverrideEntry
{
    unsigned short attachment1;
    unsigned short attachment2;
    const FxEffectDef* overrideFX;
    const FxEffectDef* altmodeFX;
    unsigned int fxType;
};

struct ReloadStateTimerEntry
{
    int attachment;
    int reloadAddTime;
    int reloadStartAddTime;
};

struct NoteTrackToSoundEntry
{
    int attachment;
    unsigned short* notetrackSoundMapKeys;
    unsigned short* notetrackSoundMapValues;
};

struct WeaponCompleteDef
{
    const char* szInternalName;
    WeaponDef* weapDef;
    const char* szDisplayName;
    unsigned short* hideTags;
    WeaponAttachment** scopes;
    WeaponAttachment** underBarrels;
    WeaponAttachment** others;
    const char** szXAnims;
    unsigned int numAnimOverrides;
    AnimOverrideEntry* animOverrides;
    unsigned int numSoundOverrides;
    SoundOverrideEntry* soundOverrides;
    unsigned int numFXOverrides;
    FXOverrideEntry* fxOverrides;
    unsigned int numReloadStateTimerOverrides;
    ReloadStateTimerEntry* reloadOverrides;
    unsigned int numNotetrackOverrides;
    NoteTrackToSoundEntry* notetrackOverrides;
    float fAdsZoomFov;
    int iAdsTransInTime;
    int iAdsTransOutTime;
    int iClipSize;
    ImpactType impactType;
    int iFireTime;
    int iFireTimeAkimbo;
    weaponIconRatioType_t dpadIconRatio;
    float penetrateMultiplier;
    float fAdsViewKickCenterSpeed;
    float fHipViewKickCenterSpeed;
    const char* szAltWeaponName;
    int altWeapon;
    int iAltRaiseTime;
    int iAltRaiseTimeAkimbo;
    Material* killIcon;
    Material* dpadIcon;
    int fireAnimLength;
    int fireAnimLengthAkimbo;
    int iFirstRaiseTime;
    int iFirstRaiseTimeAkimbo;
    int ammoDropStockMax;
    float adsDofStart;
    float adsDofEnd;
    unsigned short accuracyGraphKnotCount[2];
    float(*accuracyGraphKnots[2])[2];
    bool motionTracker;
    bool enhanced;
    bool dpadIconShowsAmmo;
};

struct weaponParms
{ 
    float forward[3]; 
    float right[3]; 
    float up[3]; 
    float muzzleTrace[3]; 
    float gunForward[3]; 
    Weapon weapon; 
    bool isAlternate; 
    const WeaponDef* weapDef; 
    const WeaponCompleteDef* weapCompleteDef; 
};

struct missileFireParms
{ 
    bool lockon; 
    gentity_s* target; 
    float targetPosOrOffset[3]; 
    bool topFire; 
};

struct pmove_t
{ 
    playerState_s* ps; 
    usercmd_s cmd; 
    usercmd_s oldcmd; 
    int tracemask; 
    int numtouch; 
    int touchents[32]; 
    Bounds bounds; 
    float speed; 
    int proneChange; 
    int viewChangeTime; 
    float viewChange; 
    float fTorsoPitch; 
    float fWaistPitch; 
    unsigned char handler; 
};

struct pml_t
{ 
    float forward[3]; 
    float right[3];
    float up[3]; 
    float frametime; 
    int msec; 
    int walking; 
    int groundPlane; 
    trace_t groundTrace; 
    float impactSpeed; 
    float previous_origin[3]; 
    float previous_velocity[3]; 
    unsigned int holdrand; 
};