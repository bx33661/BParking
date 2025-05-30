#ifndef PARKING_H
#define PARKING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// 常量定义
#define STACKSIZE 10        // 停车场容量
#define MAX_PLATE_LEN 20    // 最大车牌号长度
#define HOURLY_RATE 10.0    // 每小时停车费率（元）

// 错误代码
#define SUCCESS 0
#define ERR_FULL -1
#define ERR_EMPTY -2
#define ERR_EXISTS -3
#define ERR_NOT_FOUND -4
#define ERR_MEMORY -5

// 车辆信息结构体
typedef struct {
    char plateNumber[MAX_PLATE_LEN]; // 车牌号（支持字母数字组合）
    time_t arriveTime;               // 到达时间
    time_t leaveTime;                // 离开时间
} Car;

// 停车场栈结构
typedef struct {
    Car data[STACKSIZE];
    int top;
} ParkingStack;

// 便道队列节点
typedef struct QueueNode {
    Car car;
    struct QueueNode *next;
} QueueNode;

// 便道队列
typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;         // 队列中的车辆数量
} WaitingQueue;

// 系统配置结构体
typedef struct {
    int parkingCapacity;  // 停车场容量
    double hourlyRate;    // 每小时费率
    bool debugMode;       // 调试模式
} SystemConfig;

// 系统统计信息
typedef struct {
    int totalCars;        // 总处理车辆数
    double totalRevenue;  // 总收入
    time_t startTime;     // 系统启动时间
} SystemStats;

// 停车场栈操作
void initStack(ParkingStack *stack);
bool isStackEmpty(ParkingStack *stack);
bool isStackFull(ParkingStack *stack);
int push(ParkingStack *stack, Car car);
Car pop(ParkingStack *stack);
void displayStack(ParkingStack *stack);

// 便道队列操作
void initQueue(WaitingQueue *queue);
bool isQueueEmpty(WaitingQueue *queue);
int enqueue(WaitingQueue *queue, Car car);
Car dequeue(WaitingQueue *queue);
void clearQueue(WaitingQueue *queue);
void displayQueue(WaitingQueue *queue);
int getQueueCount(WaitingQueue *queue);

// 车辆信息操作
Car createCar(const char *plateNumber);
bool isValidPlateNumber(const char *plateNumber);
bool comparePlateNumbers(const char *plate1, const char *plate2);

// 停车场管理操作
bool isCarExists(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber);
int parkCar(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber);
int findCarPosition(ParkingStack *parkingLot, const char *plateNumber);
int leaveCar(ParkingStack *parkingLot, ParkingStack *tempLot, WaitingQueue *waitingLane, const char *plateNumber, SystemStats *stats);
void displayParkingStatus(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats);
double calculateFee(Car car);

// 系统管理
void initSystem(SystemConfig *config, SystemStats *stats);
void saveSystemState(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats);
bool loadSystemState(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats);
void displaySystemStats(SystemStats *stats);

// 用户界面
void printMenu();
void clearInputBuffer();
int getMenuChoice();
char* getPlateNumber(char *buffer, size_t size);

#endif /* PARKING_H */
