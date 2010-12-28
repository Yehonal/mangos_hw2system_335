/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "sc_creature.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_boss_aqraj(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        // Viscidus
    case 15299: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 22595;      // Poison shock School Damage (Nature) Value: 180 to 220     
        ai->Spell[0].Cooldown = 14000;          
        ai->Spell[0].First_Cast = 7000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 24099; // poison bolt volley  	Shoots poison at an enemy, inflicting Nature damage, then additional damage every 2 sec. for 10 sec. School Damage (Nature) Value: 375 to 625  Effect #2 Periodic Damage Value: 289 to 411 every 2 seconds
        ai->Spell[1].Cooldown = 14000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        // Ouro
    case 15517: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 34267;         // Inflicts 600 to 1000 damage on enemies in a cone behind the caster, knocking them back. Knock Back (80) 
        ai->Spell[0].Cooldown = 14000;          
        ai->Spell[0].First_Cast = 7000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

		ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 21832; // Hurls a boulder at an enemy. School Damage (Physical) Value: 394 to 506 Trigger Spell Knockdown
        ai->Spell[1].Cooldown = 14000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_aqraj()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_aqraj";
    newscript->GetAI = GetAI_boss_aqraj;
    m_scripts[nrscripts++] = newscript;
}
