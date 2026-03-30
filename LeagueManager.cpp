#include "LeagueManager.h"
#include <iostream>
#include <cstdlib>

//constructor
LeagueManager::LeagueManager()
        : teamLimit(0),
          playersPerTeam(0),
          hasPlayoff(false),
          regularSeasonWeeks(0),
          playoffTeamCount(0) {}

void LeagueManager::run() //entry point
{
    while (true)
    {
        setupLeague(); //asks setup questions
        mainMenu(); //enters main interface loop
    }
}

std::string LeagueManager::getLineInput() //gathering user input
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string LeagueManager::getRequiredLineInput()
{
    while (true)
    {
        std::string input = getLineInput();
        bool hasRealText = false;

        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] != ' ' && input[i] != '\t')
            {
                hasRealText = true;
                break;
            }
        }

        if (hasRealText)
            return input;

        std::cout << "Input cannot be blank. Try again: ";
    }
}

int LeagueManager::getIntegerInput() //gathers user input integer form
{
    while (true)
    {
        std::string input = getLineInput();

        if (input.empty())
        {
            std::cout << "Invalid number. Try again: ";
            continue;
        }


        try
        {
            int value = std::stoi(input);
            if (value < 0) {
                std::cout << "Must be positive. Try again: ";
                continue;
            }
            return value;
        }
        catch (...)
        {
            std::cout << "Invalid number. Try again: "; //handles invalid responses
        }
    }
}

int LeagueManager::getPositiveIntegerInput()
{
    while (true)
    {
        int value = getIntegerInput();

        if (value > 0)
            return value;

        std::cout << "Enter a number greater than 0: ";
    }
}

char LeagueManager::getCharInput() //Gathers input for (Y/N)
{
    while (true)
    {
        std::string input = getLineInput();

        if (input.length() == 1)
        {
            char c = input[0];

            if (c == 'Y' || c == 'y' || c == 'N' || c == 'n')
                return c;
        }

        std::cout << "Please enter Y or N: ";
    }
}

void LeagueManager::pause() //recieves input for pressing enter to close the page
{
    std::cout << "Close Page (press Enter)...";
    getLineInput();
}

bool LeagueManager::isValidPlayoffSize(int size) //validates proper playoff bracket
{
    if (size == 2 && teamLimit >= 2)
        return true;

    if (size == 4 && teamLimit >= 4)
        return true;

    if (size == 8 && teamLimit >= 8)
        return true;

    return false;
}

void LeagueManager::printValidPlayoffOptions() //prints options for playoff
{
    std::cout << "How many teams in playoff (";

    if (teamLimit >= 2)
        std::cout << "2";

    if (teamLimit >= 4)
        std::cout << ", 4";

    if (teamLimit >= 8)
        std::cout << ", 8";

    std::cout << "): ";
}

void LeagueManager::rebuildHashTable() //rebuilds hash table for fast team lookup
{
    Teams* arr[100];
    int count = 0;

    teams.fillArray(arr, count);

    teamTable.clear();

    for (int i = 0; i < count; i++)
        teamTable.insert(arr[i]->getName(), arr[i]);
}

void LeagueManager::setupLeague() //configuration of the league
{
    teams.clear();
    schedule.clear();
    teamTable.clear();
    playoff.clear();

    playoffTeamCount = 0;

    std::cout << "\nHello! Welcome to a League of Our Own!\n";

    std::cout << "Please Enter Name of Your League: ";
    leagueName = getRequiredLineInput(); //names the league off user input

    while (true)
    {
        std::cout << "Please Enter Quantity of Teams (max 10): ";
        teamLimit = getPositiveIntegerInput(); //enters the amount of teams the league will have

        if (teamLimit > 10)
        {
            std::cout << "Maximum teams allowed is 10.\n";
            continue;
        }

        if (teamLimit < 2)
        {
            std::cout << "You need at least 2 teams.\n";
            continue;
        }

        break;
    }

    while (true)
    {
        std::cout << "Please Enter Quantity of Players per Team (max 6): ";
        playersPerTeam = getPositiveIntegerInput(); //enters the amount of players on a team

        if (playersPerTeam > 6)
        {
            std::cout << "Maximum players per team is 6.\n";
            continue;
        }

        break;
    }

    int maxRegularSeasonWeeks = teamLimit - 1;

    std::cout << "Will There Be a Playoff (Y/N): ";
    char playoffChoice = getCharInput(); //allows user to decide if they would like a playoff or not

    hasPlayoff = (playoffChoice == 'Y' || playoffChoice == 'y');

    if (hasPlayoff) //if there is a playoff there will be a seperate prompt
    {
        while (true)
        {
            std::cout << "How Many Weeks In Regular Season (max " << maxRegularSeasonWeeks << "): ";
            regularSeasonWeeks = getPositiveIntegerInput();

            if (regularSeasonWeeks > maxRegularSeasonWeeks)
            {
                std::cout << "Maximum regular season weeks is " << maxRegularSeasonWeeks
                          << " so each team only plays each other once.\n";
                continue;
            }

            break;
        }

        while (true)
        {
            printValidPlayoffOptions();
            playoffTeamCount = getPositiveIntegerInput();

            if (!isValidPlayoffSize(playoffTeamCount))
            {
                std::cout << "Invalid playoff size. Try again.\n";
                continue;
            }

            break;
        }
    }
    else //if theres no playoff the standard question for weeks
    {
        while (true)
        {
            std::cout << "How Many Weeks in Season (max " << maxRegularSeasonWeeks << "): ";
            regularSeasonWeeks = getPositiveIntegerInput();

            if (regularSeasonWeeks > maxRegularSeasonWeeks)
            {
                std::cout << "Maximum season weeks is " << maxRegularSeasonWeeks
                          << " so each team only plays each other once.\n";
                continue;
            }

            break;
        }

        playoffTeamCount = 0;
    }

    for (int i = 0; i < teamLimit; i++) //calls add team setup for limit of teams
        addTeamSetup();

    rebuildHashTable();

    while (true)
    {
        std::cout << "Generate Schedule? (Y/N): ";//will generate schedule unless user wants to go back
        char generateChoice = getCharInput();

        if (generateChoice == 'Y' || generateChoice == 'y')
        {
            rebuildHashTable();
            schedule.generate(teams, regularSeasonWeeks);
            break;
        }

        editOrAddLoop(); //calls the edit or add loop after selecting no
    }
}

