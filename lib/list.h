#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

/** @file list.h
A list is a data structure to store sequences of elements.
*/

typedef struct Element_R* Element;
typedef struct List_R* List;

/** Creates a new list.
@return a new list
*/
List newList(void);

/** Releases the memory occupied by the list data structure itself. It does not free
any memory associated with the data elements in the list.
@param list the memory of which is to be released
*/
void deleteList(List list);

/** Retrieves the first element of the list.

This example illustrates how to iterate through a list:

Element e;
for (e=firstElem(list); e!=NULL; e=nextElem(e)) {
  void* d = dataOfElem(e);
  ...
}

@param list
@return the first element in the list
*/
Element firstElem(List list);

/** Retrieves the next element from a list. See #first for an usage example.
@param element for which the successor is to be retriebed
@return the successor of element
*/
Element nextElem(Element element);

/** Retrieve the data associated with a list element See #first for an unsage example.
@param data the list element
@return the pointer stored with a list element
*/
void* dataOfElem(Element element);

/** Append an element to a list.
@param data is the pointer to be appended
@param list is the list to which the element is to be appended
*/
void appendToList(void* data, List list);

/** Joins two lists by appending the elements of one list to the target list. The
source list is empty afterwards.
@param source the list that is appended
@param target to list to which the elements of source are appended
*/
void joinLists(List source, List target);

int numberOfElems(List list);

/** Pushes data on the stack. Data is then the first element of the list
*/
void pushElem(void*data, List list);

/** Pops one element from the stack, i.e. removes the first element of the list
*/
void* popElem(List list);

/** Returns the top element of the stack, i.e. the first element of the list
*/
void *topElem(List list);

#endif

