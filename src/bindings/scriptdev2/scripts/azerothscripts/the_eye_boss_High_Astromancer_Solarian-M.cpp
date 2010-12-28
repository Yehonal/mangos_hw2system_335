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

#include "precompiled.h"
#include "simple_ai.h"

#define SPELL_EXPLOSION	36970	//4,5 k arcane a 10 yard, + knock back potente, + -70% speed x 8 secondi - SELF
#define SPELL_ARCLIGHT	38539	// 4k arcane dmg a 50 yard, stile fulmini - SELF
#define SPELL_ARCCONFUS	36834	// 2k di danno a 50k yard(lol), + confuso x 10 secondi - SELF
#define SPELL_ILLIDAN	41078	// spell di illidian, da 8k a 11k di danno in 20 yard, shadow. tuttavia lo fa anke a me il danno... - BOH
#define SPELL_VISUAL	35850	// un bel visual effect - SELF
#define SPELL_VOIDBOLT	39329	// void bolt da 4,8k di danno - TARGET
#define SPELL_CORRUPT	25805	// x 15sec tutti qll in 45 yard hanno -100 a tt le stat, ricevono 800 danno ogni 5 sec, e vengono knockbaccati - SELF
#define SPELL_PURGE	8012	// toglie 2 buff dal target, ottimo x togliere renew e robe del genere - TARGET
#define SPELL_SHIELD	29408	// assorbe 25k di danno.


#define SAY_SUMMON2		"I vostri incubi diventano realta'!"
#define SAY_SPECIAL1	"PURA ENERGIA MAGICA... PER SBRICIOLARVI"
#define SAY_KILL1		"Il Principe non dovra' sporcarsi le mani."
#define SAY_KILL2		"Non sei morto... la tua anima vaghera' in eterno senza pace!"
#define SAY_DEATH		"Tornero' nei vostri peggiori incubi... portero' con me le vostre deboli ani... anime"
#define SAY_SUMMON1		"SCHIAVI, ALL'ATTACCO!"
#define SAY_SPECIAL2	"Da Illidan ho imparato questo, accettero' il rischio di danneggiarmi per uccidervi"
#define SAY_LASTAND		"Non avete ancora vinto! Ho ancora malefici trucchetti per voi!"
#define SAY_AGGRO		"Tal anu'men no Sin'dorei!"

#define EMOTE_SUMMON1	" evoca una bestia ferale vicino a $N..."
#define EMOTE_SUMMON2	" fa degli strani movimenti, e qualcosa appare vicino a $N..."
#define EMOTE_KILL		" ride maleficamente guardando deliziata il corpo di $N..."
#define EMOTE_DEATH		" lancia un'ultimo terrificante sguardo a $N, per cadere poi a terra, apparentemente morta..."

#define SOUND_AGGRO		11134
// random sounds da fare ogni tot secondi
#define SOUND_1			11136
#define SOUND_2         11137
#define SOUND_3         11138
#define SOUND_4         11139
#define SOUND_5         11140


struct MANGOS_DLL_DECL boss_High_Astromancer_SolarianAI : public ScriptedAI
{
    boss_High_Astromancer_SolarianAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 aoearcane_timer;
    uint32 voidbolt_timer;
    uint32 purge_timer;
	uint32 sound_timer;
	uint32 knock_timer;

	bool said;

	int cambia;

	Unit* target;
	Unit* target2;
	Unit* target3;

	Creature* Summoned1;
	Creature* Summoned2;

	uint32 hcurrentlife;
	uint32 maxlife;


    void Reset()
    {
        aoearcane_timer = 15000;
        voidbolt_timer = 8000;
        purge_timer = 17000;
		sound_timer = 45000;
		knock_timer = 61000;

		cambia = 0;

		m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
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
		maxlife = m_creature->GetMaxHealth(); // così inizializziamo la variabile solo quando è in combat, e non quando va in evade k occupiamo risorse x nulla
		m_creature->MonsterYell(SAY_AGGRO,LANG_UNIVERSAL,who->GetGUID());
		m_creature->SetInCombatWithZone();
		DoPlaySoundToSet(m_creature, SOUND_AGGRO);

    }

