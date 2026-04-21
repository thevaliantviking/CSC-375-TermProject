#ifndef PLAYOFF_H
#define PLAYOFF_H

#include "TeamTrees.h"
#include "Game.h"
#include "Scoring.h"
#include "Prediction.h"
#include "Schedule.h"

class Playoff {

private:
    int playoffTeamCount;

    bool generated;
    bool semifinalCreated;
    bool championshipCreated;
    bool finished;

    Teams* seeds[8];
    int seedCount;

    Game* firstRoundGames[4];
    Game* semifinalGames[2];
    Game* championshipGame;

    void selectPlayoffTeams(Teams* allTeams[], int totalCount, Teams* selected[], int& selectedCount,int targetCount);
    Teams* getWinner(Game* game);

    bool firstRoundComplete();
    bool semifinalRoundComplete();

    void createSemifinals();
    void createChampionshipGame();
    void tryAdvancePlayoffs();

public:
    Playoff();
    ~Playoff();

    void clear();
    void generate(TeamTrees& teams, int teamCount);
    void display(Prediction& prediction, TeamTrees& teams, Schedule& schedule, int regularSeasonWeeks);
    void scoreSelectedGame(int selection, Scoring& scoring);

    bool isGenerated() const;
    bool isFinished() const;

    Teams* getChampion();
};

#endif
