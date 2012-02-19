#include "ZZ_ScriptsPersonali.h"


INSTANTIATE_SINGLETON_1( Hw2Class );


Hw2Class::Hw2Class() //costruttore
{
	ListFilled=false;
	lista=(struct act *)malloc(sizeof(struct act));
	ConfCount=0;
    timerCambiaOra = 0;
    newHr = 0;
    m_modGameTime = time(NULL);
}

Hw2Class::~Hw2Class()
{



}

Hw2Class* Hw2Class::GetHw2()
{
    return &sHw2;
}

void Hw2Class::Update(uint32 diff)
{

    if (newHr!=0)
        if (timerCambiaOra < diff)
        {

                tm localTm = *localtime(&sHw2.GetModGameTime());
                int increment = newHr > 0 ? 1 : -1;

                if (localTm.tm_min >=59)
                {
                    localTm.tm_min  = 0;
                    localTm.tm_hour += increment;
                    newHr -= increment;
                }
                else
                if ( localTm.tm_min <=0)
                {
                    localTm.tm_min  = 59;
                    localTm.tm_hour += increment;
                    newHr -= increment;
                }

                localTm.tm_min  += increment;

                time_t newtime = mktime(&localTm);
                WorldPacket data(SMSG_LOGIN_SETTIMESPEED, 4 + 4 + 4);
                data << uint32(secsToTimeBitFields(newtime));
                data << (float)0.01666667f;                             // game speed
                data << uint32(0);                                      // added in 3.1.2
                sWorld.SendGlobalMessage( &data );

                timerCambiaOra = 50;
                sHw2.m_modGameTime = newtime;

        } else timerCambiaOra -= diff;


}



/*

Elenco comandi Az:   

TIPO1:

lvl 1 =  mute/unmute/parla/sussurra/urla/txtem

lvl 2 =  paralizza/rilascia/listapr/salvatutti/suono/tempo...

lvl 3 =  uccidi /resuscita / espelli / espellitutti 

lvl 4 =  ricarica_actrpg / chatlog_on/chatlog_off / hv_lanciaspell /mantello /divinityon /divinityoff /trans /pulizia_characters

TIPO2:

lvl 1 =  segui_pet/segui_area/segui_pos/fermo/ritorna/ostile/cammina_qui 

lvl 2 =  summon/emote_area

lvl 3 =  dio/resist/risana       
                   

*/


//
// COMANDI ( AZ )
//

