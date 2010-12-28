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

#define SPELL_FLAME_EXPLOSION	45746	// 1,5k di danno (phisical) + riduce movement speed del 25% x 15 sec - SELF
#define SPELL_FIRE_RAIN			31341	// 1250 fire dmg ogni sec x 5 sec, single target
#define SPELL_FIRE_WEAKNESS		19366	// riduce la fire res di tutti in 45 yard di 200 x 15 min - SELF
#define SPELL_AMPLIFY_FIRE		41078	// aumenta il danno da fuoco ricevuto dal target di 1000 x 2 min
#define SPELL_FLAME_BUFFET		22433	// 1k di danno e aumenta danno fire ricevuto di 1k x 20 sec, single target
#define SPELL_FLAME_BREATH		43215	// 2,2k fire dmg in 15 yard davanti - nn self
#define SPELL_FLAME_BOMB		42630	// da 6k a 8k di danno fire a 4 yard radius - SELF
#define SPELL_MAGIC_SHIELD		19645	// buff k rende immune allle school magiche x 10 sec
#define SPELL_FIREBALL			29925	// 1,5k to 2k fire dmg + 600 over 3 sec

#define EMOTE_AGGRO		" brucia con ancora più furia"
#define EMOTE_SPELL1	" scatena una luminosa fiamma, bruciando tutto intorno a sè"
#define EMOTE_SPELL2	" infiamma tutto di fronte a sè col suo temibile respiro"
#define EMOTE_SPELL3	" sembra lasciare una parte di sè sul terreno, scatendando delle fiamme furiose"
#define EMOTE_SPELL4	" con un delicato soffio magico, crea un potente scudo attorno a sè"
#define EMOTE_KILL1		" sembra ricaricarsi dell'energia vitale appena dispersa..."
#define EMOTE_KILL2		" assorbe l'energia dispersa, aumentando la sua temperatura ulteriormente"
#define EMOTE_DEATH		" si spegne, consumandosi in un'ultima vampata"

struct MANGOS_DLL_DECL boss_alarAI : public ScriptedAI
{
    boss_alarAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 aoefire_timer;
    uint32 singlefire_timer;
    uint32 weakness_timer;
	uint32 shield_timer;
    
	bool kill;

	const char* castername;

	Unit* target;
	Unit* target2;
	Unit* target3;

	uint32 maxlife;
	uint32 hcurrentlife;


    void Reset()
    {
        aoefire_timer = 11013;
        singlefire_timer = 15000;
        weakness_timer  = 101000;
		shield_timer = 361000;

		kill = false;

		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FIRE, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
    }

    void Aggro(Unit *who)
    {
					maxlife = m_creature->GetMaxHealth();
					DoTextEmote(EMOTE_AGGRO,NULL);
    }


    void KilledUnit(Unit* Victim)
    {
            switch(rand()%2)
            {
             case 0:
                 DoTextEmote(EMOTE_KILL1,NULL);
                 break;
             case 1:
				 DoTextEmote(EMOTE_KILL2,NULL);
                 break;
            }
    }

    void JustDied(Unit* Killer)
    {
        DoTextEmote(EMOTE_DEATH,NULL);
    }

	void SpellHit(Unit *caster, const SpellEntry *spell)
	{
		if(spell->School == 2)
		{
			srand(time(NULL));
			switch(rand()%2)
			{
				case 0:
					DoTextEmote(" Ingerisce il fuoco, alimentando la propria energia magica",NULL);
					break;
				case 1:
					DoTextEmote(" inghiotte nella sua massa incorporea la magia lanciata dagli avversari , aumentando il proprio potere",NULL);
					break;
			}
		}
	}

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (aoefire_timer < diff)
        {
			switch(rand()%3)
			{
			case 1:
				DoCast(m_creature,SPELL_FLAME_EXPLOSION);
			case 2:
				DoCast(m_creature->getVictim(),SPELL_FLAME_BREATH);
			case 0:
				target = SelectUnit(SELECT_TARGET_RANDOM, 0);
				DoCast(target,SPELL_FIRE_RAIN);
			}
            aoefire_timer = 20000;
        }
        else aoefire_timer -= diff;

        if (singlefire_timer < diff)
        {
			target2 = SelectUnit(SELECT_TARGET_RANDOM, 0);
			switch(rand()%3)
			{
			case 1:
				DoCast(target2,SPELL_AMPLIFY_FIRE);
			case 2:
				DoCast(target2,SPELL_FLAME_BUFFET);
			case 0:
				DoCast(target2,SPELL_FIREBALL);
			}
            singlefire_timer = 15000;
        }
        else singlefire_timer -= diff;

        if(shield_timer < diff)
        {
            DoCast(m_creature,SPELL_MAGIC_SHIELD);
			shield_timer = 120000;
        }
        else shield_timer -= diff;

		if(weakness_timer < diff)
        {
			target3 = SelectUnit(SELECT_TARGET_RANDOM, 0);
            DoCast(target3,SPELL_FIRE_WEAKNESS);
			weakness_timer = 120000;
        }
        else weakness_timer -= diff;

		hcurrentlife = m_creature->GetHealth()*100;

		if(hcurrentlife / maxlife < 10 && !kill) // ultimo tentativo di disintegrare sto raid...
        {
			m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
			DoCast(m_creature,SPELL_FLAME_BOMB);
			kill = true;
        }

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_alar(Creature *_Creature)
{
    return new boss_alarAI (_Creature);
}


void AddSC_boss_alar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_alar";
	newscript->GetAI = GetAI_boss_alar;
    m_scripts[nrscripts++] = newscript;
}
