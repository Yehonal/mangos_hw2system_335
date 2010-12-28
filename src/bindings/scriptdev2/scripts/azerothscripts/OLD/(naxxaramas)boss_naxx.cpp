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

CreatureAI* GetAI_boss_naxx(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        // feugen
    case 15930: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 19482;   // War Stomp	Rank 1 Causes 160 damage and stuns all enemies within 8 yards for 2 sec.           
        ai->Spell[0].Cooldown = 12000;                 
        ai->Spell[0].First_Cast = 5000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
		break;
        // gothik
    case 16060: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 24668;   // shadow bolt Value: 638 to 862        
        ai->Spell[0].Cooldown = 14000;            
        ai->Spell[0].First_Cast = 7000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

		ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 28679; // All stats reduced by 10%.
        ai->Spell[1].Cooldown = 14000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        // grobbulus
    case 15931: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 24840;    // poison cloud  350 dmg ogni sec x 10 sec 10 yard intorno a se      
        ai->Spell[0].Cooldown = 21000;           
        ai->Spell[0].First_Cast = 7000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

		ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38458; // slimespray School Damage (Nature) Value: 700 to 900
        ai->Spell[1].Cooldown = 21000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 34261; // slimespray Value: 278 to 322 every 3 seconds
        ai->Spell[2].Cooldown = 21000;
        ai->Spell[2].First_Cast = 21000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        // heigan
    case 15936: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 17731;    // eruption School Damage (Fire) Value: 1313 to 1687       
        ai->Spell[0].Cooldown = 14000;           
        ai->Spell[0].First_Cast = 7000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

		ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 20228; // pyroblast School Damage (Fire) Value: 1388 to 1612 Effect #2 Apply Aura: Periodic Damage Value: 200 every 3 seconds
        ai->Spell[1].Cooldown = 14000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        // sapphiron
    case 15989: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 33463;   // icebolt School Damage (Frost) Value: 167 to 225      
        ai->Spell[0].Cooldown = 2012;            
        ai->Spell[0].First_Cast = 7000;          
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

		ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 22479; // frost breath Inflicts 657 to 843 Frost damage to an enemy. Time between attacks increased by 100% Movement speed slowed by 25%.
        ai->Spell[1].Cooldown = .5000;
        ai->Spell[1].First_Cast = 14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

		ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 33405; // ice barrier assorbe 1000 danno
        ai->Spell[2].Cooldown = 7000;
        ai->Spell[2].First_Cast = 21000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;

		ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 20743; // Drains 480 health from nearby enemies over 5 sec., transferring it to the caster.
        ai->Spell[3].Cooldown = 5020;
        ai->Spell[3].First_Cast = 28000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

		ai->Spell[4].Enabled = true;
        ai->Spell[4].Spell_Id = 10187; // blizzard Ice shards pelt the target area doing 1192 Frost damage over 8 sec.
        ai->Spell[4].Cooldown = 15000;
        ai->Spell[4].First_Cast = 35000;
        ai->Spell[4].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        // stalagg
    case 15929: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 19482;    // War Stomp	Rank 1 Causes 160 damage and stuns all enemies within 8 yards for 2 sec.
        ai->Spell[0].Cooldown = 10000;          
        ai->Spell[0].First_Cast = 7000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        // thaddius
    case 15928: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 22355;      // chain lightning Strikes an enemy with a lightning bolt that arcs to another nearby enemy. The spell affects up to 3 targets, causing Nature damage to each.     
        ai->Spell[0].Cooldown = 7000;          
        ai->Spell[0].First_Cast = 3000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_naxx()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_naxx";
    newscript->GetAI = GetAI_boss_naxx;
    m_scripts[nrscripts++] = newscript;
}