bool ChatHandler::HandleAzerothSpecialCommands(char* args)
{

    Player *autore= m_session->GetPlayer();

	if (!autore)
		return false;

    if(!*args)
    {
        sHw2.Hw2SendSysMessage(autore,"Comando errato o non inserito, digita .az comandi per visualizzare i comandi disponibili");
        return true;
    }

    std::string argstr = strtok((char*)args, " ");
    char* stringa = strtok(NULL, ""); 

	if (argstr == "comandi")
	{


		sHw2.Hw2SendSysMessage(autore," ");
		sHw2.Hw2SendSysMessage(autore,"I comandi speciali (tipo0) sono:");
		sHw2.Hw2SendSysMessage(autore," ");
		sHw2.Hw2SendSysMessage(autore,"lvl0: rpg_profilo <> rpg_precettore");
		

		if (autore->AccLvl[0]>0)
		{
			sHw2.Hw2SendSysMessage(autore," ");
			sHw2.Hw2SendSysMessage(autore,"I comandi speciali (tipo1) sono:");
			sHw2.Hw2SendSysMessage(autore," ");
			sHw2.Hw2SendSysMessage(autore," lvl1: mute <> unmute <> parla <> sussurra <> urla <> txtem");
		    
			if (autore->AccLvl[0]>=2)
			{
				sHw2.Hw2SendSysMessage(autore," lvl2: paralizza <> rilascia <> listapr <> salvatutti <> suono <> tempo");
				if (autore->AccLvl[0]>=3) 
				{
					sHw2.Hw2SendSysMessage(autore," lvl3: uccidi <> resuscita <> espelli <> espellitutti ");
					if (autore->AccLvl[0]>=4)
						sHw2.Hw2SendSysMessage(autore," lvl4: conf <> rpg_ricarica <> chatlog_on <> chatlog_off <> mantello <> divinityon  <> divinityoff <> trans <> pulizia_characters(fare prima un backup! usarlo cautamente)");
					else
						sHw2.Hw2SendSysMessage(autore,"lvl superiore: accesso ai comandi non disponibile");

				} else sHw2.Hw2SendSysMessage(autore,"lvl superiore: accesso ai comandi non disponibile");

			} else sHw2.Hw2SendSysMessage(autore,"lvl superiore: accesso ai comandi non disponibile");

		} else sHw2.Hw2SendSysMessage(autore,"Comandi tipo 1: accesso ai comandi non disponibile");

		if (autore->AccLvl[1]>0)
		{
			sHw2.Hw2SendSysMessage(autore," ");
			sHw2.Hw2SendSysMessage(autore,"I comandi speciali (tipo2) sono:");
			sHw2.Hw2SendSysMessage(autore," ");
			 sHw2.Hw2SendSysMessage(autore," lvl1: segui_pet <> segui_pos <> segui_area<> fermo <> ritorna <> ostile <> cammina_qui");
			
			if (autore->AccLvl[1]>=2)
			{
				 sHw2.Hw2SendSysMessage(autore," lvl2: summon <> emote_area <> rpg_crediti <> rpg_identity <> rpg_identity_byname <> rpg_profilo_byname");
				 if (autore->AccLvl[1]>=3) 
					 sHw2.Hw2SendSysMessage(autore," lvl3: dio <> resist <> risana <> rpg_punti <> rpg_mod_precettore");
				 else
					 sHw2.Hw2SendSysMessage(autore,"lvl superiore: accesso ai comandi non disponibile");

			}else sHw2.Hw2SendSysMessage(autore,"lvl superiore: accesso ai comandi non disponibile");

		}else sHw2.Hw2SendSysMessage(autore,"Comandi tipo 2: accesso ai comandi non disponibile");

	  return true;
	}


 
//########################################################################################## 
// 
// COMANDI PUBBLICI
//
//##########################################################################################


    if (argstr=="cambia_ora")
    {
        int type=0;
        char* tipo = strtok(stringa, " ");

		if (!tipo)
            return false;

        tm localTm = *localtime(&sHw2.GetModGameTime());
        int ora = atoi(tipo);

        sHw2.newHr = localTm.tm_hour >= ora ? -1 * (localTm.tm_hour - ora) : ora - localTm.tm_hour;

        return true;

        /*tm localTm = *localtime(&sHw2.GetModGameTime());
        int now       = localTm.tm_hour;
        int diffHours = newHr - now;
        if(diffHours < 0)
            diffHours *= -1;

        time_t newtime;
        for (uint8 i=0;i<=diffHours;i++)
        {
            localTm.tm_hour = newHr >= now ? now + i : now - i;

            // current day reset time
            newtime = mktime(&localTm);
            WorldPacket data(SMSG_LOGIN_SETTIMESPEED, 4 + 4 + 4);
            data << uint32(secsToTimeBitFields(newtime));
            data << (float)0.01666667f;                             // game speed
            data << uint32(0);                                      // added in 3.1.2
            sWorld.SendGlobalMessage( &data );
        }

        if (newtime)
            sHw2.m_modGameTime = newtime; */

    }

    if (argstr=="creapet")
    {
        if(autore->GetPetGuid())
        return false;

        Creature * creatureTarget = getSelectedCreature();
		if(!creatureTarget) 
            return false;

        if(creatureTarget->IsPet())
            return false;

        CreatureCreatePos pos = CreatureCreatePos(autore, -autore->GetOrientation());

        Pet* pet = new Pet(MINI_PET);

        pet->Create(autore->GetMap()->GenerateLocalLowGuid(HIGHGUID_PET), pos,creatureTarget->GetCreatureInfo(), sObjectMgr.GeneratePetNumber());

        if(!pet)                                                // in versy specific state like near world end/etc.
        {
            delete pet;
            return false;
        }

        // kill original creature
        //creatureTarget->setDeathState(JUST_DIED);
        //creatureTarget->RemoveCorpse();
        //creatureTarget->SetHealth(0);                       // just for nice GM-mode view

        uint32 level = (creatureTarget->getLevel() < (autore->getLevel() - 5)) ? (autore->getLevel() - 5) : creatureTarget->getLevel();

        // prepare visual effect for levelup
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

        pet->SetSummonPoint(pos);
        pet->GetMap()->Add((Creature*)pet);

        pet->SetOwnerGuid(autore->GetObjectGuid());
        pet->SetCreatorGuid(autore->GetObjectGuid());
        pet->setFaction(autore->getFaction());
        pet->AIM_Initialize();
        pet->InitPetCreateSpells();                         // e.g. disgusting oozeling has a create spell as critter...
        sHw2.Hw2SendSysMessage(autore,"minipet creato");
        return true;
    }

    if (argstr=="guardian")
    {
        Creature * creature = getSelectedCreature();
		if(!creature) return false;

        creature->azGuard = !creature->azGuard;
        if (creature->IsGuard()) {
			creature->setFaction(autore->getFaction());
			sHw2.Hw2SendSysMessage(autore,"guardia attivata");
        } else {
			creature->setFaction(autore->getFaction());
			sHw2.Hw2SendSysMessage(autore,"guardian disabled");
        }
        return true;
    }


	if (argstr=="rpg_profilo")
	{
		Player* SelPl = getSelectedPlayer();

		if (SelPl)
			sHw2.RpgVisualizzaProfilo(autore,SelPl);

		return true;
	}

	if (argstr=="rpg_precettore")
	{

		ObjectGuid guid  = autore->GetSelectionGuid();
		Player* SelPl=NULL;
		if (guid.GetCounter()!=0)
		{
			SelPl=sObjectMgr.GetPlayer(guid);
		}

		char* nome = strtok(stringa, " ");

		guid=ObjectGuid(); // reset variabile

		guid=sHw2.HandleFindPlayer(nome,autore,SelPl);

		if (guid.GetCounter()==0)
		{ 
			sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: Devi selezionare un giocatore oppure inserirne il nome(opzionale) per settarlo come precettore");
			sHw2.Hw2SendSysMessage(autore,".az rpg_precettore <nomeplayer> (opzionale se il player e' gia' selezionato)");
			return false; 
		} 

		sHw2.RpgSetSupervisor(autore->GetObjectGuid(),guid,autore);
		return true;
	}


	

//##########################################################################################
//	CONTROLLI COMANDI NON PUBBLICI
//##########################################################################################

	
if (autore->AccLvl[0]<=0 && autore->AccLvl[1]<=0) return false; // se l'account non e' presente nella lista non accedera' ai prossimi comandi


//########################################################################################## 
// 
// TIPO 1
//
//##########################################################################################


    //##########################################################################################
	//	LIVELLO 1
	//##########################################################################################

if (autore->AccLvl[0]>=1)
{
   
    if (argstr == "mute")
    {
		if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,"Specifica il nome del pg seguito dal tempo della durata del mute in minuti "); return false; }
        HandleMuteCommand(stringa);
		return true;
    }
	
    if (argstr == "unmute")
    {
		if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,"Specifica il nome del pg"); return false; }
        HandleUnmuteCommand(stringa);
		return true;
    }
	
	if (argstr == "parla")
    {
		if (!stringa || !strcmp(stringa," ")) return false;
        HandleNpcSayCommand(stringa);
		getSelectedCreature()->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
		return true;
    }

	if (argstr == "sussurra")
    {    
       if (!stringa || !strcmp(stringa," ")) return false;
	   HandleNpcWhisperCommand(stringa);
	   return true;
    }

	if (argstr == "urla")
    {   
       if (!stringa || !strcmp(stringa," ")) return false;
       HandleNpcYellCommand(stringa);
	   getSelectedCreature()->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
	   return true;
    }

	if (argstr == "txtem")
    {       
	   if (!stringa || !strcmp(stringa," ")) return false;
       HandleNpcTextEmoteCommand(stringa);
	   return true;
    }

    //##########################################################################################
	//	LIVELLO 2
	//##########################################################################################

			if (autore->AccLvl[0] >= 2)
			{
				if (argstr == "paralizza")
				{	
					
					std::string name;
					Player* player;
					char* TargetName = stringa; //get entered #name
					
					if (!TargetName) //if no #name entered use target
					{
						player = getSelectedPlayer();
						if (player) //prevent crash with creature as target
						{   
						   name = player->GetName();
						   normalizePlayerName(name);
						}
					}
					else // if #name entered
					{
						name = TargetName;
						normalizePlayerName(name);
						player = sObjectMgr.GetPlayer(name.c_str()); //get player by #name
					}
					
					//effect
					if ((player) && (!(player==autore)))
					{
						sHw2.Hw2SendSysMessage(autore,"%s paralizzato",name.c_str());

						//stop combat + unattackable + duel block + stop some spells
						//TODO: Test this a bit more ingame
						player->setFaction(35);
						player->CombatStop();
						if(player->IsNonMeleeSpellCasted(true))
						player->InterruptNonMeleeSpells(true);
						player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						player->SetUInt32Value(PLAYER_DUEL_TEAM, 1);

						//if player class = hunter || warlock remove pet if alive
						if((player->getClass() == CLASS_HUNTER) || (player->getClass() == CLASS_WARLOCK))
						{
							if(Pet* pet = player->GetPet())
							{
								pet->SavePetToDB(PET_SAVE_AS_CURRENT);
								// not let dismiss dead pet
								if(pet->isAlive())
									player->RemovePet(PET_SAVE_NOT_IN_SLOT);
							}
						}

						//stop movement and disable spells
						uint32 spellID = 9454; //2647;
						SpellEntry const *spellInfo = sSpellStore.LookupEntry( spellID );
						if(spellInfo) //TODO: Change the duration of the aura to -1 instead of 5000000
						{
							for(uint32 i = 0;i<3;i++)
							{
								uint8 eff = spellInfo->Effect[i];
								if (eff>=TOTAL_SPELL_EFFECTS)
									continue;

								SpellAuraHolder *new_holder = CreateSpellAuraHolder(spellInfo, player, NULL);

								if(eff == SPELL_EFFECT_APPLY_AREA_AURA_PARTY)
								{
									Aura *Aur = new AreaAura(spellInfo, SpellEffectIndex(i),NULL, NULL, player, NULL);
									new_holder->AddAura(Aur,SpellEffectIndex(i));
								}
								else if (eff == SPELL_EFFECT_APPLY_AURA || eff == SPELL_EFFECT_PERSISTENT_AREA_AURA)
								{
									Aura *Aur = CreateAura(spellInfo, SpellEffectIndex(i), NULL, NULL, player, NULL);
									new_holder->AddAura(Aur,SpellEffectIndex(i));
								}
							}
						}
						//save player
						player->SaveToDB();
							}

					if (!player)
					{
						sHw2.Hw2SendSysMessage(autore,"Nessun player selezionato");
						return true;
					}

					if (player==autore)
					{
						sHw2.Hw2SendSysMessage(autore,"Non puoi paralizzare te stesso!");
						return true;
					}
				 return true;
				}

				if (argstr == "rilascia")
				{
					std::string name;
					Player* player;
					char* TargetName = stringa; //get entered #name
					if (!TargetName) //if no #name entered use target
					{
						player = getSelectedPlayer();
						if (player) //prevent crash with creature as target
						{   
						   name = player->GetName();
						}
					}

					else // if #name entered
					{
						name = TargetName;
						normalizePlayerName(name);
						player = sObjectMgr.GetPlayer(name.c_str()); //get player by #name
					}

					//effect
					if (player)
					{
						sHw2.Hw2SendSysMessage(autore,"Player %s liberato",name.c_str());

						//Reset player faction + allow combat + allow duels
						player->setFactionForRace(player->getRace());
						player->RemoveFlag (UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

						//allow movement and spells
						uint32 spellID = 9454; //2647;
						player->RemoveAurasDueToSpell(spellID);

						//save player
						player->SaveToDB();
					}

					if (!player)
					{
						sHw2.Hw2SendSysMessage(autore,"Nessun player selezionato");
						return true;
					}
				  return true;
				 }

				 if (argstr == "listapr")
				 {
					 //Get names from DB
					QueryResult *result = CharacterDatabase.PQuery("SELECT characters.name FROM `characters` LEFT JOIN `character_aura` ON (characters.guid = character_aura.guid) WHERE character_aura.spell = 9454");
					if(!result)
					{
						sHw2.Hw2SendSysMessage(autore,"Nessun player nella lista");
						return true;
					}
					//Header of the names
					sHw2.Hw2SendSysMessage(autore,"Lista:");
				    
					//Output of the results
					do
					{
						Field *fields = result->Fetch();
						std::string fplayers = fields[0].GetCppString();
						sHw2.Hw2SendSysMessage(autore,"- %s",fplayers.c_str());
					} while (result->NextRow());

					delete result;
					return true;
				}


				if (argstr == "suono")
				{
				 if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,"scegli il suono"); return false; }
				   HandleDebugPlaySoundCommand(stringa);
				   return true;
				}

				if (argstr == "tempo")
				{
				 if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,"scegli le condizioni atmosferiche"); return false; }
				   HandleChangeWeatherCommand(stringa);
				   return true;
				}

				if (argstr == "salvatutti")
				{
				   ObjectAccessor::Instance().SaveAllPlayers(); // salva tutti
				   return true;
				}
    
	//##########################################################################################
	//	LIVELLO 3
	//##########################################################################################

						if (autore->AccLvl[0]>=3)
						{
						 
							if (argstr == "espelli")
							{
							   if (!stringa || !strcmp(stringa," ")) stringa=NULL;    
							   HandleKickPlayerCommand(stringa);
							   return true;
							}

							if (argstr == "espellitutti")
							{
								if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,"scegli il l'acclvl da espellere"); return false; }
							   HandleServerPLimitCommand(stringa);
							   return true;
							}


							if (argstr == "uccidi")
							{	    
								Player *plr=NULL;
								
								if (!stringa || !strcmp(stringa," ")) 
								{ plr = getSelectedPlayer(); 
								  if (!plr) { sHw2.Hw2SendSysMessage(autore,"Seleziona un player o digita il nome"); return false; }
								}
								else plr = sObjectMgr.GetPlayer(stringa);
									
								if (!plr) 
								{
									sHw2.Hw2SendSysMessage(autore,"Il player %s non e' online oppure non esiste.",stringa);
									return true;
								}

								if(plr->isDead())
								{
									sHw2.Hw2SendSysMessage(autore,"Il player %s e' gia' morto.",plr->GetName());
								} 
								else 
								{
									plr->SetUInt32Value(UNIT_FIELD_HEALTH, 0); // Die, insect
									plr->KillPlayer();
									sHw2.Hw2SendSysMessage(plr,"Tu sei stato ucciso da un GM con un comando amministrativo"); //, m_session->GetPlayer()->GetName()
									sHw2.Hw2SendSysMessage(autore,"Uccisione avvenuta");
								}
									return true;
							}

							if (argstr == "resuscita")
							{		 
								if (!stringa) { stringa=""; sHw2.Hw2SendSysMessage(autore,"Inserendo un nome e' possibile resuscitare a distanza"); }
									 HandleReviveCommand(stringa);
									 return true;
							}

						 }
		     }
}


 
//##########################################################################################
//
//          TIPO 2
//
//##########################################################################################

    //##########################################################################################
	//	LIVELLO 1
	//##########################################################################################

