#pragma once

#include <SDL.h>
#include <SDL_thread.h>

#include <list>

class ThreadManager
{
	struct State {
		State() { thread=0; owner=0; waitCond=0; waitMutex=0; finished=false;}
		SDL_Thread* thread;
		ThreadManager* owner;
		SDL_cond* waitCond;
		SDL_mutex* waitMutex;
		bool finished;
	};
	std::vector<State> threads;

	struct WorkItem{
		void (*func)(void*);
		void *state;
	};

	std::list<WorkItem> todo;
	SDL_mutex* todoMutex;
	bool end;

	static int ThreadMain(void *state);

public:
	ThreadManager();
	~ThreadManager();

	void AddWorkItem(void (*func)(void*state), void* state);

	void Start(int n);
	void Stop();
	bool AllDone();
	void WaitForAllDone();
};

