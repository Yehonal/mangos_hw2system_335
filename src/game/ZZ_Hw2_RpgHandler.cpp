#include "ZZ_ScriptsPersonali.h"

int dldelete(struct Hw2Class::act **start) {
    if (sHw2.ListFilled && *start) {
        struct Hw2Class::act *n = *start;
        //*start = (*start)->next;
        free(n);
    }

    return 0; /* niente da cancellare */
}

int Hw2Class::dlstore(struct act **start) {

    QueryResult *result = NULL;
    Field *fields = NULL;


    //INIZIO RIEMPIMENTO ARRAY RANGHI GDR
    result = Hw2Database.PQuery("SELECT punteggio,titolo_rank_m,titolo_rank_f FROM `a_rpg_ranks`");
    if (!result) {
        sLog.outError("ERRORE: LA TABELLA DEI RANKS CONTIENE ERRORI O E' VUOTA");
    } else {
        uint16 count = 0;
        do {
            fields = result->Fetch();
            cl[count].grado = fields[0].GetInt32();
            cl[count].Titolo[0] = new std::string();
            cl[count].Titolo[0]->append(fields[1].GetCppString());
            cl[count].Titolo[1] = new std::string();
            cl[count].Titolo[1]->append(fields[2].GetCppString());
            ++count;
        } while (result->NextRow() && count <= STANDARD);

        cl_count = count;

        sLog.outString();
        sLog.outString(">> Caricati %u Ranghi per la classifica RPG", count);

        delete result;
    }

    // FINE RIEMPIMENTO

    result = NULL;
    fields = NULL;

    // INIZIO RIEMPIMENTO LINKED LIST RPG ACTION

    result = Hw2Database.PQuery("SELECT * FROM `a_azeroth_rpgaction`");
    if (!result) {
        sLog.outError("ERRORE: LA TABELLA DEI TESTI NPC E' VUOTA O PRESENTA ERRORI , LA LISTA NON SARA' MODIFICATA");
        return -1; /* not match */
    }

    count_list = 0;

    struct act *n;

    dldelete(start); // SVUOTA LA LinkedList

    do {
        n = (struct act *) malloc(sizeof (struct act));
        if (!n) return -1; /* Out of Memory */

        fields = result->Fetch();
        // la field 0 e' l'index
        n->indice = fields[0].GetUInt32();
        n->flag = fields[1].GetUInt16();
        n->loc_type = fields[2].GetUInt8();
        n->loc_id = fields[3].GetUInt32();
        n->min_rep_rank = fields[4].GetUInt16();
        n->max_rep_rank = fields[5].GetUInt16();

        n->speak_type = fields[6].GetUInt8();
        n->text_male = new std::string();
        n->text_male->append(fields[7].GetCppString());
        n->text_female = new std::string();
        n->text_female->append(fields[8].GetCppString());
        n->txt_emote_m = new std::string();
        n->txt_emote_m->append(fields[9].GetCppString());
        n->txt_emote_f = new std::string();
        n->txt_emote_f->append(fields[10].GetCppString());
        n->language = fields[11].GetUInt16();
        n->emote_id_1 = fields[12].GetUInt16();
        n->emote_id_2 = fields[13].GetUInt16();

        n->chance = fields[14].GetFloat();

        if (*start == NULL) { /* vuoto */
            *start = n;
        } else { /* non vuoto */
            n->next = *start;
            *start = n;
        }

        ++count_list;

    } while (result->NextRow());

    if (count_list > 0) ListFilled = true;

    sLog.outString(">> Caricate %u definizioni per l'action RPG", count_list);

    //INIZIO CARICAMENTO DELLE CONFIGURAZIONI
    //Hw2Config(true);

    sLog.outString();

    return count_list; /* allocazione riuscita */
}

