/* Copyright (C) 2006,2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//*** NEEDS ADDITIOINAL REVIEW ***

#include "sc_creature.h"
#include "sc_gossip.h"
#include "Player.h"

#define ATTACK        "In Nagrand, Orco caccia cibo!!"
#define ATTACK2       "Cio' vuol dire Guerra! Guerra ho detto!"
#define GOSSIP        "Ho ottenuto cio' che volevo..e ora non farti vedere in giro in MIA presenza!"
#define GOSSIP2       "Non la passerai liscia, pagherai il tuo affronto!"
#define SPELL_PLAYERPULL    32265    //	Player Pull (125)




struct MANGOS_DLL_DECL mob_lump2AI : public ScriptedAI
{
    mob_lump2AI(Creature *c) : ScriptedAI(c) 
    {
        Evade();
    }

    uint32 hostile_timer;
    bool Defeated;
    bool Defeated2;

    void Reset()
    {
       if(!Defeated) Evade();
    }


    void riattacco(Player* target)
	{
     m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
     m_creature->setFaction(1711);
     AttackStart(target);
     m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
     Defeated = true;
     Defeated2 = false;
	}

    void Evade()
    {
        hostile_timer = 120000; //two minutes should be enough to complete quest     
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS,0);       // not a npc                        
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3);  // sleeping
        m_creature->setFaction(1711); // hostile
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,10170); 
	InCombat = false;
        Defeated = false;
	      Defeated2 = false;
    }

	void JustDied(Unit *who)
    {
        Evade();
    }

    void KilledUnit(Unit* Victim)
    {
       
    }

    void AttackStart(Unit *who)
    {
        if (!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            // say stuff
            if(!InCombat && !Defeated)
            {
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); // stand up :-)
                m_creature->Say(ATTACK, LANG_UNIVERSAL, NULL);
                m_creature->CastSpell(m_creature, SPELL_PLAYERPULL, true);
                InCombat = true;
            }
            //Begin melee attack if we are within range		
            DoStartAttackAndMovement(who);
        }
    }

    void Aggro(Unit *who)
	{
	}

    void UpdateAI(const uint32 diff)
    {
       
        if (hostile_timer < diff && Defeated2) 
           {
              Evade();
           }else hostile_timer -= diff;

     //Return since we have no target if we weren't defeated already
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

     // under 30 stop attack,become friendly and talk nice stuff finish quest
	    if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() <= 30)
          {	  
        //We give up, let's become friendly
		if (!Defeated) // prevent it calling repeatedly...should save some resources
                {
                    m_creature->Say("mi arrendo! ..ti diro' tutto", LANG_UNIVERSAL, NULL);
		
		            Defeated = true;
		            Defeated2 = true;
                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS,1);                // Gossip
		            m_creature->setFaction(35);  // Become Friendly 
		            m_creature->RemoveAllAuras();
                    m_creature->DeleteThreatList();
                    m_creature->CombatStop();
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 2);  // si siede
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT); // mantiene gli hp bassi
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                }
		
	  }
	if (!Defeated2) DoMeleeAttackIfReady();
    } 
};

void SendDefaultMenu_mob_lump2(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        player->CLOSE_GOSSIP_MENU();
    }
}

bool GossipHello_mob_lump2(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(2, GOSSIP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM(2, GOSSIP2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->SEND_GOSSIP_MENU(384,_Creature->GetGUID());
    return true;
}

bool GossipSelect_mob_lump2(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{


	 if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        // player->CompleteQuest(9918); // looks like we haven't supported this kind of quests yet?
        player->KilledMonster(18351,_Creature->GetGUID()); // so we work it out as slain >.^
    }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
    ((mob_lump2AI*)_Creature->AI())->DoYell(ATTACK2, LANG_UNIVERSAL, NULL);
    ((mob_lump2AI*)_Creature->AI())->riattacco(player);
     player->PlayerTalkClass->CloseGossip();
    }

    return true;
}

CreatureAI* GetAI_mob_lump2(Creature *_Creature)
{
    return new mob_lump2AI(_Creature);
}

void AddSC_mob_lump2()
{
    Script *newscript;
    newscript = new Script;    
    newscript->GetAI = GetAI_mob_lump2;
    newscript->Name="mob_lump2";
    newscript->pGossipHello = &GossipHello_mob_lump2;
    newscript->pGossipSelect = &GossipSelect_mob_lump2;

    m_scripts[nrscripts++] = newscript;
}

