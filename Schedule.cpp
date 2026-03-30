#include "Schedule.h"
#include <iostream>

//constructor
Schedule::Schedule()
{
    clear(); //empties initially
}

void Schedule::clear() //used for clear, empties data
{
    totalWeeks = 0;

    for (int w = 0; w < MAX_WEEKS; w++)
    {
        gamesPerWeek[w] = 0;
        for (int g = 0; g < MAX_GAMES_PER_WEEK; g++)
            weeks[w][g] = nullptr;
    }
}

void Schedule::generate(TeamTrees& tree, int seasonWeeks) //generates weeks with teams
{

    clear(); //clear on initial generation

    Teams* teamArray[100];
    int teamCount = 0;

    tree.fillArray(teamArray, teamCount); //gets teams and sorts them

    if (teamCount < 2) //scheduling not possible with less than 2 teams
    {
        std::cout << "Error: Need at least 2 teams to generate schedule.\n";  // ← ADD THIS
        return;
    }

    bool hasBye = false;

    if (teamCount % 2 != 0) //sets bye weeks with odd number of teams
    {
        teamArray[teamCount] = nullptr;
        teamCount++;
        hasBye = true;
    }

    int rotationCount = teamCount - 1; //number of unique matchups with total teams -1 because you cant play yourself

    totalWeeks = seasonWeeks; //how many weeks needed to generate

    for (int week = 0; week < seasonWeeks; week++) //main schedule loop
    {

        gamesPerWeek[week] = 0; //resets weekly counter

        for (int i = 0; i < teamCount / 2; i++) //pairs teams
        {

            Teams* home = teamArray[i];
            Teams* away = teamArray[teamCount - 1 - i];

            if (home == nullptr || away == nullptr) //creates bye week
            {
                weeks[week][gamesPerWeek[week]++] =
                        new Game(home, away, true);
            }
            else
            {
                if (home != away)//creates game
                {
                    weeks[week][gamesPerWeek[week]++] =
                            new Game(home, away, false);
                }
            }
        }

        Teams* last = teamArray[teamCount - 1]; //begins round robin style matchmaking

        for (int i = teamCount - 1; i > 1; i--)
            teamArray[i] = teamArray[i - 1];

        teamArray[1] = last;
    }
}

void Schedule::display() //displays schedule
{

    for (int w = 0; w < totalWeeks; w++)
    {

        std::cout << "\nWeek " << w + 1 << ":\n"; //prints each week of games

        int gameNumber = 1;

        for (int g = 0; g < gamesPerWeek[w]; g++) //g is game inside w week
        {

            Game* game = weeks[w][g]; //retrieves both w and g

            if (game == nullptr) //no game = skip
                continue;

            if (!game->isByeGame()) //only true matchups can enter this, no byes
            {

                std::cout << "  " << gameNumber++ << ". " //prints 1. 2. 3. on matchups
                          << game->getHomeTeam()->getName()
                          << " vs "
                          << game->getAwayTeam()->getName(); //prints team name vs team name

                if (game->isPlayed()) //looks if game has been played
                {
                    std::cout << " : "
                              << game->getHomeScore()
                              << " - "
                              << game->getAwayScore(); //returns score after game if game has been played
                }

                std::cout << "\n";
            }
        }

        for (int g = 0; g < gamesPerWeek[w]; g++) //same as before
        {

            Game* game = weeks[w][g]; //same as before

            if (game == nullptr) //skips empty slots
                continue;

            if (game->isByeGame()) //enters if team is on bye
            {

                if (game->getHomeTeam() == nullptr)
                    std::cout << "  "
                              << game->getAwayTeam()->getName()
                              << " - BYE\n";
                else
                    std::cout << "  "
                              << game->getHomeTeam()->getName()
                              << " - BYE\n";
            }
        }
    }
}

Game* Schedule::getGame(int weekIndex, int gameIndex) //game pointer
{

    if (weekIndex < 0 || weekIndex >= totalWeeks) //checks validity
        return nullptr;

    if (gameIndex < 0 || gameIndex >= gamesPerWeek[weekIndex]) //checks validity
        return nullptr;

    return weeks[weekIndex][gameIndex];
}