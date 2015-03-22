#include "list.h"

#include <stdlib.h>
#include<assert.h>

struct Element_R {
  void* data;
  Element next;
};

struct List_R {
  Element first;
  Element last;
  int count;
};


static Element newElement(void* data)
{
  Element elem;

  elem = malloc(sizeof(struct Element_R));
  assert(elem!=NULL);

  elem->data = data;
  elem->next = NULL;

  return elem;
}


List newList(void)
{
  List l;

  l = malloc(sizeof(struct List_R));
  assert(l!=NULL);

  l->first = l->last = NULL;
  l->count = 0;

  return l;
}

void appendToList(void* data, List list)
{
  assert(list!=NULL);

  if (data!=NULL) {
    Element elem = newElement(data);

    if (list->first==NULL) {
      list->first = list->last = elem;
    }
    else {
      list->last->next = elem;
      list->last = elem;
    }
    list->count++;
  }
}


int numberOfElems(List list)
{
  assert(list!=NULL);
  return list->count;
}


Element firstElem(List list)
{
  return list->first;
}


Element nextElem(Element element)
{
  return element!=NULL? element->next : NULL;
}


void* dataOfElem(Element element)
{
  return element->data;
}


void joinLists(List source, List target)
{
  if (source->first!=NULL) {
    if (target->last==NULL) {
      target->first = source->first;
      target->last = source->last;
    }
    else {
      target->last->next = source->first;
      target->last = source->last;
    }
  }

  target->count += source->count;
  source->first = source->last = NULL;
  source->count = 0;
}


void deleteList(List list)
{
  Element e;

  assert(list!=NULL);

  e = list->first;
  while (e!=NULL) {
    Element t = e->next;
    free(e);
    e = t;
  }
  free(list);
}


void pushElem(void*data, List list)
{
  assert(list!=NULL);

  Element elem = newElement(data);

  if (list->first==NULL) {
    list->first = list->last = elem;
  }
  else {
    elem->next = list->first;
    list->first = elem;
  }
  list->count++;
}


void* popElem(List list)
{
  void *data = NULL;
  Element elem;

  assert(list!=NULL);

  if (list->first!=NULL) {
    elem = list->first;
    list->first = elem->next;
    list->count--;
    data = elem->data;
    if (list->last==elem) {
      list->last = NULL;
      assert(list->count==0);
      assert(list->first==NULL);
    }
    free(elem);
  }

  return data;
}


void *topElem(List list)
{
  assert(list!=NULL);
  if (list->first!=NULL) {
    return list->first->data;
  }
  else {
    return NULL;
  }
}

