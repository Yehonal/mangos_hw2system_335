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
SDName: bosses_mechanar
SD%Complete: 100
SDComment: mechanar's custom script
EndScriptData */

#include "sc_creature.h"
#include "../../../creature/simple_ai.h"

CreatureAI* GetAI_bosses_mechanar(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

	// yeho gay

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        // Mechano-Lord Capacitus
    case 19219: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 35161;   // riduce sta di 200 x 15 sec        
        ai->Spell[0].Cooldown = 120000;                 
        ai->Spell[0].First_Cast = 30000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 35159; // fa 750 di danno a ki lo attacca in melee. belloooo
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 1000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

		ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 19821; // 15 yard, sukkia 1k mana, fa il 50% del mana in danno. silenzia x 5 sec.
        ai->Spell[2].Cooldown = 29000;
        ai->Spell[2].First_Cast = 29000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Gatewatcher Gyro-Kill
    case 19218: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 35322;   // att speed +100% x 15 sec          
        ai->Spell[0].Cooldown = 41000;            
        ai->Spell[0].First_Cast = 6000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 35318;    // fa tipo 1,8k a caso + 550 ogni 2 sec x 8 sec
        ai->Spell[1].Cooldown = 23000;
        ai->Spell[1].First_Cast = 11000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 35311; // in cono di fronte, riduce armor e att speed del 35% x 10 sec
        ai->Spell[2].Cooldown = 12500;
        ai->Spell[2].First_Cast = 16000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Pathaleon the Calculator
    case 19220: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 29881;    // drain 1250 mana        
        ai->Spell[0].Cooldown = 20000;           
        ai->Spell[0].First_Cast = 15000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 36022;    // silence x 4 sec, 25 yard
        ai->Spell[1].Cooldown = 12000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;        
    }

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
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_bosses_mechanar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="bosses_mechanar";
    newscript->GetAI = GetAI_bosses_mechanar;
    m_scripts[nrscripts++] = newscript;
}

// extern void AddSC_bosses_mechanar();

// AddSC_bosses_mechanar();

