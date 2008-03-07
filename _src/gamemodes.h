#ifndef _GAMEMODES_H
#define _GAMEMODES_H

#define GAMEMODE_NUM_OPTIONS 21

//Enum for each game mode
enum GameModeType{game_mode_frag, game_mode_timelimit, game_mode_coins, game_mode_classic, game_mode_chicken, game_mode_tag, game_mode_frenzy, game_mode_survival, game_mode_eggs, game_mode_domination, game_mode_owned, game_mode_jail, game_mode_stomp, game_mode_race, game_mode_star, game_mode_ctf, game_mode_koth, game_mode_greed, game_mode_health, game_mode_collection, GAMEMODE_LAST, game_mode_boss, game_mode_bonus, game_mode_pipe_minigame};
enum PlayerKillType{player_kill_none, player_kill_normal, player_kill_removed, player_kill_nonkill};

struct SModeOption
{
	char szName[64];
	short iValue;
};

struct TourStop;

//gamemode base class
class CGameMode
{
	public:
		
		CGameMode();
        virtual ~CGameMode();
		
		virtual void init();  //called once when the game is started
		virtual void think() {}	//called once a frame
		virtual void draw_background() {}
		virtual void draw_foreground() {}
		//called when a player stomps another player, after the p2p logic has run
		//returns true if the other was deleted
		virtual short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		virtual short playerkilledself(CPlayer &player, killstyle style);
		virtual void playerextraguy(CPlayer &player, short iType);

		virtual short CheckWinner(CPlayer &) {return player_kill_normal;}

		void transferbobombifneeded(CPlayer &inflictor, CPlayer &other);
		void displayplayertext();
		void playwarningsound();

		short winningteam;
		bool gameover;

		CPlayer *chicken;
		CPlayer *tagged;
		CPlayer *star;
		CPlayer *frenzyowner;

		GameModeType getgamemode(){return gamemode;}
		GameModeType gamemode;

		char * GetModeName() {return szModeName;}
		char * GetGoalName() {return szGoalName;}
		SModeOption * GetOptions() {return modeOptions;}
		
		bool playedwarningsound;
		short goal;

		short GetClosestGoal(short iGoal);
		bool GetReverseScoring() {return fReverseScoring;}

#ifdef _DEBUG
		virtual void setdebuggoal() {};
#endif
		
	protected:
		char szModeName[64];
		char szGoalName[64];
		SModeOption modeOptions[GAMEMODE_NUM_OPTIONS];

		void SetupModeStrings(char * szMode, char * szGoal, short iGoalSpacing);

		CPlayer * GetHighestScorePlayer(bool fGetLowest);

		bool fReverseScoring;
};



//Fraglimit
class CGM_Frag : public CGameMode
{
	public:
        CGM_Frag();
		virtual ~CGM_Frag() {}
		
		virtual void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

		short CheckWinner(CPlayer &player);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif
		
};


//Timelimit
class CGM_TimeLimit : public CGameMode
{
	public:
        CGM_TimeLimit();
		virtual ~CGM_TimeLimit() {}
		
		void init();
		void think();
		void draw_foreground();
		void drawtime();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);

		void addtime(short iTime);

#ifdef _DEBUG
		void setdebuggoal() {goal = 30;}
#endif

	protected:
		void SetDigitCounters();

		short timeleft;
		short framesleft_persecond;
		short iDigitLeftSrcX;
		short iDigitMiddleSrcX;
		short iDigitRightSrcX;
		short iDigitLeftDstX;
		short iDigitMiddleDstX;
		short iDigitRightDstX;
		short iScoreOffsetX;

		short iFramesPerSecond;
};


//mariowar classic
class CGM_Classic : public CGameMode
{
	public:
        CGM_Classic();
		virtual ~CGM_Classic() {}
		
		virtual void init();
		virtual void think();
		virtual short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		virtual short playerkilledself(CPlayer &player, killstyle style);
		virtual void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif

};


//capture the chicken
class CGM_Chicken : public CGameMode
{
	public:
        CGM_Chicken();
		virtual ~CGM_Chicken() {}
	
		void think();
		void draw_foreground();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);
		short CheckWinner(CPlayer &player);

#ifdef _DEBUG
		void setdebuggoal() {goal = 100;}
#endif

};

class CGM_Tag : public CGameMode
{
	public:
        CGM_Tag();
		virtual ~CGM_Tag() {}
		
		void init();
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 100;}
#endif

};

class CGM_Coins : public CGameMode
{
		public:
        CGM_Coins();
		virtual ~CGM_Coins() {}
		
		virtual void init();
		virtual void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif

};

class CGM_Eggs : public CGameMode
{
	public:
        CGM_Eggs();
		virtual ~CGM_Eggs() {}
		
		void init();
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif

};

//Just like frag limit, but firepower cards appear randomly
class CGM_Frenzy : public CGM_Frag
{
	public:
        CGM_Frenzy();
		virtual ~CGM_Frenzy() {}
		
		void init();
		void think();
		char *getMenuString(char *buffer64);

	private:
		short timer;
		short iSelectedPowerup;
		short iItemWeightCount;
};

class CGM_Survival : public CGM_Classic
{
	public:
        CGM_Survival();
		virtual ~CGM_Survival() {}
		
