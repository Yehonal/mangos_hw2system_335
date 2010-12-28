#include "sc_creature.h"
#include "Player.h"
#include "sc_gossip.h"
#include "azerothscripts.h"

#define HELLO "Salve! Dove posso teletrasportarvi?"
#define VIAGGIO "Buona fortuna!"


#define SPELL_TELEEFFECT2 21649 // da utilizzare

//
// PORTALI CON CONTROLLO LIVELLO
//


bool GOHello_go_not_selectable(Player *player, GameObject *go)
{
uint32 level;
level = player->getLevel();

if (level<58 && player->isAlive())
 {
//        questo killa il player che non ha il level giusto per poter accedere al portale
//        player->DealDamage(player, player->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, 0, false);

	player->CastSpell(player, 14821, true); //selfstun
	player->CastSpell(player, 5, true); //death touch
	player->CastSpell(player, 33728, true); //shattrath tele
  }

return false;
}


//
// KILLER
//

struct MANGOS_DLL_DECL npc_teleAI: public ScriptedAI
{
    npc_teleAI(Creature *c) : ScriptedAI(c) {Reset();}
	uint32 timer;
 
    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		m_creature->addUnitState(UNIT_STAT_ROOT);
		timer=10000;
    }
 
    void Aggro(Unit *who)
    {
    }
 
    void MoveInLineOfSight(Unit *who)
    {
 
        if (!who || who->GetTypeId() != TYPEID_PLAYER)
            return;


		if(who->isAttacked() && m_creature->GetDistance(who) < VISIBLE_RANGE+90.0f && !InCombat) 
		   {
              if(who->HasStealthAura()) who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
 			  m_creature->Yell("COMBATTI CONTRO I TUOI AVVERSARI!...TU, NON PUOI..PASSARE!  -- YOU CAN NOT PASS ..! ", LANG_UNIVERSAL, NULL);
		      InCombat=true;
			  return;
		   }


        if (m_creature->GetDistance(who) < VISIBLE_RANGE+30.0f)
		{
          if(who->HasStealthAura()) who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

		  // se non sei abilitato al tournament verrai teleportato "modificare coordinate"
		  //if(!((Player*)who)->IsForTournament) {((Player*)who)->TeleportTo(1, 4700.22f, -3747.74f, 948.00f, 1.7f); return;}

          if(who->isAttacked()) 
		   {
			  m_creature->CastSpell(who,38007,true); //link viola
			  who->DealDamage(who, who->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		      return;
		   }

		  m_creature->CastSpell(m_creature,39180,true); //Flash Feathery Explosion Visual
		 ((Player*)who)->CastSpell(who,36704, true); //moonfire visual
			switch (rand()%5)
			  {
				case 0:
				 ((Player*)who)->TeleportTo( 1,4687.4,-3195.48,1048.05,3.03011);
					break;
				case 1:
				 ((Player*)who)->TeleportTo( 1,4341.79,-2656.27,1091.96,4.19015);
					break;
				case 2:
				 ((Player*)who)->TeleportTo(1,4620.59,-2092.93,1236.93,0.463646);
					break;
				case 3:
				 ((Player*)who)->TeleportTo(1,5106,-1752.06,1321.63,3.6206);
					break;
				case 4:
				 ((Player*)who)->TeleportTo(1,5527.17,-2643.68,1479.2,3.73056);
					break;
				default:
				 ((Player*)who)->TeleportTo(1,5390.15,-2434.85,1478.11,6.15058);
					break; 	     	
			}  
         ((Player*)who)->PlaySoundA(8280,true); //  gamebegin	nefarius
		 return;
		}               
 
    }

bool IsVisible(Unit* who) const
{
    if (!who)
        return false;
 
    return m_creature->IsWithinDistInMap(who, VISIBLE_RANGE);
}

    void UpdateAI(const uint32 diff)
    {
     if (timer < diff)
        {
            InCombat=false;
            timer = 10000;
        }else timer -= diff;
    }
};

CreatureAI* GetAI_npc_tele(Creature *_Creature)
{
    return new npc_teleAI(_Creature);
}


//
//  Azeroth Teleporters
//



