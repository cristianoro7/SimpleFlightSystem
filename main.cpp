#include <iostream>
#include <printf.h>
#include <cstring>

#define OK 1;
typedef int Status;
#define OVERFLOW -1;
#define ERROR 0;

/**
 * 候补客户类型
 */
typedef struct {
    char name[15]; //名字
    int book_count; //订票数量
} WaitingCustomerType;

/**
 * 候补队列
 */
typedef struct LQNode {
    WaitingCustomerType waitingCustomerType;
    LQNode *next;
} LQNode;

typedef struct {
    LQNode *front; //头指针
    LQNode *rear; //尾指针
} waiting_customer;

/**
 * 顾客类型
 */
typedef struct {
    char name[15];
    int book_mount;// 订票数
    int rank; //舱位等级
    char destination[15]; //目的地
    int ticket_number[3]; //每位顾客最多只能定三张票,ticket_number对应的是座位
} customer;

typedef struct LNode {
    customer cust;
    struct LNode *next;
} LNode, *Customer_List;

/**
 * 航线信息
 */
typedef struct {
    char destination[15]; //终点站名字
    char line_number[15]; //航班号
    char plane_number[15]; //飞机号
    char start[15]; //飞行日期
    int capacity; //乘员人数
    int rest_of_tickets; //余票量
    Customer_List has_book_ticket; //已定的客户
    waiting_customer waiting_ticket; //替补人员
} line_info;

/**
 * 航线表
 */
typedef struct {
    line_info *elem;
    int length;
    int size;
    int increment;
} Line_List;

/**
 * 初始化替补客户队列
 * @param w 替补客户队列
 * @return OK表示初始化成功
 */
Status initWaitingCustomerQueue(waiting_customer &w) {
    w.front = w.rear = NULL;
    return OK;
}

/**
 * 往替补客户队列中添加元素
 * @param in 航线
 * @param type 替补客户的信息
 * @return OK表示添加成功, OVERFLOW表示分配结点内存空间失败
 */
Status addWaitingQueue(line_info &in, WaitingCustomerType type) {
    LQNode *q;
    q = (LQNode *) malloc(sizeof(LQNode));
    if (q == NULL) {
        return OVERFLOW;
    }
    q->waitingCustomerType = type;
    q->next = NULL;
    if (NULL == in.waiting_ticket.front) { //插入空队列中
        in.waiting_ticket.front = q;
    } else {
        in.waiting_ticket.rear->next = q;
    }
    in.waiting_ticket.rear = q;
    return OK;
}

/**
 * 删除队列头结点
 * @param in 航线
 * @return OK表示删除成功, ERROR表示队列为空,删除失败
 */
Status deleteWaitingQueue(line_info &in) {
    LQNode *p;
    if (in.waiting_ticket.front == NULL) {
        return ERROR;
    }
    p = in.waiting_ticket.front;
    in.waiting_ticket.front = p->next;
    if (in.waiting_ticket.rear == NULL) {
        in.waiting_ticket.rear = NULL;
    }
    free(p);
    return OK;
}

/**
 * 删除候补队列中制定的候补客户
 * @param in 航线
 * @param waitingCustomer 指定删除的候补客户
 */
void deleteWaitingCustomer(line_info &in, WaitingCustomerType waitingCustomer) {
    LQNode *p = in.waiting_ticket.front;
    LQNode *q = NULL;
    while (p != NULL) {
        if (p->waitingCustomerType.book_count == waitingCustomer.book_count &&
            strcmp(p->waitingCustomerType.name, waitingCustomer.name) == 0) {
            if (q == NULL) { //删除队列的队头
                deleteWaitingQueue(in);
                return;
            }
            q->next = p->next;
            free(p);
            return;
        }
        q = p;
        p = p->next;
    }
}

/**
 * 初始化航线的线性表
 * @param L 储存航线的线性表
 * @param size 线性表的容量
 * @param inc 容量不足时，扩容的大小
 * @return OK表示初始化成功， OVERFLOW表示分配内存失败
 */
Status InitList(Line_List &L, int size, int inc) {

    L.elem = (line_info *) malloc(size * sizeof(line_info));
    if (L.elem == NULL) {
        return OVERFLOW;
    }

    L.size = size;
    L.length = 0;
    L.increment = inc;
    return OK;
}

/**
 * 添加航线
 * @param L 储存航线的线性表
 * @param line 要添加的航线
 * @return OK表示初始化成功， OVERFLOW表示分配内存失败
 */
