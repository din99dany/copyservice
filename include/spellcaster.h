#include "job.h"

int CreateSocket();
struct job CreateJob( const char* src, const char* dest);
void ChangeStatus( struct job* toCHange, int command );
void CancelJob( struct job* tomod);
void PauseJob( struct job* tomod);
void StartJob( struct job* tomod);
void PrinntJobWithId( int id );
void PrintJob( struct job toPrint);
void PrintAllJobs();