    void KilledUnit(Unit* Victim)
    {
            switch(rand()%2)
            {
             case 0:
                 m_creature->MonsterYell(SAY_KILL1,LANG_UNIVERSAL,NULL);
				 m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
				 m_creature->MonsterTextEmote(EMOTE_KILL,Victim->GetGUID());
                 break;
             case 1:
				 m_creature->MonsterYell(SAY_KILL2,LANG_UNIVERSAL,NULL);
				 m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
				 m_creature->MonsterTextEmote(EMOTE_KILL,Victim->GetGUID());
                 break;
            }
    }

    void JustDied(Unit* Killer)
    {
        m_creature->MonsterYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
		m_creature->MonsterTextEmote(EMOTE_DEATH,Killer->GetGUID());
    }


    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
            return;

		if (sound_timer < diff)
        {
			switch(rand()%5)
            {
             case 0:
                 DoPlaySoundToSet(m_creature, SOUND_1);
                 break;
             case 1:
				 DoPlaySoundToSet(m_creature, SOUND_2);
                 break;
			 case 2:
				 DoPlaySoundToSet(m_creature, SOUND_3);
				 break;
			 case 3:
				 DoPlaySoundToSet(m_creature, SOUND_4);
				 break;
			 case 4:
				 DoPlaySoundToSet(m_creature, SOUND_5);
				 break;
            }
            sound_timer = 30000;
        }
		else sound_timer -= diff;

        if (voidbolt_timer < diff)
        {
			target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            DoCast(target,SPELL_VOIDBOLT);
            voidbolt_timer = 8000;
        }
        else voidbolt_timer -= diff;

        if (purge_timer < diff)
        {
			target2 = SelectUnit(SELECT_TARGET_RANDOM, 0);
            DoCast(target2,SPELL_PURGE);
            purge_timer = 13000;
        }
        else purge_timer -= diff;

        if(aoearcane_timer < diff)
        {
            switch(rand()%2)
			{
			case 0:
				DoCast(m_creature,SPELL_ARCLIGHT);
				break;
			case 1:
				DoCast(m_creature,SPELL_ARCCONFUS);
				break;
			}
			aoearcane_timer = 20000;
        }
        else aoearcane_timer -= diff;

		if(knock_timer < diff)
		{
			m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
			DoCast(m_creature,SPELL_EXPLOSION);
			knock_timer = 360000;
		}else knock_timer -= diff;

		hcurrentlife = m_creature->GetHealth()*100;