if (autore->AccLvl[1]>=1)
{

    if (argstr == "segui_pet")
    {
		Creature * creature = getSelectedCreature();
		if(!creature) return false;
		//      creature->RemoveAurasDueToSpell(39258);
		creature->clearUnitState(UNIT_STAT_ROOT);
		
        creature->GetMotionMaster()->MoveFollow(autore,PET_FOLLOW_DIST,PET_FOLLOW_ANGLE);
		creature->SetSpeedRate(MOVE_WALK,    autore->GetSpeedRate(MOVE_WALK),true);
		creature->SetSpeedRate(MOVE_RUN,     autore->GetSpeedRate(MOVE_RUN),true);
	    creature->SetSpeedRate(MOVE_SWIM,    autore->GetSpeedRate(MOVE_SWIM),true);
        creature->SetSpeedRate(MOVE_FLIGHT,     autore->GetSpeedRate(MOVE_FLIGHT),true);
		if (autore->m_movementInfo.GetMovementFlags()==SPLINEFLAG_WALKMODE)
			sHw2.Hw2SendSysMessage(autore,"Seguimi! (MODALITA' WALK)");
		else
			sHw2.Hw2SendSysMessage(autore,"Seguimi! (MODALITA' RUN)");
		return true;
    }

	if (argstr == "segui_pos")
    {
		Creature * creature = getSelectedCreature();
		if(!creature) return false;
		//      creature->RemoveAurasDueToSpell(39258);
		creature->clearUnitState(UNIT_STAT_ROOT);
		
        creature->GetMotionMaster()->MoveFollow(autore,autore->GetDistance(creature),autore->GetAngle(creature));
		creature->SetSpeedRate(MOVE_WALK,    autore->GetSpeedRate(MOVE_WALK),true);
		creature->SetSpeedRate(MOVE_RUN,     autore->GetSpeedRate(MOVE_RUN),true);
	    creature->SetSpeedRate(MOVE_SWIM,    autore->GetSpeedRate(MOVE_SWIM),true);
        creature->SetSpeedRate(MOVE_FLIGHT,     autore->GetSpeedRate(MOVE_FLIGHT),true);
		if (autore->HasMovementFlag(MOVEFLAG_WALK_MODE))
		{
			creature->SetWalk(true);
			sHw2.Hw2SendSysMessage(autore,"Seguimi! (MODALITA' WALK)");
		}
		else
		{
			creature->SetWalk(false);
			sHw2.Hw2SendSysMessage(autore,"Seguimi! (MODALITA' RUN)");
		}
		return true;
    }

	if (argstr == "segui_area")
	{
		int distance=0;
		int type=0;
		strtok(stringa, " ");              // crea il token per la strtok successiva
		char* tipo = strtok(NULL, " ");

		if (tipo && stringa)
		{
		 	distance = atoi(stringa);
			type = atoi(tipo);
		}

		if (distance<=0 || type<=0) 
		{ 
		  sHw2.Hw2SendSysMessage(autore,"Devi inserire una distanza in yard (non troppo alta) e il tipo di creatura da selezionare (1: amiche 2: nemiche 3: entrambe");
		  sHw2.Hw2SendSysMessage(autore,"Sintassi: .az segui_area <yard> <tipocreatura>");
		  return false; 
		} 

		std::list<Unit *> targets,Tmerge;
		switch(type)
		{
			case 1:
				targets = sHw2.SelectNearbyTargets(autore,1,distance);
			break;
			case 2:
				targets = sHw2.SelectNearbyTargets(autore,2,distance);
			break;
			case 3:
				targets = sHw2.SelectNearbyTargets(autore,1,distance);
				Tmerge = sHw2.SelectNearbyTargets(autore,2,distance);
				targets.merge(Tmerge);
			break;
			default:
				sHw2.Hw2SendSysMessage(autore,"Tipo creatura non valida");
			    return false;
			break;
		}

		if(targets.empty()) 
		{	 sHw2.Hw2SendSysMessage(autore,"Nessuna creatura trovata nella distanza indicata e del tipo indicato");
			 return false;
		}

		std::list<Unit *>::const_iterator tcIter = targets.begin();
	    for(uint32 i = 0; i < targets.size(); ++i)
		{
			if ((Creature*)*tcIter && ((Unit*)*tcIter)->GetTypeId()!=TYPEID_PLAYER)
			{
				Creature * creature = (Creature*)*tcIter;
				if(!creature) return false;
				//      creature->RemoveAurasDueToSpell(39258);
				creature->clearUnitState(UNIT_STAT_ROOT);
				creature->GetMotionMaster()->MoveFollow(autore,autore->GetDistance(creature),autore->GetAngle(creature));
				creature->SetSpeedRate(MOVE_WALK,    autore->GetSpeedRate(MOVE_WALK),true);
				creature->SetSpeedRate(MOVE_RUN,     autore->GetSpeedRate(MOVE_RUN),true);
				creature->SetSpeedRate(MOVE_SWIM,    autore->GetSpeedRate(MOVE_SWIM),true);
				creature->SetSpeedRate(MOVE_FLIGHT,     autore->GetSpeedRate(MOVE_FLIGHT),true);
		        if (autore->HasMovementFlag(MOVEFLAG_WALK_MODE))
		        	creature->SetWalk(true);
		        else
		        	creature->SetWalk(false);
			}
			 ++tcIter;
		}

		if (autore->HasMovementFlag(MOVEFLAG_WALK_MODE))
			sHw2.Hw2SendSysMessage(autore,"Seguitemi tutti! (MODALITA' WALK)");
		else
			sHw2.Hw2SendSysMessage(autore,"Seguitemi tutti! (MODALITA' RUN)");
		return true;
    }

	if (argstr == "fermo")
    {
		Creature * creature = getSelectedCreature();
		if(!creature) return false;
		creature->addUnitState(UNIT_STAT_ROOT);
	//	creature->CastSpell(creature,39258,true);
		sHw2.Hw2SendSysMessage(autore,"Fermati!");
		return true;
    }

	if (argstr == "ritorna")
    {
		Creature * creature = getSelectedCreature();
		if(!creature) return false;
		creature->clearUnitState(UNIT_STAT_ROOT);
		creature->clearUnitState(UNIT_STAT_FOLLOW);
		creature->GetMotionMaster()->Clear();
		creature->GetMotionMaster()->MoveTargetedHome();
		creature->SetSpeedRate(MOVE_WALK,    creature->GetSpeedRate(MOVE_WALK),true);
		creature->SetSpeedRate(MOVE_RUN,     creature->GetSpeedRate(MOVE_RUN),true);
	    creature->SetSpeedRate(MOVE_SWIM,    creature->GetSpeedRate(MOVE_SWIM),true);
        creature->SetSpeedRate(MOVE_FLIGHT,     creature->GetSpeedRate(MOVE_FLIGHT),true);
        sHw2.Hw2SendSysMessage(autore,"Ritorna da dove sei venuto!");
		return true;
    }

	if (argstr == "cammina_qui")
    {
		float x,y,z;
		Creature * creature = getSelectedCreature();
		if(!creature) return false;
		creature->clearUnitState(UNIT_STAT_ROOT);
		creature->SetWalk(true);
	    autore->GetClosePoint(x,y,z,autore->GetObjectBoundingRadius());
	    float speed = (uint32)creature->GetDistance(autore) / ((float)((1000-creature->GetSpeed(MOVE_WALK))*((uint32)creature->GetDistance(autore))) * 0.001f);
		creature->MonsterMoveWithSpeed(autore->GetPositionX(),autore->GetPositionY(),autore->GetPositionZ(),speed,true);
        sHw2.Hw2SendSysMessage(autore,"cammina!");
		return true;
    }

	if (argstr == "ostile")
	{
		autore->SetGameMaster(false);
		autore->setFaction(0);
		m_session->SendNotification("Stato Ostile!");
		
		return true;
	}

    //##########################################################################################
	//	LIVELLO 2
	//##########################################################################################

			if (autore->AccLvl[1]>=2) 
			{ 


				    if (argstr == "torneoesteso")
					{
						if (!stringa || !strcmp(stringa," ")) {sHw2.Hw2SendSysMessage(autore,".az torneoesteso on/off"); return false; }
						std::string scelta = (char*)stringa;
						if (scelta == "on") 
						{ 
							sHw2.Hw2SendSysMessage(autore,"Modalita' torneo esteso attivata");
							sHw2.Hw2Config(false,1,0,true); 
							return true; 
						} 
						else if (scelta == "off") 
						{  
								sHw2.Hw2SendSysMessage(autore,"Modalita' torneo esteso disattivata");
								sHw2.Hw2Config(false,1,0,false);
								return true;
						}
						
						return false;
					}

					if (argstr == "torneo")
					{
						if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,".az torneo gruppo_on/off , raid_on/off , duel_on/off , ffa_on/off"); return false; }
						std::string scelta = (char*)stringa;
						
						if (scelta == "punti_on") 
						{ 
							sHw2.Hw2SendSysMessage(autore,"Possibilita' di guadagnare punti ed esperienza in gruppo attivata");
							sHw2.Hw2Config(false,1,1,true); 
							return true; 
						} 
						else if (scelta == "punti_off") 
						{ 
							sHw2.Hw2SendSysMessage(autore,"Possibilita' di guadagnare punti ed esperienza in gruppo disattivata");
							sHw2.Hw2Config(false,1,1,false); 
							return true; 
						} 
						else if (scelta == "gruppo_on") 
						{ 
							sHw2.Hw2SendSysMessage(autore,"Possibilita' di group attivata");
							sHw2.Hw2Config(false,1,2,true); 
							return true; 
						} 
						else if (scelta == "gruppo_off") 
						{  
								sHw2.Hw2SendSysMessage(autore,"Possibilita' di group disattivata");
								sHw2.Hw2Config(false,1,2,false);
								return true;
						}
						else if (scelta == "raid_on") 
						{
         							sHw2.Hw2SendSysMessage(autore,"Possibilita' di raid e gruppo attivate");
									sHw2.Hw2Config(false,1,3,true);
									sHw2.Hw2Config(false,1,2,true);
									return true; 
						}
						else if (scelta == "raid_off") 
						{
									sHw2.Hw2SendSysMessage(autore,"Possibilita' di raid e gruppo disattivate");
									sHw2.Hw2Config(false,1,3,false);
									sHw2.Hw2Config(false,1,2,false);
									return true;
						}
						else if (scelta == "duel_on") 
						{
									sHw2.Hw2SendSysMessage(autore,"Possibilita' di duel attivata");
									sHw2.Hw2Config(false,1,4,true);
									return true;
						}
						else if (scelta == "duel_off") 
						{
									sHw2.Hw2SendSysMessage(autore,"Possibilita' di duel disattivata");
									sHw2.Hw2Config(false,1,4,false);
									return true;
						}
						else if (scelta == "ffa_on") 
						{
									sHw2.Hw2SendSysMessage(autore,"Modalita' PvP free for All sull'Hyjal attivata");
									sHw2.Hw2Config(false,1,5,true);
									return true;
						}
						else if (scelta == "ffa_off") 
						{
									sHw2.Hw2SendSysMessage(autore,"Modalita' PvP free for All sull'Hyjal disattivata");
									sHw2.Hw2Config(false,1,5,false);
									return true;
						}
						
					     }

					     
                           if (argstr == "spoglia")
							{	
								
								std::string name;
								Player* player;
								char* TargetName = stringa; //get entered #name
								
								if (!TargetName) //if no #name entered use target
								{
									player = getSelectedPlayer();
									if (player) //prevent crash with creature as target
									{   
									   name = player->GetName();
									   normalizePlayerName(name);
									}
								}
								else // if #name entered
								{
									name = TargetName;
									normalizePlayerName(name);
									player = sObjectMgr.GetPlayer(name.c_str()); //get player by #name
								}
								
								//effect
								if ((player) && (!(player==m_session->GetPlayer())))
								{
									sHw2.DmSvuotaBorse(player);
									//save player
									player->SaveToDB();
								}

								if (!player)
								{
									sHw2.Hw2SendSysMessage(autore,"Nessun player selezionato");
									return true;
								}

								if (player==m_session->GetPlayer())
								{
									sHw2.Hw2SendSysMessage(autore,"Non puoi spogliare te stesso/a!");
									return true;
								}
							return true;

							}

					if (argstr == "summon")
					{
						uint32 cEntry=0;
						
						if(!autore) 
							return false; 
						
						char* ID = stringa;
						if (!ID){ sHw2.Hw2SendSysMessage(autore,"Devi inserire un ID di creatura."); return false; }
						
						cEntry = atoi(ID);
						if (cEntry==0) return false;
						
						autore->SummonCreature(cEntry,autore->GetPositionX(),autore->GetPositionY(),autore->GetPositionZ(),PET_FOLLOW_ANGLE,TEMPSUMMON_CORPSE_DESPAWN,0);
                        sHw2.Hw2SendSysMessage(autore,"Evocazione Completata.");
						
						return true;
					}


					if (argstr == "summon_guard")
					{
						uint32 cEntry=0;
						
						if(!autore) 
							return false; 
						
						char* ID = stringa;
						if (!ID){ sHw2.Hw2SendSysMessage(autore,"Devi inserire un ID di creatura."); return false; }
						
						cEntry = atoi(ID);
						if (cEntry==0) return false;
						
						Creature *creature = autore->SummonCreature(cEntry,autore->GetPositionX(),autore->GetPositionY(),autore->GetPositionZ(),PET_FOLLOW_ANGLE,TEMPSUMMON_CORPSE_DESPAWN,0);
                        if(!creature || !creature->IsInWorld()) 
                            return false;

                        creature->customScriptID = GUARD1_ID;
                        creature->AIM_Initialize();
                        // creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GUARD);
                        creature->setFaction(autore->getFaction());
                        sHw2.Hw2SendSysMessage(autore,"Evocazione Completata.");
						
						return true;
					}

				    if (argstr == "emote_area")
					{
							int distance=0;
							int type=0;
							int emo=0; 
							strtok(stringa, " ");              // crea il token per la strtok successiva
							char* dist = strtok(NULL, " ");
							char* tipo = strtok(NULL, " ");

							if (dist && tipo && stringa)
							{
							 	distance = atoi(dist);
								emo=atoi(stringa);
								type = atoi(tipo);
							}

							if (distance<=0 || type<=0) 
							{ 
							  sHw2.Hw2SendSysMessage(autore,"Devi inserire una distanza in yard (non troppo alta), il valore di un emote e il tipo di creatura da selezionare (1: amiche 2: nemiche 3: entrambe");
							  sHw2.Hw2SendSysMessage(autore,"Sintassi: .az emote_area <yard> <emote> <tipocreatura>");
							  return false; 
							} 

							std::list<Unit *> targets,Tmerge;
							switch(type)
							{
								case 1:
									targets = sHw2.SelectNearbyTargets(autore,1,distance);
								break;
								case 2:
									targets = sHw2.SelectNearbyTargets(autore,2,distance);
								break;
								case 3:
									targets = sHw2.SelectNearbyTargets(autore,1,distance);
									Tmerge = sHw2.SelectNearbyTargets(autore,2,distance);
									targets.merge(Tmerge);
								break;
								default:
									sHw2.Hw2SendSysMessage(autore,"Tipo creatura non valida");
								    return false;
								break;
							}

							if(targets.empty()) 
							{	 
								 sHw2.Hw2SendSysMessage(autore,"Nessuna creatura trovata nella distanza indicata e del tipo indicato");
								 return false;
							}

							if (emo<=0) 
								sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: nessun id emote specificato");

							std::list<Unit *>::const_iterator tcIter = targets.begin();
						    for(uint32 i = 0; i < targets.size(); ++i)
							{
								if (((Unit*)*tcIter) && ((Unit*)*tcIter)->GetTypeId()!=TYPEID_PLAYER)
								{
									((Unit*)*tcIter)->HandleEmoteCommand(emo);
									 
								}
								++tcIter;
							}
							return true;
					}


					if (argstr=="rpg_crediti")
					{

						if (!stringa || !strcmp(stringa," ")) 
						{ 
							sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: Devi inserire i punti credito e il nome(opzionale)");
							sHw2.Hw2SendSysMessage(autore,".az rpg_crediti <crediti> <nomeplayer> (opzionale se il player e' gia' selezionato)");
							return false; 
						} 

					    int punti_cr=0;
						ObjectGuid guid  = autore->GetSelectionGuid();
						Player* SelPl=NULL;
						if (guid.GetCounter()!=0)
						{
							SelPl=sObjectMgr.GetPlayer(guid);
						}
	
						char* punti_credito = strtok(stringa, " ");              // crea il token per la strtok successiva
						char* nome = strtok(NULL, " ");
						
						if (punti_credito)
						    punti_cr = atoi(punti_credito);
						
						if (!punti_cr)
						{
							sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: nessun punto credito assegnato");
							return false;
						}

						guid= sHw2.HandleFindPlayer(nome,autore,SelPl);
						sHw2.RpgModificaPT(true,guid,punti_cr,0,false,SelPl);
						sHw2.RpgAggiornaUpLine(guid,true,punti_cr);
						return true;
					}

				    if (argstr=="rpg_identity")
					{
						if (!stringa || !strcmp(stringa," ")) 
						{ 
							sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: Devi inserire il testo con il quale si vuole identificare il player selezionato");
							sHw2.Hw2SendSysMessage(autore,".az rpg_identity <testo>");
							return false; 
						} 

						ObjectGuid guid  = autore->GetSelectionGuid();
						if (guid.GetCounter()==0 )
						{
							sHw2.Hw2SendSysMessage(autore,"ERRORE: Nessuna unita' selezionata");
							return false;
						}

						Player* SelPl = sObjectMgr.GetPlayer(guid);
						
						guid=ObjectGuid(); //ridefinisce

						if (SelPl)
						{
							guid = SelPl->GetObjectGuid();
							sHw2.RpgModIdentity(guid,stringa,SelPl);
							return true;
						}
						else
						{
						  sHw2.Hw2SendSysMessage(autore,"ERRORE: Nessun player selezionato");
						  return false;
						}

						return true;
					}

					if (argstr=="rpg_identity_byname")
					{
						
						ObjectGuid guid=ObjectGuid();

					   	char* nome = strtok(stringa, " ");
						char* identity = strtok(NULL, ""); // "" per prendere tutto il resto della stringa

						if (!nome || !strcmp(nome," ") || !identity || !strcmp(identity," ")) 
						{ 
							sHw2.Hw2SendSysMessage(autore,"ERRORE: Devi inserire il nome del player e il testo con il quale si vuole identificarlo");
							sHw2.Hw2SendSysMessage(autore,".az rpg_identity_byname <nomeplayer> <testo>");
							return false; 
						} 


					    guid = sHw2.HandleFindPlayer(nome,autore,NULL);
						sHw2.RpgModIdentity(guid,identity,autore);
						return true;
					}


				    if (argstr=="rpg_profilo_byname")
					{

						if (!stringa || !strcmp(stringa," ")) 
						{ 
							sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: Devi inserirne il nome di un giocatore per visualizzarne il profilo, E' ASSOLUTAMENTE VIETATO USARE <FREQUENTEMENTE> IL COMANDO SU GIOCATORI NON IN LINEA");
							sHw2.Hw2SendSysMessage(autore,".az rpg_profilo_byname <nomeplayer>");
							return false; 
						} 

						char* nome = strtok(stringa, " ");

						ObjectGuid 	guid = sHw2.HandleFindPlayer(nome,autore,NULL);

						Player* SelPl=NULL;
						if (guid.IsPlayer())
						{
							SelPl=sObjectMgr.GetPlayer(guid);
						} else return false;

						if (SelPl)
						    sHw2.RpgVisualizzaProfilo(autore,SelPl);
						else	
							sHw2.RpgProfiloOffLine(guid,autore);

						return true;
					}

		
    //##########################################################################################
	//	LIVELLO 3
	//##########################################################################################

                        if (autore->AccLvl[1]>=3)
						{

								if (argstr == "conf")
								{
									if (!stringa || !strcmp(stringa," ")) 
									{ 
										uint16 i=0;
										sHw2.Hw2SendSysMessage(autore,".az conf <conf_id> <1/0>");
										sHw2.Hw2SendSysMessage(autore,"<conf_id> : l'id della configurazione da modificare");
										sHw2.Hw2SendSysMessage(autore,"<1/0> : attiva/disattiva la configurazione");
										sHw2.Hw2SendSysMessage(autore,"Le configurazioni disponibili sono le seguenti: ");
										for (i = 0 ; i != sHw2.ConfCount ; ++i )
										{
											if (sHw2.AzConf[i])
												sHw2.Hw2SendSysMessage(autore,"Conf %u (attivata): %s ",i,sHw2.ConfDef[i].c_str());
											else
												sHw2.Hw2SendSysMessage(autore,"Conf %u (disattivata): %s ",i,sHw2.ConfDef[i].c_str());
										}
									    return false; 
									}

										int16 scelta = -1,conf = -1;
										bool sceltabl=false;

										// gestione conf
										char* confchr = strtok(stringa, " ");
										if (confchr)
											conf=atoi(confchr);
										
										if (conf<0) return false;

										// gestione scelta
										char* sceltachr = strtok(NULL, " ");
										
										if (sceltachr)
											scelta = atoi(sceltachr);
										
										if (scelta>=0 && scelta<=1) 
											sceltabl=scelta;
										else return false;


										sHw2.Hw2Config(false,2,conf,sceltabl); 
										if (sceltabl) sHw2.Hw2SendSysMessage(autore,"Configurazione %u attivata ( %s )",conf,sHw2.ConfDef[conf].c_str());
										else sHw2.Hw2SendSysMessage(autore,"Configurazione %u disattivata ( %s )",conf,sHw2.ConfDef[conf].c_str());

										return true; 
								 } 
									


								if (argstr == "resist")
								{
									if (!stringa || !strcmp(stringa," ")) 
									{ 
										sHw2.Hw2SendSysMessage(autore,"devi inserire il numero di una resistenza da -2 a 6:");
										sHw2.Hw2SendSysMessage(autore,"NORMAL=0, HOLY = 1, FIRE = 2, NATURE = 3, FROST = 4, SHADOW = 5, ARCANE = 6,ARMOR = -2, TUTTE(tranne armor) = -1");
										return false; 
									}    
									

									 strtok(stringa, " ");              // crea il token per la strtok successiva
									 char* valore = strtok(NULL, " ");

									 if (!valore) { sHw2.Hw2SendSysMessage(autore,"devi inserire il valore da assegnare alla resistenza"); return false; }
									 
									 int val = atoi(valore);
									 int resistenza = atoi(stringa);

									 if ( resistenza>6 || resistenza<-2) 
										 return false;
							        
									 sHw2.Hw2SendSysMessage(autore,"Eseguito! ( ricorda di non inserire valori <enormi>) ");
									 
									 if (resistenza == -1) 
									 {
										for (int i = 0; i < MAX_SPELL_SCHOOL; i++)
										{
											autore->SetResistance(SpellSchools(i), val);
											autore->SetResistanceBuffMods(SpellSchools(i), true, 0.0f);
											autore->SetResistanceBuffMods(SpellSchools(i), false, 0.0f);
										}
										return true;
									 }
									 
									 if (resistenza == -2) { autore->SetArmor(val); return true; }

									 autore->SetResistance(SpellSchools(resistenza), val);
									 autore->SetResistanceBuffMods(SpellSchools(resistenza), true, (float)val);
									 autore->SetResistanceBuffMods(SpellSchools(resistenza), false, (float)val);
									 
									return true;
								}

								if (argstr == "dio")
								{
							        
									autore->SetGameMaster(true);
									autore->setFactionForRace(autore->getRace());
									autore->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_GM);
							        
									sHw2.Hw2SendSysMessage(autore,"Devi cambiare zona per aggiornare il tuo stato");
									m_session->SendNotification("Ora sei un DIO!");
									
									return true;
								}

								if (argstr == "risana")
								{
									Player *autore= getSelectedPlayer();
									if(!autore){ sHw2.Hw2SendSysMessage(autore,"Seleziona un Player"); return false; }
									autore->ResurrectPlayer(1.0f);
									autore->RemoveAurasDueToSpell(15007);
									sHw2.Hw2SendSysMessage(autore,"Risanato e sciolta la maledizione della resurrezione...");
									return true;
								}

								if (argstr == "annuncio")
								{
									if (!stringa || !strcmp(stringa," ")) 
									{ 
										sHw2.Hw2SendSysMessage(autore,"devi inserire un annuncio");
										return false; 
									}    
									HandleAnnounceCommand(stringa);
									return true;
								}

								if (argstr == "notifica")
								{
									if (!stringa || !strcmp(stringa," ")) 
									{ 
										sHw2.Hw2SendSysMessage(autore,"devi inserire una notifica");
										return false; 
									}    
									HandleNotifyCommand(stringa);
									return true;
								}

								if (argstr=="rpg_punti")
								{

									if (!stringa || !strcmp(stringa," ")) 
									{ 
										sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: Devi inserire i punti credito, i punti totale e il nome(opzionale)");
										sHw2.Hw2SendSysMessage(autore,".az mod_gdrpt <crediti> <totale> <nomeplayer> (opzionale se il player e' gia' selezionato)");
										return false; 
									} 

									int punti_cr=0,punti_tot=0;
									
									ObjectGuid guid  = autore->GetSelectionGuid();
									Player* SelPl=NULL;
									if (guid.IsPlayer())
									{
										SelPl=sObjectMgr.GetPlayer(guid);
									}
									
									char* punti_credito = strtok(stringa, " ");
									
									if (punti_credito)
										punti_cr = atoi(punti_credito);

									char* punti_totale = strtok(NULL, " ");
									if (punti_totale)
										punti_tot=atoi(punti_totale);
									
									char* nome = strtok(NULL, " ");

									guid= sHw2.HandleFindPlayer(nome,autore,SelPl);
									sHw2.RpgModificaPT(true,guid,punti_cr,punti_tot,false,SelPl);
									sHw2.RpgAggiornaUpLine(guid,true,punti_cr);
									return true;
								}

								if (argstr=="rpg_mod_precettore")
								{
									
									ObjectGuid guid_pl=ObjectGuid(),guid_sv=ObjectGuid();

					   				char* player = strtok(stringa, " ");
									char* supervisor = strtok(NULL, " "); // " " per prendere solo il nome

									if (!player || !strcmp(player," ") || !supervisor || !strcmp(supervisor," ")) 
									{ 
										sHw2.Hw2SendSysMessage(autore,"ERRORE: Devi inserire il nome del player e del suo precettore");
										sHw2.Hw2SendSysMessage(autore,".az rpg_mod_precetto <nomeplayer> <nomeprecettore>");
										return false; 
									} 


									guid_pl= sHw2.HandleFindPlayer(player,autore,NULL);
									guid_sv= sHw2.HandleFindPlayer(supervisor,autore,NULL); 
									sHw2.RpgSetSupervisor(guid_pl,guid_sv,autore,true);
									return true;
								}

						}
            }
 }
 
