/**
 *
 * @file sync.c
 * @author 智识之道
 * @brief 数据同步（涉及信号量、锁、多线程等）
 * @version 0.1
 * @date 2021-12-31
 *
 * @copyright Copyright (c) 2021
 *
 */

///////////////////////////////////////////////////////////////////////////////
//头文件区域

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

// TODO：请自行添加必要的头文件

///////////////////////////////////////////////////////////////////////////////
//宏定义区域

//缓冲区大小，测试的时候自己可以修改这个参数，建议不要设置那么大，小于10以内的方便观察。
#define BUFFER_SIZE 9
//随机休眠的秒数，这里设置为（缓冲区大小-1），即为随机休眠0～（缓冲区大小-1）秒
#define SECONDS (BUFFER_SIZE - 1)
//默认值
#define BUFFER_DEFAULT_VALUE -1

///////////////////////////////////////////////////////////////////////////////
//结构体区域

/**
 * @brief 命令行参数
 *
 */
typedef struct
{
    //生产者数量
    int producerNumber;
    //消费者数量
    int consumerNumber;
} CommandLineArgument;

/**
 * @brief 线程参数
 *
 */
typedef struct
{
    int threadPollIndex;
    pthread_t threadId;
    // TODO：请自行设计，如有需要自由添加成员
} ThreadParameter;

///////////////////////////////////////////////////////////////////////////////
//变量区域

//命令行参数
CommandLineArgument commandLineArgument;
//生产者线程参数，多个线程是一个数组，注意动态分配空间
ThreadParameter *producerThreadParameter;
//消费者线程参数，多个线程是一个数组，注意动态分配空间
ThreadParameter *consumerThreadParameter;
//缓冲区大小
int buffer[BUFFER_SIZE];
//生产者写缓冲区索引
int producerBufferIndex = 0;
//消费者读缓冲区索引
int consumerBufferIndex = 0;
//线程退出状态
//void *retval[BUFFER_SIZE];

// TODO：锁、信号量等必要的在这里自行添加

//producer semaphore
sem_t empty;
//consumer semaphore
sem_t full;

//mutex
pthread_mutex_t mutex;

///////////////////////////////////////////////////////////////////////////////
//函数区域

/**
 * @brief 解析命令行参数
 *
 * 先判断参数个数，不符合要求则打印使用说明，举例说明，并退出程序。
 * 参数个数正确情况，则解析参数，存入commandLineArgument结构体中。
 * 需要判断生产者、消费者数量，必须在1～BUFFER_SIZE之间，如果不符合则退出程序。
 *
 * @param argc 参数个数
 * @param argv 参数值数组
 */
