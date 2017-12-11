
#include "structures.h"
#include "world_structure.h"
#include "read_value.h"

//road to master
int main(int argc, char** argv) {


    Config_t myConfig;
    if (initConfig(&myConfig)==NULL){
        fprintf(stderr,"\nCouldn't get config, aborting ...\n\n\n");
        exit(EXIT_FAILURE);
    }

    if (argc>1)myConfig.CELLS=(unsigned long)atol(argv[1]);
    if (argc>2)myConfig.STEPS=(unsigned long)atoi(argv[2]);
    if (argc>3)myConfig.EXPORT=(unsigned int)atoi(argv[3]);
    if (argc>4)myConfig.THREADS=(unsigned int)atoi(argv[4]);


    FILE* exportFile;
    pthread_barrier_init(&allBarrier,NULL,myConfig.THREADS+1);
    pthread_t monThread[myConfig.THREADS];
    pthread_attr_t attr;
    int currentStep = 0,threadRC,i=0;
    ThreadParam_t threadParams[myConfig.THREADS];
    void* status;

    struct timeval start, stop, export_start, export_stop;
    double startTime=0, stopTime=0, stackExport=0;
    gettimeofday(&start, NULL);
    startTime = (start.tv_sec*1E6+start.tv_usec)/1E6;





    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    printf("\n*******************\nNB cells: %lu\nNB steps: %lu\n*******************\n",myConfig.CELLS,myConfig.STEPS);

    clean_worlds_dir();
    exportFile = initExport(&myConfig);
    if (exportFile==NULL && myConfig.EXPORT==1){
        fprintf(stderr,"\nNo file to export into, aborting ...\n\n\n");
        closeExport(exportFile,&myConfig);
        exit(EXIT_FAILURE);
    }
    printf("\nCreating worlds\n");

    current_world = create_world_t(&myConfig);
    tempWorld = create_world_t(&myConfig);
    if (current_world==NULL || tempWorld==NULL){
        fprintf(stderr,"\nThe world couldn't be created, aborting ...\n\n\n");
        closeExport(exportFile,&myConfig);
        delete_world_t();
        exit(EXIT_FAILURE);
    }
    currentStep++;

    printf("\nProcessing and exporting worlds\n");

    for(i=0;i<myConfig.THREADS;i++){
        threadParams[i].world = current_world;
        threadParams[i].tempWorld= tempWorld;
        threadParams[i].config =&myConfig;
        threadParams[i].threadNum=i;
        threadRC= pthread_create(&monThread[i], &attr, all_steps, (void *) &threadParams[i]);
        if (threadRC) {
            fprintf(stderr,"Couldn't create threads\n");
            closeExport(exportFile,&myConfig);
            pthread_barrier_destroy(&allBarrier);
            delete_world_t();
            exit(EXIT_FAILURE);
        }
    }
    pthread_attr_destroy(&attr);
    while (currentStep < myConfig.STEPS){
        gettimeofday(&export_start,NULL);
        if(export_world_t(exportFile,&myConfig)==0){
            fprintf(stderr,"\nStep %d couldn't be exported, aborting ...\n\n\n",currentStep);
            closeExport(exportFile,&myConfig);
            pthread_barrier_destroy(&allBarrier);
            delete_world_t();
            exit(EXIT_FAILURE);
         }
        gettimeofday(&export_stop,NULL);
        stackExport += (export_stop.tv_sec*1E6 +export_stop.tv_usec)-(export_start.tv_sec*1E6 +export_start.tv_usec);
        pthread_barrier_wait(&allBarrier);
        currentStep++;
    }
    printf("\nExporting : %.3lf",stackExport/1E6);

    for(i=0; i<myConfig.THREADS; i++) {
        threadRC = pthread_join(monThread[i], &status);
        if (threadRC) {
            fprintf(stderr,"ERROR; return code from pthread_join() is %d\n", threadRC);
            closeExport(exportFile,&myConfig);
            pthread_barrier_destroy(&allBarrier);
            delete_world_t();
            exit(EXIT_FAILURE);
        }
    }

    closeExport(exportFile,&myConfig);
    pthread_barrier_destroy(&allBarrier);
    delete_world_t();

    gettimeofday(&stop, NULL);
    stopTime = (stop.tv_sec*1E6 +stop.tv_usec)/1E6;
    printf("\n*****************\nTotal -- %.3lf\n\n",stopTime-startTime);
  return 0;
}