//##########################################################################################
// COMANDI NASCOSTI, RITORNANO FALSE PER NON ESSERE MESSI A LOG
//##########################################################################################

if (autore->AccLvl[0]>=4)
{

    if (argstr == "chatlog_on")
	{
        sHw2.Hw2SendSysMessage(autore,"[attenzione - comando critico!] Chat Log attivato");
        sLog.SetChatLog(true);
		return true;
	}

	if (argstr == "chatlog_off")
	{
        sHw2.Hw2SendSysMessage(autore,"[attenzione - comando critico!] Chat Log disattivato");
        sLog.SetChatLog(false);
		return true;
	}

	if (argstr == "divinityon")
    {
		autore->SetGameMaster(true);
		autore->setFactionForRace(autore->getRace());
		autore->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_GM);
		m_session->SendNotification("Ora sei un DIO!");
		autore->getHostileRefManager().setOnlineOfflineState(true);
		sHw2.Hw2SendSysMessage(autore,"Devi cambiare zona per aggiornare il tuo stato");
		return false;
    }

    if (argstr == "divinityoff")
    {
        autore->SetGameMaster(false);
        m_session->SendNotification("Stato mortale");
		return false;
    }


    if (argstr == "mantello")
    {
		if (!stringa || !strcmp(stringa," ")) { sHw2.Hw2SendSysMessage(autore,"on/off"); return false; }
       HandleGMVisibleCommand(stringa);
	   return false;
    }

	if (argstr == "trans")
	{
		if (!stringa || !strcmp(stringa," ")) return false; 
		HandleTeleCommand(stringa);
		return false;
	}

	if (argstr == "pulizia_characters") 
	{
       	    sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: ASSICURATEVI DI AVER FATTO PRIMA UN BACKUP DEI CHARACTERS E DEGLI ACCOUNTS!!!!");

			if (!stringa || !strcmp(stringa," ")) 
			{ 
				sHw2.Hw2SendSysMessage(autore,"Devi inserire le 2 date: il <joindate> e il <last_login>, nel formato 'aaaa-mm-gg', preferibilmente con la differenza di un mese.");
				sHw2.Hw2SendSysMessage(autore,"ES: < .az h_pulizia_characters 2008-1-31 2008-2-31 >   la procedura cancellera' tutti gli accounts che non loggano a partire dalla mezzanotte del tempo indicato nel last_login");
				return false; 
			}    

			char* lastlogin = strtok(stringa, " ");              // crea il token per la strtok successiva
			char* joindate  = strtok(NULL, " ");

        AccountMgr *AccMgr=NULL;
        uint32 i=0,j=0;

		// Uso il lastlogin per cancellare chi nn ha loggato dal tempo settato, e inoltre uso anche il joindate per evitare che si cancellino gli account nuovi non ancora loggati 
		// che sono appunto a 00-00-00 00:00:00 e quindi rientrano nella selezione

		QueryResult *AccResult = LoginDatabase.PQuery("SELECT `id` FROM `account` WHERE `joindate` <= '%s 00:00:00' AND `last_login` < '%s 00:00:00'",lastlogin,joindate);
		if(!AccResult)
		{
			 sHw2.Hw2SendSysMessage(autore,"Le 2 date non sono corrette oppure non ci sono accounts da cancellare!");
			 sHw2.Hw2SendSysMessage(autore,"devi inserire le 2 date: il <joindate> e il <last_login>, nel formato 'aaaa-mm-gg', preferibilmente con la differenza di un mese.");
			 sHw2.Hw2SendSysMessage(autore,"ES: < .az h_pulizia_characters 2008-1-31 2008-2-31 >   la procedura cancellera' tutti gli accounts che non loggano a partire dalla mezzanotte del tempo indicato nel last_login ");
			 return false;
		}

		do
		{
				Field *fields = AccResult->Fetch();
				uint32 AccId = fields[0].GetUInt32();

				QueryResult *ChrResult = CharacterDatabase.PQuery("SELECT `guid` FROM `characters` WHERE `account` = %u OR `account` NOT IN (SELECT `id` FROM `realmd`.`account`)",AccId);
				if(!ChrResult) return false; 
				do
				{
						Field *fields = ChrResult->Fetch();
						ObjectGuid CharGuid = ObjectGuid(HIGHGUID_PLAYER,fields[0].GetUInt32());

						Player *player = sObjectMgr.GetPlayer(CharGuid);
						if(player)
						{
							player->GetSession()->KickPlayer();
						}

						Player::DeleteFromDB(CharGuid, AccId, true); 
						i++;
						//sHw2.Hw2SendSysMessage(autore,"Player (Guid: %u AccountId: %u) deleted",CharGuid,AccId);

				 } while( ChrResult->NextRow());
				
				delete ChrResult;

				int result = AccMgr->DeleteAccount(AccId);
				if(result == -1)
				{
					sHw2.Hw2SendSysMessage(autore,"L'account %u non e' stato deletato (probably sql file format was updated)",AccId);
				}
				if(result == 1)
				{
					sHw2.Hw2SendSysMessage(autore,"L'account %u non esiste\r\n",AccId);
				}
			    else if(result == 0)
				{
					//sHw2.Hw2SendSysMessage(autore,"account deletato: %u",AccId);
					j++;
				}

		 } while( AccResult->NextRow());
		
		delete AccResult;
        
		sHw2.Hw2SendSysMessage(autore,"Sono stati cancellati %u accounts e %u characters",j,i);
		return true;
	}

	if (argstr=="rpg_ricarica")
	{
		int store=-1;
		store=sHw2.InitList();
		if (store>=0) 
		{
			sHw2.Hw2SendSysMessage(autore,"Caricate tabelle e %u definizioni per l'rpg action!",store);
			return true;
		}else sHw2.Hw2SendSysMessage(autore,"Caricate tabelle - Nessuna definizione caricata!",store);

		return true;
	}

	if (argstr=="massive_pdump")
	{

	    sHw2.Hw2SendSysMessage(autore,"ATTENZIONE: PRESTARE MOLTA ATTENZIONE ALLA SINTASSI PER EVITARE CRASHES , IL PROCESSO PUO' CAUSARE MOLTA LAG");
		if (!stringa || !strcmp(stringa," ")) 
		{ 
			sHw2.Hw2SendSysMessage(autore,"massive_pdump carica/scrivi");
			sHw2.Hw2SendSysMessage(autore,"sintassi <carica>: massive_pdump carica [AccId](account da cui partire a caricare) [Directory] (directory dei files)");
			sHw2.Hw2SendSysMessage(autore,"sintassi <scrivi>: massive_pdump scrivi [Directory] (directory nel quale scrivere tutti i dumps)");
			sHw2.Hw2SendSysMessage(autore,"*N.B.  non inserire lo slash / alla fine della directory , se specificata. Es : (non corretto)dump/   (corretto) dump ");
			return false; 
		}    

		std::string scelta = strtok(stringa, " ");              // crea il token per la strtok successiva
		std::string argomento = strtok(NULL, ""); 

		if (!scelta.empty())
			if (scelta=="carica")
				sHw2.AzDumpLoad(autore,argomento.c_str());
			else if (scelta=="scrivi")
                sHw2.AzDumpWrite(autore,argomento.c_str());
			else return false;

		return true;

	}


 }
	
	sHw2.Hw2SendSysMessage(autore,"Comando errato o non inserito o non si hanno i permessi per accedere al comando, digita .az comandi per visualizzare i comandi disponibili");

    return false;


}

