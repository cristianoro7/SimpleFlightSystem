#include <iostream>
#include <printf.h>
#include <cstring>

#define OK 1;
typedef int Status;
#define OVERFLOW -1;
#define NOT_FOUND 0;
#define ERROR 0;

typedef struct {
    char name[15];
    int book_count;
} WaitingCustomerType;

typedef struct LQNode {
    WaitingCustomerType waitingCustomerType;
    LQNode *next;
} LQNode;

typedef struct {
    LQNode *front; //头指针
    LQNode *rear; //尾指针
} waiting_customer;

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

int searchAirLineByDestination(Line_List L, char destination[]) {
    int i;
    for (i = 0; i < L.length; i++) {
        if (strcmp(L.elem[i].destination, destination) == 0) {
            return i;
        }
    }
    return 404;
}

int searchAirLineByLineNumber(Line_List L, char line_number[]) {
    int i;
    for (i = 0; i < L.length; i++) {
        if(strcmp(L.elem[i].line_number, line_number) == 0) {
            return i;
        }
    }
    return 404;
}

int searchAirLineByPlaneNumber(Line_List L, char plane_number[]) {
    int i;
    for(i = 0; i < L.length; i++) {
        if(strcmp(L.elem[i].plane_number, plane_number) == 0) {
            return i;
        }
    }
    return 404;
}

line_info makeAirLine(char des[], char number[], char plane_number[], char start[], int capacity, int left) {
    line_info in;
    strcpy(in.destination, des);
    strcpy(in.line_number, number);
    strcpy(in.plane_number, plane_number);
    strcpy(in.start, start);
    in.capacity = capacity;
    in.rest_of_tickets = left;
    in.has_book_ticket = NULL;
    initWaitingCustomerQueue(in.waiting_ticket);
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

void initCustomerHeader(Line_List &L) {
    int i;
    for (i = 0; i < L.length; i++) {
        L.elem[i].has_book_ticket = (LNode *) malloc(sizeof(LNode));
        L.elem[i].has_book_ticket->next = NULL;
    }
}

void insertAfter(Customer_List &start, LNode *waitToInsert) {
    if (waitToInsert == NULL) {
        return;
    }

    LNode *p = start->next;
    LNode *q = start;
    if (p == NULL) {
        start->next = waitToInsert;
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
//        if(p->next == NULL) {
//            p = waitToInsert; //插入到表尾
//            return;
//        }
    }
    q->next = waitToInsert; //链表为空,或者插到表尾的情况;
}

Status deleteCustomer(line_info &in, char name[15]) {

    LNode *p = in.has_book_ticket->next;
    LNode *q = in.has_book_ticket;
    while( p != NULL) {
        if(strcmp(p->cust.name, name) == 0) { //遍历链表
            in.rest_of_tickets += p->cust.book_mount;
            q->next = p->next;
            free(p);
            return OK;
        }
        q = p;
        p = p->next;
    }
    return ERROR;
}

void addCustomer(Line_List &L, customer elem) {

    int index = searchAirLineByDestination(L, elem.destination);
    if (index != 404) {
        insertAfter(L.elem[index].has_book_ticket, MakeNode(elem));
    }
}

void deleteCustomerByLineNumber(Line_List &L, char *line_number) {
    int index = searchAirLineByLineNumber(L, line_number);
    if(index != 404) {
        printf("请输入名字:\n");
        char name[15];
        scanf("%s", name);
        if(deleteCustomer(L.elem[index], name)) {
            printf("退订成功!\n");
        } else {
            printf("找不到该顾客!");
        }
    } else {
        printf("找不到对应的航班!\n");
    }
}

void deleteCustomerByPlaneNumber(Line_List &L, char *plane_number) {
    int index = searchAirLineByPlaneNumber(L, plane_number);
    if(index != 404) {
        printf("请输入名字:\n");
        char name[15];
        scanf("%s", name);
        if(deleteCustomer(L.elem[index], name)) {
            printf("退订成功!\n");
        } else {
            printf("找不到该名字");
        }
    } else {
        printf("找不到对应的飞机号");
    }
}

//=========view层===========\\

void view_printf_author_info() {
    printf("class: 计科三班\n");
    printf("author: 林锦涛\n");
    printf("ID: 3115004970\n");
}

void view_printf_tip() {
    printf("==========================\n");
    printf("请根据下面提示来操作航空系统\n");
    printf("1: 查询航线\n");
    printf("2: 承办订票业务\n");
    printf("3: 承办退票业务\n");
    printf("4: 退出系统\n");
    printf("5: 根据目的地查询该航班上的已订票名单\n");
    printf("请输入:\n");
}

void view_printf_air_line_info(line_info info) {
    printf("航班号: %s\n", info.line_number);
    printf("飞机号: %s\n", info.plane_number);
    printf("起飞日期: %s\n", info.start);
    printf("剩余票数: %d\n", info.rest_of_tickets);
}

void view_show_air_line(Line_List L, char destination[]) {
    int index = searchAirLineByDestination(L, destination);
    if (index != 404) {
        view_printf_air_line_info(L.elem[index]);
    } else {
        printf("没有到达该目的地的航班!请查正后再输入\n");
    }
}

void view_add_waiting_customer(line_info &in, WaitingCustomerType type) {
    int flag = addWaitingQueue(in, type);
    if (flag) {
        printf("登记成功!\n");
    }
}

void view_show_ticket_number(int ticketCount, int ticketNumber[]) {
    if (ticketCount <= 0) {
        return;
    }
    int i;
    for (i = 0; i < ticketCount; i++) {
        printf("第%d张票的座位号是:%d\n", i + 1, ticketNumber[i]);
    }
}

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

void view_book_ticket(Line_List &L) {
    printf("请输入目的地:\n");
    char des[15];
    scanf("%s", des);
    int index = searchAirLineByDestination(L, des);
    if (index == 404) {
        printf("对不起!没有该到达该目的地的航班!\n");
    } else {
        view_add_customer(L, index, des);
    }
}

void view_printf_customer_by_destination(Line_List L, char des[]) {
    int index = searchAirLineByDestination(L, des);
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

            while (l != NULL) {
                printf("name: %s\n", l->waitingCustomerType.name);
                l = l->next;
            }

            p = p->next;
        }
    } else {
        printf("找不到目的地!\n");
    }
}

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


