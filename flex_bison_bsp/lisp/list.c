#include <stdlib.h>
#include <assert.h>

#include "list.h"

List make_list(void)
{
  List l = malloc(sizeof(struct List_R));
  assert(l!=NULL);

  l->first = NULL;
  l->last = NULL;

  return l;
}

void append(ListElement e, List l)
{
  assert(e!=NULL);
  assert(l!=NULL);

  if (l->first==NULL) {
    l->first = e;
  }
  else {
    l->last->next = e;
  }
  l->last = e;
}

ListElement make_symbol_element(const char* s)
{
  ListElement e = malloc(sizeof(struct ListElement_R));
  assert(e!=NULL);

  e->type = T_SYMBOL;
  e->d.symbol = s;
  e->next = NULL;

  return e;
}

ListElement make_number_element(int number)
{
  ListElement e = malloc(sizeof(struct ListElement_R));
  assert(e!=NULL);

  e->type = T_NUMBER;
  e->d.number = number;
  e->next = NULL;

  return e;
}

ListElement make_list_element(List list)
{
  ListElement e = malloc(sizeof(struct ListElement_R));
  assert(e!=NULL);

  e->type = T_LIST;
  e->d.list = list;
  e->next = NULL;

  return e;
}
