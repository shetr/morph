#ifndef MORPH_JOB_MANAGER_HPP
#define MORPH_JOB_MANAGER_HPP

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace Morph {

class Job
{
    friend class JobManager;
public:
    virtual void Run() = 0;

    bool IsFinished() const { return _isFinished; }
private:
    void MarkAsFinished() { _isFinished = true; }

    bool _isFinished = false;
};

class JobManager
{
public:
    JobManager();
    ~JobManager();

    inline int GetMaxNumberOfWorkers() const { return _maxWorkers; }

    void SubmitJob(Job* job);
    void SubmitJobs(Job* jobBegin, Job* jobEnd);
    void WaitForJobsToFinish();
private:
    void WorkerLoop();

    int _maxWorkers = 1;
    std::vector<std::thread> _workers;
    std::queue<Job*> _jobsQueue;
    bool _shutdown = false;
    int _jobsInProgress = 0;
    std::mutex _jobsQueueMutex;
    std::condition_variable _jobsQueueMutexCondVar;
    std::condition_variable _waitFroJobsToFinishCondVar;
};

}

#endif // JOB_MANAGER_H