void view_main_view(Line_List &L) {
    int control = 1;
    int manger;
    view_printf_author_info();
    while (control) {
        view_printf_tip();
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
            default:
                printf("您输入的信息有误,请重新输入!\n");
                break;
        }
    }
}


//------------测试数据-------------\\

void printf_info(Line_List L) {
    line_info in;
    int i;
    for (i = 0; i < L.length; i++) {
        in = L.elem[i];
        printf("haha %s\n", L.elem[i].destination);
        printf("line_number %s\n", L.elem[i].line_number);
        printf(" start %s\n", in.start);
        printf("capacity%d\n", in.capacity);
        printf("plane line_number %s\n", in.plane_number);
        printf("customer: %s\n", in.has_book_ticket->next->cust.name);
    }
}

void addCustomer(Line_List &L) {
    customer c;
    printf("你的名字:\n");
    char name[15];
    scanf("%s", name);
    strcpy(c.name, name);
    printf("目的地:\n");
    char destination[15];
    scanf("%s", destination);
    strcpy(c.destination, destination);
    addCustomer(L, c);
}

void addLine(Line_List &L) {
    printf("desc:\n");
    char des[15];
    scanf("%s", des);

    printf("line_number:\n");
    char num[15];
    scanf("%s", num);

    printf("plane_number:\n");
    char plane[15];
    scanf("%s", plane);

    printf("capacity:\n");
    int capacity;
    scanf("%d", &capacity);

    printf("date:\n");
    char date[15];
    scanf("%s", date);

    line_info in;
    in.capacity = capacity;
    strcpy(in.destination, des);
    strcpy(in.line_number, num);
    strcpy(in.plane_number, plane);
    strcpy(in.start, date);
    addAirLine(L, in);
}

void printf_all_customer(Line_List &L) {

    line_info in = L.elem[0];
    LNode *p;
    p = in.has_book_ticket->next;
    while (p != NULL) {
        printf("customer: %s\n", p->cust.name);
        p = p->next;
    }
}

int main() {
    Line_List L;
    InitList(L, 10, 6);
    createDefaultAirLine(L);
    initCustomerHeader(L);
    view_main_view(L);
    return 0;
}
