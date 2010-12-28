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

#define SPELL_AOEPOISON  34780	// 1400 to 1800 ad area + 280 to 411 ogni 2 sec x 6 sec, curabile (volley)
#define SPELL_CLEAVE     26548	// 2,5 sec cast, normal dmg(dicono k ha 1 melee potente) + 250

#define SAY_AGGRO	"Grhgnggnn..."
#define SAY_DEATH	"Oughgn..."
#define SAY_SLAY	"GNCHGNG!"

// nn è fortissimo, ma ricordati k è pur sempre un boss k droppa blu

// quando lo feci, nn sapissi della simple ai, quindi x giustificare uno script apposito mettessi ora dei say simpatici... alla fine è 1 essere senza cervello, rude, bvoso e grosso.
// http://www.wowwiki.com/Quagmirran


struct MANGOS_DLL_DECL boss_quagmirranAI : public ScriptedAI
{
	boss_quagmirranAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 poison_timer;
    uint32 cleave_timer;

    Unit* target;

    void Reset()
    {
        poison_timer = 18000;
        cleave_timer = 13000;


		m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
    }

	void KilledUnit(Unit *victim)
    {
		DoYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
	}

	void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
	}

	void Aggro(Unit *who)
    {
        }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (cleave_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);
            cleave_timer = 13000;
        }
        else cleave_timer -= diff;

        if (poison_timer < diff)
        {
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_AOEPOISON);

            poison_timer = 18000;

        }
        else poison_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_quagmirran(Creature *_Creature)
{
    return new boss_quagmirranAI (_Creature);
}

void AddSC_boss_quagmirran()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_quagmirran";
    newscript->GetAI = GetAI_boss_quagmirran;
    m_scripts[nrscripts++] = newscript;
}


// update creature_template set scriptname=boss_quagmirran where entry=17942;

// extern void AddSC_boss_quagmirran();

// AddSC_boss_quagmirran();