Status addAirLine(Line_List &L, line_info line) {

    if (L.elem == NULL) {
        return OVERFLOW;
    }

    line_info *newbase;
    if (L.length >= L.size) {
        newbase = (line_info *) malloc((L.size + L.increment) * sizeof(line_info));
        if (newbase == NULL) {
            return OVERFLOW;
        }
        L.elem = newbase;
        L.size = L.size + L.increment;
    }
    L.elem[L.length] = line;
    L.length++;
    return OK;
}

/**
 * 利用航线目的地来搜索航线
 * @param L 航线表
 * @param destination 航线目的地
 * @return 404代表找不到 i对应的航线索引
 */
int searchAirLineByDestination(Line_List L, char destination[]) {
    int i;
    for (i = 0; i < L.length; i++) {
        if (strcmp(L.elem[i].destination, destination) == 0) {
            return i;
        }
    }
    return 404;
}

/**
 * 通过航班号来查找航线
 * @param L 航线表
 * @param line_number 航线号
 * @return 404:找不到对应的航线, i:航线对应的索引
 */
int searchAirLineByLineNumber(Line_List L, char line_number[]) {
    int i;
    for (i = 0; i < L.length; i++) {
        if (strcmp(L.elem[i].line_number, line_number) == 0) {
            return i;
        }
    }
    return 404;
}

/**
 * 通过飞机号来搜索航线
 * @param L 航线表
 * @param plane_number 飞机号
 * @return 404:找不到对应航线, i:航线对应的索引
 */
int searchAirLineByPlaneNumber(Line_List L, char plane_number[]) {
    int i;
    for (i = 0; i < L.length; i++) {
        if (strcmp(L.elem[i].plane_number, plane_number) == 0) {
            return i;
        }
    }
    return 404;
}

/**
 * 生成航线
 * @param des 航线目的地
 * @param number 航线号
 * @param plane_number 飞机号
 * @param start 飞行日期
 * @param capacity 容量
 * @param left 剩下的位置
 * @return 根据航线信息创建的航线
 */
line_info makeAirLine(char des[], char number[], char plane_number[], char start[], int capacity, int left) {
    line_info in;
    strcpy(in.destination, des);
    strcpy(in.line_number, number);
    strcpy(in.plane_number, plane_number);
    strcpy(in.start, start);
    in.capacity = capacity;
    in.rest_of_tickets = left;
    in.has_book_ticket = NULL;
    initWaitingCustomerQueue(in.waiting_ticket); //初始化候补客户队列
    return in;
}

/**
 * 创建默认的航空路线
 *
 */
void createDefaultAirLine(Line_List &L) {
    addAirLine(L, makeAirLine("广州", "000001", "fl000001", "星期一", 3, 3));
    addAirLine(L, makeAirLine("厦门", "000002", "fl000002", "星期二", 40, 40));
    addAirLine(L, makeAirLine("武汉", "000003", "fl000003", "星期三", 40, 40));
    addAirLine(L, makeAirLine("三亚", "000004", "fl000004", "星期四", 40, 40));
    addAirLine(L, makeAirLine("惠州", "000005", "fl000005", "星期五", 40, 40));
    addAirLine(L, makeAirLine("上海", "000006", "fl000006", "星期六", 40, 40));
    addAirLine(L, makeAirLine("澳门街", "000007", "fl000007", "星期日", 40, 40));
    addAirLine(L, makeAirLine("马德里", "000008", "fl000008", "星期一", 40, 40));
    addAirLine(L, makeAirLine("巴黎", "000009", "fl000009", "星期二", 40, 40));
    addAirLine(L, makeAirLine("布达拉宫", "000010", "fl000010", "星期二", 40, 40));
}

/**
 * 创建已定票的客户链表结点
 * @param elem 客户
 * @return 指向该客户的指针
 */
LNode *MakeNode(customer elem) {

    LNode *p;
    p = (LNode *) malloc(sizeof(LNode));
    if (p != NULL) {
        p->cust = elem;
        p->next = NULL;
    }
    return p;
}

/**
 * 初始化订票客户链表头
 * @param L 航线表
 */
void initCustomerHeader(Line_List &L) {
    int i;
    for (i = 0; i < L.length; i++) {
        L.elem[i].has_book_ticket = (LNode *) malloc(sizeof(LNode));
        L.elem[i].has_book_ticket->next = NULL;
    }
}

