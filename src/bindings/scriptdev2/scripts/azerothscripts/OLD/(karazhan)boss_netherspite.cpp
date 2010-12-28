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
SDName: Boss_Netherspite
SD%Complete: 0
SDComment: Place Holder
SDCategory: Karazhan
EndScriptData */

#include "sc_creature.h"

#define SPELL_EXPLOSION		19712	// circa 1000 danno(arcane) area 30 yard. da fare spesso. 0,5 sec cast
// le 4 seguenti spell le casto cn timer unico cambiandole cn 1 switch rand
#define SPELL_MISSILES		39414	// 3k esatti di danno(arcane), target random
#define SPELL_BOLT		38823	// da 2500 a 3500 di danno(arcane), target random
#define SPELL_STARFIRE		30130	// 4500 a 5500 di danno(arcane), lo faccio tipo ogni minuto, al tanker, 4 sec cast
#define SPELL_SHADOWBOLT	20690	// 3600 a 4400 di danno(shadow), 1 sec cast

#define SPELL_MANGLE		25653	// 1600 a 2400 subito + 500 ogni 3 sec x 15 sec di danno(normale), al tanker
#define SPELL_BITE		19771	// 1500 di danno(normale) over 30 sec. (50 ogni sec)
#define SPELL_SUPERARC		40425	// 7K to 8k, 50 yard. muhahaha! 5 sec cast
#define SPELL_FROSTNOVA		5403	// fa tipo 100 di danno, e stunna tutti i player in 10yard per 10 secondi

#define SAY_FROSTNOVA	"il freddo entra nel profondo delle vostre anime... non resisterete!"
#define SAY_BEFORENOVA	"non siate frettolosi... non vi salverà"
#define SAY_DIECIPERC	"NON RIUSCIRETE A VINCERMI, SONO SUPERIORE A VOI MORTALI!NON FATEMI RICORRERE A QUELLA TECNICA!"
#define SAY_KILL	"non potete nulla contro di me"
#define SAY_DEATH	"... accadrà anche a voi"
#define SAY_TECNICA	"NON HO SCELTA... E' UN RISCHIO DA CORRERE!"

struct MANGOS_DLL_DECL boss_netherspiteAI : public ScriptedAI
{
    boss_netherspiteAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 explosion_timer;
    uint32 spell_timer; // timer unico xk si alternano
    uint32 mangle_timer;
    uint32 bite_timer;
    

    Unit* target;

	int cambia;
    

    void Reset()
    {
        // alcuni timer iniziali sono + brevi
        explosion_timer = 1000; // dopo diventa ogni 16 sec
        spell_timer = 10000; // diventa 20 sec
        mangle_timer = 36000; // rimane 36
        bite_timer = 81000; // rimane uguale (dura 30 sec)

		cambia = 0;

        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BLEED, true); // non vedo xk un drago non corporeo dovrebbe poter sanguinare...
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_ARCANE, true);
    }


    void KilledUnit(Unit* Victim)
    {
        DoYell(SAY_KILL,LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
    }

	void Aggro(Unit* Victim)
	{
	}

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (explosion_timer < diff)
        {
            DoCast(m_creature,SPELL_EXPLOSION);
            explosion_timer = 16000;
        }
        else explosion_timer -= diff;

        if (mangle_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MANGLE);
            mangle_timer = 36000;
        }
        else mangle_timer -= diff;

        if(bite_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_BITE);
            bite_timer = 81000;
        }
        else bite_timer -= diff;

        if(spell_timer < diff)
        {
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

        switch(rand()%4)
        {
        case 0:
            DoCast(target,SPELL_MISSILES);
            break;
        case 1:
            DoCast(target,SPELL_BOLT);
            break;
        case 2:
            DoCast(target,SPELL_STARFIRE);
            break;
        case 3:
            DoCast(target,SPELL_SHADOWBOLT);
            break;
        }
    
            spell_timer = 20000;
        }
        else spell_timer -= diff;

        if ( (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 60 && cambia == 0)
        {    
            DoYell(SAY_BEFORENOVA,LANG_UNIVERSAL,NULL);
			cambia = 1;
        }

        if ( (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 55 && cambia == 1)
        {   
            DoYell(SAY_FROSTNOVA,LANG_UNIVERSAL,NULL);
            DoCast(m_creature,SPELL_FROSTNOVA);
			cambia = 2;
        }

        if ( (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 10 && cambia == 2)
        {    
            DoYell(SAY_DIECIPERC,LANG_UNIVERSAL,NULL);
			cambia = 3;
        }

        if ( (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 5 && cambia == 3)
        {   
            DoYell(SAY_TECNICA,LANG_UNIVERSAL,NULL);
            DoCast(m_creature,SPELL_SUPERARC);
			cambia = 4; // evita k cominci a spammar sta spell
        }

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_netherspite(Creature *_Creature)
{
    return new boss_netherspiteAI(_Creature);
}

void AddSC_boss_netherspite()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_netherspite";
    newscript->GetAI = GetAI_boss_netherspite;
    m_scripts[nrscripts++] = newscript;
}


// update creature_template set scriptname=boss_netherspite where entry=15689;

// extern void AddSC_boss_netherspite();

// AddSC_boss_netherspite();

