#include "Rankings.h"
#include <iostream>

void Rankings::sortTeamsByWins(Teams* arr[], int left, int right) //function to divide then merge sorted team wins
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        sortTeamsByWins(arr, left, mid);
        sortTeamsByWins(arr, mid + 1, right);
        mergeTeams(arr, left, mid, right);     // combine
    }
}


void Rankings::mergeTeams(Teams* arr[], int left, int mid, int right) //function to sort when merging
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Teams* L[50]; Teams* R[50];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        bool leftFirst = false;
        if (L[i]->getWins() > R[j]->getWins()) leftFirst = true;
        else if (L[i]->getWins() == R[j]->getWins() &&
                 L[i]->getPointsFor() >= R[j]->getPointsFor()) leftFirst = true;
        arr[k++] = leftFirst ? L[i++] : R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}



void Rankings::sortPlayersByRegularPoints(PlayerEntry arr[], int left, int right) //function to divide then merge sorted players by regular points
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        sortPlayersByRegularPoints(arr, left, mid);         //divides
        sortPlayersByRegularPoints(arr, mid + 1, right);
        mergePlayersByRegularPoints(arr, left, mid, right); //combine
    }
}

void Rankings::mergePlayersByRegularPoints(PlayerEntry arr[], int left, int mid, int right) //function to sort when merging
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    PlayerEntry L[250];  //temp left half
    PlayerEntry R[250];  //temp right half


    for (int i = 0; i < n1; i++)    //left and right arrays
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];


    int i = 0, j = 0, k = left; //array merge back

    while (i < n1 && j < n2)    //sort in decending order
    {

        if (L[i].regularPoints >= R[j].regularPoints)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) //copy the remaining
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void Rankings::sortPlayersByPlayoffPoints(PlayerEntry arr[], int left, int right)   //function to divide then merge sorted players by playoff points
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        sortPlayersByPlayoffPoints(arr, left, mid);         //divides
        sortPlayersByPlayoffPoints(arr, mid + 1, right);
        mergePlayersByPlayoffPoints(arr, left, mid, right); //combines
    }
}

void Rankings::mergePlayersByPlayoffPoints(PlayerEntry arr[], int left, int mid, int right) //function to sort when merging
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    PlayerEntry L[250];  //temp left half
    PlayerEntry R[250];  //temp right half


    for (int i = 0; i < n1; i++)    //left and right arrays
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];


    int i = 0, j = 0, k = left; //array merge back

    while (i < n1 && j < n2)    //sort in decending order
    {

        if (L[i].playoffPoints >= R[j].playoffPoints)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) //copy the remaining
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void Rankings::display(TeamTrees& teams)
{
    Teams* teamRankings[100];
    int teamCount = 0;

    teams.fillArray(teamRankings, teamCount);
    sortTeamsByWins(teamRankings, 0, teamCount - 1);

    std::cout << "\n=== Team Rankings By Wins ===\n";

    for (int i = 0; i < teamCount; i++)
    {
        std::cout << i + 1 << ". "
                  << teamRankings[i]->getName()
                  << " | Wins: " << teamRankings[i]->getWins()
                  << " | Losses: " << teamRankings[i]->getLosses()
                  << " | Ties: " << teamRankings[i]->getTies()
                  << " | Points For: " << teamRankings[i]->getPointsFor()
                  << "\n";
    }

    PlayerEntry players[500];
    int playerCount = 0;

    for (int i = 0; i < teamCount; i++)
    {
        for (int j = 0; j < teamRankings[i]->getPlayerCount(); j++)
        {
            Player* p = teamRankings[i]->getPlayer(j);

            if (p != nullptr)
            {
                players[playerCount].name = p->getName();
                players[playerCount].regularPoints = p->getRegularPoints();
                players[playerCount].playoffPoints = p->getPlayoffPoints();
                playerCount++;
            }
        }
    }

    sortPlayersByRegularPoints(players, 0, playerCount - 1);

    std::cout << "\n=== Top 5 Players By Regular Season Points ===\n";

    int regularLimit = (playerCount < 5) ? playerCount : 5;
    for (int i = 0; i < regularLimit; i++)
    {
        std::cout << i + 1 << ". "
                  << players[i].name
                  << " | Regular Points: " << players[i].regularPoints
                  << "\n";
    }

    sortPlayersByPlayoffPoints(players, 0, playerCount - 1);

    std::cout << "\n=== Top 5 Players By Playoff Points ===\n";

    int playoffLimit = (playerCount < 5) ? playerCount : 5;
    for (int i = 0; i < playoffLimit; i++)
    {
        std::cout << i + 1 << ". "
                  << players[i].name
                  << " | Playoff Points: " << players[i].playoffPoints
                  << "\n";
    }
}
