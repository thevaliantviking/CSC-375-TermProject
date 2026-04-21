#ifndef RANKINGS_H
#define RANKINGS_H

#include "TeamTrees.h"

class Rankings {

private:
    struct PlayerEntry
    {
        std::string name;
        int regularPoints;
        int playoffPoints;
    };

    void sortTeamsByWins(Teams* arr[], int left, int right);
    void mergeTeams(Teams* arr[], int left, int mid, int right);

    void sortPlayersByRegularPoints(PlayerEntry arr[], int left, int right);
    void mergePlayersByRegularPoints(PlayerEntry arr[], int left, int mid, int right);

    void sortPlayersByPlayoffPoints(PlayerEntry arr[], int left, int right);
    void mergePlayersByPlayoffPoints(PlayerEntry arr[], int left, int mid, int right);

public:
    void display(TeamTrees& teams);
};

#endif