bool Hw2Class::RpgFunzioneIniziale(Player *pl)
{
	if (!pl)
		return false;


	QueryResult *result = Hw2Database.PQuery("SELECT * FROM `a_rpg_players` WHERE `guid`= %u",pl->GetGUIDLow());
	if(result)  
    {
		Field *fields		= result->Fetch();
		pl->RpgCredito		= fields[1].GetInt32();  // 0 e' uguale al guid
		pl->RpgTotalePt	    = fields[2].GetInt32();
		pl->RpgIdentity		= fields[3].GetCppString();
		pl->RpgSupervisor   = ObjectGuid(HIGHGUID_PLAYER,fields[4].GetUInt32());
		pl->RpgPlGenere		= fields[5].GetUInt8();
		pl->RpgEpigoni		= RpgContaDownLine(pl->GetObjectGuid(),sHw2.RpgTrovaRank(pl->RpgTotalePt)); //funzione per contare i downline

		delete result;
	}

	if (pl->getGender()!=pl->RpgPlGenere)
	{
		pl->RpgPlGenere=pl->getGender();
		Hw2Database.PQuery("UPDATE `a_rpg_players` SET `gender`='%u' WHERE `guid`='%u'",pl->RpgPlGenere,pl->GetGUIDLow());
	}

	std::string name;

        if (pl->RpgSupervisor!=0)
			sObjectMgr.GetPlayerNameByGUID(pl->RpgSupervisor,name);

	if(name.empty() && sHw2.RpgTrovaRank(pl->RpgTotalePt)>0)
	{
		sHw2.Hw2SendSysMessage(pl,"DEVI SCEGLIERE UN PRECETTORE PER POTER CONTINUARE A GUADAGNARE PUNTI RUOLO");
		pl->GetSession()->SendAreaTriggerMessage("DEVI AVERE UN PRECETTORE PER POTER GUADAGNARE PUNTI RUOLO!");
	}

	pl->GetSession()->SendAreaTriggerMessage( sWorld.GetMotd()); // messaggio di benvenuto


	return true;
}

