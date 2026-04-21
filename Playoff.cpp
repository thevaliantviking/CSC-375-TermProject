#include "Playoff.h"
#include <iostream>

//constructor
Playoff::Playoff()
        : playoffTeamCount(0),
          generated(false),
          semifinalCreated(false),
          championshipCreated(false),
          finished(false),
          seedCount(0),
          championshipGame(nullptr)
{
    for (int i = 0; i < 8; i++)
        seeds[i] = nullptr;

    for (int i = 0; i < 4; i++)
        firstRoundGames[i] = nullptr;

    for (int i = 0; i < 2; i++)
        semifinalGames[i] = nullptr;
}

//destructor
Playoff::~Playoff()
{
    clear();
}

void Playoff::clear()
{
    for (int i = 0; i < 4; i++)
    {
        delete firstRoundGames[i];
        firstRoundGames[i] = nullptr;
    }

    for (int i = 0; i < 2; i++)
    {
        delete semifinalGames[i];
        semifinalGames[i] = nullptr;
    }

    delete championshipGame;
    championshipGame = nullptr;

    playoffTeamCount = 0;
    generated = false;
    semifinalCreated = false;
    championshipCreated = false;
    finished = false;
    seedCount = 0;

    for (int i = 0; i < 8; i++)
        seeds[i] = nullptr;
}

void Playoff::selectPlayoffTeams(Teams* allTeams[], int totalCount, Teams* selected[], int& selectedCount,int targetCount) //function to select playoff
{
    bool used[100] = {};     //record teams have already been selected
    selectedCount = 0;


    while (selectedCount < targetCount && selectedCount < totalCount)
    {
        int bestIdx = -1;


        for (int i = 0; i < totalCount; i++)    //look for the best team that hasn't been selected yet
        {

            if (used[i])                        //sskip teams that have already been selected
                continue;


            if (bestIdx == -1)                  //if this is the first candidate, select it
            {
                bestIdx = i;
                continue;
            }

            bool isBetter = false;

            //Compare current team with current best candidate

            //most wins
            if (allTeams[i]->getWins() > allTeams[bestIdx]->getWins())
                isBetter = true;

            //highest points scored
            else if (allTeams[i]->getWins() == allTeams[bestIdx]->getWins() && allTeams[i]->getPointsFor() > allTeams[bestIdx]->getPointsFor())
                isBetter = true;

            //best point differential
            else if (allTeams[i]->getWins() == allTeams[bestIdx]->getWins() &&
                     allTeams[i]->getPointsFor() == allTeams[bestIdx]->getPointsFor() &&
                     allTeams[i]->getPointDifferential() > allTeams[bestIdx]->getPointDifferential())
                isBetter = true;

            if (isBetter)   //update the best candidate if current team is better
                bestIdx = i;
        }

        if (bestIdx == -1)  //no valid team found
            break;

        selected[selectedCount] = allTeams[bestIdx];    //select this team
        used[bestIdx] = true;
        selectedCount++;
    }
}

Teams* Playoff::getWinner(Game* game)
{
    if (game == nullptr || !game->isPlayed())
        return nullptr;

    if (game->getHomeScore() > game->getAwayScore())
        return game->getHomeTeam();

    return game->getAwayTeam();
}

bool Playoff::firstRoundComplete() //first round is finished
{
    if (playoffTeamCount == 2 || playoffTeamCount == 4)
        return true;

    for (int i = 0; i < 4; i++)
    {
        if (firstRoundGames[i] == nullptr || !firstRoundGames[i]->isPlayed())
            return false;
    }

    return true;
}

bool Playoff::semifinalRoundComplete() //2nd round/semifinal is finished
{
    if (playoffTeamCount == 2)
        return true;

    if (playoffTeamCount == 4)
    {
        for (int i = 0; i < 2; i++)
        {
            if (semifinalGames[i] == nullptr || !semifinalGames[i]->isPlayed())
                return false;
        }

        return true;
    }

    if (!semifinalCreated)
        return false;

    for (int i = 0; i < 2; i++)
    {
        if (semifinalGames[i] == nullptr || !semifinalGames[i]->isPlayed())
            return false;
    }

    return true;
}

void Playoff::createSemifinals() //creates final semifinal matchup
{
    if (semifinalCreated)
        return;

    if (playoffTeamCount == 4)
    {
        semifinalGames[0] = new Game(seeds[0], seeds[3], false);
        semifinalGames[1] = new Game(seeds[1], seeds[2], false);
        semifinalCreated = true;
        return;
    }

    if (playoffTeamCount == 8)
    {
        Teams* winner1 = getWinner(firstRoundGames[0]);
        Teams* winner2 = getWinner(firstRoundGames[1]);
        Teams* winner3 = getWinner(firstRoundGames[2]);
        Teams* winner4 = getWinner(firstRoundGames[3]);

        if (winner1 == nullptr || winner2 == nullptr || winner3 == nullptr || winner4 == nullptr)
            return;

        semifinalGames[0] = new Game(winner1, winner2, false);
        semifinalGames[1] = new Game(winner3, winner4, false);
        semifinalCreated = true;
    }
}

void Playoff::createChampionshipGame() //creates championship matchup
{
    if (championshipCreated)
        return;

    Teams* teamA = nullptr;
    Teams* teamB = nullptr;

    if (playoffTeamCount == 2)
    {
        teamA = seeds[0];
        teamB = seeds[1];
    }
    else
    {
        teamA = getWinner(semifinalGames[0]);
        teamB = getWinner(semifinalGames[1]);
    }

    if (teamA == nullptr || teamB == nullptr)
        return;

    championshipGame = new Game(teamA, teamB, false);
    championshipCreated = true;
}

