#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for(int i = 0; i < candidate_count; i++){
        printf("Candidate: %s\n", candidates[i].name);
        printf("Input: %s\n", name);
        if(strcmp(candidates[i].name, name) == 0){
            candidates[i].votes += 1;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int max = 0;
    int winners = 0;
    string max_names[candidate_count];

    for(int i = 0; i < candidate_count; i++){
        //for each candidate in the list of candidates
        if(candidates[i].votes > max){
            //if the number of votes that candidate has is larger than the previous highest number
            //the new max is that number of votes
            //we now have 1 winner
            //we set the first name in max_names to be the candidate that has the most name
            max = candidates[i].votes;
            winners = 1;
            max_names[0] = candidates[i].name;
        }else if(candidates[i].votes == max){//if the candidate votes are tied
            //increase the number of winners
            //set the next index
            max_names[winners] = candidates[i].name;
            winners += 1;
        }
    }
    for(int i = 0; i < winners; i++){
        printf("Winner: %s\n", max_names[i]);//either referencing out of bounds or referencing null?
    }
    return;
}