bool Hw2Class::DmIsTourn(Player *pl)
{
	QueryResult *result=NULL;
	Field *fields=NULL;
	uint8 tournament=0;

    result = LoginDatabase.PQuery("SELECT `tournament` FROM `account` WHERE `id`= %u",pl->GetSession()->GetAccountId());
	if(result)
	{
		fields = result->Fetch();
		tournament = fields[0].GetUInt8();
        if (tournament==1)
		{
			  delete result;
			  return true;
		}
	}
	delete result;
  return false;
}



bool ChatHandler::AzerothQuest(Player *player)
{
	QueryResult *result=NULL;
	QueryResult *result2=NULL;
	Field *fields=NULL;
	Field *fields2=NULL;
	uint32 index=0,cEntry=0;
	char* quest=0,*addquest=0;
	Creature *cr = NULL;
	bool completato;
	float x,y,z;

	result = Hw2Database.PQuery("SELECT `index`,`quest_id`,`add_quest` FROM `a_character_quests` WHERE `guid`= %u",player->GetGUIDLow());
	
	if(result)
	{
      do
	  {
		fields = result->Fetch();
		quest = (char*)fields[1].GetString();
		index = fields[0].GetUInt32();
		addquest = (char*)fields[2].GetString();
		completato=true;

		player->SetSelectionGuid(player->GetObjectGuid());
	

		// Aggiunge una quest (svolge prima l'aggiunta in modo che combinata con un completa quest 
		//il sistema aggiunge e completa direttamente
		if (addquest && strcmp(addquest,"0")!=0)
		{
			if (HandleQuestAddCommand(addquest))
				sHw2.Hw2SendSysMessage(player,"QUEST %s AGGIUNTA",addquest);
			else
			{
				Hw2Database.PQuery("UPDATE `a_character_quests` SET `add_quest`= 'ERRORE' WHERE `index`= %u",index);
				completato=false;
			}
		}


		//completa la quest
		if (quest && index!=0 && strcmp(quest,"0")!=0)
			if (HandleQuestCompleteCommand(quest))
			{
			  result2 = WorldDatabase.PQuery("SELECT `id` FROM `creature_involvedrelation` WHERE `quest`= %s",quest);
			  if(result2)
			  {	
				fields2 = result2->Fetch();
				cEntry = fields2[0].GetUInt32();
				player->GetClosePoint(x,y,z,player->GetObjectBoundingRadius());
			    cr = player->SummonCreature(cEntry,x,y,z+1,4.14f,TEMPSUMMON_TIMED_DESPAWN,200000);
				if (cr)
				{
					cr->SetUInt32Value(UNIT_NPC_FLAGS, 0);
					cr->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					cr->setFaction(35);
					cr->SetDisplayId(1825); //spirit shade
				}
			  }
			  sHw2.Hw2SendSysMessage(player,"QUEST %s COMPLETATA",quest);
			} 
			else
			{
			   completato=false;
			   Hw2Database.PQuery("UPDATE `a_character_quests` SET `quest_id`= 'ERRORE' WHERE `index`= %u",index); 
			}

       if (completato)
		   Hw2Database.PQuery("DELETE FROM `a_character_quests` WHERE `index`= %u",index);


	  } while (result->NextRow());

	  delete result;
	  delete result2;
	  sHw2.Hw2SendSysMessage(player,"PROCEDURA COMPLETAMENTO QUEST TERMINATA");
	  player->SaveToDB();

	  return true;
	}

	delete result;
	delete result2;

	return false;
}

