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

#define SPELL_RENEW  37260             //Heals an ally every 3 sec. for 15 sec. Value: 1200 every 3 seconds
#define SPELL_IMPALE            26548  //875 to 1125 damage inflicted every 2 sec. 8 seconds remaining
#define SPELL_WARLORDS_RAGE     36453

#define SAY_AGGRO1	"Non costringetemi a uccidervi!"
#define SAY_AGGRO2	"L'avete voluto voi"
#define SAY_AGGRO3	"Il lavoro deve continuare"
#define SAY_SLAY1	"Doveva esser fatto."
#define SAY_SLAY2	"Non saresti dovuto venire"
#define SAY_DEATH	"Io... me lo merito..."

struct MANGOS_DLL_DECL boss_mennuAI : public ScriptedAI
{
    boss_mennuAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 renew_Timer;
    uint32 Impale_Timer;
    uint32 Rage_Timer;


    int RandTime(int time) { return ((rand()%time)*1000); }

    void Reset()
    {
        renew_Timer = 10000;
        Impale_Timer = 30000;
        Rage_Timer = 60000;


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

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY1, LANG_UNIVERSAL, NULL);
            break;
        case 1:
            DoYell(SAY_SLAY2, LANG_UNIVERSAL, NULL);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
    }

    void Aggro(Unit *who)
    {

                    switch(rand()%3)
                    {
                    case 0:
                        DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
                        break;
                    case 1:
                        DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
                        break;
                    case 2:
                        DoYell(SAY_AGGRO3, LANG_UNIVERSAL, NULL);
                        break;
                    }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (renew_Timer < diff)
        {
            DoCast(m_creature,SPELL_RENEW);
            renew_Timer = 35000;
        }else renew_Timer -= diff;

        if (Impale_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (target)
            {
                DoCast(target,SPELL_IMPALE);
            }
            else
            {
                DoCast(m_creature->getVictim(),SPELL_IMPALE);
            }
            Impale_Timer = RandTime(20);
        }else Impale_Timer -= diff;

        if (Rage_Timer < diff)
        {
            DoCast(m_creature,SPELL_WARLORDS_RAGE);
            Rage_Timer = 600000;
        }else Rage_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_mennu(Creature *_Creature)
{
    return new boss_mennuAI (_Creature);
}

void AddSC_boss_mennu()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_mennu";
    newscript->GetAI = GetAI_boss_mennu;
    m_scripts[nrscripts++] = newscript;
}