void LeagueManager::addTeamSetup()
{
    std::cout << "\nAdd Team Name: ";//user input for team name
    std::string teamName = getRequiredLineInput();

    Teams newTeam(teamName);

    std::cout << "Add Players:\n";

    for (int i = 0; i < playersPerTeam; i++)
    {
        std::cout << "Player " << i + 1 << ": "; //asks for user name and assigns number
        std::string playerName = getRequiredLineInput();
        newTeam.addPlayer(Player(playerName));
    }

    teams.insert(newTeam); //adds team/players
    rebuildHashTable();
}

void LeagueManager::editOrAddLoop() //loop to add teams or edit players
{
    while (true)
    {
        std::cout << "\n1. Add Teams\n";
        std::cout << "2. Edit Players\n";
        std::cout << "3. Back\n";
        std::cout << "Select Option: ";

        int option = getIntegerInput(); //gathers input

        if (option == 1)
            addTeamSetup();
        else if (option == 2)
            editPlayers();
        else if (option == 3)
            return;
        else
            std::cout << "Invalid Option.\n";
    }
}

void LeagueManager::editPlayers()
{
    rebuildHashTable();

    teams.displayAll(); //displays teams

    std::cout << "Enter Team Name To Edit: ";//search for team to edit player
    std::string teamName = getRequiredLineInput();

    Teams* team = teamTable.get(teamName);

    if (team == nullptr) //validation
    {
        std::cout << "Team not found.\n";
        return;
    }

    team->displayRoster(); //displays roster with name and assigned number

    std::cout << "Select Player To Edit (number): ";
    int selection = getPositiveIntegerInput();

    Player* player = team->getPlayer(selection - 1);

    if (player != nullptr)
    {
        std::cout << "Enter New Player Name: ";
        std::string newName = getRequiredLineInput();
        player->setName(newName); //changed player name
        std::cout << "Player Updated.\n";
    }
    else
    {
        std::cout << "Invalid selection.\n";
    }
}

void LeagueManager::mainMenu() //main interface for league
{
    while (true)
    {
        if (playoff.isFinished())
        {
            showChampionScreen();
            continue;
        }

        std::cout << "\n===== " << leagueName << " =====\n";
        std::cout << "1. View Schedule\n";

        if (!playoff.isGenerated())
            std::cout << "2. Add Score\n";
        else
            std::cout << "2. View Regular Season\n";

        std::cout << "3. View Stats\n";
        std::cout << "4. Rankings\n";
        std::cout << "5. Predictions\n";

        if (hasPlayoff)
        {
            std::cout << "6. Playoffs\n";
            std::cout << "7. Clear\n";
            std::cout << "8. Close Application\n";
        }
        else
        {
            std::cout << "6. Clear\n";
            std::cout << "7. Close Application\n";
        }

        std::cout << "Select Option: ";

        int choice = getIntegerInput(); //gathers input for what user would like to do in interface

        if (hasPlayoff)
        {
            switch (choice) //assigns numbers to interface for user selection
            {
                case 1: viewSchedule(); break;
                case 2:
                    if (!playoff.isGenerated()) addRegularSeasonScore();
                    else viewSchedule();
                    break;
                case 3: viewScores(); break;
                case 4: rankingsMenu(); break;
                case 5: viewPredictions(); break;
                case 6: playoffMenu(); break;
                case 7: clearAll(); break;

                case 8:
                {
                    std::cout << "Would You Like To Close (Y/N): ";
                    char c = getCharInput();
                    if (c == 'Y' || c == 'y')
                        std::exit(0);
                    break;
                }

                default:
                    std::cout << "Invalid Option.\n";
            }
        }
        else
        {
            switch (choice)
            {
                case 1: viewSchedule(); break;
                case 2: addRegularSeasonScore(); break;
                case 3: viewScores(); break;
                case 4: rankingsMenu(); break;
                case 5: viewPredictions(); break;
                case 6: clearAll(); break;

                case 7:
                {
                    std::cout << "Would You Like To Close (Y/N): ";
                    char c = getCharInput();
                    if (c == 'Y' || c == 'y')
                        std::exit(0);
                    break;
                }

                default:
                    std::cout << "Invalid Option.\n";
            }
        }
    }
}

