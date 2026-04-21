#ifndef LEAGUEMANAGER_H
#define LEAGUEMANAGER_H

#include "TeamTrees.h"
#include "Schedule.h"
#include "Scoring.h"
#include "HashTable.h"
#include "Prediction.h"
#include "Playoff.h"
#include "Rankings.h"
#include <string>

class LeagueManager {

private:
    TeamTrees teams;
    Schedule schedule;
    Scoring scoring;
    HashTable teamTable;
    Prediction prediction;
    Playoff playoff;
    Rankings rankings;

    std::string leagueName;

    int teamLimit;
    int playersPerTeam;

    bool hasPlayoff;
    int regularSeasonWeeks;
    int playoffTeamCount;

    void setupLeague();
    void addTeamSetup();
    void editOrAddLoop();
    void editPlayers();

    void mainMenu();
    void viewSchedule();
    void viewScores();
    void addRegularSeasonScore();
    void viewPredictions();
    void rankingsMenu();
    void playoffMenu();
    void clearAll();

    bool regularSeasonComplete();
    bool isValidPlayoffSize(int size);
    void printValidPlayoffOptions();
    Player* getMVP();

    void rebuildHashTable();

    std::string getLineInput();
    std::string getRequiredLineInput();
    int getIntegerInput();
    int getPositiveIntegerInput();
    char getCharInput();
    void pause();
    void showChampionScreen();

    void loadTeamsFromFile();
    void loadScoresFromFile();
    bool TeamsFile(const std::string& filename);
    bool ScoresFile(const std::string& filename);

    std::string clean(const std::string& str);
    bool startsWith(const std::string&str, const std::string& prefix);

public:
    LeagueManager();
    void run();
};

#endif
