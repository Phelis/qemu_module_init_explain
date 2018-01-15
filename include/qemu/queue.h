

// 暫時先把 glib.h 放在此，以便編譯的方式可以通過
#include <glib.h>

/*
 * 在 qeum 的 queue.h 檔案內，定義了兩個形態 (linked-List 和 Queue)
 *
 */
/**
 * Double-Linked List
 */
// 定義一個 entry 擁有 previous 和 next pointer
#define QLIST_ENTRY(type)	\
struct {					\
	struct type *next;		\
	struct type *previous;	\
}


// 建立 header 的結構
#define QLIST_HEAD(name, type)							\
struct name {											\
	struct type *first;			/* first element */		\
}

// while(0) 目的是要 QLIST_INIT 放在任何地方，例如：if-cond,ition 內都可以使用。
#define QLIST_INIT(head) do {	\
	(head)->first = NULL;		\
} while(0)


#define QLIST_FOREACH(var, head, field)			\
	for ((var) == ((head)->first);				\
			(var);								\
			(var) = ((var)->field.next))


#define QLIST_INSERT_AFTER(listelm, elm, field) do {          	\
	if (((elm)->field.next = (listelm)->field.next) != NULL)  	\
			(listelm)->field.next->field.previous =             \
				&(elm)->field.next;                           \
	(listelm)->field.next = (elm);                            \
	(elm)->field.previous = &(listelm)->field.next;           \
} while (/*CONSTCOND*/0)

/**
 * 一般常用的結構，會在一個 structure 內定義 QLIST_ENTRY(type)
 */
#define QLIST_INSERT_BEFORE(listelm, elm, field) do {							\
	(elm)->field.previous = (listelm)->field.previous;							\
	(elm)->field.next = (listelm);												\
	*(listelm)->field.previous = (elm);				/* 指定原本 previous 的 next pointer 為 elm  */	    		 \
	(listelm)->field.previous = &(elm)->field.next;	/* 給予 listelm 的 previous 為 elm 的 next 的 field address */	 \
} while(0)


/*
 * Tail queue definitions.
 */
#define Q_TAILQ_HEAD(name, type, qual)                                  \
struct name {                                                           \
		qual type *tqh_first;           /* first element */             \
		qual type *qual *tqh_last;      /* addr of last next element */ \
}

// 建構一個 queue head 結構
#define QTAILQ_HEAD(name, type) Q_TAILQ_HEAD(name, struct type,)

// 初始化一個 queue head 結構
#define QTAILQ_HEAD_INITIALIZER(head)                                   \
		{ NULL, &(head).tqh_first }


// 注意 TAILQ 多一個 Q 在尾巴代表 queue 的意思
#define Q_TAILQ_ENTRY(type, qual)                                       	\
struct {                                                                	\
	qual type *tqe_next;            /* next element */             		 	\
	qual type *qual *tqe_prev;      /* address of previous next element */	\
}

// 初始化一個 entry，設定 data type
#define QTAILQ_ENTRY(type)       Q_TAILQ_ENTRY(struct type,)


/**
 * head 的 last 為 elm 的 next field structure, 不用麻煩的 head -> last -> next = elm 而是把 elm 的 next address 直接給 head last
 * 這樣簡單的 *(head) -> last 代表的意思就是 elm 的 next pointer。
 */
#define QTAILQ_INSERT_TAIL(head, elm, field) do {	\
	(elm)->field.tqe_next = NULL;                    /* 以下兩項只是單純的設定 elm 的 tqe_next 和 tqe_prev */            			\
	(elm)->field.tqe_prev = (head)->tqh_last;            	           															\
	*(head)->tqh_last = (elm);                    /* 讓原本的 last element 指向新的 element */             							\
	(head)->tqh_last = &(elm)->field.tqe_next;    /* 一般會讓 tqh_last 指向 elm 物件，但是在此卻反而指派 elm->next 的位址給 tqh_last，
													 如此可以經由 *(head)->tqh_last 方式去控制 (elm)->field.tqe_next */ 				\
} while (/*CONSTCOND*/0)


