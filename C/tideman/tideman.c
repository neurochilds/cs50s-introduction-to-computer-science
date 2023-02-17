#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool makes_circle(int candidate_a, int candidate_b);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(candidates[i], name) == 0) // if name matches one of the candidates, add candidates index to ranks whereby ranks[i] is the voters ith preference
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int first = ranks[0];
    int second = ranks[1];
    int third = ranks[2];

    preferences[first][second]++;
    preferences[first][third]++;
    preferences[second][third]++;

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            bool candidate_i_wins = (preferences[i][j] > preferences[j][i]);
            bool candidatej_wins = (preferences[j][i] > preferences[i][j]);

            if (candidate_i_wins)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }

            else if (candidatej_wins)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bool unsorted = true;

    // Bubble sort pairs (NB: merge sort is fastest, but n candidates should be low enough that bubble sort is suitable)
    while (unsorted)
    {
        unsorted = false;
        for (int i = 0; i < pair_count - 1; i++)
        {
            int j = i + 1;

            int winner_i_score = preferences[pairs[i].winner][pairs[i].loser];
            int winner_j_score = preferences[pairs[j].winner][pairs[j].loser];

            if (winner_i_score < winner_j_score)
            {
                pair pairs_i = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = pairs_i;
                unsorted = true;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if (!makes_circle(loser, winner)) // if there is no path from loser pointing to winner, winner can point to loser without forming a circle
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

// check to see if there is a continuous link between candidate_a --> candidate_b. If so, linking candidate_b --> candidate_a will create a circle.
bool makes_circle(int candidate_a, int candidate_b)
{
    for (int candidate_c = 0; candidate_c < candidate_count; candidate_c++)
    {
        if (locked[candidate_a][candidate_c]) // if candidate_a points to any other candidates
        {
            if (locked[candidate_c][candidate_b]) // check if that candidate (candidate_c) points to candidate_b, creating a link from a --> c --> b
            {
                return true;
            }
            else if (makes_circle(candidate_c, candidate_b)) // if not, repeat the above with candidate_c
            {
                return true;
            }
        }
    }
    return false; // if there is no path from candidate_a --> candidate_b, candidate_b can point to candidate_a without forming a circle
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                break;
            }
            else if (j == candidate_count - 1)
            {
                printf("%s\n", candidates[i]);
                return;
            }
        }
    }
    return;
}