int Hw2Class::dlprocess(uint16 flag, Player *pl, Creature *cr, struct act *start) {

    if (!start || !pl || !cr) {
        sLog.outError("ERRORE: dlprocess No.1, argomenti mancanti");
        return 70;
    }

    if (rand() % 100 < 50) // 50% di possibilit� di interagire
        return 70;

    ReputationRank rank;
    uint8 gender = 70;
    uint32 map = 0, zone = 0, area = 0, count = 1;
    const char * testo;
    const char * testo_emote;
    uint32 lingua = NULL;

    rank = pl->GetReputationRank(cr->getFactionTemplateEntry()->faction);
    gender = pl->getGender();
    map = pl->GetMapId();
    zone = pl->GetZoneId();
    area = pl->GetAreaId();

    sLog.outDebug("Interazione avviata, Informazioni player: %u %u %u %u", gender, map, zone, area);

    struct act *Ls_act = start;

    while (Ls_act && count <= count_list) {

        sLog.outDebug("scorrimento while count %u su %u", count, count_list);

        if (Ls_act->flag == flag && rank >= Ls_act->min_rep_rank && rank <= Ls_act->max_rep_rank)
            if ((Ls_act->loc_type == 1 && Ls_act->loc_id == map) || (Ls_act->loc_type == 2 && Ls_act->loc_id == zone) || (Ls_act->loc_type == 3 && Ls_act->loc_id == area))
                if (rand() % 100 <= Ls_act->chance) {
                    sLog.outDebug("Cella raccolta");
                    lingua = LANG_UNIVERSAL;
                    LanguageDesc const* langDesc = GetLanguageDescByID(Ls_act->language);

                    if (langDesc && pl->HasSkill(langDesc->skill_id))
                        lingua = Ls_act->language;

                    if (cr->GetDefaultMovementType() == WAYPOINT_MOTION_TYPE && rand() % 100 <= 80) {
                        sLog.outDebug("Avviata esecuzione movimento");
                        pl->GetClosePoint(x, y, z, pl->GetObjectBoundingRadius());
                        float speed = (uint32) cr->GetDistance(pl) / ((float) ((1000)*((uint32) cr->GetDistance(pl))) * 0.001f);
                        cr->MonsterMoveWithSpeed(pl->GetPositionX(), pl->GetPositionY(), pl->GetPositionZ(), speed, true);
                        cr->walk_timer = 10000;
                    }


                    if (gender == GENDER_FEMALE) {
                        testo_emote = Ls_act->txt_emote_f->c_str();
                        testo = Ls_act->text_female->c_str();
                    } else {
                        testo_emote = Ls_act->txt_emote_m->c_str();
                        testo = Ls_act->text_male->c_str();
                    }

                    sLog.outDebug("Informazioni cella: %s %s %u %u", testo, testo_emote, lingua, rank);

                    if (testo && strcmp(testo, "") != 0) //previene testo vuoto
                        if (Ls_act->speak_type == 2)
                            cr->MonsterYell(testo, lingua, pl);
                        else
                            cr->MonsterSay(testo, lingua, pl);

                    if (testo_emote && strcmp(testo_emote, "") != 0) //previene testo vuoto
                        cr->MonsterTextEmote(testo_emote, pl, false);

                    if (rand() % 2 == 0 || Ls_act->emote_id_2 == 0)
                        cr->HandleEmoteCommand(Ls_act->emote_id_1);
                    else
                        cr->HandleEmoteCommand(Ls_act->emote_id_2);

                    return 0; // match, ritorna senza continuare 
                }

        if (count < count_list) {
            Ls_act = Ls_act->next;
            sLog.outDebug("Avanzamento Lista");
        }

        ++count;
    }


    return 70; // no match 
}

Player* Hw2Class::AzerothSelectNearbyFrTarget(Unit *unit) const {
    if (!unit)
        return NULL;

    Player* pl = NULL;

    {
        float radius = 20.0f;
        MaNGOS::AnyPlayerInObjectRangeCheck u_check(unit, radius);
        MaNGOS::PlayerSearcher<MaNGOS::AnyPlayerInObjectRangeCheck> searcher(pl, u_check);

        Cell::VisitWorldObjects(unit, searcher, radius);
    }

    if (pl && pl->IsFriendlyTo(unit))
        return pl;
    else
        return NULL;
}