struct MANGOS_DLL_DECL TeleportationAI : public ScriptedAI
{
   TeleportationAI(Creature *c) : ScriptedAI(c) {Reset();}
    
    uint32 Parla_Timer;
	bool trovato;

    void Reset()
    {     
		Parla_Timer = 500;
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS,65);
		m_creature->addUnitState(UNIT_STAT_ROOT);
		trovato=false;
    }

    void KilledUnit(Unit* Victim)
    {
    m_creature->Say("E' cio' che ti meritavi!", LANG_UNIVERSAL, NULL); 
    }

    void JustDied(Unit* Killer)
    { 
    m_creature->Yell("La pagherai.. Disonorabile creatura!", LANG_UNIVERSAL, NULL);
    }

	void Aggro(Unit* Victim)
	{
	}
    
    void UpdateAI(const uint32 diff)
    {
		if (Parla_Timer < diff)
		   { 
			m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
			switch (rand()%2)
					{
					case 0:
					DoSay("Venite avventurieri! lasciate che vi mostri i luoghi dove ho viaggiato",LANG_UNIVERSAL,NULL);
					break;
					case 1:
					DoSay("Salve avventurieri, ho visto luoghi pericolosi nei miei molti viaggi. Vi sentite pronti a visitarli?",LANG_UNIVERSAL,NULL);
					break;
					}
				Parla_Timer = 50000;
			}else Parla_Timer -= diff;
   }

}; 




 
bool GossipHello_teleport_npc_1(Player *player, Creature *_Creature)

{

player->ADD_GOSSIP_ITEM( 7, "Per sbloccare nuovi teleports procurati <'A Carefully-packed Crate'> dai Reagent Vendor delle maggiori capitali, e' possibile utilizzarne 1 al giorno", GOSSIP_SENDER_MAIN, 0);	

if (player->HasItemCount(9507, 1))
{	
player->ADD_GOSSIP_ITEM( 7, "Teleport per Molten Core per lvl 60 ad 1 oro", GOSSIP_SENDER_MAIN, 1212);	
player->ADD_GOSSIP_ITEM( 7, "Teleport per Onyxia's Lair per lvl 60 ad 1 oro", GOSSIP_SENDER_MAIN, 1213);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Blackwing Lair per lvl 60 a 2 ori", GOSSIP_SENDER_MAIN, 1214);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Cenarion Hold per lvl 60 a 3 ori", GOSSIP_SENDER_MAIN, 1215);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Naxxramas per lvl 60 a 3 ori", GOSSIP_SENDER_MAIN, 1216);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Hinterlands per lvl 55 a 2 ori", GOSSIP_SENDER_MAIN, 1217);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Azshara per lvl 55 a 2 ori", GOSSIP_SENDER_MAIN, 1218);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Feralas per lvl 55 a 2 ori", GOSSIP_SENDER_MAIN, 1219);

}

player->ADD_GOSSIP_ITEM( 7, "Teleport per Shattrath per lvl 70 a 1 oro e 50 argenti", GOSSIP_SENDER_MAIN, 1211);

	if  (player->GetTeam() == ALLIANCE )  {

//player->ADD_GOSSIP_ITEM( 7, "Teleport per il monte Hyjal gratuito", GOSSIP_SENDER_MAIN, 1205);
        if (player->GetZoneId() != 3703) {
player->ADD_GOSSIP_ITEM( 7, "Teleport per Darnassus a 20 argenti", GOSSIP_SENDER_MAIN, 1202);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Exodar a 20 argenti", GOSSIP_SENDER_MAIN, 1203);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Stormwind a 20 argenti", GOSSIP_SENDER_MAIN, 1204);
                                       }
} else if  (player->GetTeam() == HORDE )  {

//player->ADD_GOSSIP_ITEM( 7, "Teleport per il monte Hyjal gratuito", GOSSIP_SENDER_MAIN, 1210);

if (player->GetZoneId() != 3703) {
player->ADD_GOSSIP_ITEM( 7, "Teleport per Orgrimmar a 20 argenti", GOSSIP_SENDER_MAIN, 1207);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Silvermoon a 20 argenti", GOSSIP_SENDER_MAIN, 1208);
player->ADD_GOSSIP_ITEM( 7, "Teleport per Undercity a 20 argenti", GOSSIP_SENDER_MAIN, 1209);
                               }

   }

