#include "TaskRunUserTask.h"
#include "App.h"

/*
 * userTask
 */
void userTask(void)
{
    App::get()->runUserTask();
}