void LeagueManager::viewSchedule() //shows generated schedule
{
    schedule.display();
    pause();
}

void LeagueManager::addRegularSeasonScore() //interface to add regular season score by week
{
    schedule.display();

    std::cout << "\nSelect Week Number: ";
    int week = getPositiveIntegerInput();
    int weekIndex = week - 1;

    std::cout << "Select Game Number: ";
    int selection = getPositiveIntegerInput();

    int realGameCounter = 0;
    Game* selectedGame = nullptr;

    for (int i = 0; i < 50; i++)
    {
        Game* g = schedule.getGame(weekIndex, i);

        if (g == nullptr)
            break;

        if (!g->isByeGame())
        {
            realGameCounter++;

            if (realGameCounter == selection)
            {
                selectedGame = g;
                break;
            }
        }
    }

    if (selectedGame == nullptr)
    {
        std::cout << "Invalid selection.\n";
        return;
    }

    scoring.updateGame(selectedGame, false);
}

void LeagueManager::viewScores() //shows team and player scores
{
    teams.displayAll();
    pause();
}

void LeagueManager::viewPredictions() //shows predictions of games
{
    prediction.displayPrediction(teams, schedule, regularSeasonWeeks);
    pause();
}

void LeagueManager::rankingsMenu() //shows rankings
{
    rankings.display(teams);
    pause();
}

bool LeagueManager::regularSeasonComplete() //when season is complete
{
    for (int week = 0; week < regularSeasonWeeks; week++)
    {
        for (int gameIndex = 0; gameIndex < 50; gameIndex++)
        {
            Game* game = schedule.getGame(week, gameIndex);

            if (game == nullptr)
                break;

            if (!game->isByeGame() && !game->isPlayed())
                return false;
        }
    }

    return true;
}

void LeagueManager::playoffMenu() //shows playoff menu
{
    if (!hasPlayoff)
        return;

    if (!regularSeasonComplete())
    {
        std::cout << "Playoffs have not been established yet.\n"; //fun little block so regular season has to be finished prior to generate
        pause();
        return;
    }

    if (!playoff.isGenerated())
    {
        std::cout << "Generate Playoffs (Y/N): "; //generate playoffs
        char choice = getCharInput();

        if (choice == 'Y' || choice == 'y')
            playoff.generate(teams, playoffTeamCount);
        else
            return;
    }

    playoff.display(prediction, teams, schedule, regularSeasonWeeks);

    if (playoff.isFinished())
    {
        pause();
        return;
    }

    std::cout << "\nSelect Game: ";
    int choice = getPositiveIntegerInput();

    playoff.scoreSelectedGame(choice, scoring);
    pause();
}

Player* LeagueManager::getMVP() //takes player with most points as MVP
{
    Teams* arr[100];
    int count = 0;
    teams.fillArray(arr, count);

    Player* best = nullptr;

    for (int i = 0; i < count; i++)
    {
        Player* teamBest = arr[i]->getHighestTotalPointPlayer();

        if (teamBest == nullptr)
            continue;

        if (best == nullptr || teamBest->getTotalPoints() > best->getTotalPoints())
            best = teamBest;
    }

    return best;
}

void LeagueManager::showChampionScreen() //final screen shows champion and mvp
{
    Teams* champ = playoff.getChampion();
    Player* mvp = getMVP();

    std::cout << "\n=== Congratulations "
              << ((champ != nullptr) ? champ->getName() : "Champion")
              << " ===\n";

    std::cout << "=== MVP "
              << ((mvp != nullptr) ? mvp->getName() : "N/A")
              << " ===\n";

    std::cout << "1. Clear\n";
    std::cout << "2. Close Application\n";
    std::cout << "Select Option: ";

    int choice = getIntegerInput();

    if (choice == 1)
        clearAll();
    else if (choice == 2)
        std::exit(0);
    else
        std::cout << "Invalid Option.\n";
}

void LeagueManager::clearAll() //clears all stored info back to beginning
{
    teams.clear();
    schedule.clear();
    teamTable.clear();
    playoff.clear();

    hasPlayoff = false;
    regularSeasonWeeks = 0;
    playoffTeamCount = 0;

    std::cout << "\nSystem Cleared.\n";
    setupLeague();
}