player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
_Creature->Say(HELLO, LANG_UNIVERSAL, NULL);
_Creature->HandleEmoteCommand(EMOTE_ONESHOT_BOW);

return true;

}

void SendDefaultMenu_teleport_npc_1(Player *player, Creature *_Creature, uint32 action )
{
uint32 livello;
uint32 money;
uint32 costo;

if(player->isAttacked()) {
      _Creature->Say("Sei in combat, sconfiggi prima i tuoi nemici!", LANG_UNIVERSAL, NULL);
   return;
 }

livello = player->getLevel();

if ( livello < 15  )   {
      _Creature->Say("Devi essere minimo al lvl 15 per poter essere teleportato", LANG_UNIVERSAL, NULL);
      return;
   }

money = player-> GetMoney();
costo = 2000;

if (money < costo && action != 1210 && action != 1205) { 
	_Creature->Say("Non hai abbastanza argenti", LANG_UNIVERSAL, NULL);
   return;
 } 

if (action==0)
{
    player->CLOSE_GOSSIP_MENU();
        if  (player->GetTeam() == ALLIANCE )  {
	    _Creature->Say("I mercanti che vendono l'item sono Keldric Boucher a Stormwind e Fantei a Shattrath", LANG_UNIVERSAL, NULL); 
	    return;
	    } else if (player->GetTeam() == HORDE ) {
        _Creature->Say("I mercanti che vendono l'item sono Hagrus ad Orgrimmar e Fantei a Shattrath", LANG_UNIVERSAL, NULL); 
        return;
		}
}


_Creature->CastSpell(_Creature, SPELL_TELEEFFECT2, true);

	switch(action)
	{
	case 1202: // Teleport to Darnassus
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 9947.52f, 2482.73f, 1316.21f, 0.0f);
	player->ModifyMoney(-1*costo);
	break;

	// Teleport to Exodar
	case 1203:
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, -4073.03f, -12020.4f, -1.47f, 0.0f);

	player->ModifyMoney(-1*costo);
	break;


	// Teleport to Stormwind
	case 1204:
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -8960.14f, 516.266f, 96.3568f, 0.0f);

	player->ModifyMoney(-1*costo);
	break;

	// Teleport to hyjal ally
	case 1205:
    if (player->GetZoneId() == 3703 && livello < 58) return;
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 4700.22f, -3747.74f, 948.00f, 1.7f);
	break;

	// Teleport to Orgrimmar
	case 1207:
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 1552.5f, -4420.66f, 8.94802f, 0.0f);

	player->ModifyMoney(-1*costo);
	break;

	// Teleport to Silvermoon
	case 1208:
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 9338.74f, -7277.27f, 13.7895f, 0.0f);

	player->ModifyMoney(-1*costo);
	break;

	// Teleport to Undercity
	case 1209:
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, 1819.71f, 238.79f, 60.5321f, 0.0f);

	player->ModifyMoney(-1*costo);
	break;


	// Teleport to hyjal horde
	case 1210:
    if (player->GetZoneId() == 3703 && livello < 58) return;
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 4672.96f, -3610.52f, 973.00f, 4.7f);
	break;

        // teleport to shattrath
        case 1211:
        player->CLOSE_GOSSIP_MENU();
        if (livello < 70 || money < 15000) return;
        player->TeleportTo(530, -1879.501f, 5409.462f, 1.428f, 1.0333f);
        player->ModifyMoney(-1*15000);                
        break; 

	 //teleport to molten
	case 1212:
    player->CLOSE_GOSSIP_MENU();
	if (livello < 60 || money < 10000) return;
	player->TeleportTo(409, 1078.4160f, -479.409f, -108.301f, 0.0f);
	if (player->GetZoneId() != 2717) return;
	player->DestroyItemCount(9507, 1, true);
	break;

        //teleport to onyxia
	case 1213:
	player->CLOSE_GOSSIP_MENU();	 
	if (livello < 60 || money < 10000) return;
	player->TeleportTo(249, 29.4548f, -68.9609f, -5.98402f, 0.0f);
	if (player->GetZoneId() != 3459) return;
	player->DestroyItemCount(9507, 1, true);

	

	break;
        //teleport to bwl
	case 1214:
    player->CLOSE_GOSSIP_MENU();
	if (livello < 60 || money < 20000) return;
	player->TeleportTo(469, -7665.55f, -1102.49f, 400.679f, 0.0f);
	if (player->GetZoneId() != 2677) return;
	player->DestroyItemCount(9507, 1, true);
	break;     

        //teleport to cenarion hold
	case 1215:
	player->CLOSE_GOSSIP_MENU();	 
	if (livello < 60 || money < 30000) return;
	player->TeleportTo(1, -6824.15f, 821.273f, 50.6675f, 0.0f);
	player->DestroyItemCount(9507, 1, true);
	break;
   
        //teleport to naxxramas
	case 1216:
    player->CLOSE_GOSSIP_MENU();
	if (livello < 60 || money < 30000) return;
	player->TeleportTo(533, 3005.87f, -3435.01f, 294.882f, 0.0f);
	if (player->GetZoneId() != 3456) return;
	player->DestroyItemCount(9507, 1, true);
	break;

        //teleport to hinterlands
	case 1217:
	player->CLOSE_GOSSIP_MENU();	 
	if (livello < 55 || money < 20000) return;
	player->TeleportTo(0, 109.496f, -3928.047f, 137.8f, 5.5f);
	player->DestroyItemCount(9507, 1, true);
	break;

        //teleport to azshara
	case 1218:
    player->CLOSE_GOSSIP_MENU();
	if (livello < 55 || money < 20000) return;
	player->TeleportTo(1, 2747.285f, -3919.89f, 91.73f, 5.7f);
	player->DestroyItemCount(9507, 1, true);
	break;   

        //teleport to feralas
        case 1219:
    player->CLOSE_GOSSIP_MENU();
	if (livello < 55 || money < 20000) return;
	player->TeleportTo(1, -4522.22f, 2038.54f, 51.1436f, 0.0f);
	player->DestroyItemCount(9507, 1, true);
	break;    	  	  	
   }


