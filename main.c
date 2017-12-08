
#include <sys/types.h>
#include <time.h>
#include "structures.h"
#include "world_structure.h"
#include "read_value.h"
//road to master
int main(int argc, char** argv) {

    pthread_barrier_init(&allBarrier,NULL,NB_THREAD+1);
    Config_t myConfig;

    if (initConfig(&myConfig)==NULL){
        fprintf(stderr,"\nCouldn't get config, aborting ...\n\n\n");
        exit(EXIT_FAILURE);
    }
    pthread_t monThread[NB_THREAD];

    pthread_attr_t attr;
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (argc>1)myConfig.CELLS=(unsigned int)atoi(argv[1]);
    if (argc>2)myConfig.STEPS=(unsigned int)atoi(argv[2]);
    int currentStep = 0,threadRC;
    int i=0;
    int temp,j;
    unsigned long a ,b;
    ThreadParam_t threadParams[NB_THREAD];
    void* status;
    time_t startTime;
    clean_worlds_dir();

    printf("\nNB cells: %d\nNB steps: %d\n",myConfig.CELLS,myConfig.STEPS);
    printf("Cleaning worlds folder\n");

    printf("\nProcessing and exporting worlds\n");

#ifdef TRACE
    printf("\nProcessing step and file writing step %d",currentStep);
#endif
    startTime =time(NULL);
    World_t my_world = create_world_t(&myConfig);
    World_t tempWorld = create_world_t(&myConfig);
    if (my_world==NULL){
        fprintf(stderr,"\nThe world couldn't be created, aborting ...\n\n\n");
        delete_world_t(my_world,&myConfig);
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    if(export_world_t(my_world,currentStep ,&myConfig)==0){
        fprintf(stderr,"\nStep %d couldn't be exported, aborting ...\n\n\n",currentStep);
        delete_world_t(my_world,&myConfig);
        exit(EXIT_FAILURE);
    }
    currentStep++;

    for(i=0;i<NB_THREAD;i++){
        threadParams[i].world = my_world;
        threadParams[i].tempWorld= tempWorld;
        threadParams[i].config =&myConfig;
        threadParams[i].threadNum=i;

        threadRC= pthread_create(&monThread[i], &attr, all_steps, (void *) &threadParams[i]);
        printf("thread %d launched\n",i);
        if (threadRC) {
            fprintf(stderr,"ERROR; return code from pthread_create() is %d\n", threadRC);
            exit(EXIT_FAILURE);
        }
    }
    pthread_attr_destroy(&attr);
    while (currentStep < myConfig.STEPS){
      //  printf("\nProcessing step and file writing step %d\n",currentStep);
        pthread_barrier_wait(&allBarrier);
        if(export_world_t(tempWorld,currentStep ,&myConfig)==0){
            fprintf(stderr,"\nStep %d couldn't be exported, aborting ...\n\n\n",currentStep);
            exit(EXIT_FAILURE);
        }
        currentStep++;
    }

    for(i=0; i<NB_THREAD; i++) {
        threadRC = pthread_join(monThread[i], &status);
        if (threadRC) {
            fprintf(stderr,"ERROR; return code from pthread_join() is %d\n", threadRC);
            exit(EXIT_FAILURE);
        }
    }

    a = startTime;
    b = (unsigned long)time(NULL);
    printf(" -- %lu s\n",b-a);
    pthread_barrier_destroy(&allBarrier);
    delete_world_t(my_world,&myConfig);
    delete_world_t(tempWorld,&myConfig);
  return 0;
}
