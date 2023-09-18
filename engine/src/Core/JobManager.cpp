#include "JobManager.hpp"

namespace Morph {

JobManager::JobManager()
{
    _maxWorkers = std::max(1, (int)std::thread::hardware_concurrency());
    _workers.resize(_maxWorkers);
    for (int i = 0; i < _maxWorkers; i++) {
        _workers[i] = std::thread(&JobManager::WorkerLoop, this);
    }
}

JobManager::~JobManager()
{
    {
        std::unique_lock<std::mutex> lock(_jobsQueueMutex);
        _shutdown = true;
    }
    _jobsQueueMutexCondVar.notify_all();
    for (std::thread& worker : _workers) {
        worker.join();
    }
    _workers.clear();
}

void JobManager::SubmitJob(Job* job)
{
    {
        std::unique_lock<std::mutex> lock(_jobsQueueMutex);
        _jobsQueue.push(job);
        ++_jobsInProgress;
    }
    _jobsQueueMutexCondVar.notify_one();
}

void JobManager::SubmitJobs(Job* jobBegin, Job* jobEnd)
{
    {
        std::unique_lock<std::mutex> lock(_jobsQueueMutex);
        for (Job* job = jobBegin; jobBegin != jobEnd; ++jobBegin) {
            _jobsQueue.push(job);
            ++_jobsInProgress;
        }
    }
    _jobsQueueMutexCondVar.notify_all();
}

void JobManager::WaitForJobsToFinish()
{
    std::unique_lock<std::mutex> lock(_jobsQueueMutex);
    _waitFroJobsToFinishCondVar.wait(lock, [&] { return _jobsInProgress == 0; });
}

void JobManager::WorkerLoop()
{
    while (!_shutdown)
    {
        Job* job;
        {
            std::unique_lock<std::mutex> lock(_jobsQueueMutex);
            _jobsQueueMutexCondVar.wait(lock, [&] { return _shutdown || !_jobsQueue.empty(); });
            if (_shutdown)
                return;
            if (_jobsQueue.empty())
                continue;
            job = _jobsQueue.front();
            _jobsQueue.pop();
        }
        job->Run();
        job->MarkAsFinished();
        {
            std::unique_lock<std::mutex> lock(_jobsQueueMutex);
            --_jobsInProgress;
        }
        _waitFroJobsToFinishCondVar.notify_one();
    }
}

}