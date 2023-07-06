﻿// Supported with union (c) 2018-2021 Union team

#ifndef __OPROTOTYPES_H__VER3__
#define __OPROTOTYPES_H__VER3__

namespace Gothic_II_Addon {
  class oCAICamera;
  class oCAIHuman;
  class oCAIHuman_Stand;
  class oCAISound;
  class oCAIArrowBase;
  class oCAIArrow;
  class oCAIDrop;
  class oCAIVobMove;
  class oCAIVobMoveTorch;
  class oCAniCtrl_Human;
  struct myVert;
  struct myThunder;
  struct myPoly;
  class oCBarrier;
  class oCSkyControler_Barrier;
  class oCBinkPlayer;
  class oCCollObjectCharacter;
  class oCCSManager;
  class oCCSPlayer;
  class oCCSProps;
  class oCCSTrigger;
  class oCAskBox;
  class oCDoc;
  class oCDocumentManager;
  class oCNpcFocus;
  class oCGame;
  class oCGameInfo;
  class CGameManager;
  class oCGuilds;
  class oCHelp;
  class oCInfoChoice;
  class oCInfo;
  class oCInfoManager;
  class oCInformationManager;
  class oCItemContainer;
  class oCStealContainer;
  class oCNpcContainer;
  class oCNpcInventory;
  class oCItem;
  class oCTradeManager;
  class oCItemReactModule;
  class oCLogEntry;
  class oCLogTopic;
  class oCLogManager;
  class oCMagFrontier;
  class oCMag_Book;
  class oCMenuSavegame;
  struct oSMenuKey;
  class oCMenu_Help;
  class oCHelpScreen;
  class oCMenu_ChgKeys;
  class oCMenu_Main;
  struct oSMenuInfoAttribute;
  struct oSMenuInfoArmor;
  struct oSMenuInfoTalent;
  class oCMenu_Log;
  class oCMenu_Status;
  class oCStatusScreen;
  class oCLogScreen;
  class oCMapScreen;
  class oCMissionManager;
  class oCMission;
  struct TMobOptPos;
  class oCMOB;
  class oCMobInter;
  class oCMobBed;
  class oCMobSwitch;
  class oCMobItemSlot;
  class oCMobLockable;
  class oCMobContainer;
  class oCMobDoor;
  class oCMobFire;
  class oCMobWheel;
  class oCMobLadder;
  class oCDummyVobGenerator;
  class oCMobMsg;
  struct oCZoneMusicList;
  class oCZoneMusic;
  class oCZoneMusicDefault;
  class oCNews;
  class oCNewsMemory;
  class oCNewsManager;
  class oCNpcTalent;
  struct TNpcSlot;
  struct TNpcPerc;
  class oCNpc;
  class oCNpcMessage;
  class oCMsgDamage;
  class oCMsgWeapon;
  class oCMsgMovement;
  class oCMsgAttack;
  class oCMsgUseItem;
  class oCMsgState;
  class oCMsgManipulate;
  class oCMsgConversation;
  class oCMsgMagic;
  struct TNpcAIState;
  class oCNpc_States;
  struct oSDamageDescriptor;
  struct oSActionBlock;
  struct oSFightAI;
  struct oSDirectionInfo;
  struct oTRobustTrace;
  class oCObjectFactory;
  class oCParticleControl;
  class oCPlayerInfo;
  class oCPortalRoom;
  class oCPortalRoomManager;
  class oCRtnCutscene;
  class oCRtnEntry;
  class oCRtnManager;
  class oCSavegameInfo;
  class oCSavegameManager;
  class oCSpawnManager;
  class oCSpell;
  class oCSVMManager;
  class oCSVM;
  class oCSystemInfo;
  class oCText;
  class oCTriggerChangeLevel;
  class oCTriggerScript;
  class oCObjectGenerator;
  class oCViewDialogInventory;
  class oCViewDialogItem;
  class oCViewDialogItemContainer;
  class oCViewDialogStealContainer;
  class oCViewDialogTrade;
  class oCViewDocument;
  class oCViewDocumentMap;
  class oCViewGothicProgressBar;
  class oCViewStatusBar;
  class oCTrajectory;
  class oCEmitterKey;
  struct zSVisualFXColl;
  class oCVisualFX;
  class oCVisualFXAI;
  class oCVisualFX;
  class oCVisFX_MultiTarget;
  class oCVob;
  class oCTouchDamage;
  class oCWaypoint;
  class oCWay;
  class oCWorld;
  class oCWorldTimer;
  enum oTMenuSavegameMode;
  enum oTMenuAttrib;
  enum oTMenuArmor;
  enum oTEnumNpcTalent;
  enum oEBloodMode;
  enum oEInventoryAlignment;
  enum oEItemContainerAlignment;
  enum oEStealContainerAlignment;
  enum oTGameDialogView;
  enum oEDescDamageFlags;
} // namespace Gothic_II_Addon

#endif // __OPROTOTYPES_H__VER3__