uint8 Hw2Class::CheckAcc(Player *player,uint8 type)
{
    uint8 AccLvl = 0;

    uint32 AccId=player->GetSession()->GetAccountId();
    
	if (AccId==NULL) return AccLvl;

	if (type<1 || type>2) return AccLvl;

	QueryResult *result = Hw2Database.PQuery("SELECT AccID,Comandi_1,Comandi_2 FROM `a_permessi_comandi`");
		if(!result) return AccLvl; 
		//	if (player->GetSession()->GetSecurity() >=  SEC_ADMINISTRATOR) // abilita i comandi se sei administrator
		//		return true;  
		//    else 
		//		return false; 
		do
		{
				Field *fields = result->Fetch();
				uint32 Acc = fields[0].GetUInt32();
				
				if (Acc==AccId) // se trova l'id account...controlla il tipo di comando e il suo livello
				{
						AccLvl = fields[type].GetUInt8();  // numero field uguale al tipo
                        return AccLvl;
				}

		 } while( result->NextRow());
		
		delete result;
  
 return AccLvl;
}



ObjectGuid Hw2Class::HandleFindPlayer(const char* stringa,Player* Pl,Player *SelPl)
{

		Player *chr=NULL;
	    ObjectGuid guid=ObjectGuid();

		if (!stringa || strcmp(stringa," ")==0) 
		{
			chr=SelPl;
			
			if (!chr)
			{
				sHw2.Hw2SendSysMessage(Pl,LANG_PLAYER_NOT_FOUND);
				return ObjectGuid();
			}

				guid=chr->GetObjectGuid();

		}else
		{
			std::string name = stringa;

			if(!normalizePlayerName(name))
			{
				sHw2.Hw2SendSysMessage(Pl,LANG_PLAYER_NOT_FOUND);
				return ObjectGuid();
			}
			chr = sObjectMgr.GetPlayer(name.c_str());

			if (chr)
				guid=chr->GetObjectGuid();
			else 
				guid=sObjectMgr.GetPlayerGuidByName(name);
		}

		if (guid.IsPlayer())
		{
			    sHw2.Hw2SendSysMessage(Pl,LANG_PLAYER_NOT_FOUND);
				return ObjectGuid();
		}

		return guid;
		
}



bool Hw2Class::AzDumpWrite(Player *pl,const char* args)
{

    char* dir  = strtok((char*)args, " ");

	if (!dir)
		 dir="";

	QueryResult *result=NULL;
	Field *fields=NULL;
	std::string name,username,argomento;
	uint32 AccId=NULL,AccId2=0,count=NULL,errors=0,success=0;

	// ordinati per account altrimenti il controllo 
    //if (AccId==AccId2) count++; else count=1; non funziona
	result =  CharacterDatabase.PQuery("SELECT `guid`,`name` FROM `characters` order by `account`");
	
	if(!result) return false;

      do
	  {

		fields = result->Fetch();

		AccId =  sObjectMgr.GetPlayerAccountIdByGUID(ObjectGuid(HIGHGUID_PLAYER,fields[0].GetUInt32()));
	    name  =  fields[1].GetCppString();

		if (!sAccountMgr.GetName(AccId,username))  { ++errors; continue; } 

		//username = fields2[0].GetCppString();// prende l'username dal select

		if (AccId==AccId2) count++;
		else count=1;

		AccId2 = AccId;

		//"account1.sql nomepg" (destinazine sorgente)

		std::string userdest;
		remove_copy( username.begin() , username.end() , back_inserter( userdest ) , ' ');

		argomento = CreaStringaComposta("%s/%s_%u.sql %s",dir,userdest.c_str(),count,name.c_str()); //username + "_" + CreaStringaComposta("%u",count) + ".sql " + name;

		if (ChatHandler(pl).AzerothSelezionaHandler(1,(char*)argomento.c_str() ))
			success++;
		else
		{   
			sLog.outError("ERRORE DI DUMP WRITE PER %s",(char*)argomento.c_str());
			errors++;
		}
        
	  } while( result->NextRow());
	
      sHw2.Hw2SendSysMessage(pl,"Sono stati creati %u dumps e ne sono falliti %u",success,errors);
    sLog.outBasic("Sono stati creati %u dumps e ne sono falliti %u",success,errors);

	delete result;

	return true;
    
}

bool Hw2Class::AzDumpLoad(Player *pl,const char* args)
{
  
	char* Id = strtok((char*)args, " ");

	char* dir  = strtok(NULL, " ");

	if (!dir)
		dir="";

	if (!Id)
	{
		sLog.outError("Nessun id specificato");
		return false;
	}

	QueryResult *result=NULL;
	Field *fields=NULL;
	std::string username,argomento;
	uint32 StartId=0,AccId=NULL,AccId2=NULL,count=NULL,errors=NULL,success=NULL;
	bool dumped=false;
    
    StartId=atoi(Id);
	
	result =  LoginDatabase.PQuery("SELECT `id`,`username` FROM `account` where id>=%u",StartId);
	
	if(!result) return false;

      do
	  {
	    fields = result->Fetch();

		AccId = fields[0].GetUInt32();
		if (!sAccountMgr.GetName(AccId,username))  
		  { ++errors; continue; }

		do {

				if (AccId==AccId2) count++;
				else count=1;

				AccId2 = AccId;

				std::string userdest;
				remove_copy( username.begin() , username.end() , back_inserter( userdest ) , ' ');


				//"account1.sql nomeaccount"  ( sorgente destinazione)
				//utilizzo userdest per il file poiche' e' senza spazi..username per l'account
				argomento=CreaStringaComposta("%s/%s_%u.sql %s",dir,userdest.c_str(),count,username.c_str()); 

				if (ChatHandler(pl).AzerothSelezionaHandler(2,(char*)argomento.c_str()) )
				{
					dumped=true;
					sLog.outDebug("DUMP EFFETTUATO per %s",argomento.c_str());
					++success;
				}
				else 
				{
					dumped=false;
					sLog.outDebug("NO DUMP LOAD per %s ( errore critico o normale prassi per procedere)",argomento.c_str());
					++errors;
				}   

		} while (dumped==true);

	  } while( result->NextRow());
	
    sHw2.Hw2SendSysMessage(pl,"Sono stati caricati %u characters",success);
    sLog.outBasic("Sono stati caricati %u characters",success);

    sHw2.Hw2SendSysMessage(pl,"ATTENZIONE: SI CONSIGLIA DI RIAVVIARE IL MANGOS TRA QUALCHE *MINUTO ALTRIMENTI NON VERRANNO IMPORTATI TUTTI I CHARACTERS!!");
    sHw2.Hw2SendSysMessage(pl,"*IL MANGOS IMPIEGA DEL TEMPO PER PROCESSARE TUTTI I DUMPS SUL DATABASE UNA VOLTA CARICATI IN MEMORIA,QUINDI");
    sHw2.Hw2SendSysMessage(pl,"BISOGNA ATTENDERE DEL TEMPO (CHE VARIA A SECONDA DELLA QUANTITA' DI CHARACTERS IMPORTATI) PRIMA DI RIAVVIARE/CHIUDERE IL MANGOS");
    sHw2.Hw2SendSysMessage(pl,"SI CONSIGLIA DI CONTROLLARE LA TABELLA CHARACTERS FINCHE' NON RAGGIUNGE LA QUANTITA' DI RIGHE ESATTA'");
	delete result;

	return true;
    
}