std::list<Unit *> Hw2Class::SelectNearbyTargets(Unit* unit, uint8 tipo, float distanza) const {

    std::list<Unit *> targets;
    targets.clear();

    if (!unit)
        return targets;

    // tipo1: friendly tipo2: unfriendly
    if (tipo == 1) {
        MaNGOS::AnyFriendlyUnitInObjectRangeCheck u_check(unit, distanza);
        MaNGOS::UnitListSearcher<MaNGOS::AnyFriendlyUnitInObjectRangeCheck> searcher(targets, u_check);

        Cell::VisitAllObjects(unit, searcher, distanza);
    } else if (tipo == 2) {
        MaNGOS::AnyFriendlyUnitInObjectRangeCheck u_check(unit, distanza);
        MaNGOS::UnitListSearcher<MaNGOS::AnyFriendlyUnitInObjectRangeCheck> searcher(targets, u_check);

        Cell::VisitAllObjects(unit, searcher, distanza);
    }


    // remove not LoS targets
    for (std::list<Unit *>::iterator tIter = targets.begin(); tIter != targets.end();) {
        if (!unit->IsWithinLOSInMap(*tIter) || ((Unit*) * tIter)->GetTypeId() == TYPEID_PLAYER) {
            std::list<Unit *>::iterator tIter2 = tIter;
            ++tIter;
            targets.erase(tIter2);
        } else
            ++tIter;
    }

    // no appropriate targets
    //if(targets.empty())
    return targets;

}

void Creature::AzerothRpgFunctions(uint8 tipo, uint32 diff) {

    Player *target = NULL;
    int parlato = 70; // un numero qualsiasi tranne che 0

    target = sObjectMgr.GetPlayer(saved_target);
    if (walk_timer != 0)
        if (walk_timer < diff) {
            walk_timer = 0;
        } else {
            if (target && GetDistance(target) <= 1.5f) {
                StopMoving();
                target->TalkedToCreature(GetEntry(), GetObjectGuid());
                target->PrepareGossipMenu(this);
                target->SendPreparedGossip(this);
                walk_timer = 0;
                return;
            }
        }


    switch (tipo) {

        case 1:

            if (!isServiceProvider())
                return;

            if (react_timer < diff) {
                react_timer = 0;
                if (isInCombat()) // ritorna se e' in combattimento
                    return;

                target = (Player*) sHw2.AzerothSelectNearbyFrTarget(this);

                if (!target || target->GetObjectGuid() == saved_target)
                    return;

                if (isInFrontInMap(target, ATTACK_DISTANCE + 2.0f, 4)) {

                    if (IsGuard()) {
                        parlato = sHw2.ProcessList(1, target, this);
                        if (parlato == 0) react_timer = 50000;
                    }
                    else if (isQuestGiver()) {
                        parlato = sHw2.ProcessList(3, target, this);
                        if (parlato == 0) react_timer = 150000;
                    }
                    else if (isArmorer()) {
                        parlato = sHw2.ProcessList(4, target, this);
                        if (parlato == 0) react_timer = 70000;
                    }
                    else if (isBanker()) {
                        parlato = sHw2.ProcessList(5, target, this);
                        if (parlato == 0) react_timer = 50000;
                    }
                    else if (isBattleMaster()) {
                        parlato = sHw2.ProcessList(7, target, this);
                        if (parlato == 0) react_timer = 120000;
                    }
                    else if (isInnkeeper()) {
                        parlato = sHw2.ProcessList(8, target, this);
                        if (parlato == 0) react_timer = 70000;
                    }
                    else if (isTrainer()) {
                        parlato = sHw2.ProcessList(9, target, this);
                        if (parlato == 0) react_timer = 120000;
                    }
                    else if (isAuctioner()) {
                        parlato = sHw2.ProcessList(10, target, this);
                        if (parlato == 0) react_timer = 80000;
                    }
                    else if (isTaxi()) {
                        parlato = sHw2.ProcessList(11, target, this);
                        if (parlato == 0) react_timer = 120000;
                    } else if (isVendor()) {
                        parlato = sHw2.ProcessList(2, target, this);
                        if (parlato == 0) react_timer = 110000;
                    }
                    else if (IsCivilian()) {
                        parlato = sHw2.ProcessList(6, target, this);
                        if (parlato == 0) react_timer = 120000;
                    }

                    // if ( parlato==0) 
                    saved_target = target->GetObjectGuid(); //salva anche se non ha parlato , cosi' evita di salutare dopo tanto tempo
                }

            } else react_timer -= diff;

            break;
    }

    return;
}

