﻿#include "stdafx.h"
#include "gmthreads.h"

GMThread::GMThread()
{
	D(d);
	d->state = NotRunning;
	d->callback = nullptr;
}

static DWORD WINAPI gmthread_run(LPVOID lpThreadParameter)
{
	GMThread* thread = static_cast<GMThread*>(lpThreadParameter);
	thread->d()->state = Running;
	if (thread->d()->callback)
		thread->d()->callback->beforeRun(thread);
	thread->run();
	if (thread->d()->callback)
		thread->d()->callback->afterRun(thread);
	thread->d()->state = Finished;
	thread->d()->event.set();
	return 0;
}

void GMThread::start()
{
	D(d);
#if _WINDOWS
	d->event.reset();
	d->handle = ::CreateThread(NULL, NULL, gmthread_run, this, 0, 0);
	if (d->callback)
		d->callback->onCreateThread(this);
#endif
}

void GMThread::wait(GMuint milliseconds)
{
	D(d);
#if _WINDOWS
	::WaitForSingleObject(d->handle, milliseconds == 0 ? INFINITE : milliseconds);
#endif
}

void GMThread::setCallback(IThreadCallback* callback)
{
	D(d);
	d->callback = callback;
}

void GMThread::terminate()
{
	D(d);
#if _WINDOWS
	::TerminateThread(d->handle, 0);
#endif
}

GMThread::Data* GMThread::d()
{
	D(d);
	return d;
}

GMThreadId GMThread::getCurrentThreadId()
{
#if _WINDOWS
	return ::GetCurrentThreadId();
#endif
}

GMSustainedThread::GMSustainedThread()
{
	D(d);
	d->terminate = false;
	d->terminateEvent.reset();
	d->innerEvent.reset();
}

GMSustainedThread::~GMSustainedThread()
{
	D(d);
	d->innerEvent.set();
	terminate();
}

void GMSustainedThread::run()
{
	D(d);
	while (!d->terminate)
	{
		d->innerEvent.wait();
		sustainedRun();
		d->outterEvent.set();
		d->innerEvent.reset();
	}
	d->terminateEvent.set();
}

void GMSustainedThread::wait(GMint milliseconds)
{
	D(d);
	d->outterEvent.wait(milliseconds);
}

void GMSustainedThread::trigger()
{
	D(d);
	d->innerEvent.set();
	d->outterEvent.reset();
}

void GMSustainedThread::stop()
{
	D(d);
	d->terminate = true;
}

GMJobPool::~GMJobPool()
{
	D(d);
	for (auto iter = d->threads.begin(); iter != d->threads.end(); iter++)
	{
		delete (*iter);
	}
}

void GMJobPool::addJob(AUTORELEASE GMThread* thread)
{
	D(d);
	d->threads.push_back(thread);
	thread->setCallback(this);
	thread->start();
}

void GMJobPool::waitJobs(GMuint milliseconds)
{
	D(d);
#if _WINDOWS
	::WaitForMultipleObjects(d->handles.size(), d->handles.data(), TRUE, milliseconds == 0 ? INFINITE : milliseconds);
#endif
}

void GMJobPool::onCreateThread(GMThread* thread)
{
	D(d);
	d->handles.push_back(thread->d()->handle);
}

void GMJobPool::afterRun(GMThread* thread)
{
}

void GMJobPool::beforeRun(GMThread* thread)
{
}