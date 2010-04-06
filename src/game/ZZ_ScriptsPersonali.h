#ifndef MANGOS_SCRIPTPERSONALI_H
#define MANGOS_SCRIPTPERSONALI_H

#include "Common.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "GossipDef.h"
#include "TemporarySummon.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "Group.h"
#include "ObjectAccessor.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "Chat.h"
#include "TargetedMovementGenerator.h"
#include "AccountMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "CreatureAI.h"
#include "LootMgr.h"

#define LOOT_MIN 3
#define LOOT_MAX 6 // da moltiplicare con il rate loot.referenced

#define STANDARD   20


enum Azeroth_Values
{
   AZEROTH_TOURNAMENT = 800002,
   AZEROTH_AIUTANTE = 800002,
   MONASTERO = 44, //MAPPA
   ISOLE = 2317, // AREA
   HYJAL = 616, //ZONA - AREA
   FRENESIA_MAX = 6,
   AURA_FRENESIA = 40735,  //defias and enraged
   DEMONIC_FRENZY = 23257,
   ELECTROMEDAL = 37248,
   SPELL_DAMAGE = 40142,

   P_SUICIDIO = 5,
   P_UCCISIONE = 3,

};


/* configurazioni per i mod del torneo:
   tipi:
     0 -> torneo esteso 
	 1 -> torneo esteso per adv  NON UTILIZZATA   SOstituita con : guadagno honor da gruppi
	 2 -> gruppi
	 3 -> raid
	 4 -> duelli
	 5 -> flag FFA_PVP nelle zone tournament.
	 6 -> GuildInstance Attiva/disattiva  < non attivo
	 7 -> Isole Tanaris Attiva/disattiva  < non attivo
*/


/* configurazioni per i mod rpg:
   tipi:
     0 -> loot system
	 1 -> attiva controllo loot duplicati
	 2 -> attiva guadagno credito dalle quest
	 3 -> attiva il servizio di teleport
	 4 -> attiva il moltiplicatore di esperienza gruppo
*/


class MANGOS_DLL_SPEC Hw2Class
{
	public:
		
		Hw2Class();
		~Hw2Class();

		typedef struct act {
			uint32 indice, loc_id ;
			uint8 loc_type,speak_type;
			uint16 flag , min_rep_rank, max_rep_rank , language ,  emote_id_1, emote_id_2;
			float chance;
			std::string *text_male,*text_female,*txt_emote_m,*txt_emote_f;
		    struct act *next;
		} action;

		struct classifica 
		{
			std::string *Titolo[2]; // 2 = maschio/femmina
			int32 grado;
		};


		//variabili
		
		uint32 count_list,ConfCount;
		struct act *lista;
		struct classifica cl[STANDARD];
		uint16 cl_count;
		bool AzConf[STANDARD],ListFilled,TrMod[STANDARD];
		std::string ConfDef[STANDARD];


		//procedure

		//lista rpg
		int dlstore(struct act **start);
		int dlprocess(uint16 flag, Player *pl, Creature *cr, struct act *start);
		int InitList() { return dlstore(&lista); }
		int ProcessList(uint16 flag, Player *pl, Creature *cr) { return dlprocess(flag,pl,cr,lista); }

		//special function
		Player * AzerothSelectNearbyFrTarget(Unit* unit) const;
	    std::list<Unit *> SelectNearbyTargets(Unit* unit,uint8 tipo,float distanza) const;
		uint64 HandleFindPlayer(const char* stringa,Player *Pl,Player *SelPl);
		bool AzDumpWrite(Player *pl,const char* args);
		bool AzDumpLoad(Player *pl,const char* args);
		std::string CreaStringaComposta(const char *format, ...);
		bool DoubleLoot(LootStoreItem const & i, std::vector<LootItem> quest_items,std::vector<LootItem> items);
		uint16 ProcessaRefCount(LootStoreItemList Entries,uint16 *tot,uint16 *count, float rate);
		uint16 ProcessaGruppi(Loot& loot,float rate);

		// azeroth (generale)
	    uint8 CheckAcc(Player *player,uint8 type);
		bool ImpostaGiocatore(Player *pl);
		bool Hw2Config(bool start,uint8 mode=0,uint8 tipo=0,bool scelta=false);
		
		//rpg functions
		bool RpgFunzioneIniziale(Player *pl);
		bool RpgModificaPT(bool salva,uint64 guid,int32 credito,int32 totale,bool indirect_pt=false, Player *Modder=NULL);
		bool RpgModIdentity(uint64 guid,char* stringa,Player *pl);

		bool RpgGestioneEmote(Player *pl,uint32 emote);
		bool RpgVisualizzaProfilo(Player* pl,Player* selected);
		bool RpgSetSupervisor(uint64 guid, uint64 supervisor,Player *Modder,bool Force=false);
		bool RpgAggiornaUpLine(uint64 guid,bool salva=false,float punti=0);
		bool RpgProfiloOffLine(uint64 guid,Player *Modder);
		int32 RpgContaDownLine(uint64 guid,uint16 pl_rango);
		uint16 RpgTrovaRank(int32 TotRpg);

		//Tournament Functions
		void DmAzerothPlayerInfo(Player *pl);
		bool DmIsTourn(Player *pl);
		void DmPlayerInfo(Player *pl);
		void DmAddItemSet(Player* plTarget , uint32 itemsetId);
        void DmCreaPet(Player *pl,Creature *Pet);
		void DmAddSpell(Player *pl,bool tutte=false,uint32 npclista=0);
		void DmSvuotaBorse(Player *plTarget);
		void DmSetTournament(Player *pl);
		void DmGestionePunti(Player *pl,bool SalvaSulDb=false,bool HonorKill=true,Player *soggetto=NULL,bool suicidio=false);
		void DmGestioneSpawning(Player *pl,uint32 zona);
		bool DmGestioneMusica(Player *pl,uint32 suono);
		bool DmCheckTournament(Player *pl, bool tele, uint32 mapid = -1);



	protected:

	private:
};


#define sHw2 MaNGOS::Singleton<Hw2Class>::Instance()

#endif