/**
 * 向已订票的客户链表中插入顾客
 * @param start 链表头
 * @param waitToInsert 需要插入的顾客
 */
void insert(Customer_List &start, LNode *waitToInsert) {
    if (waitToInsert == NULL) {
        return;
    }

    LNode *p = start->next;
    LNode *q = start;
    if (p == NULL) {
        start->next = waitToInsert; //插入到链表头
        return;
    }

    while (p != NULL) {
        int flag = strcmp(p->cust.name, waitToInsert->cust.name);
        printf("flag is : %d\n", flag);
        if (flag >= 0) {
            q->next = waitToInsert;
            waitToInsert->next = p;
            return;
        }
        q = p;
        p = p->next;
    }
    q->next = waitToInsert; //插入到表尾的情况;
}

/**
 * 删除已定票的顾客
 * @param in 航线
 * @param name 要删除的顾客名字
 * @param c 要候补进来的顾客
 * @return OK代表成功; ERROR代表失败
 */
Status deleteCustomer(line_info &in, char name[15], customer &c) {

    LNode *p = in.has_book_ticket->next;
    LNode *q = in.has_book_ticket;
    while (p != NULL) {
        if (strcmp(p->cust.name, name) == 0) { //遍历链表
            in.rest_of_tickets += p->cust.book_mount;
            q->next = p->next;
            c = p->cust;
            free(p);
            return OK;
        }
        q = p;
        p = p->next;
    }
    return ERROR;
}

/**
 * 向航线中添加顾客
 * @param L 航线表
 * @param elem 要插入的顾客
 */
void addCustomer(Line_List &L, customer elem) {

    int index = searchAirLineByDestination(L, elem.destination); //找到顾客对应的航线
    if (index != 404) {
        insert(L.elem[index].has_book_ticket, MakeNode(elem)); //插入
    }
}

/**
 * 将候补顾客登记进航线
 * @param in 航线
 * @param c 顾客
 */
void addWaitingCustomerToAirLine(line_info &in, customer c) {
    customer customer = c;
    insert(in.has_book_ticket, MakeNode(c));
    int i;
    for (i = 0; i < c.book_mount; i++) {
        in.rest_of_tickets--; //减少座位容量
    }
}

/**
 * 询问候补队列中的客户是否要登记航线
 * @param in 航线
 * @param c 顾客
 */
void askWaitingCustomer(line_info &in, customer c) {
    waiting_customer waiting_customer = in.waiting_ticket;
    LQNode *q = waiting_customer.front;
    int select;
    int isFirst = 1;
    int control = 1;
    while (q != NULL) {
        if (q->waitingCustomerType.book_count == c.book_mount) {
            while (control) {
                if (isFirst) {
                    printf("候补名单中有合适的客户,请询问是否要订票: 1: 是; 2: 否.\n");
                    isFirst = 0;
                } else {
                    printf("候补名单中还有合适的顾客,请询问下一个顾客: 1: 是; 2: 否.\n");
                }
                printf("请选择:\n");
                scanf("%d", &select);
                switch (select) {
                    case 1:
                        strcpy(c.name, q->waitingCustomerType.name); //修改名字为替补进去的客户名字
                        addWaitingCustomerToAirLine(in, c);
                        deleteWaitingCustomer(in, q->waitingCustomerType);
                        printf("订票成功\n");
                        control = 0;
                        break;
                    case 2:
                        control = 0;
                        break;
                    default:
                        printf("您输入的操作有误请重新输入！\n");
                        break;
                }
            }
        }
        q = q->next;
    }
}

/**
 * 通过航线号来删除对应的顾客
 * @param L 航线表
 * @param line_number 航线号
 */
void deleteCustomerByLineNumber(Line_List &L, char *line_number) {
    int index = searchAirLineByLineNumber(L, line_number);
    customer c;
    if (index != 404) {
        printf("请输入名字:\n");
        char name[15];
        scanf("%s", name);
        if (deleteCustomer(L.elem[index], name, c)) {
            printf("退订成功!\n");
            askWaitingCustomer(L.elem[index], c);
        } else {
            printf("找不到该顾客!\n");
        }
    } else {
        printf("找不到对应的航班!\n");
    }
}

/**
 * 通过飞机号来删除指定的客户
 * @param L 航线表
 * @param plane_number 飞机号
 */
