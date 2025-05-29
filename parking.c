#include "parking.h"

// 初始化停车场栈
void initStack(ParkingStack *stack) {
    stack->top = -1;
}

// 检查栈是否为空
int isStackEmpty(ParkingStack *stack) {
    return stack->top == -1;
}

// 检查栈是否已满
int isStackFull(ParkingStack *stack) {
    return stack->top == STACKSIZE - 1;
}

// 入栈操作
int push(ParkingStack *stack, Car car) {
    if (isStackFull(stack)) {
        return 0; // 栈满，无法入栈
    }
    stack->data[++(stack->top)] = car;
    return 1; // 入栈成功
}

// 出栈操作
Car pop(ParkingStack *stack) {
    Car emptyCar = {0};
    if (isStackEmpty(stack)) {
        return emptyCar; // 栈空，返回空车
    }
    return stack->data[(stack->top)--];
}

// 显示停车场栈内容
void displayStack(ParkingStack *stack) {
    printf("停车场内车辆（从北到南）：\n");
    if (isStackEmpty(stack)) {
        printf("停车场内没有车辆\n");
        return;
    }
    
    for (int i = 0; i <= stack->top; i++) {
        printf("位置 %d: 车牌号 %d\n", i + 1, stack->data[i].carNumber);
    }
}

// 初始化便道队列
void initQueue(WaitingQueue *queue) {
    queue->front = queue->rear = NULL;
}

// 检查队列是否为空
int isQueueEmpty(WaitingQueue *queue) {
    return queue->front == NULL;
}

// 入队操作
void enqueue(WaitingQueue *queue, Car car) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    
    newNode->car = car;
    newNode->next = NULL;
    
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// 出队操作
Car dequeue(WaitingQueue *queue) {
    Car emptyCar = {0};
    if (isQueueEmpty(queue)) {
        return emptyCar; // 队列为空，返回空车
    }
    
    QueueNode *temp = queue->front;
    Car car = temp->car;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    return car;
}

// 清空队列并释放所有内存
void clearQueue(WaitingQueue *queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
}

// 显示便道队列内容
void displayQueue(WaitingQueue *queue) {
    printf("便道等候车辆：\n");
    if (isQueueEmpty(queue)) {
        printf("便道上没有等候车辆\n");
        return;
    }
    
    QueueNode *current = queue->front;
    int position = 1;
    
    while (current != NULL) {
        printf("位置 %d: 车牌号 %d\n", position++, current->car.carNumber);
        current = current->next;
    }
}

// 计算停车费用
void calculateFee(Car car) {
    if (car.leaveTime == 0 || car.arriveTime == 0) {
        return;
    }
    
    // 计算停车时间（秒）
    double parkingTime = difftime(car.leaveTime, car.arriveTime);
    
    // 将到达时间和离开时间转换为可读格式
    char arriveTimeStr[30];
    char leaveTimeStr[30];
    struct tm *arriveInfo = localtime(&car.arriveTime);
    struct tm *leaveInfo = localtime(&car.leaveTime);
    
    strftime(arriveTimeStr, sizeof(arriveTimeStr), "%Y-%m-%d %H:%M:%S", arriveInfo);
    strftime(leaveTimeStr, sizeof(leaveTimeStr), "%Y-%m-%d %H:%M:%S", leaveInfo);
    
    // 计算小时、分钟和秒
    int hours = (int)(parkingTime / 3600);
    int minutes = (int)((parkingTime - hours * 3600) / 60);
    int seconds = (int)(parkingTime - hours * 3600 - minutes * 60);
    
    // 计算费用（假设每小时收费10元，不足一小时按一小时计算）
    double fee;
    if (hours == 0 && (minutes > 0 || seconds > 0)) {
        // 不足一小时按一小时收费
        fee = 10.0;
    } else {
        fee = hours * 10.0;
        // 如果有分钟或秒，则多收取一小时费用
        if (minutes > 0 || seconds > 0) {
            fee += 10.0;
        }
    }
    
    printf("\n===== 停车费用计算 =====\n");
    printf("车牌号: %d\n", car.carNumber);
    printf("进入时间: %s\n", arriveTimeStr);
    printf("离开时间: %s\n", leaveTimeStr);
    printf("停车时间: %d小时 %d分钟 %d秒\n", hours, minutes, seconds);
    printf("停车费用: %.2f 元\n", fee);
    printf("==========================\n");
}

