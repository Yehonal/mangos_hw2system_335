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
SDName: simple_leotheras
SD%Complete: 100
SDComment: simple ai x pwnare.
EndScriptData */

#include "../../../sc_defines.h"
#include "../../../creature/simple_ai.h"

CreatureAI* GetAI_simple_leotheras(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

     
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 36051;   // ogni 3 sec, x sempre, 400 dmg.                      
        ai->Spell[0].First_Cast = 1000;  
		ai->Spell[0].Cooldown = 9000000;
        ai->Spell[0].Cast_Target_Type = CAST_SELF;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 41274; // stun x 3 sec + 4k dmg ad area (15 yards), stomp
        ai->Spell[1].Cooldown = 15000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

		ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 29978; // 6800 dmg ad area, 100 yard.
        ai->Spell[2].Cooldown = 15000;
        ai->Spell[2].First_Cast = 5000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 29953; // 5k dmg, fireball.
        ai->Spell[3].Cooldown = 15000;
        ai->Spell[3].First_Cast = 15000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[4].Enabled = true;
        ai->Spell[4].Spell_Id = 36819; // 45k dmg, pyroblast.
        ai->Spell[4].Cooldown = 90000;
        ai->Spell[4].First_Cast = 51000;
        ai->Spell[4].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);

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

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_simple_leotheras()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="simple_leotheras";
    newscript->GetAI = GetAI_simple_leotheras;
    m_scripts[nrscripts++] = newscript;
}