bool Hw2Class::RpgGestioneEmote(Player *pl, uint32 emote) {
    Creature *cr = NULL;
    // AZEROTH EMOTE PER NPC 
    ObjectGuid guid = pl->GetSelectionGuid();
    if (guid.IsCreature()) {
        cr = pl->GetMap()->GetAnyTypeCreature(guid);
        if (cr && cr->IsFriendlyTo(pl) && pl->isInFrontInMap(cr, 20.0f, 4) && cr->isInFrontInMap(pl, 20.0f, 4)) {
            if (emote == EMOTE_ONESHOT_TALK || emote == EMOTE_ONESHOT_BOW || emote == EMOTE_ONESHOT_SALUTE || emote == EMOTE_ONESHOT_SALUTE || emote == EMOTE_ONESHOT_WAVE)
                cr->HandleEmoteCommand(emote);
                // 10% delle possibilita' di effettuare altri tipi di emote escludendo il dance.
            else if (rand() % 100 < 25 && emote != EMOTE_STATE_DANCE)
                cr->HandleEmoteCommand(emote);

            return true;
        }
    }
    return false;
}

bool Hw2Class::RpgModificaPT(bool salva, ObjectGuid guid, int32 credito, int32 totale, bool indirect_pt, Player *Modder) {
    if (!guid)
        return false;

    Player* pl = NULL;
    pl = sObjectMgr.GetPlayer(guid); // recupera il player dal guid

    if (salva) {

        QueryResult *result = Hw2Database.PQuery("SELECT * FROM `a_rpg_players` WHERE `guid`= %u", guid.GetCounter());
        if (!result) {
            uint8 sesso = 0;

            if (pl) {


                pl->RpgCredito = credito;

                if (totale != 0) // se la variabile totale e' diversa da zero , allora bisogna modificare il totale 
                    pl->RpgTotalePt = totale;
                else if (credito > 0) // se il credito viene speso..non modifica il totale
                    pl->RpgTotalePt = credito;

                sesso = pl->getGender();

            }

            Hw2Database.PQuery("INSERT INTO `a_rpg_players` VALUES ('%u','%d','%d','Non definita','0','%u')", guid.GetCounter(), credito, credito, sesso);
        }

        if (pl) {

            if (pl->RpgSupervisor == 0 && pl->RpgTotalePt >= cl[1].grado) // se non ha il supervisore dopo il primo lvl , ritorna
            {
                ChatHandler(pl).PSendSysMessage("Non puoi guadagnare altri punti se non scegli prima un precettore!");

                if (Modder)
                    ChatHandler(Modder).PSendSysMessage("Il player indicato, ora, deve prima scegliere un precettore", guid.GetCounter());
                return false;
            }

            pl->RpgCredito += credito;

            if (totale != 0) // se la variabile totale e' diversa da zero , allora bisogna modificare il totale 
                pl->RpgTotalePt += totale;
            else if (credito > 0 && pl->RpgCredito > 0) // se il credito viene speso..non modifica il totale
                pl->RpgTotalePt += credito;

            Hw2Database.PQuery("UPDATE `a_rpg_players` SET `credito_rpg` ='%d' , `totale_rpg` ='%d' WHERE `guid`='%u'", pl->RpgCredito, pl->RpgTotalePt, guid.GetCounter());
            delete result;

        } else {

            Field *fields = result->Fetch();

            int32 totals = 0, credits = 0;
            credits = fields[1].GetInt32();
            totals = fields[2].GetInt32();

            if (fields[4].GetUInt64() == 0 && totals >= cl[1].grado) // se non ha il supervisore dopo il primo lvl , ritorna
            {
                if (Modder)
                    ChatHandler(Modder).PSendSysMessage("Il player indicato, ora, deve prima scegliere un precettore", guid.GetCounter());
                return false;
            }

            credits += credito;


            if (totale != 0) // se la variabile totale e' diversa da zero , allora bisogna modificare il totale 
                totals += totale;
            else if (credito > 0 && credits > 0) // se il credito viene speso ,cioe' <0, o il credito e' negativo ..non modifica il totale 
                totals += credito;

            Hw2Database.PQuery("UPDATE `a_rpg_players` SET `credito_rpg` ='%d' , `totale_rpg` ='%d' WHERE `guid`='%u'", credits, totals, guid.GetCounter());
            delete result;

        }

    } else if (pl) { //modalita' modifica punti senza salvataggio

        if (pl->RpgSupervisor == 0 && pl->RpgTotalePt >= cl[1].grado) // se non ha il supervisore dopo il primo lvl , ritorna
        {
            ChatHandler(pl).PSendSysMessage("Non puoi guadagnare altri punti se non scegli prima un precettore!");

            if (Modder)
                ChatHandler(Modder).PSendSysMessage("Il player indicato, ora, deve prima scegliere un precettore", guid.GetCounter());
            return false;
        }

        pl->RpgCredito += credito;

        if (totale != 0) // se la variabile totale e' diversa da zero , allora bisogna modificare il totale 
            pl->RpgTotalePt += totale;
        else if (credito > 0 && pl->RpgCredito > 0) // se il credito viene speso..non modifica il totale
            pl->RpgTotalePt += credito;
    }


    if (pl && credito != 0) {
        if (!indirect_pt)
            ChatHandler(pl).PSendSysMessage("Hai guadagnato %d punti credito gdr!", credito);
        else
            ChatHandler(pl).PSendSysMessage("Hai guadagnato %d punti credito gdr dalla percentuale di uno dei tuoi epigoni!", credito);
    }

    if (Modder) // nel caso sia un modder ad eseguira la procedura..gli sara' segnalato il feedback
        ChatHandler(Modder).PSendSysMessage("I punti gdr del player %u sono stati modificati", guid.GetCounter());

    return true;

}