		// ora inizia il bello, huahuah m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
		if(hcurrentlife / maxlife < 75) // al 75% cominciano i problemassi
        {
            switch(cambia)  // aumento var cambia ogni volta k la vita diminuisce e quindi faccio 1 cast, è come memorizzare il livello di vita corrente.
			{
				//efficiente e professionale, by edoz. scopiazzato da the lurker below da edoz.
				case 0:
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					m_creature->MonsterYell(SAY_SPECIAL1, LANG_UNIVERSAL, NULL);
					DoCast(m_creature,SPELL_CORRUPT);
					cambia++;
                break;
				case 1:
				if(hcurrentlife / maxlife < 65)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					DoCast(m_creature, SPELL_SHIELD);
					cambia++;
				}
                break;
				case 2:
				if(hcurrentlife / maxlife < 55)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					target3 = SelectUnit(SELECT_TARGET_RANDOM, 0);
					m_creature->MonsterTextEmote(EMOTE_SUMMON1,target3->GetGUID());
					Summoned1 = NULL;
					Summoned1 = m_creature->SummonCreature(18847,target3->GetPositionX(),target3->GetPositionY(),target3->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,360000);
					if (Summoned1)
					{
						Summoned1->AI()->AttackStart(target3);
                        Summoned1->AddThreat(target3,10000.0f);
						Summoned1->setFaction(m_creature->getFaction());
					}
					m_creature->MonsterYell(SAY_SUMMON1, LANG_UNIVERSAL, NULL);
					cambia++;
				}
                break;
				case 3:
				if(hcurrentlife / maxlife < 45)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					DoCast(m_creature,SPELL_CORRUPT);
					cambia++;
				}
                break;
				case 4:
				if(hcurrentlife / maxlife < 40)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					DoCast(m_creature,SPELL_SHIELD);
					cambia++;
				}
                break;
				case 5:
				if(hcurrentlife / maxlife < 35)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					target3 = SelectUnit(SELECT_TARGET_RANDOM, 0);
					m_creature->MonsterTextEmote(EMOTE_SUMMON2,target3->GetGUID());
					Summoned2 = NULL;
					Summoned2 = m_creature->SummonCreature(18847,target3->GetPositionX(),target3->GetPositionY(),target3->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,360000);
					if (Summoned2)
					{
						Summoned2->AI()->AttackStart(target3);
                        Summoned2->AddThreat(target3,10000.0f);
						Summoned2->setFaction(m_creature->getFaction());
					}
					m_creature->MonsterYell(SAY_SUMMON2, LANG_UNIVERSAL, NULL);
					cambia++;
				}
                break;
				case 6:
				if(hcurrentlife / maxlife < 25)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					DoCast(m_creature,SPELL_CORRUPT);
					cambia++;
				}
                break;
				case 7:
				if(hcurrentlife / maxlife < 15)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					DoCast(m_creature, SPELL_SHIELD);
					cambia++;
				}
                break;
				case 8:
				if(hcurrentlife / maxlife < 10)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					m_creature->MonsterYell(SAY_LASTAND, LANG_UNIVERSAL, NULL);
					DoCast(m_creature,SPELL_VISUAL);
					m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
					m_creature->MonsterTextEmote(" ride maleficamente",NULL);
					cambia++;
				}
                break;
				case 9:
				if(hcurrentlife / maxlife < 3)
				{
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
					m_creature->MonsterYell(SAY_SPECIAL2, LANG_UNIVERSAL, NULL);
					DoCast(m_creature->getVictim(), SPELL_ILLIDAN);
					cambia++;
				}
                break;
			}
        }

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_Wild_Fel_Stalker(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

	ai->Spell[0].Enabled = true;               
    ai->Spell[0].Spell_Id = 17012;      //	penso tolga un magic effect dal target, ma siccome lui nn si fa buff dovrebbe funzionare anche come dispel, al massimo non fa niente.
		// inoltre qst spell cura il caster di 1000
    ai->Spell[0].Cooldown = 8000;                 
    ai->Spell[0].First_Cast = 4000; 
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

	ai->Spell[1].Enabled = true;               
    ai->Spell[1].Spell_Id = 32963;      //	fa 1,5k to 2k di danno ad area, shadowbolt
    ai->Spell[1].Cooldown = 8000;                 
    ai->Spell[1].First_Cast = 8000; 
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

	ai->EnterEvadeMode();

		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
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
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SHACKLE, true);
		ai->m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, true);

    return ai;

}

CreatureAI* GetAI_boss_High_Astromancer_Solarian(Creature *_Creature)
{
    return new boss_High_Astromancer_SolarianAI (_Creature);
}

void AddSC_boss_High_Astromancer_Solarian_azsc()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_High_Astromancer_Solarian_azsc";
    newscript->GetAI = GetAI_boss_High_Astromancer_Solarian;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="mob_Wild_Fel_Stalker_azsc";
    newscript->GetAI = GetAI_mob_Wild_Fel_Stalker;
    newscript->RegisterSelf();
}