// 检查车牌号是否已存在于停车场或便道中
int isCarNumberExists(ParkingStack *parkingLot, WaitingQueue *waitingLane, int carNumber) {
    // 检查停车场
    for (int i = 0; i <= parkingLot->top; i++) {
        if (parkingLot->data[i].carNumber == carNumber) {
            return 1; // 车牌号已存在于停车场
        }
    }
    
    // 检查便道
    QueueNode *current = waitingLane->front;
    while (current != NULL) {
        if (current->car.carNumber == carNumber) {
            return 2; // 车牌号已存在于便道
        }
        current = current->next;
    }
    
    return 0; // 车牌号不存在
}

// 车辆进入停车场
void parkCar(ParkingStack *parkingLot, WaitingQueue *waitingLane, int carNumber) {
    // 检查车牌号是否已存在
    int existsStatus = isCarNumberExists(parkingLot, waitingLane, carNumber);
    if (existsStatus != 0) {
        if (existsStatus == 1) {
            printf("车牌号为 %d 的车辆已在停车场内，不能重复进入！\n", carNumber);
        } else {
            printf("车牌号为 %d 的车辆已在便道上等候，不能重复进入！\n", carNumber);
        }
        return;
    }
    
    Car newCar = {carNumber, time(NULL), 0};
    
    if (!isStackFull(parkingLot)) {
        // 停车场有空位，直接进入
        if (push(parkingLot, newCar)) {
            printf("车牌号为 %d 的车辆已进入停车场\n", carNumber);
        }
    } else {
        // 停车场已满，进入便道等候
        enqueue(waitingLane, newCar);
        printf("停车场已满，车牌号为 %d 的车辆在便道等候\n", carNumber);
    }
}

// 检查车辆是否在停车场内
int findCarPosition(ParkingStack *parkingLot, int carNumber) {
    if (isStackEmpty(parkingLot)) {
        return -1;
    }
    
    for (int i = 0; i <= parkingLot->top; i++) {
        if (parkingLot->data[i].carNumber == carNumber) {
            return i;
        }
    }
    
    return -1;
}

// 车辆离开停车场
void leaveCar(ParkingStack *parkingLot, ParkingStack *tempLot, WaitingQueue *waitingLane, int carNumber) {
    if (isStackEmpty(parkingLot)) {
        printf("停车场内没有车辆！\n");
        return;
    }
    
    // 首先检查车辆是否在停车场内
    int position = findCarPosition(parkingLot, carNumber);
    
    if (position == -1) {
        printf("停车场内没有车牌号为 %d 的车辆！\n", carNumber);
        return;
    }
    
    // 计算需要移动的车辆数量
    int carsToMove = parkingLot->top - position;
    printf("要让出 %d 辆车为车牌号 %d 的车辆让路\n", carsToMove, carNumber);
    
    // 将车辆上方的车辆移到临时栈
    for (int i = 0; i < carsToMove; i++) {
        Car car = pop(parkingLot);
        push(tempLot, car);
        printf("车牌号 %d 的车辆暂时移出停车场\n", car.carNumber);
    }
    
    // 移除要离开的车辆
    Car leavingCar = pop(parkingLot);
    leavingCar.leaveTime = time(NULL);
    calculateFee(leavingCar);
    printf("车牌号为 %d 的车辆已离开停车场\n", leavingCar.carNumber);
    
    // 将临时栈中的车辆移回停车场
    while (!isStackEmpty(tempLot)) {
        Car car = pop(tempLot);
        push(parkingLot, car);
        printf("车牌号 %d 的车辆返回停车场\n", car.carNumber);
    }
    
    // 如果便道上有等候的车辆，让其进入停车场
    if (!isQueueEmpty(waitingLane) && !isStackFull(parkingLot)) {
        Car waitingCar = dequeue(waitingLane);
        waitingCar.arriveTime = time(NULL); // 更新进入停车场的时间
        push(parkingLot, waitingCar);
        printf("车牌号为 %d 的车辆从便道进入停车场\n", waitingCar.carNumber);
    }
}

// 显示停车场状态
void displayParkingStatus(ParkingStack *parkingLot, WaitingQueue *waitingLane) {
    printf("\n===== 停车场当前状态 =====\n");
    displayStack(parkingLot);
    printf("\n");
    displayQueue(waitingLane);
    printf("==========================\n\n");
}
