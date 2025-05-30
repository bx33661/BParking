#include "parking.h"

// 初始化停车场栈
void initStack(ParkingStack *stack) {
    stack->top = -1;
}

// 检查栈是否为空
bool isStackEmpty(ParkingStack *stack) {
    return stack->top == -1;
}

// 检查栈是否已满
bool isStackFull(ParkingStack *stack) {
    return stack->top == STACKSIZE - 1;
}

// 入栈操作
int push(ParkingStack *stack, Car car) {
    if (isStackFull(stack)) {
        return ERR_FULL; // 栈满，无法入栈
    }
    stack->data[++(stack->top)] = car;
    return SUCCESS; // 入栈成功
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
        printf("位置 %d: 车牌号 %s\n", i + 1, stack->data[i].plateNumber);
    }
}

// 初始化便道队列
void initQueue(WaitingQueue *queue) {
    queue->front = queue->rear = NULL;
    queue->count = 0;
}

// 检查队列是否为空
bool isQueueEmpty(WaitingQueue *queue) {
    return queue->front == NULL;
}

// 获取队列中的车辆数量
int getQueueCount(WaitingQueue *queue) {
    return queue->count;
}

// 入队操作
int enqueue(WaitingQueue *queue, Car car) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return ERR_MEMORY;
    }
    
    newNode->car = car;
    newNode->next = NULL;
    
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    
    queue->count++;
    return SUCCESS;
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
    queue->count--;
    return car;
}

// 清空队列并释放所有内存
void clearQueue(WaitingQueue *queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
    queue->count = 0;
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
        printf("位置 %d: 车牌号 %s\n", position++, current->car.plateNumber);
        current = current->next;
    }
}

// 创建车辆
Car createCar(const char *plateNumber) {
    Car newCar;
    strncpy(newCar.plateNumber, plateNumber, MAX_PLATE_LEN - 1);
    newCar.plateNumber[MAX_PLATE_LEN - 1] = '\0'; // 确保结尾
    newCar.arriveTime = time(NULL);
    newCar.leaveTime = 0;
    return newCar;
}

// 验证车牌号格式
bool isValidPlateNumber(const char *plateNumber) {
    // 检查长度
    size_t len = strlen(plateNumber);
    if (len < 5 || len > MAX_PLATE_LEN - 1) {
        return false;
    }
    
    // 这里可以添加更复杂的验证逻辑，比如符合中国车牌号格式
    // 目前只做简单检查，允许字母数字组合
    return true;
}

// 比较两个车牌号
bool comparePlateNumbers(const char *plate1, const char *plate2) {
    return strcmp(plate1, plate2) == 0;
}

