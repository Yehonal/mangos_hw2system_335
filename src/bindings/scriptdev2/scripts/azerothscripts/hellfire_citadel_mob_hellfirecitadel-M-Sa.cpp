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

/* ScriptData
SDName: mob_hellfirecitadel
SD%Complete: 100
SDComment: alcuni trash.
EndScriptData */

#include "precompiled.h"
#include "simple_ai.h"

CreatureAI* GetAI_mob_hellfirecitadel(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        // Bleeding Hollow Darkcaster
    case 17269: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 38636;   // scorch, 2500 circa           
        ai->Spell[0].Cooldown = 15000;                 
        ai->Spell[0].First_Cast = 2000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        // Bleeding Hollow Scryer
    case 17478: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 21077;   // shadow bolt, 1500 danno circa        
        ai->Spell[0].Cooldown = 10000;            
        ai->Spell[0].First_Cast = 6000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        // Bonechewer Destroyer
    case 17271: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 29574;    // rend, 300 ogni 3        
        ai->Spell[0].Cooldown = 25000;           
        ai->Spell[0].First_Cast = 5000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        // Bonechewer Hungerer
    case 17259: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 22427;    // stun x 5 sec (talento warrior arms)       
        ai->Spell[0].Cooldown = 20000;           
        ai->Spell[0].First_Cast = 10000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        // Shattered Hand Acolyte
    case 19415: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 26048;   // circa 1400 di danno      
        ai->Spell[0].Cooldown = 9000;            
        ai->Spell[0].First_Cast = 2000;          
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        // Shattered Hand Assassin
    case 17695: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 15667;    // wpn + 800
        ai->Spell[0].Cooldown = 13000;          
        ai->Spell[0].First_Cast = 1000;        
        ai->Spell[0].Cast_Target_Type = CAST_SELF;
        break;
        // Shattered Hand Gladiator
    case 17464: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 40504;      // cleave 110% dmg a 3 pg     
        ai->Spell[0].Cooldown = 10000;          
        ai->Spell[0].First_Cast = 2000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        // Shattered Hand Houndmaster
    case 17670: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 32846;         // 275 + interrompe spell 
        ai->Spell[0].Cooldown = 10000;          
        ai->Spell[0].First_Cast = 8000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        // Shattered Hand Reaver
    case 16699: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 40504;      // cleave 110% dmg a 3 pg
        ai->Spell[0].Cooldown = 15000;          
        ai->Spell[0].First_Cast = 5000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        // Shattered Hand Sharpshooter
    case 16704: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 32846;    // interrompe spell + 275 danno    
        ai->Spell[0].Cooldown = 8000;          
        ai->Spell[0].First_Cast = 6000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_hellfirecitadel_azsc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_hellfirecitadel_azsc";
    newscript->GetAI = GetAI_mob_hellfirecitadel;
    newscript->RegisterSelf();
}