void parseCommandLineArgument(int argc, char *argv[])
{
    int producerOrConsumerNumber = -1;
    
    if(argc != 3) {
        printf("Usage: %s <producer number> <consumer number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    producerOrConsumerNumber = atoi(argv[1]);
    if(producerOrConsumerNumber < 0 || producerOrConsumerNumber > BUFFER_SIZE) {
        exit(EXIT_FAILURE);
    }
    commandLineArgument.producerNumber = producerOrConsumerNumber;
    
    producerOrConsumerNumber = atoi(argv[2]);
    if(producerOrConsumerNumber < 0 || producerOrConsumerNumber > BUFFER_SIZE) {
        exit(EXIT_FAILURE);
    }
    commandLineArgument.consumerNumber = producerOrConsumerNumber;
    
    //printf("producernum : %d consumernum : %d\n", commandLineArgument.producerNumber, commandLineArgument.consumerNumber);
    //printf("TODO: parseCommandLineArgument\n");
}

/**
 * @brief 随机数发生器初始化
 *
 */
void initRandom()
{
    srand((unsigned int)time(NULL));
    //printf("TODO: initRandom\n");
}

/**
 * @brief 初始化信号量
 *
 * 需要初始化二个信号量：分别是标识缓冲区空、满的信号量。
 *
 */
void initSemaphore()
{
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    
    //printf("TODO: initSemaphore\n");
}

/**
 * @brief 初始化锁
 *
 */
void initLock()
{
    //mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&mutex, NULL);
    //printf("TODO: initLock\n");
}

/**
 * @brief 初始化缓冲区
 *
 * 默认都要赋值为-1
 *
 */
void initBuffer()
{
    int i = 0;
    
    for(; i < BUFFER_SIZE; i++) {
        buffer[i] = BUFFER_DEFAULT_VALUE;
    }
    
    //printf("TODO: initBuffer\n");
}

/**
 * @brief 打印出缓冲区当前的内容，方便调试用。
 *
 */
void dumpBuffer()
{
    int i = 0;
    
    printf("current buffer: ");
    
    for(; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    
    printf("\n");
    //printf("TODO: dumpBuffer\n");
}

/**
 * @brief 插入一个数据到缓冲区
 *
 * @param value 数据的值
 * @return 返回数据存在缓冲区的索引值
 */
int insertData(int value)
{
    buffer[producerBufferIndex] = value;
    producerBufferIndex++;
    //printf("TODO: insertData\n");
    if(producerBufferIndex >= 9) {
        producerBufferIndex = 0;
        return 8;
    }
    
    return producerBufferIndex - 1;
}

/**
 * @brief 从缓冲区中移除一个数据
 *
 * @param value 返回删除数据的值，方便后续打印使用
 * @return 返回数据移除的缓冲区的索引值
 */
int removeData(int *value)
{
    *value = buffer[consumerBufferIndex];
    buffer[consumerBufferIndex] = -1;
    consumerBufferIndex++;
    
    if(consumerBufferIndex >= 9) {
        consumerBufferIndex = 0;
        return 8;
    }
    //printf("TODO: removeData\n");

    return consumerBufferIndex - 1;
}

/**
 * @brief 生成随机休眠时间
 *
 * 随机休眠时间范围：0～（缓冲区大小-1）秒
 *
 * @return  休眠的秒数
 */
int generateRandomSleepTime()
{
    //printf("TODO: generateRandomSleepTime\n");
    
    return (rand() % (SECONDS + 1));
}

/**
 * @brief 生成随机值
 *
 * 值范围：任意正整数
 *
 * @return 任意正整数
 */
int generateRandomValue()
{
    //printf("TODO: generateRandomValue\n");    
    
    return rand();
}

/**
 * @brief 生产者线程执行的函数
 *
 * 1.休眠约定范围的随机秒数
 * 2.生成数据
 * 3.插入数据
 * 4.打印信息
 *
 * @see generateRandomSleepTime
 * @see generateRandomValue
 * @see insertData
 *
 * @param arg 生产者线程参数
 */
void *producerRoutine(void *arg)
{
    //printf("TODO: producerRoutine\n");
    int value;
    int index;
    ThreadParameter producer = *(ThreadParameter *)arg;
    
    while(1) {
        sleep(generateRandomSleepTime());
    
        value = generateRandomValue();
    
    
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        index = insertData(value);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    
        printf("Producer[%d] insert value 0x%08x into buffer[%d]\n", producer.threadPollIndex, value, index);
    }     
}

/**
 * @brief 消费者线程执行的函数
 *
 * 1.休眠约定范围的随机秒数
 * 2.插入数据
 * 3.打印信息
 *
 * @see generateRandomSleepTime
 * @see removeData
 *
 * @param arg 消费者线程参数
 */
void *consumerRoutine(void *arg)
{
    //printf("TODO: consumerRoutine\n");
    int value = 0;
    int index;
    ThreadParameter consumer = *(ThreadParameter *)arg; 
    
    while(1) {
        sleep(generateRandomSleepTime());
    
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        index = removeData(&value);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    
        printf("Consumer[%d] remove value 0x%08x from buffer[%d]\n", consumer.threadPollIndex, value, index);
    }    
}

/**
 * @brief 创建生产者线程
 *
 * 需要考虑一个或多个线程的情况
 *
 * @see producerRoutine
 *
 */
void createProducerThreads()
{
    //printf("TODO: createProducerThreads\n");
    const int num = commandLineArgument.producerNumber;
    int i;
    
    producerThreadParameter = (ThreadParameter *)malloc(num * sizeof(ThreadParameter));
    
    if(producerThreadParameter == NULL) {
        exit(EXIT_FAILURE);
    }
    
    for(i = 0;i < num; i++) {
        producerThreadParameter[i].threadPollIndex = i;
    }
    
    for(i = 0; i < num; i++) {
        pthread_create(&producerThreadParameter[i].threadId, NULL, (void *)&producerRoutine, (void *)&producerThreadParameter[i]);
        pthread_detach(producerThreadParameter[i].threadId);
    }
    
    
    
}

/**
 * @brief 创建消费者线程
 *
 * 需要考虑一个或多个线程的情况
 *
 * @see consumerRoutine
 *
 */
void createConsumerThreads()
{
    //printf("TODO: createConsumerThreads\n");
    const int num = commandLineArgument.consumerNumber;
    int i;
    
    consumerThreadParameter = (ThreadParameter *)malloc(num * sizeof(ThreadParameter));
    
    if(consumerThreadParameter == NULL) {
        exit(EXIT_FAILURE);
    }
    
    for(i = 0;i < num; i++) {
        consumerThreadParameter[i].threadPollIndex = i;
    }
    
    for(i = 0; i < num; i++) {
        pthread_create(&consumerThreadParameter[i].threadId, NULL, (void *)&consumerRoutine, (void *)&consumerThreadParameter[i]);
        pthread_detach(consumerThreadParameter[i].threadId);
    }
    
}

void handler(int sig)
{
    if(producerThreadParameter != NULL) {
        free(producerThreadParameter);
        producerThreadParameter = NULL;
    }
    
    if(consumerThreadParameter != NULL) {
        free(consumerThreadParameter);
        consumerThreadParameter = NULL;
    }
    
    _exit(0);
}

/**
 * @brief 最终要处理的内容
 *
 * 自行考虑细节。
 * 比如：
 * 怎么让进程保持运行
 * 要退出时释放资源
 *
 */
void finally()
{
    //printf("TODO: finally\n");
    
    signal(SIGINT, handler);
    
    while(1) {
        
    }
    
    exit(0);
}

/**
 * @brief 主函数入口
 *
 * 请不要在主函数里面添加内容。
 *
 * @param argc 参数个数
 * @param argv 参数数组
 * @return int
 */
int main(int argc, char *argv[])
{
    parseCommandLineArgument(argc, argv);
    initRandom();
    initSemaphore();
    initLock();
    initBuffer();
    createProducerThreads();
    createConsumerThreads();
    finally();

    return 0;
}