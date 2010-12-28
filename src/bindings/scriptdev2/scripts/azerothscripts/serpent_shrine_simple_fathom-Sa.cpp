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
SDName: simple_fathom
SD%Complete: 100
SDComment: simple ai x pwnare.
EndScriptData */

#include "precompiled.h"
#include "simple_ai.h"

CreatureAI* GetAI_simple_fathom(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

     
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 38627;   // 3,5k ad area, 125 di knockback, x 15 sec 1500 spell dmg taken increase.                      
        ai->Spell[0].First_Cast = 5000;  
		ai->Spell[0].Cooldown = 15000;
        ai->Spell[0].Cast_Target_Type = CAST_SELF;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 36972; // 6k to 7k, single target, shadowbolt.
        ai->Spell[1].Cooldown = 15000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 29310; // drain mana ad area, 2,6k mana drained, 2,6k dmg. 30 yard.
        ai->Spell[2].Cooldown = 15000;
        ai->Spell[2].First_Cast = 5000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 41597; // 2600 life drained every 2 sec, debuff, last 1 minute.
        ai->Spell[3].Cooldown = 61000;
        ai->Spell[3].First_Cast = 18000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[4].Enabled = true;
        ai->Spell[4].Spell_Id = 38330; // 110k to 150k di cura, serve cm "enrage" 
        ai->Spell[4].Cooldown = 180000; // dp 2 min si rifà
        ai->Spell[4].First_Cast = 600000; // dp 10 min
        ai->Spell[4].Cast_Target_Type = CAST_SELF;

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
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_simple_fathom_azsc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="simple_fathom_azsc";
    newscript->GetAI = GetAI_simple_fathom;
    newscript->RegisterSelf();
}