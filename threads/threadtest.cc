// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"

// testnum is set in main.cc
int testnum = 5;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    /*for (num = 0; num < 5; num++) {
	//printf("*** thread %d looped %d times\n", which, num);
	    printf("**************** 线程名字:%s 用户id:%d  线程id:%d 循环了(%d)次....***********************\n ",
				currentThread->getName(),currentThread->getUserID(),currentThread->getThreadID(), num+1);
	currentThread->Yield();
    }*/
	for(num = 0 ; num < 5 ; num ++){
	    printf("=====线程名字:%s===线程优先级:%d===循环了%d次=====\n",
				currentThread->getName() , currentThread->getPriority() , num+1);
	}
    
	
}
//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

/*void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

	
    t->Fork(SimpleThread, (void*)1);
    
    SimpleThread(0);
}*/
void 
ThreadTest2(){
	DEBUG('t', "进入线程测试2...");
    Thread *t1 = new Thread("线程1");

    Thread *t2 = new Thread("线程2");

    Thread *t3 = new Thread("线程3");

    Thread *t4 = new Thread("线程4");
	//printf("t1的id:%d\n",t1->getThreadID());
	t1->Fork(SimpleThread,(void*)(t1->getThreadID()));

	t2->Fork(SimpleThread,(void*)(t2->getThreadID()));

	t3->Fork(SimpleThread,(void*)(t3->getThreadID()));

	t4->Fork(SimpleThread,(void*)(t4->getThreadID()));

	SimpleThread(0);
}
void 
ReadyListPrint(int what){
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("执行线程 ======》name:%s , threadID:%d , status:%s \n",currentThread->getName() , currentThread->getThreadID() , 
			ThreadStatusInChar[currentThread->getStatus()]);
	List *list = new List();
	list = scheduler->getReadyList();

	if(!list->IsEmpty()){
	    list->Mapcar(MyThreadPrint);//打印readylist队列中的就绪线程....``
	}
	currentThread->Yield();
	interrupt->SetLevel(oldLevel);
}

/*void
ReadyListPrint2(int what){
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf();.............
	List *list = new List();
	list = scheduler->getReadyList();
	if(!list->IsEmpty()){
	    list->Mapcar(MyThreadPrint);
	}
	currentThread->Yield();
	interrupt->SetLevel(oldLevel);
}*/

void 
ThreadTest3(){
    DEBUG('t' , "Entering Test3----------");
	Thread *t1 = new Thread("线程1");
	Thread *t2 = new Thread("线程2");
	Thread *t3 = new Thread("线程3");
    
	t1->Fork(ReadyListPrint , 0);
	t2->Fork(ReadyListPrint , 0);
	t3->Fork(ReadyListPrint , 0);
}
void
ThreadLimit(){
    DEBUG("t","线程极限测试...");
	int count = 0;
	for (int i = 0 ; i <= 128 ; i ++){
	    Thread *t = new Thread("forked thread");
		//t->Fork(SimpleThread , (void*)(t->getThreadID()));
		count++;
		//SimpleThread(count);
	    printf("线程名字:%s , 线程id:%d , UserId:%d\n",t->getName(),t->getThreadID() , t->getUserID());
	}
}
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

//线程优先级测试
void p1(int which);
void p2(int whic);
void 
ThreadTestPriority(){
    Thread *t1 = new Thread("线程1",3);

    //Thread *t2 = new Thread("线程2",3);
	
    //Thread *t3 = new Thread("线程3",4);


	t1->Fork(p1,(void *)(t1->getThreadID()));

	//t2->Fork(SimpleThread,(void*)(t2->getThreadID()));

	//t3->Fork(SimpleThread,(void*)(t3->getThreadID()));

}
void p1(int which){
	
    int num;
	for(num = 0 ; num < 5 ; num ++){
		printf("=====线程名字:%s===线程ID:%d===线程优先级:%d===循环了%d次=====\n",
				currentThread->getName(),currentThread->getThreadID() , currentThread->getPriority() , num+1);
	    if(num == 0){
		    Thread *t2 = new Thread("线程2",2);
			t2->Fork(p2 , (void *)(t2->getThreadID()));
		}
		
	}
}
void p2(int which){
    int num;
	
	for(num = 0 ; num < 5 ; num ++){
		printf("=====线程名字:%s===线程优先级:%d===循环了%d次=====\n",
				currentThread->getName() , currentThread->getPriority() , num+1);
	    if(num == 0){
		    Thread *t3 = new Thread("线程3",1);
			t3->Fork(SimpleThread , (void *)(t3->getThreadID()));
		}
		
	}
}
//============================================================
/*
 *
 *下面是生产者消费者问题...
 *
 * */
/*int food_nums = 0;//定义最初的食品数量为0
int buffer = 0;//缓存区最初为0
int max = 5;//存放物品的最大值5
int get_nums = 0;//消费者获得的产品数量
Lock* lock;//锁变量

Condition* condition;//条件变量
void produce_item(){
    food_nums ++; //生产一个产品
}
void put_item(){
    buffer ++;  //放入缓存区
}
int get_item(){
    return buffer;//获取缓存区食物数量
}

void producer(int nums){
	for(int i = 0 ; i < nums ; i ++){
	    produce_item();//生产产品
		lock->Acquire();//获得锁
		if(food_nums > max){
	    	printf("产品装满了.....");
			food_nums == 1;
			condition->Wait();
		}else{
			put_item();//放入缓存区
			printf("将产品(%d)放入缓存区...,缓存区数量为(%d)",i+1 , buffer);
			if(buffer == 1){
			    condition->Signal();//如果是第一个产品
			}
		}
	lock->Release();//释放锁
	}
}


void consumer(int nums){
    //消费者线程
	for(int i = 0 ; i < nums ; i ++){
	    lock->Acquire();
		if(buffer == 0){
		    condition->Wait();
		}else{
		    get_nums++;
			printf("消费者消费了第%d个...\n",i+1);
			buffer--;
			if(get_nums == max){
				printf("消费达到最大值%d,等待生产者生产...\n",max);
			    condition->Signal();//唤醒等待的生产者线程...
			}
		}

		lock->Release();
	}
}

void conditionTest(){
    lock = new Lock("MyLock");
	condition = new Condition("MyCondition");
	Thread *t1 = new Thread("生产者线程...");
	Thread *t2 = new Thread("消费者线程...");
	t1->Fork(producer , 8);
	t2->Fork(consumer , 10);
}
*/
//*****************************************************************

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	printf("===运行了Test2=====\n");
	ThreadTest2();
	break;
	case 2:
	printf("=====运行了极限测试11=======\n");
	//ThreadTest2();
	ThreadLimit();
	break;
	case 4:
	printf("=========3333===========\n");
    ThreadTest3();	
	break;
	case 5:
	printf("=========线程优先级测试==========\n");
	ThreadTestPriority();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

