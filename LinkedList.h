#ifndef LinkedList_h
#define LinkedList_h

template<class T>
struct ListNode
{
	T data;
	ListNode<T> *next;
};

template <typename T>
class LinkedList{

  protected:
	int _size;
	ListNode<T> *root;
	ListNode<T>	*last;

	// Helps "get" method, by saving last position
	ListNode<T> *lastNodeGot;
	int lastIndexGot;

	ListNode<T>* getNode(int index);
        void resetCache() { lastIndexGot = 0; lastNodeGot = root; }

  public:
	LinkedList();
	/*
		Returns current size of LinkedList
	*/
	int size();
	/*
		Adds a T object in the specified index;
		Unlink and link the LinkedList correcly;
		Increment _size
	*/
	bool add(int index, T);
	/*
		Adds a T object in the end of the LinkedList;
		Increment _size;
	*/
	bool add(T);
	/*
		Adds a T object in the start of the LinkedList;
		Increment _size;
	*/
	bool set(int index, T);
	/*
		Remove object at index;
		If index is not reachable, returns false;
		else, decrement _size
	*/
	bool remove(int index);
	/*
		Clear the entire array
	*/
	void clear();

        T* get(int index) { ListNode<T>* item = getNode(index); if(item) return &(item->data); return NULL; }

};

// Initialize LinkedList with false values
template<typename T>
LinkedList<T>::LinkedList()
{
	root=false;
	last=false;
	_size=0;
	resetCache();
}

/*
	Actualy "logic" coding
*/
template<typename T>
ListNode<T>* LinkedList<T>::getNode(int index)
{
	int _pos = 0;
	ListNode<T>* current = root;

	_pos = lastIndexGot;
	current = lastNodeGot;

	while(_pos < index && current){
		current = current->next;
		_pos++;
	}

	// Check if the object index got is the same as the required
	if(_pos == index){
		lastIndexGot = index;
		lastNodeGot = current;
		return current;
	}

	return false;
}

template<typename T>
int LinkedList<T>::size()
{
	return _size;
}

template<typename T>
bool LinkedList<T>::add(int index, T _t)
{
	if(index >= _size)
		return add(_t);

	ListNode<T> *tmp = new ListNode<T>(),
				 *_prev = getNode(index-1);
	tmp->data = _t;
	tmp->next = _prev->next;
	_prev->next = tmp;

	_size++;
        resetCache();

	return true;
}

template<typename T>
bool LinkedList<T>::add(T _t){

	ListNode<T> *tmp = new ListNode<T>();
	tmp->data = _t;
	tmp->next = false;
	
	if(root){
		// Already have elements inserted
		last->next = tmp;
		last = tmp;
	}else{
		// First element being inserted
		root = tmp;
		last = tmp;
	}

	_size++;
	resetCache();

	return true;
}

template<typename T>
bool LinkedList<T>::set(int index, T _t){
	// Check if index position is in bounds
	if(index < 0 || index >= _size)
		return false;

	getNode(index)->data = _t;
	return true;
}

template<typename T>
bool LinkedList<T>::remove(int index){
	if (index < 0 || index >= _size)
          return false;

	if(index == 0)
        {
          ListNode<T> *tmp = root;
          root = root->next;
          delete(tmp);
          return true;
        }
	
	ListNode<T> *tmp = getNode(index - 1);
	ListNode<T> *toDelete = tmp->next;
	T ret = toDelete->data;
	tmp->next = tmp->next->next;
	delete(toDelete);

	if (index == _size-1)
          last = tmp;

	_size--;
	resetCache();

	return ret;
}


template<typename T>
void LinkedList<T>::clear(){
	while(size() > 0)
		remove(0);
}

#endif
