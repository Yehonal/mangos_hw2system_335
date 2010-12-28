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
SDName: bosses_botanica
SD%Complete: 100
SDComment: Uldaman trash mobs
EndScriptData */

// NOTA CHE C'E' UN BLOOD ELF IN QST ISTANCE, K SPAWNA 4 BOSS, OGNI TOT SECONDI, DI CUI L'ULTIMO E' Harbinger Skyriss, IO Harbinger Skyriss LO SCRIPTO, MA NN SO SE E' GIA' SPAWNATO...

#include "sc_creature.h"
#include "../../../creature/simple_ai.h"

CreatureAI* GetAI_bosses_botanica(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

	bool isBoss = false; // uso qst script x tutti i trash(tranne serpentshrine k li ha già), mettendo 1 semplice enrage per loro.

    switch (CreatureID) 
    {
        // Commander Sarannis
    case 17976: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 34820;   // 500 dmg, arcane strike.          
        ai->Spell[0].Cooldown = 8000;                 
        ai->Spell[0].First_Cast = 4000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38823; // arcane bolt da 3k
        ai->Spell[1].Cooldown = 8000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

		isBoss = true;
        break;
        //High Botanist Freywinn - interrompibile
    case 17975: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 29339;   // healing touch, 11k, 3 sec cast, c'e' tt il tempo x interromperlo, poi se 1 è furbo...    
        ai->Spell[0].Cooldown = 20000;            
        ai->Spell[0].First_Cast = 5000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 27013;    // 700 nat dmg over 12 sec, riduce chance to hit del 2%
        ai->Spell[1].Cooldown = 20000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 20690; // monnfire da 1,5k + 500 ogni 3 x 12 (istant)
        ai->Spell[2].Cooldown = 20000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 20698;    // wrath da 1k dmg
        ai->Spell[3].Cooldown = 20000;
        ai->Spell[3].First_Cast = 20000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		isBoss = true;
        break;
        //Thorngrin the Tender
    case 17978: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 34661;    // channeling, stunna x 8 sec e draina 650 vita ogni sec        
        ai->Spell[0].Cooldown = 18000;           
        ai->Spell[0].First_Cast = 2000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 39132;    // 2k cmg, fire dmg ad area
        ai->Spell[1].Cooldown = 18000;
        ai->Spell[1].First_Cast = 13000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

		isBoss = true;
        break;

		// Laj (pianta k cammina, hai presente? lol)
		case 17980: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 34697;    // 900 dmg ogni 3 sec, in + aumenta danno ricevuto di 500, in + può attaccare ad altri player la stessa spell. dispellabile. 18 sec.       
        ai->Spell[0].Cooldown = 14000;           
        ai->Spell[0].First_Cast = 7000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 28796;    // 1,3k ad area, + 450 ogni 2 x 8 sec
        ai->Spell[1].Cooldown = 14000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

		isBoss = true;
        break;
		// Warp Splinter - albero con gambe, ma nn nature, bensì arcane! suprising!
		case 17977: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 31408;   // war stomp ad area, normal dmg + 1k, + stun x 5 sec      
        ai->Spell[0].Cooldown = 15000;            
        ai->Spell[0].First_Cast = 5000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38633;    // 3,5k arcane ad area
        ai->Spell[1].Cooldown = 15000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

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
	else	// se è un boss, applichiamo le immunità ai crowd control - tranne interrupt, k 1 tipa nn ce l'ha
	{
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);		
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
	}

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_bosses_botanica()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="bosses_botanica";
    newscript->GetAI = GetAI_bosses_botanica;
    m_scripts[nrscripts++] = newscript;
}