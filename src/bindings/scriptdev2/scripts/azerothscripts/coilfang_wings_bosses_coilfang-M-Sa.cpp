/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: bosses_coilfang
SD%Complete: 100
SDComment: coilfang's boss & trash
EndScriptData */

#include "precompiled.h"
#include "simple_ai.h"

CreatureAI* GetAI_bosses_coilfang(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

	bool isBoss = false; // uso qst script x tutti i trash(tranne serpentshrine k li ha già), mettendo 1 semplice enrage per loro.

    switch (CreatureID) 
    {
        // Swamplord Musel'ek. una spell ogni 7
    case 17826: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 30990;   // ranged strike, up to 3 targets for normal damage plus 1157 to 1343.           
        ai->Spell[0].Cooldown = 14000;                 
        ai->Spell[0].First_Cast = 7000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 35945; // shot 959 to 1171 Fire damage plus 238 additional Fire damage every 3 seconds for 12 sec.
        ai->Spell[1].Cooldown = 14000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		isBoss = true;
        break;
        //The Black Stalker, boss finale di 1 dei dungeon. una spell ogni 7 secondi.
    case 17882: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 31330;   // chain lighting da 2,5k. colpisce 6 target          
        ai->Spell[0].Cooldown = 28000;            
        ai->Spell[0].First_Cast = 7000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 36664;    // cleave + 250
        ai->Spell[1].Cooldown = 28000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 40078; // fa circa 2,3k di dmg, + fa 1 dot di 2,2k ogni 5 sec. il tutto con ad area di 8 yard.
        ai->Spell[2].Cooldown = 28000;
        ai->Spell[2].First_Cast = 21000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 23895;    // renew, 2k ogni 3 sec
        ai->Spell[3].Cooldown = 28000;
        ai->Spell[3].First_Cast = 28000;
        ai->Spell[3].Cast_Target_Type = CAST_SELF;

		isBoss = true;
        break;
        //Mekgineer Steamrigger
    case 17796: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 31485;    // riduce il dmg del 35% x 15 sec        
        ai->Spell[0].Cooldown = 16000;           
        ai->Spell[0].First_Cast = 8000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 37123;    // 2k dmg, + 800 ogni 2 x 8 sec
        ai->Spell[1].Cooldown = 16000;
        ai->Spell[1].First_Cast = 16000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		isBoss = true;
        break;        
    }

	if(!isBoss)
	{
		ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 28131;    // attacca +40% velocemente, e fa 25 dmg in + ogni volta, e diventa + grosso del 25%        
        ai->Spell[0].Cooldown = 305000;   // dopo 5 min finisce, e allora la rifacciamo!        
        ai->Spell[0].First_Cast = 40000;  // gle la facciamo fare tardi. magari coi gruppi di mob succederà + spesso, xk il combattimento è lungo 
        ai->Spell[0].Cast_Target_Type = CAST_SELF;
	}
	else	// se è un boss, applichiamo le immunità ai crowd control
	{
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
	}

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_bosses_coilfang_azsc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="bosses_coilfang_azsc";
    newscript->GetAI = GetAI_bosses_coilfang;
    newscript->RegisterSelf();
}