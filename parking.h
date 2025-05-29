#ifndef PARKING_H
#define PARKING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 停车场容量
#define STACKSIZE 10

// 车辆信息结构体
typedef struct {
    int carNumber;      // 车牌号
    time_t arriveTime;  // 到达时间
    time_t leaveTime;   // 离开时间
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
} WaitingQueue;

// 停车场栈操作
void initStack(ParkingStack *stack);
int isStackEmpty(ParkingStack *stack);
int isStackFull(ParkingStack *stack);
int push(ParkingStack *stack, Car car);
Car pop(ParkingStack *stack);
void displayStack(ParkingStack *stack);

// 便道队列操作
void initQueue(WaitingQueue *queue);
int isQueueEmpty(WaitingQueue *queue);
void enqueue(WaitingQueue *queue, Car car);
Car dequeue(WaitingQueue *queue);
void clearQueue(WaitingQueue *queue);
void displayQueue(WaitingQueue *queue);

// 停车场管理操作
int isCarNumberExists(ParkingStack *parkingLot, WaitingQueue *waitingLane, int carNumber);
void parkCar(ParkingStack *parkingLot, WaitingQueue *waitingLane, int carNumber);
int findCarPosition(ParkingStack *parkingLot, int carNumber);
void leaveCar(ParkingStack *parkingLot, ParkingStack *tempLot, WaitingQueue *waitingLane, int carNumber);
void displayParkingStatus(ParkingStack *parkingLot, WaitingQueue *waitingLane);
void calculateFee(Car car);

#endif /* PARKING_H */