_Creature->Say(VIAGGIO, LANG_UNIVERSAL, NULL);
_Creature->TextEmote("Saluta il viaggiante", 0);
_Creature->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
player->CastSpell(player, SPELL_TELEEFFECT2, true);

return;
}


bool GossipSelect_teleport_npc_1(Player *player, Creature *_Creature, uint32 sender, uint32 action )

{
// Main menu
if (sender == GOSSIP_SENDER_MAIN)
SendDefaultMenu_teleport_npc_1(player, _Creature, action);


return true;

}


bool ReceiveEmote_teleport_npc_1(Player *player, Creature *_Creature, uint32 emote)
{
    _Creature->HandleEmoteCommand(emote);
 
    if (emote == EMOTE_STATE_DANCE)
        _Creature->Say("Danziamo insieme!",LANG_UNIVERSAL,NULL);
 
    if (emote == EMOTE_ONESHOT_SALUTE)
        _Creature->Say("Onore e gloria soldato.",LANG_UNIVERSAL,NULL);
 
    return true;
}

CreatureAI* GetAI_teleport_npc_1(Creature *_creature)
{
    return new TeleportationAI(_creature);
}


void AddSC_azeroth_scripts()
{

Script *newscript;

	newscript = new Script;
	newscript->Name="teleport_npc_1";
	newscript->GetAI = GetAI_teleport_npc_1;
	newscript->pGossipHello = &GossipHello_teleport_npc_1;
	newscript->pGossipSelect = &GossipSelect_teleport_npc_1;
	newscript->pReceiveEmote = &ReceiveEmote_teleport_npc_1;
	m_scripts[nrscripts++] = newscript;

	newscript = new Script;
	newscript->Name="go_not_selectable";
	newscript->pGOHello = GOHello_go_not_selectable;
	m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="npc_sviatore";
	newscript->GetAI = GetAI_npc_tele;
    m_scripts[nrscripts++] = newscript;

}