void deleteCustomerByPlaneNumber(Line_List &L, char *plane_number) {
    int index = searchAirLineByPlaneNumber(L, plane_number);
    customer c;
    if (index != 404) {
        printf("请输入名字:\n");
        char name[15];
        scanf("%s", name);
        if (deleteCustomer(L.elem[index], name, c)) {
            askWaitingCustomer(L.elem[index], c);
            printf("退订成功!\n");
        } else {
            printf("找不到该名字\n");
        }
    } else {
        printf("找不到对应的飞机号\n");
    }
}

//=========view层===========\\

/**
 * 打印提示信息
 */
void view_printf_tip() {
    printf("==========================\n");
    printf("请根据下面提示来操作航空系统\n");
    printf("1: 查询航线\n");
    printf("2: 承办订票业务\n");
    printf("3: 承办退票业务\n");
    printf("4: 退出系统\n");
    printf("5: 根据目的地查询该航班上的已订票名单\n");
    printf("6: 根据目的地查询该航班上的候补顾客名单\n");
    printf("7: 查看所有的航班\n");
    printf("请输入:\n");
}

/**
 * 打印航线信息
 * @param info 航线
 */
void view_printf_air_line_info(line_info info) {
    printf("航班号: %s\n", info.line_number);
    printf("飞机号: %s\n", info.plane_number);
    printf("起飞日期: %s\n", info.start);
    printf("剩余票数: %d\n", info.rest_of_tickets);
}

/**
 * 显示航线
 * @param L 航线表
 * @param destination 目的地
 */
void view_show_air_line(Line_List L, char destination[]) {
    int index = searchAirLineByDestination(L, destination);
    if (index != 404) {
        view_printf_air_line_info(L.elem[index]);
    } else {
        printf("没有到达该目的地的航班!请查正后再输入\n");
    }
}

/**
 * 向候补队列中添加候补顾客
 * @param in 航线
 * @param type 候补顾客
 */
void view_add_waiting_customer(line_info &in, WaitingCustomerType type) {
    int flag = addWaitingQueue(in, type);
    if (flag) {
        printf("登记成功!\n");
    }
}

/**
 * 打印出订票的座位
 * @param ticketCount 订票数
 * @param ticketNumber 座位号数组
 */
void view_show_ticket_number(int ticketCount, int ticketNumber[]) {
    if (ticketCount <= 0) {
        return;
    }
    int i;
    for (i = 0; i < ticketCount; i++) {
        printf("第%d张票的座位号是:%d\n", i + 1, ticketNumber[i]);
    }
}

/**
 * 添加顾客
 * @param L 航线表
 * @param airLineIndex 航线索引
 * @param des 目的地
 */
void view_add_customer(Line_List &L, int airLineIndex, char des[]) {
    line_info in = L.elem[airLineIndex];
    customer c;
    printf("你的名字:\n");
    char name[15];
    scanf("%s", name);
    strcpy(c.name, name);
    strcpy(c.destination, des);

    int num = 0;
    printf("请输入订票数量:\n");
    int con = 1;
    while (con) {
        scanf("%d", &num);
        if (num > 3) {
            printf("每个客户最多只能订3张票, 请重新输入:\n");
        } else {
            c.book_mount = num;
            con = 0;
        }
    }

    if (L.elem[airLineIndex].rest_of_tickets < num) {
        printf("该航线剩下的票数为: %d, 而您订的票数为: %d, 您现在可以有两种选择: 1: 取消订票操作; 2: 选择全部票数登记入替补队列.\n",
               in.rest_of_tickets, num);
        int choice;
        int control = 1;
        printf("请选择:\n");
        while (control) {
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    control = 0;
                    break;
                case 2:
                    WaitingCustomerType type;
                    strcpy(type.name, name);
                    type.book_count = num;
                    view_add_waiting_customer(L.elem[airLineIndex], type);
                    control = 0;
                    break;
                default:
                    printf("您输入有误,请重新输入:\n");
                    break;
            }
        }
    } else {
        int i;
        for (i = 0; i < c.book_mount; i++) {
            c.ticket_number[i] = (L.elem[airLineIndex].capacity - L.elem[airLineIndex].rest_of_tickets) + 1;
            L.elem[airLineIndex].rest_of_tickets--;
        }
        addCustomer(L, c);
        printf("订票成功!\n");
        view_show_ticket_number(c.book_mount, c.ticket_number);
    }
}

/**
 * 订票业务
 * @param L 航线表
 */
void view_book_ticket(Line_List &L) {
    printf("请输入目的地:\n");
    char des[15];
    scanf("%s", des);
    int index = searchAirLineByDestination(L, des);
    if (index == 404) {
        printf("对不起!没有该到达该目的地的航班!\n");
    } else {
        view_add_customer(L, index, des); //添加顾客
    }
}

