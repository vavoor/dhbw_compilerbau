#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef struct ListElement_R {
  enum { T_NUMBER, T_SYMBOL, T_LIST } type;
  union {
    int number;
    const char* symbol;
    struct List_R* list;
  } d;
  struct ListElement_R* next;
} *ListElement;

typedef struct List_R {
  ListElement first;
  ListElement last;
} *List;

List make_list(void);
void append(ListElement e, List l);

ListElement make_symbol_element(const char* s);
ListElement make_number_element(int number);
ListElement make_list_element(List list);

#endif