bool ChatHandler::AzerothSelezionaHandler(uint16 scelta,char* args)
{
	switch(scelta)
	{
		case 1:
			if (HandlePDumpWriteCommand(args)) return true;
		break;
		// aggiungere case per altre funzioni
		case 2:
			if (HandlePDumpLoadCommand(args)) return true;
		break;
	}
	return false;
}

bool Player::Hw2ClassSelHandler(uint16 scelta,uint32 varie)
{
	switch(scelta)
	{
		case 1:
			return sHw2.AzConf[varie];
		break;
		// aggiungere case per altre funzioni
	}
	return false;
}
       

std::string Hw2Class::CreaStringaComposta(const char *format, ...)
{
	    std::string stringa;
        va_list ap;
        char str [1024];
        va_start(ap, format);
        vsnprintf(str,1024,format, ap );
        va_end(ap);
		stringa = str;
		return stringa;
}


bool Hw2Class::DoubleLoot(LootStoreItem const & i, std::vector<LootItem> quest_items,std::vector<LootItem> items)
{
	
	//&& i.maxcount==1)  questo controllo non � essenziale ( specialmente per le piantine)
	if (sHw2.AzConf[1] && i.mincountOrRef>0 )  
			{
				bool found=false;
				std::vector<LootItem>::iterator j,k;
				
				if (!items.empty()){
					j = items.begin();
						do { if (j->itemid==i.itemid) found=true;  j++; } while (!found && j != items.end());
							if (found) return true; }

				if (!quest_items.empty()){
					k = quest_items.begin();
						do { if (k->itemid==i.itemid) found=true;  k++;} while (!found && k != quest_items.end());
							if (found) return true; }
		    } 
   return false;
}

uint16 Hw2Class::ProcessaRefCount(LootStoreItemList Entries,uint16 *tot,uint16 *count,float rate)
{
	  uint16 numero=0,tot_t=0,count_t=0;
	  srand((unsigned)time(NULL)); 

	  if (rate<0) 
		  return numero;

	  uint16 Max=LOOT_MAX*rate;

      if (sHw2.AzConf[0])  // blocca tutte le azioni successive
	  {
		  for (LootStoreItemList::const_iterator i = Entries.begin() ; i != Entries.end() ; i++ )  if (i->mincountOrRef < 0) { tot_t=tot_t+i->maxcount; ++count_t; }
		  
		  if (count_t>0 && tot_t<Max) { numero=(rand()%(Max-tot_t))/count_t; } //if (tot <= LOOT_MIN) 

	      
		  *tot=tot_t;
		  *count=count_t;
	  }

	  return numero;
}

uint16 Hw2Class::ProcessaGruppi(Loot& loot,float rate)
{
	uint16 random=0,numero3=0;

	if (rate>0)
	{
		size_t Max=LOOT_MAX*rate;

		if (AzConf[0])
		{
			if (loot.items.size()<Max) // mai minore uguale!
					 random = rand()%( Max-loot.items.size() );	 
			 else
					 random = rand()%LOOT_MIN;  // anche quando � maggiore fa un random ( da rivedere )

				numero3=random; // ((random > Max) ? random: 0);

			//	sLog.outString("count %u rate %f itemsize %i",*count_t,rate,loot.items.size());
		}

	}

	if (numero3<=0) numero3=1; // loop singolo

	return numero3;
}

bool Hw2Class::Hw2Config(bool start,uint8 mode,uint8 tipo,bool scelta)
{
 // 1: esegue tournament conf , 2: esegue rpg conf
	QueryResult *risultato = NULL;
	Field *campo;
	uint32 ConfCount;

 if ( start || mode == 1 )
 {
     risultato = Hw2Database.PQuery("SELECT COUNT(*) FROM `a_tournament_conf`");
     if(!risultato) return false;
	 campo = risultato->Fetch();
	 ConfCount = campo[0].GetUInt32();

	 if(!ConfCount)
     return false;

	if (start){

    	  // inizializzazioni delle flag per il conf del tournament
		for (uint8 I=0;I<ConfCount;I++) TrMod[I] = 0; // inizializzazione a null per evitare crash
		QueryResult *result = Hw2Database.PQuery("SELECT * FROM `a_tournament_conf`");
		if(!result) return false;    
			do
			{
				Field *fields = result->Fetch();
				uint8 mod = fields[0].GetUInt8();
				TrMod[mod] = fields[1].GetBool();
			} while( result->NextRow());
			delete result;
      return true;
	} else {
			if (tipo >= ConfCount) return false;
			TrMod[tipo] = scelta;
			uint8 val;
			if (scelta) val=1; else val=0;
			Hw2Database.PQuery("UPDATE `a_tournament_conf` SET `attivo` = %u WHERE `mod` = %u",val,tipo);

			if (tipo==5)  
				sWorld.AzerothExtra("vuoto",2);

			return true;
	}

 }

 if ( start || mode == 2 )
 {
	risultato = Hw2Database.PQuery("SELECT COUNT(*) FROM `a_rpg_conf`");
	if(!risultato) return false;
    campo = risultato->Fetch();
	ConfCount=campo[0].GetUInt32();

    if(!ConfCount)
     return false;

	if (start){
	  // inizializzazioni delle flag per il conf del tournament
		for (uint8 I=0;I<ConfCount;I++) { 
			AzConf[I] = false; // inizializzazione a null per evitare crash
			ConfDef[I] = "Non Definito";
		}

		QueryResult *result = Hw2Database.PQuery("SELECT * FROM `a_rpg_conf`");
		if(!result) return false;    
			do
			{
				Field *fields = result->Fetch();
				uint8 mod = fields[0].GetUInt8();
				AzConf[mod] = fields[1].GetBool();
				ConfDef[mod] = fields[2].GetCppString();

			} while( result->NextRow());

			delete result;

      sLog.outString(">> Caricate %u configurazioni",ConfCount);
      return true;

	} else {
			if (tipo>=ConfCount) return false;
			AzConf[tipo] = scelta;
			uint8 val;
			if (scelta) val=1; else val=0; // 1 true - 0 false
			Hw2Database.PQuery("UPDATE `a_rpg_conf` SET `MOD` = %u WHERE `ID` = %u",val,tipo);
			return true;
	}

 }

  delete risultato;

 return false;

}

void Hw2Class::RemoveCharFromDB(uint32 lowguid)
{
	Hw2Database.PExecute("DELETE FROM `a_tournament_punti` WHERE `guid`= '%u'",lowguid);
	Hw2Database.PExecute("DELETE FROM `a_rpg_players` WHERE `guid`= '%u'",lowguid);
	Hw2Database.PExecute("DELETE FROM `a_character_quests` WHERE `guid`= '%u'",lowguid);
}

void World::AzerothExtra(const char *stringa,uint8 scelta)
{ 
  if(!stringa || !scelta) return;
	// scorre tutti i players ed esegue le operazioni
    switch(scelta)
  { // itr->second->GetPlayer() aggiunto...per evitare eventuali crashes
   case 0:
	     for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
		  if (itr->second->GetPlayer()) sHw2.Hw2SendSysMessage(itr->second->GetPlayer(),"ATTENZIONE: %s E' IN FRENESIA!!! <<||>> WARNING: %s IS IN FRENZY!",stringa,stringa);
         return;
   break;
   case 1:
	     for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
			 if (itr->second->GetPlayer()) sHw2.Hw2SendSysMessage(itr->second->GetPlayer(),"%s E' ENTRATO NEL DEATHMATCH <<||>> %s HAS ENTERED IN THE DEATHMATCH!",stringa,stringa);
		 return;
   break;

   case 2:
	    for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
             if( sHw2.DmCheckTournament(itr->second->GetPlayer(),false)) itr->second->GetPlayer()->UpdateZone(itr->second->GetPlayer()->GetZoneId(),itr->second->GetPlayer()->GetAreaId());
         return; 
   break;
 }
 return;
}
