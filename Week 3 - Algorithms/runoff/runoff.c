#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// Preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    // Assign characteristics to array
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            // Get candidate rank starting from 1 (0+1)
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // Go through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate name matches voter input name
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Add candidate rank to voter
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    // Go through all voters
    for (int i = 0; i < voter_count; i++)
    {
        // Go through all candidates
        for (int j = 0; j < candidate_count; j++)
        {
            // Candidate of jth rank of ith voter
            int candidate_index = preferences[i][j];
            if (candidates[candidate_index].eliminated == false)
            {
                // Add candidate vote
                candidates[candidate_index].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    // Go through all voters
    for (int i = 0; i < voter_count; i++)
    {
        // If candidate's votes is the majority (more than half)
        if (candidates[i].votes > (voter_count / 2))
        {
            // Candidate wins
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int minvotes = voter_count;
    // Go through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes < minvotes)
            {
                // Update minimum number of votes of a candidate
                minvotes = candidates[i].votes;
            }
        }
    }
    // Return value
    return minvotes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    // Go through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate's vote is not the minimum
        if (candidates[i].eliminated == false && candidates[i].votes != min)
        {
            return false;
        }
    }
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    // Go through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate has lowest number of votes
        if (candidates[i].votes == min)
        {
            // Candidate is eliminated
            candidates[i].eliminated = true;
        }
    }
    return;
}