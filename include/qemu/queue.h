

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

/**
 * elm
 * +------+------+
 * | prev | next |
 * +------+------+
 *
 */
#define QLIST_INSERT_BEFORE(listelm, elm, field) do {							\
	(elm)->field.previous = (listelm)->field.previous;							\
	(elm)->field.next = (listelm);												\
	*(listelm)->field.previous = (elm);				/* 給予 element 的內容 */	    \
	(listelm)->field.previous = &(elm)->field.next;	/* 給予 element next 的位置/空間, 讓 listelm 指向 elm 的 next 位置，考慮到 container_of */	 \
} while(0)