bool Hw2Class::RpgModIdentity(ObjectGuid guid, char* stringa, Player *Modder) {

    if (!guid)
        return false;

    Player* pl = sObjectMgr.GetPlayer(guid); // recupera il player dal guid

    QueryResult *result = Hw2Database.PQuery("SELECT * FROM `a_rpg_players` WHERE `guid`= %u", guid);
    if (!result) {
        uint8 sesso = 0;

        if (pl) {
            sesso = pl->getGender();
            pl->RpgIdentity = stringa;
        }


        Hw2Database.PQuery("INSERT INTO `a_rpg_players` VALUES ('%u','0','0','%s','0','%u')", guid.GetCounter(), stringa, 0, sesso);
    }
    else {

        if (pl)
            pl->RpgIdentity = stringa;

        Hw2Database.PQuery("UPDATE `a_rpg_players` SET `identity`='%s' WHERE `guid`='%u'", stringa, guid.GetCounter());
        delete result;
    }

    if (Modder)
        ChatHandler(Modder).PSendSysMessage("L'identita' del player %u e' stata modificata in: %s", guid.GetCounter(), stringa);

    return true;
}

bool Hw2Class::RpgSetSupervisor(ObjectGuid guid, ObjectGuid supervisor, Player *Modder, bool Force) {

    if (!guid || guid == supervisor || !supervisor) //non esegue nulla sotto queste condizioni
        return false;


    Player* pl = sObjectMgr.GetPlayer(guid); // recupera il player dal guid

    QueryResult *rtemp = Hw2Database.PQuery("SELECT supervisor,totale_rpg FROM `a_rpg_players` WHERE `guid`= %u", supervisor.GetCounter());
    if (!rtemp) // tutti i supervisori devono essere associati ad un amministratore/GM
    {
        if (pl)
            ChatHandler(pl).PSendSysMessage("Non puoi scegliere come precettore un accolito");
        return false;
    }

    Field *ftemp = rtemp->Fetch();
    uint64 stemp = 0;
    int32 ttemp = 0;
    stemp = ftemp[0].GetUInt32();
    ttemp = ftemp[1].GetInt32();
    delete rtemp;

    if (pl->RpgSupervisor.GetCounter() == stemp || sHw2.RpgTrovaRank(ttemp) < 1) // non puoi scegliere come supervisore un tuo supervisore o qualcuno di livello 0
    {
        if (pl)
            ChatHandler(pl).PSendSysMessage("Non puoi scegliere come precettore un accolito, o il tuo stesso supervisore");
        return false;
    }

    QueryResult *result = Hw2Database.PQuery("SELECT supervisor FROM `a_rpg_players` WHERE `guid`= %u", guid.GetCounter());
    if (!result) {
        uint8 sesso = 0;

        if (pl) {
            sesso = pl->getGender();
            pl->RpgSupervisor = supervisor;
        }

        Hw2Database.PQuery("INSERT INTO `a_rpg_players` VALUES ('%u','0','0','Non definita','%u','%u')", guid.GetCounter(), supervisor.GetCounter(), sesso);
    }
    else {

        Field *fields = result->Fetch();
        uint32 temp = 0;
        temp = fields[0].GetUInt32();

        delete result;

        if (temp != 0 && !Force) // se l'attributo supervisore non e' 0 e se non e' stata forzata la modifica , ritorna
            return true;

        if (pl)
            pl->RpgSupervisor = supervisor;


        Hw2Database.PQuery("UPDATE `a_rpg_players` SET `supervisor`='%u' WHERE `guid`='%u'", supervisor.GetCounter(), guid.GetCounter());

    }

    std::string name = "Non definito";

    if (pl->RpgSupervisor != 0)
        sObjectMgr.GetPlayerNameByGUID(supervisor, name);


    if (Modder) {
        ChatHandler(Modder).PSendSysMessage("Ti e' stato appena assegnato il precettore %s", name.c_str());
        sHw2.RpgAggiornaUpLine(Modder->GetObjectGuid());
    }


    return true;
}

