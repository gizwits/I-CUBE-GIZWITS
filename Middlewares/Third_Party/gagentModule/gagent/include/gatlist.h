#ifndef _GAGENT_LIST_H
#define _GAGENT_LIST_H


struct gatListHead
{
    struct gatListHead* prev;
    struct gatListHead* next;
};

//#define CONFIG_DEBUG_LIST

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define GAT_LIST_HEAD_INIT(name) { &(name), &(name) }

#define GAT_LIST_HEAD(name) \
    struct gatListHead name = GAT_LIST_HEAD_INIT(name)

void gatListHeadInit(struct gatListHead *list);
/**
 * gat_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void gatListAdd(struct gatListHead *new, struct gatListHead *head);
/**
 * gatListAddTail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void gatListAddTail(struct gatListHead *new, struct gatListHead *head);


/**
 * gat_list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: gatListEmpty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
void gatListDel(struct gatListHead *entry);

/**
 * gatListEmpty - tests whether a list is empty
 * @head: the list to test.
 */
int gatListEmpty(const struct gatListHead *head);

/**
 * gatListEmptyCareful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using gatListEmptyCareful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is gat_list_del_init(). Eg. it cannot be used
 * if another CPU could re-gat_list_add() it.
 */
int gatListEmptyCareful(const struct gatListHead *head);

#define gat_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/**
 * gatContainerOf - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define gatContainerOf(ptr, type, member) ({          \
    const typeof(((type *)0)->member)*__mptr = (ptr);    \
             (type *)((char *)__mptr - gat_offsetof(type, member)); })

/**
 * gatListEntry - get the struct for this entry
 * @ptr:    the &struct gatListHead pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */

// #define gatListEntry(ptr, type, member) \
//     gatContainerOf(ptr, type, member)
//modify by Alex.lin  2017-12-12
#define gatListEntry(ptr, type, member) \
     ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * gatListFirstEntry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define gatListFirstEntry(ptr, type, member) \
    gatListEntry((ptr)->next, type, member)

/**
 * gatListForEachEntry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
// #define gatListForEachEntry(pos, head, member)              \
//     for (pos = gatListEntry((head)->next, typeof(*pos), member);    \
//          &pos->member != (head);    \
//          pos = gatListEntry(pos->member.next, typeof(*pos), member))
//modify by Alex.lin  2017-12-12
#define gatListForEachEntry(pos, head,member ) \
      for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * gatListForEachEntryReverse - iterate backwards over list of given type.
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
// #define gatListForEachEntryReverse(pos, head, member)           \
//     for (pos = gatListEntry((head)->prev, typeof(*pos), member);    \
//          &pos->member != (head);    \
//          pos = gatListEntry(pos->member.prev, typeof(*pos), member))
//modify by Alex.lin  2017-12-12
#define gatListForEachEntryReverse(pos, head, member)           \
        for (pos = (head)->prev; pos != (head); pos = pos->prev)
/**
 * list_for_each_safe   -   iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop counter.
 * @n:      another &struct list_head to use as temporary storage
 * @head:   the head for your list.
 */
// #define gatListForEachEntrySafe(pos, n, head, member)                  \
//     for (pos = gatListEntry((head)->next, typeof(*pos), member),      \
//             n = gatListEntry(pos->member.next, typeof(*pos), member); \
//          &pos->member != (head);                                    \
//          pos = n, n = gatListEntry(n->member.next, typeof(*n), member))
//modify by Alex.lin  2017-12-12
#define gatListForEachEntrySafe(pos, n, head,member) \
      for (pos = (head)->next, n = pos->next; pos != (head); \
            pos = n, n = pos->next)

#endif