		void init();
		void think();
		char *getMenuString(char *buffer64);

	protected:
		short timer;
		short ratetimer;
		short rate;
		short iSelectedEnemy;
		short iEnemyWeightCount;
};


//Domination (capture the area blocks)
class CGM_Domination : public CGameMode
{
	public:
        CGM_Domination();
		virtual ~CGM_Domination() {}
		
		void init();
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 100;}
#endif
};

//Similar to frag limit, but players get bonus frags for the number of players they have "owned"
class CGM_Owned : public CGameMode
{
	public:
        CGM_Owned();
		virtual ~CGM_Owned() {}
		
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 100;}
#endif

	private:
		short CheckWinner(CPlayer &player);

};

//Similar to frag limit but:
//When a player is killed by another player, they get a "jail" marker
//Jailed players move slowly
//Jailed players can be freed by teammates
//If all players on a team are jailed, bonus kill goes to other team
//Similar to frag limit, but players get bonus frags for the number of players they have "owned"
class CGM_Jail : public CGM_Frag
{
	public:
        CGM_Jail();
		virtual ~CGM_Jail() {}
		
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		void playerextraguy(CPlayer &player, short iType);
		char *getMenuString(char *buffer64);
};

//Similar to coin mode but you have to smash the most goombas/cheeps/koopas
class CGM_Stomp : public CGameMode
{
	public:
        CGM_Stomp();
		virtual ~CGM_Stomp() {}
		
		void init();
		void think();

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

		char *getMenuString(char *buffer64);

	private:
		void ResetSpawnTimer();
		short CheckWinner(CPlayer & player);

		short spawntimer;
		short iSelectedEnemy;
		short iEnemyWeightCount;
};

//Players tag flying targets in order to complete laps
class CGM_Race : public CGameMode
{
	public:
        virtual ~CGM_Race() {}
		CGM_Race();
		void init();
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

		short getNextGoal(short teamID) {return nextGoal[teamID];}
		void setNextGoal(short teamID);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif

	protected:
		void PenalizeRaceGoals(CPlayer &player);
		short nextGoal[4];
		short quantity;
		short penalty;
};


//Star mode - shared timer ticks down and the players must pass around
//a hot potato.  When the timer hits zero, the player with the star
//loses a point.
class CGM_Star : public CGM_TimeLimit
{
	public:
        CGM_Star();
		virtual ~CGM_Star() {}
		
		void init();
		void think();
		void draw_foreground();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 1;}
#endif

	private:
		CO_Star * starItem;

};

//Capture The Flag mode - each team has a base and a flag
//Protect your colored flag from being taken and score a point
//for stealing another teams flag and returning it to your base
class CGM_CaptureTheFlag : public CGameMode
{
	public:
        CGM_CaptureTheFlag();
		virtual ~CGM_CaptureTheFlag() {}
		
		void init();
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif
};


//Domination (capture the area blocks)
class CGM_KingOfTheHill : public CGM_Domination
{
	public:
        CGM_KingOfTheHill();
		virtual ~CGM_KingOfTheHill() {}
		
		void init();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);

#ifdef _DEBUG
		void setdebuggoal() {goal = 100;}
#endif
};

//Greed mode (players try to steal each other's coins)
class CGM_Greed : public CGM_Classic
{
	public:
        CGM_Greed();
		virtual ~CGM_Greed() {}
		
		void init();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

		short ReleaseCoins(CPlayer &player, killstyle style);

#ifdef _DEBUG
		void setdebuggoal() {goal = 10;}
#endif

};

//health mode
class CGM_Health : public CGM_Classic
{
	public:
        CGM_Health();
		virtual ~CGM_Health() {}
		
		virtual void init();
		virtual short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		virtual short playerkilledself(CPlayer &player, killstyle style);
		virtual void playerextraguy(CPlayer &player, short iType);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif

};

//Collection (Collect cards for points)
class CGM_Collection : public CGameMode
{
	public:
        CGM_Collection();
		virtual ~CGM_Collection() {}
		
		void init();
		void think();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

		void ReleaseCard(CPlayer &player);

#ifdef _DEBUG
		void setdebuggoal() {goal = 5;}
#endif

	private:
		short timer;
};


/*
//Special mode where players try to kill a boss
class CGM_Boss : public CGameMode
{
	public:
        CGM_Boss();
		virtual ~CGM_Boss() {}
		
		void init();
		void think();
		void draw_foreground();

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		short playerkilledself(CPlayer &player, killstyle style);
		void playerextraguy(CPlayer &player, short iType);

		char *getMenuString(char *buffer64);

		bool SetWinner(CPlayer * player);
		void SetBossType(short bosstype);
		short GetBossType() {return iBossType;}

	private:
		
		short enemytimer, poweruptimer;
		short iBossType;
};
*/

//Similar to coin mode but you have to smash the most goombas/cheeps/koopas
class CGM_Bonus : public CGameMode
{
	public:
        CGM_Bonus();
		virtual ~CGM_Bonus() {}
		
		void init();
		void draw_background();

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) {return false;}
		short playerkilledself(CPlayer &player, killstyle style) {return false;}
		void playerextraguy(CPlayer &player, short iType) {}

		//char *getMenuString(char *buffer64);

	private:

		TourStop * tsTourStop;
};

#endif