bool Hw2Class::RpgVisualizzaProfilo(Player* pl, Player *selected) {
    if (!pl || !selected) return false;


    std::string name = "Non definito";

    if (selected->RpgSupervisor != 0)
        sObjectMgr.GetPlayerNameByGUID(selected->RpgSupervisor, name);

    ChatHandler(pl).PSendSysMessage("o================== PROFILO GDR =======================");
    ChatHandler(pl).PSendSysMessage("|   [Player]: %s", selected->GetName());
    ChatHandler(pl).PSendSysMessage("|   [Crediti]: %d [Totale Accumulato]: %d", selected->RpgCredito, selected->RpgTotalePt);
    ChatHandler(pl).PSendSysMessage("|   [Grado]: %s", cl[sHw2.RpgTrovaRank(selected->RpgTotalePt)].Titolo[selected->getGender()]->c_str());
    ChatHandler(pl).PSendSysMessage("|   [Precettore/rice]: %s   [Tot Epigoni]: %u", name.c_str(), selected->RpgEpigoni);
    ChatHandler(pl).PSendSysMessage("|   [Identita']: %s", selected->RpgIdentity.c_str());
    ChatHandler(pl).PSendSysMessage("o==============================================================");

    return true;
}

bool Hw2Class::RpgProfiloOffLine(ObjectGuid guid, Player *Modder) {
    if (!guid)
        return false;

    QueryResult *result = Hw2Database.PQuery("SELECT * FROM `a_rpg_players` WHERE `guid`= %u", guid);
    if (result) {
        Field *fields = result->Fetch();
        int32 RpgCredito = fields[1].GetInt32(); // 0 e' uguale al guid
        int32 RpgTotalePt = fields[2].GetInt32();
        std::string RpgIdentity = fields[3].GetCppString();
        uint32 RpgSupervisor = fields[4].GetUInt32();
        uint8 RpgPlGender = fields[5].GetUInt8();
        int32 RpgEpigoni = sHw2.RpgContaDownLine(guid, sHw2.RpgTrovaRank(RpgTotalePt)); //funzione per contare i downline

        delete result;

        std::string name = "Non definito", name2 = "Non trovato";

        if (RpgSupervisor != 0)
            sObjectMgr.GetPlayerNameByGUID(ObjectGuid(HIGHGUID_PLAYER, RpgSupervisor), name);

        sObjectMgr.GetPlayerNameByGUID(guid, name2);

        ChatHandler(Modder).PSendSysMessage("o================== PROFILO GDR =======================");
        ChatHandler(Modder).PSendSysMessage("|   [Player]: %s", name2.c_str());
        ChatHandler(Modder).PSendSysMessage("|   [Crediti]: %d [Totale Accumulato]: %d", RpgCredito, RpgTotalePt);
        ChatHandler(Modder).PSendSysMessage("|   [Grado]: %s", cl[sHw2.RpgTrovaRank(RpgTotalePt)].Titolo[RpgPlGender]->c_str());
        ChatHandler(Modder).PSendSysMessage("|   [Precettore/rice]: %s   [Tot Epigoni]: %u", name.c_str(), RpgEpigoni);
        ChatHandler(Modder).PSendSysMessage("|   [Identita']: %s", RpgIdentity.c_str());
        ChatHandler(Modder).PSendSysMessage("o==============================================================");

    }

    return true;
}

