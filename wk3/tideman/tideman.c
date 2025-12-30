#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} pair;

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
bool creates_cycle(int winner, int loser);
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
    for(int i = 0; i < candidate_count; i++){
        if(strcmp(candidates[i], name) == 0){
            //update ranks array
            //Each time the user is prompted
            //it will set that rank to the corresponding candidate index
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //After each person ranks their votes
    //iterates through the rankings and compares candidates
    //the lower index is preferred over all the higher indices
    for(int i = 0; i < candidate_count; i++){
        for(int j = i + 1; j < candidate_count; j++){
            int preferred = ranks[i];
            int not_preferred = ranks[j];

            preferences[preferred][not_preferred] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //add all pairs where [i][j] > [j][i] or vice versa
    //update pair_count each time a pair is made
    //for each candidate
    //if [i][j] is preferred (bigger) than [j][i],
        //set struct winner and loser
        //increase pair count
    //else if the reverse is true
        //still increase the pair count and all that
    //if they are the same do nothing
    for(int i = 0; i < candidate_count; i++){
        for(int j = i + 1; j < candidate_count; j++){
            if(preferences[i][j] > preferences[j][i]){//pair is made
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }else if(preferences[i][j] < preferences[j][i]){
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
    //gonna compare the difference between the two
    //don't need to compare last pair
    //j only needs to be half the size of pair count
    for(int i = 0; i < pair_count - 1; i++){
        for(int j = 0; j < pair_count - i - 1; j++){
            //compare votes between each adjacent pair in the list
            int votes1 = preferences[pairs[j].winner][pairs[j].loser];
            int votes2 = preferences[pairs[j+1].winner][pairs[j+1].loser];
            //swap the values
            if(votes1 < votes2){
                pair temp = pairs[j];
                pairs[j] = pairs[j+1];
                pairs[j+1] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for(int i = 0; i < pair_count -1; i++){
        //locking the pairs doesn't create a cycle
        if(!creates_cycle(pairs[i].winner, pairs[i].loser)){
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

bool creates_cycle(int winner, int loser){
    //start at the winner and go through the different locked branches
    //if you encounter the winner, return true
    //allegedly there is a problem where the branch forks into two branches?
    //use recursion to call creates cycle with the pair loser as the paremeter
    if(winner == loser){
        return true;
    }
    for(int i = 0; i < candidate_count; i++){
        if(locked[loser][i]){
            //if no lock yet
            if(creates_cycle(winner, i)){
                //if the winner is favored?
                return true;
            }
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    //winner is the one nobody is preferred over
    bool is_winner = true;
    for(int i = 0; i < candidate_count; i++){
        for(int j = 0; j < candidate_count; j++){
            if(locked[j][i] == true){
                is_winner = false;
            }
        }
        if(is_winner){
            printf("Winner: %s\n", candidates[i]);
            return;
        }
    }
    return;
}
