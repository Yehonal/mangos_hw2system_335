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
SDName: bosses_arcatraz
SD%Complete: 100
SDComment: Uldaman trash mobs
EndScriptData */

// NOTA CHE C'E' UN BLOOD ELF IN QST ISTANCE, K SPAWNA 4 BOSS, OGNI TOT SECONDI, DI CUI L'ULTIMO E' Harbinger Skyriss, IO Harbinger Skyriss LO SCRIPTO, MA NN SO SE E' GIA' SPAWNATO...

#include "sc_creature.h"
#include "../../../creature/simple_ai.h"

CreatureAI* GetAI_bosses_arcatraz(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

	bool isBoss = false; // uso qst script x tutti i trash(tranne serpentshrine k li ha già), mettendo 1 semplice enrage per loro.

    switch (CreatureID) 
    {
        // Dalliah the Doomsayer - le sue spell sono interrompibili, quindi metto brevi cooldown. la gente sceglie se interrompere la cura o la bolt
    case 20885: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 39016;   // 2,5k shadow dmg          
        ai->Spell[0].Cooldown = 12000;                 
        ai->Spell[0].First_Cast = 6000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 35096; // si cura da 9k a 11k
        ai->Spell[1].Cooldown = 12000;
        ai->Spell[1].First_Cast = 12000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

		isBoss = true;
        break;
        //Wrath-Scryer Soccothrates
    case 20886: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 22423;   // spell ufficiale, 2k dmg + 350 every 3 sec, cleanabile. (shadowbolt)         
        ai->Spell[0].Cooldown = 28000;            
        ai->Spell[0].First_Cast = 7000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 39666;    // cloack of shadows
        ai->Spell[1].Cooldown = 28000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 19134; // intimidating shout, fa fear in 10 yard
        ai->Spell[2].Cooldown = 28000;
        ai->Spell[2].First_Cast = 21000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 13847;    // Recklessness, riduce la sua armor del 35%, ma fa solo colpi critici x 15 secondi.
        ai->Spell[3].Cooldown = 28000;
        ai->Spell[3].First_Cast = 28000;
        ai->Spell[3].Cast_Target_Type = CAST_SELF;

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

		isBoss = true;
        break;
        //Harbinger Skyriss
    case 20912: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 36924;    // channeling, stunna x 3 sec e fa 1250 dmg ogni sec.        
        ai->Spell[0].Cooldown = 12000;           
        ai->Spell[0].First_Cast = 6000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 28741;    // 2k cmg, nature dmg.
        ai->Spell[1].Cooldown = 12000;
        ai->Spell[1].First_Cast = 12000;
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

void AddSC_bosses_arcatraz()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="bosses_arcatraz";
    newscript->GetAI = GetAI_bosses_arcatraz;
    m_scripts[nrscripts++] = newscript;
}