/* -----------------------------------------------------------------------------
* Author: Suchir Reddy Punuru
* Name: Cigarette Smokers Problem, for the OS(AI3102) course Lab 7
* Revision: 1.0
* Date: 27/11/23
* ------------------------------------------------------------------------------ */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SMOKERS 3                                                   // total number of smoker threads


/** Bools for items */
int tobacco = 0, paper = 0, match = 0;

// number convention
// 0 -> tobacco, 1 -> paper, 2 -> match
void *smoker(void * arg) {                                          // arg represnts which of the smokers specifically this thread represents
    int id = *(int*)arg;                                            // Convert argument to int for later

    while (1) {
        if( id == 0 ) {                                             // tobacco
            if(paper > 0 && match > 0) {                            // check for the presence of the items on the table

                paper = 0; match = 0;                               // consume items placed on the table
                printf("tobacco(0) is smoking\n");                  // represnts smoking
                sleep(2);
                printf("%d: consumed paper(1) and match(2)\n", id);

            }
            else {
                printf("%d: SCHEDULING ERROR: paper(1) or match(2) is not avaliable on the table\n", id);
            }
        }

        else if(id == 1) {                                          // paper

            if(tobacco > 0 && match > 0) {
                tobacco = 0; match = 0;
                printf("paper(1) is smoking\n");
                sleep(2);
                printf("%d: consumed tobacco(0) and match(2)\n", id);

            }
            else {
                printf("%d: SCHEDULING ERROR: tobacco(0) or match(2) is not avalible on the table\n", id);
            }
        }

        else if(id == 2) {                                          // match

            if(tobacco > 0 && paper > 0) {
                tobacco = 0; paper = 0;
                printf("mathc(2) is smoking\n");
                sleep(2);
                printf("%d: consumed tobacco(0) and paper(1)\n", id);

            }
            else {
                printf("%d: SCHEDULING ERROR: tobacco(0) or paper(1) is not avalible on the table\n", id);
            }
        }
        else {                                                      // Error case
            printf("ERROR: Invalid argument ID: %d\n", id);
            exit(1);
        }
    }
}

// scheduling agent that selects radom elements to place on the table
void *agent(void * arg) {
    while(1) {

        sleep(1);                                                   // Delay for better visability
        int id = rand() % 3;                                        // Generate a random ID for a random item

        // selecting 2 items is the same as not selecting 1 item
        if(id == 0) {                                               // tobacco
            printf("Agent sets paper(1) and match(2) on the table\n");

            paper++; match++;                                       // increment the boolean to indicate presence of the item

        }

        else if (id == 1) {                                         // paper
            printf("Agent sets tobacco(0) and match(2) on the table\n");

            tobacco++; match++;

        }

        else if(id == 2) {                                          // match
            printf("Agent sets tobacco(0) and paper(1) on the table\n");

            tobacco++; paper++;

        }

        else {
            printf("ERROR: wrong Agent ID: %d\n", id);
        }
        printf("----------------------------------------------------------------------------------------\n");
    }
}

/*
 * Main
 */
int main(int argc, char* argv[]) {

    // thread creation
    pthread_t tSmokers[SMOKERS], tAgent;
    int smokersID[SMOKERS];
    for(int i = 0; i < SMOKERS; i++) {
        smokersID[i] = i;
        pthread_create(&tSmokers[i], NULL, smoker, &smokersID[i]);
    }
    printf("Smokers created\n");
    pthread_create(&tAgent, NULL, agent, NULL);
    printf("Agent created\n");

    // thread joins
    // in reality these do not matter since the threads run indefinetly
    for (int i = 0; i < SMOKERS; i++)
        pthread_join(tSmokers[i], NULL);
    pthread_join(tAgent, NULL);

  exit(0);
}