void Playoff::tryAdvancePlayoffs() //auto advances to next round
{
    if (playoffTeamCount == 2)
    {
        if (!championshipCreated)
            createChampionshipGame();

        if (championshipGame != nullptr && championshipGame->isPlayed())
            finished = true;

        return;
    }

    if (playoffTeamCount == 8 && firstRoundComplete() && !semifinalCreated)
        createSemifinals();

    if (semifinalRoundComplete() && !championshipCreated)
        createChampionshipGame();

    if (championshipCreated && championshipGame != nullptr && championshipGame->isPlayed())
        finished = true;
}

void Playoff::generate(TeamTrees& teams, int teamCount)
{
    clear();

    Teams* arr[100];
    int count = 0;

    teams.fillArray(arr, count);

    playoffTeamCount = teamCount;
    selectPlayoffTeams(arr, count, seeds, seedCount, teamCount);

    if (playoffTeamCount == 8)
    {
        firstRoundGames[0] = new Game(seeds[0], seeds[7], false);
        firstRoundGames[1] = new Game(seeds[3], seeds[4], false);
        firstRoundGames[2] = new Game(seeds[1], seeds[6], false);
        firstRoundGames[3] = new Game(seeds[2], seeds[5], false);
    }
    else if (playoffTeamCount == 4)
    {
        createSemifinals();
    }
    else if (playoffTeamCount == 2)
    {
        createChampionshipGame();
    }

    generated = true;

    std::cout << "\nPlayoffs Generated.\n";
}

void Playoff::display(Prediction& prediction, TeamTrees& teams, Schedule& schedule, int regularSeasonWeeks) //displays the predictions and rankings and power
{
    prediction.buildModel(teams, schedule, regularSeasonWeeks);

    std::cout << "\n=== Playoff Seeds ===\n";

    for (int i = 0; i < seedCount; i++)
    {
        std::cout << i + 1 << ". "
                  << seeds[i]->getName()
                  << " | Power: " << prediction.getTeamPower(seeds[i])
                  << " | Wins: " << seeds[i]->getWins()
                  << " | Points For: " << seeds[i]->getPointsFor()
                  << "\n";
    }

    if (playoffTeamCount == 8)
    {
        std::cout << "\n=== Quarterfinals ===\n";

        for (int i = 0; i < 4; i++)
        {
            std::cout << i + 1 << ". "
                      << firstRoundGames[i]->getHomeTeam()->getName()
                      << " vs "
                      << firstRoundGames[i]->getAwayTeam()->getName();

            if (firstRoundGames[i]->isPlayed())
                std::cout << " : " << firstRoundGames[i]->getHomeScore() << " - " << firstRoundGames[i]->getAwayScore();

            std::cout << "\n";
        }
    }

    tryAdvancePlayoffs();

    if (semifinalCreated)
    {
        std::cout << "\n=== Semifinals ===\n";

        for (int i = 0; i < 2; i++)
        {
            std::cout << i + 1 << ". "
                      << semifinalGames[i]->getHomeTeam()->getName()
                      << " vs "
                      << semifinalGames[i]->getAwayTeam()->getName();

            if (semifinalGames[i]->isPlayed())
                std::cout << " : " << semifinalGames[i]->getHomeScore() << " - " << semifinalGames[i]->getAwayScore();

            std::cout << "\n";
        }
    }

    if (championshipCreated && championshipGame != nullptr)
    {
        double chanceHome = prediction.predictChance(championshipGame->getHomeTeam(), championshipGame->getAwayTeam());

        std::cout << "\n=== Championship Game ===\n";
        std::cout << "1. "
                  << championshipGame->getHomeTeam()->getName()
                  << " vs "
                  << championshipGame->getAwayTeam()->getName();

        if (championshipGame->isPlayed())
            std::cout << " : " << championshipGame->getHomeScore() << " - " << championshipGame->getAwayScore();

        std::cout << "\nPredicted Winner: "
                  << ((chanceHome >= 50.0) ? championshipGame->getHomeTeam()->getName() : championshipGame->getAwayTeam()->getName())
                  << "\n";
    }
}

void Playoff::scoreSelectedGame(int selection, Scoring& scoring) //updates score in selected game
{
    if (!generated || finished)
        return;

    if (playoffTeamCount == 8 && !firstRoundComplete())
    {
        if (selection >= 1 && selection <= 4)
            scoring.updateGame(firstRoundGames[selection - 1], true);
        else
            std::cout << "Invalid selection.\n";
    }
    else if ((playoffTeamCount == 8 || playoffTeamCount == 4) && semifinalCreated && !championshipCreated)
    {
        if (selection >= 1 && selection <= 2)
            scoring.updateGame(semifinalGames[selection - 1], true);
        else
            std::cout << "Invalid selection.\n";
    }
    else if (championshipCreated)
    {
        if (selection == 1)
            scoring.updateGame(championshipGame, true);
        else
            std::cout << "Invalid selection.\n";
    }

    tryAdvancePlayoffs();
}

bool Playoff::isGenerated() const
{
    return generated;
}

bool Playoff::isFinished() const
{
    return finished;
}

Teams* Playoff::getChampion()
{
    if (!finished || championshipGame == nullptr)
        return nullptr;

    return getWinner(championshipGame);
}