/**
 * 通过目的地输入所有乘客的名字
 * @param L 航线表
 * @param des 目的地
 */
void view_printf_customer_by_destination(Line_List L, char des[]) {
    int index = searchAirLineByDestination(L, des); //获得对应航线索引号
    if (index != 404) {
        line_info in = L.elem[index];
        LNode *p;
        p = in.has_book_ticket->next;
        int i = 0;
        LQNode *l = in.waiting_ticket.front;
        while (p != NULL) {
            printf("customer: %s\n", p->cust.name);
            for (i = 0; i < p->cust.book_mount; i++) {
                printf("座位: %d\n", p->cust.ticket_number[i]);
            }
            printf("=========================\n");
            p = p->next;
        }
    } else {
        printf("找不到目的地!\n");
    }
}

/**
 * 退订业务
 * @param L 航线表
 */
void view_canel(Line_List &L) {
    printf("请输入要退订客户的相关信息:\n");
    printf("1:航班号; 2:飞机号\n");
    printf("请选择:\n");
    int control = 1;
    int select = 0;
    while (control) {
        scanf("%d", &select);
        switch (select) {
            case 1:
                printf("请输入航班号:\n");
                char line_number[15];
                scanf("%s", line_number);
                deleteCustomerByLineNumber(L, line_number);
                control = 0;
                break;
            case 2:
                printf("请输入航班号:\n");
                char plane_number[15];
                scanf("%s", plane_number);
                deleteCustomerByPlaneNumber(L, plane_number);
                control = 0;
                break;
            default:
                printf("您输入有误,请重新输入!\n");
                break;
        }
    }
}

/**
 * 通过目的地来显示该航线上的候补顾客
 * @param in 航线
 * @param des 目的地
 */
void view_printf_waiting_customer(Line_List L, char des[]) {
    int index = searchAirLineByDestination(L, des);
    if (index != 404) {
        line_info in = L.elem[index];
        LQNode *waitingCustomer = in.waiting_ticket.front;
        if (waitingCustomer == NULL) {
            printf("该航线暂时没有候补顾客!\n");
        } else {
            while (waitingCustomer != NULL) {
                printf("名字: %s\n", waitingCustomer->waitingCustomerType.name);
                waitingCustomer = waitingCustomer->next;
            }
        }
    } else {
        printf("对不起,找不到对应的航线!\n");
    }
}

void view_printf_all_air_line(Line_List L) {
    int i;
    for(i = 0; i < L.length; i++) {
        printf("=======================\n");
        printf("目的地:%s\n", L.elem[i].destination);
        printf("航线号:%s\n", L.elem[i].line_number);
        printf("飞机号%s\n", L.elem[i].plane_number);
        printf("容量:%d\n", L.elem[i].capacity);
        printf("剩余量:%d\n", L.elem[i].rest_of_tickets);
        printf("飞行日期:%s\n", L.elem[i].start);
        printf("=======================\n");
    }
}

/**
 * 操作界面
 * @param L 航线表
 */
void view_main_view(Line_List &L) {
    int control = 1;
    int manger;
//    view_printf_author_info(); //输入作者信息
    while (control) {
        view_printf_tip(); //输出操作信息
        scanf("%d", &manger);
        switch (manger) {
            case 1:
                char des[15];
                printf("请输入查询的目的地:\n");
                scanf("%s", des);
                view_show_air_line(L, des);
                break;
            case 2:
                view_book_ticket(L);
                break;
            case 3:
                view_canel(L);
                break;
            case 4:
                control = 0;
                printf("退出成功\n");
                break;
            case 5:
                char desc[15];
                printf("请输入要查询的目的地:\n");
                scanf("%s", desc);
                view_printf_customer_by_destination(L, desc);
                break;
            case 6:
                printf("请输入要查询的目的地:\n");
                char destination[15];
                scanf("%s", destination);
                view_printf_waiting_customer(L, destination);
                break;
            case 7:
                view_printf_all_air_line(L);
                break;
            default:
                printf("您输入的信息有误,请重新输入!\n");
                break;
        }
    }
}

int main() {
    Line_List L;
    InitList(L, 10, 6); //初始化航线表
    createDefaultAirLine(L); //创建航线
    initCustomerHeader(L); //初始化顾客链表
    view_main_view(L); //操作界面
    return 0;
}