int32 Hw2Class::RpgContaDownLine(ObjectGuid guid, uint16 pl_rango) {
    if (!guid)
        return 0;

    int32 count = 0;
    uint16 rango = 0;

    Field *fields = NULL;
    QueryResult *result = NULL;

    result = Hw2Database.PQuery("SELECT guid,totale_rpg FROM `a_rpg_players` WHERE `supervisor`= %u", guid.GetCounter());
    if (!result) {
        return count; // fine ramo albero
    } else {
        do {
            fields = result->Fetch();
            guid = ObjectGuid(HIGHGUID_PLAYER, fields[0].GetUInt32());
            rango = sHw2.RpgTrovaRank(fields[1].GetInt32());
            if (pl_rango >= rango)
                ++count;
            count += sHw2.RpgContaDownLine(guid, rango); //ricorsione WARNING!
        } while (result->NextRow());
    }

    delete result;
    return count;

}

bool Hw2Class::RpgAggiornaUpLine(ObjectGuid guid, bool salva, float punti) {
    if (!guid)
        return false;
    int32 tot_rpg = 0;

    Player *sup = NULL;
    Field *fields = NULL;
    QueryResult *result = NULL;
    result = Hw2Database.PQuery("SELECT supervisor,totale_rpg FROM `a_rpg_players` WHERE `guid`= %u", guid);
    if (!result)
        return false;

    fields = result->Fetch();
    uint32 lowGuid = fields[0].GetUInt32();
    if (lowGuid == 0)
        return false;

    ObjectGuid uplineGuid = ObjectGuid(HIGHGUID_PLAYER, lowGuid);


    uint16 pl_rango = sHw2.RpgTrovaRank(fields[1].GetInt32());
    uint16 rango2 = pl_rango;
    delete result;


    do {
        result = NULL; //resetta la variabile
        result = Hw2Database.PQuery("SELECT supervisor,totale_rpg FROM `a_rpg_players` WHERE `guid`= %u", uplineGuid.GetCounter());
        if (!result) {
            return true;
        } else {
            fields = result->Fetch();
            tot_rpg = fields[1].GetInt32();
            uint16 rango = sHw2.RpgTrovaRank(tot_rpg);

            if (punti != 0) // assegnazione punti agli upline
            {

                if (punti > 0) {
                    float percentuale = 0;

                    switch (rango) {
                        case 1: // i gradi partono da 0 quindi 1 e' gia' supervisore
                            percentuale = punti / 100;

                            if (rango > rango2)
                                percentuale *= 25;
                            else if (rango == rango2)
                                percentuale *= 5;

                            break;

                        default:
                            percentuale = punti / 100;

                            if (rango >= rango2)
                                percentuale *= (rango - pl_rango) + 5; //5 e' il bonus

                            break;

                    }


                    if (percentuale <= 0)
                        return true;

                    sHw2.RpgModificaPT(salva, uplineGuid, (int32) percentuale, 0, true);

                    rango2 = rango;

                } else return false;

            } else // ricalcolamento del totale downline
            {
                sup = NULL; //resetta
                sup = sObjectMgr.GetPlayer(uplineGuid);
                if (sup) {
                    if (sHw2.RpgTrovaRank(sup->RpgTotalePt) < rango)
                        return true;
                    else
                        ++sup->RpgEpigoni;

                    ChatHandler(sup).PSendSysMessage("Il totale dei tuoi epigoni e' stato appena aggiornato");

                }

            }

            lowGuid = fields[0].GetUInt32();
            uplineGuid = ObjectGuid(HIGHGUID_PLAYER, lowGuid); // recupera il guid del prossimo elemento

        }

        delete result;

    } while (lowGuid != 0);

    return false;

}

uint16 Hw2Class::RpgTrovaRank(int32 TotPunti) {
    uint16 count = 0;
    bool trovato = false;

    do {

        if (TotPunti < cl[count].grado) trovato = true;
        else ++count;

    } while (!trovato && count < cl_count);

    if (!trovato) --count; // se trova un rank superiore usa l'ultimo esistente 

    return count;
}
