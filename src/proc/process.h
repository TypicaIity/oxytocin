#pragma once

// TODO: multithreading, symmetric multiprocessing

#define PROCESS_STACK_SIZE 4096

#define PROCESS_STATE_READY		0
#define PROCESS_STATE_RUNNING	1
#define PROCESS_STATE_BLOCKED	2
#define PROCESS_STATE_ZOMBIE	3

typedef struct {
	uintptr_t r15, r14, r13, r12, r11, r10, r9, r8;
	uintptr_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	
	uintptr_t rip, rsp, rflags;
} Context;

typedef struct Process {
	uint32_t id : 24;
	uint32_t state : 2;
	uint32_t flags : 6;

	int ec;
	void* stack;
	char name[32];
	
	Context ctx;

	struct Process* parent;
	struct Process* next;
} Process;

void process_init();

Process* process_create(const char* name, void(*entry)());

void process_exit(int ec);
int process_wait(Process* proc);

Process* process_getCurrent();