// 计算停车费用
double calculateFee(Car car) {
    if (car.leaveTime == 0 || car.arriveTime == 0) {
        return 0.0;
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
    
    // 计算费用（每小时收费HOURLY_RATE元，不足一小时按一小时计算）
    double fee;
    if (hours == 0 && (minutes > 0 || seconds > 0)) {
        // 不足一小时按一小时收费
        fee = HOURLY_RATE;
    } else {
        fee = hours * HOURLY_RATE;
        // 如果有分钟或秒，则多收取一小时费用
        if (minutes > 0 || seconds > 0) {
            fee += HOURLY_RATE;
        }
    }
    
    // 打印停车费用信息
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                  停车费用计算单                             ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║ 车牌号: %-50s        ║\n", car.plateNumber);
    printf("║ 进入时间: %-48s        ║\n", arriveTimeStr);
    printf("║ 离开时间: %-48s        ║\n", leaveTimeStr);
    printf("║ 停车时长: %d 小时 %d 分钟 %d 秒%33s        ║\n", hours, minutes, seconds, "");
    printf("║ 收费标准: %.2f 元/小时%41s        ║\n", HOURLY_RATE, "");
    printf("║ 应收费用: %.2f 元%44s        ║\n", fee, "");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    
    return fee;
}

// 检查车牌号是否已存在于停车场或便道中
bool isCarExists(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber) {
    // 检查停车场
    for (int i = 0; i <= parkingLot->top; i++) {
        if (comparePlateNumbers(parkingLot->data[i].plateNumber, plateNumber)) {
            return true; // 车牌号已存在于停车场
        }
    }
    
    // 检查便道
    QueueNode *current = waitingLane->front;
    while (current != NULL) {
        if (comparePlateNumbers(current->car.plateNumber, plateNumber)) {
            return true; // 车牌号已存在于便道
        }
        current = current->next;
    }
    
    return false; // 车牌号不存在
}

// 车辆进入停车场
int parkCar(ParkingStack *parkingLot, WaitingQueue *waitingLane, const char *plateNumber) {
    // 检查车牌号是否已存在
    if (isCarExists(parkingLot, waitingLane, plateNumber)) {
        return ERR_EXISTS; // 车牌号已存在
    }
    
    // 创建新车辆
    Car newCar = createCar(plateNumber);
    
    if (!isStackFull(parkingLot)) {
        // 停车场有空位，直接进入
        return push(parkingLot, newCar);
    } else {
        // 停车场已满，进入便道等候
        return enqueue(waitingLane, newCar);
    }
}

// 查找车辆在停车场中的位置
int findCarPosition(ParkingStack *parkingLot, const char *plateNumber) {
    if (isStackEmpty(parkingLot)) {
        return -1;
    }
    
    for (int i = 0; i <= parkingLot->top; i++) {
        if (comparePlateNumbers(parkingLot->data[i].plateNumber, plateNumber)) {
            return i;
        }
    }
    
    return -1; // 未找到
}

// 车辆离开停车场
int leaveCar(ParkingStack *parkingLot, ParkingStack *tempLot, WaitingQueue *waitingLane, const char *plateNumber, SystemStats *stats) {
    if (isStackEmpty(parkingLot)) {
        return ERR_EMPTY; // 停车场为空
    }
    
    // 首先检查车辆是否在停车场内
    int position = findCarPosition(parkingLot, plateNumber);
    
    if (position == -1) {
        return ERR_NOT_FOUND; // 未找到车辆
    }
    
    // 计算需要移动的车辆数量
    int carsToMove = parkingLot->top - position;
    
    // 将车辆上方的车辆移到临时栈
    for (int i = 0; i < carsToMove; i++) {
        Car car = pop(parkingLot);
        push(tempLot, car);
    }
    
    // 移除要离开的车辆
    Car leavingCar = pop(parkingLot);
    leavingCar.leaveTime = time(NULL);
    
    // 计算费用并更新统计信息
    double fee = calculateFee(leavingCar);
    if (stats != NULL) {
        stats->totalCars++;
        stats->totalRevenue += fee;
    }
    
    // 将临时栈中的车辆移回停车场
    while (!isStackEmpty(tempLot)) {
        Car car = pop(tempLot);
        push(parkingLot, car);
    }
    
    // 如果便道上有等候的车辆，让其进入停车场
    if (!isQueueEmpty(waitingLane) && !isStackFull(parkingLot)) {
        Car waitingCar = dequeue(waitingLane);
        waitingCar.arriveTime = time(NULL); // 更新进入停车场的时间
        push(parkingLot, waitingCar);
    }
    
    return SUCCESS;
}

// 显示停车场状态
void displayParkingStatus(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                停车场管理系统当前状态                         ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ 时间: %-52s    \n", timeStr);
    printf("║ 停车场容量: %-46d    \n", STACKSIZE);
    printf("║ 停车场当前车辆数: %-40d    \n", parkingLot->top + 1);
    printf("║ 停车场剩余空位: %-42d    \n", STACKSIZE - (parkingLot->top + 1));
    printf("║ 便道等候车辆数: %-42d    ║\n", getQueueCount(waitingLane));
    
    if (stats != NULL) {
        double runTime = difftime(now, stats->startTime) / 3600.0; // 运行时间（小时）
        printf("║ 系统运行时间: %.1f 小时%42s ║\n", runTime, "");
        printf("║ 总处理车辆数: %-44d    ║\n", stats->totalCars);
        printf("║ 总收入: %-50.2f    ║\n", stats->totalRevenue);
    }
    
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    
    // 显示停车场内车辆
    printf("║ 停车场内车辆（从北到南）:                                     ║\n");
    if (isStackEmpty(parkingLot)) {
        printf("║ 停车场内没有车辆                                            ║\n");
    } else {
        for (int i = 0; i <= parkingLot->top; i++) {
            printf("║ 位置 %2d: 车牌号 %-46s    ║\n", i + 1, parkingLot->data[i].plateNumber);
        }
    }
    
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    
    // 显示便道等候车辆
    printf("║ 便道等候车辆:                                                 ║\n");
    if (isQueueEmpty(waitingLane)) {
        printf("║ 便道上没有等候车辆                                            ║\n");
    } else {
        QueueNode *current = waitingLane->front;
        int position = 1;
        while (current != NULL) {
            printf("║ 位置 %2d: 车牌号 %-46s    ║\n", position++, current->car.plateNumber);
            current = current->next;
        }
    }
    
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
}

// 初始化系统
void initSystem(SystemConfig *config, SystemStats *stats) {
    if (config != NULL) {
        config->parkingCapacity = STACKSIZE;
        config->hourlyRate = HOURLY_RATE;
        config->debugMode = false;
    }
    
    if (stats != NULL) {
        stats->totalCars = 0;
        stats->totalRevenue = 0.0;
        stats->startTime = time(NULL);
    }
}

// 显示系统统计信息
void displaySystemStats(SystemStats *stats) {
    if (stats == NULL) {
        printf("统计信息不可用\n");
        return;
    }
    
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char currentTimeStr[30];
    strftime(currentTimeStr, sizeof(currentTimeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    struct tm *startTimeInfo = localtime(&stats->startTime);
    char startTimeStr[30];
    strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", startTimeInfo);
    
    // 计算系统运行时间
    double runningTime = difftime(now, stats->startTime);
    int days = (int)(runningTime / 86400); // 86400 秒 = 1 天
    int hours = (int)((runningTime - days * 86400) / 3600);
    int minutes = (int)((runningTime - days * 86400 - hours * 3600) / 60);
    
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                停车场管理系统统计信息                         ║\n");
    printf("╠════════════════════════════════════════════════════════════════\n");
    printf("║ 当前时间: %-50s\n", currentTimeStr);
    printf("║ 系统启动时间: %-46s\n", startTimeStr);
    printf("║ 系统运行时间: %d 天 %d 小时 %d 分钟\n", days, hours, minutes);
    printf("║ 总处理车辆数: %-46d\n", stats->totalCars);
    printf("║ 总收入: %-52.2f\n", stats->totalRevenue);
    
    // 计算平均每小时收入
    double hoursRunning = runningTime / 3600.0;
    if (hoursRunning > 0) {
        double hourlyAverage = stats->totalRevenue / hoursRunning;
        printf("║ 平均每小时收入: %-44.2f  \n", hourlyAverage);
    }
    
    printf("╚════════════════════════════════════════════════════════════════\n\n");
}

// 保存系统状态到文件
void saveSystemState(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats) {
    FILE *file = fopen("parking_state.dat", "wb");
    if (file == NULL) {
        printf("无法创建保存文件！\n");
        return;
    }
    
    // 保存统计信息
    if (stats != NULL) {
        fwrite(stats, sizeof(SystemStats), 1, file);
    }
    
    // 保存停车场信息
    if (parkingLot != NULL) {
        // 保存停车场车辆数量
        int carCount = parkingLot->top + 1;
        fwrite(&carCount, sizeof(int), 1, file);
        
        // 保存停车场中的车辆
        for (int i = 0; i < carCount; i++) {
            fwrite(&parkingLot->data[i], sizeof(Car), 1, file);
        }
    }
    
    // 保存便道信息
    if (waitingLane != NULL) {
        // 保存便道车辆数量
        int queueCount = getQueueCount(waitingLane);
        fwrite(&queueCount, sizeof(int), 1, file);
        
        // 保存便道中的车辆
        QueueNode *current = waitingLane->front;
        while (current != NULL) {
            fwrite(&current->car, sizeof(Car), 1, file);
            current = current->next;
        }
    }
    
    fclose(file);
}

// 从文件加载系统状态
bool loadSystemState(ParkingStack *parkingLot, WaitingQueue *waitingLane, SystemStats *stats) {
    FILE *file = fopen("parking_state.dat", "rb");
    if (file == NULL) {
        return false; // 文件不存在或无法打开
    }
    
    // 清空当前状态
    initStack(parkingLot);
    clearQueue(waitingLane);
    
    // 加载统计信息
    if (stats != NULL) {
        if (fread(stats, sizeof(SystemStats), 1, file) != 1) {
            fclose(file);
            return false;
        }
    }
    
    // 加载停车场信息
    int carCount;
    if (fread(&carCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return false;
    }
    
    // 加载停车场中的车辆
    for (int i = 0; i < carCount; i++) {
        Car car;
        if (fread(&car, sizeof(Car), 1, file) != 1) {
            fclose(file);
            return false;
        }
        push(parkingLot, car);
    }
    
    // 加载便道信息
    int queueCount;
    if (fread(&queueCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return false;
    }
    
    // 加载便道中的车辆
    for (int i = 0; i < queueCount; i++) {
        Car car;
        if (fread(&car, sizeof(Car), 1, file) != 1) {
            fclose(file);
            return false;
        }
        enqueue(waitingLane, car);
    }
    
    fclose(file);
    return true;
}
