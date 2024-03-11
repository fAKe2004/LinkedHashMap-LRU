#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP

#define QUADRATIC_PROBING 1 // 2次性探测，不开默认线性探测

#include <memory>

#include "utility.hpp"
#include "exceptions.hpp"
#include "class-integer.hpp"
#include "class-matrix.hpp"
class Hash {
public:
	unsigned int operator () (Integer lhs) const {
		int val = lhs.val;
		return std::hash<int>()(val);
	}
};
class Equal {
public:
	bool operator () (const Integer &lhs, const Integer &rhs) const {
		return lhs.val == rhs.val;
	}
};

namespace sjtu {
template<class T> class double_list{
protected:
		struct node {
		T* data_;
		node *pre_, *next_;
		bool operator==(const node& other) const {
			return data_ == other.data_;
		}
		node(T* data, node* pre, node* next) 
			: data_(data), pre_(pre), next_(next) {}
		node() : node(nullptr, nullptr, nullptr) {}
		node(node&& rhs) {
			data_ = rhs.data_;
			rhs.data_ = nullptr;
		}
		node(const node& rhs) {
			data_ = new T(*rhs.data_);
			pre_ = rhs.pre_, next_ = rhs.next_;
		}
		~node() {
			delete data_;
		}
		node& operator=(const node& other) = default;
	};
private:
	node tail_node_, *head_, *tail_;
public:
	/**
	 * elements
	 * add whatever you want
	*/

// --------------------------
	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	*/
	double_list()
		: tail_node_(), head_(&tail_node_), tail_(&tail_node_) {
	}
	double_list(const double_list<T> &other) {
		for (const auto &item : other)
			insert_tail(item);
	}
	~double_list(){
		clear();
	}

	double_list& operator=(const double_list<T> &other) {
		this->clear();
		for (const auto &item : other)
			insert_tail(item);
		return *this;
	}

	class iterator{
		friend double_list;
	protected:
		node* nd_;
		iterator(node* nd)
			: nd_(nd) {}
	public:
    	/**
		 * elements
		 * add whatever you want
		*/
	    // --------------------------
        /**
		 * the follows are constructors and destructors
		 * you can also add some if needed.
		*/
		iterator() 
			: nd_(nullptr) {}
		iterator(const iterator &t)
			: nd_(t.nd_) {}
		~iterator() { }
        /**
		 * iter++
		 */

		iterator operator++(int) {
			iterator hold = *this;
			++*this;
			return hold;
		}
        /**
		 * ++iter
		 */
		iterator &operator++() {
			nd_ = nd_->next_;
			return *this;
		}
        /**
		 * iter--
		 */
		iterator operator--(int) {
			iterator hold = *this;
			--*this;
			return hold;
		}
        /**
		 * --iter
		 */
		iterator &operator--() {
			nd_ = nd_->pre_;
			return *this;
		}
		/**
		 * if the iter didn't point to a value
		 * throw " invalid"
		*/
		T &operator*() const {
			if (!nd_ || !(nd_->data_))
				throw "invalid";
			return *(nd_->data_);
		}

		// const	T &operator*() const {
		// 	return *(nd_->data_);
		// }
        /**
         * other operation
        */
		T *operator->() const noexcept {
			return nd_->data_;
		}
		bool operator==(const iterator &rhs) const {
			return nd_ == rhs.nd_;
		}
		bool operator!=(const iterator &rhs) const {
			return nd_ != rhs.nd_;
		}
	};
	/**
	 * return an iterator to the beginning
	 */
	iterator begin() const {
		return iterator(head_);
	}
	/**
	 * return an iterator to the ending
	 * in fact, it returns the iterator point to nothing,
	 * just after the last element.
	 */
	iterator end() const {
		return iterator(tail_);
	}
	/**
	 * if the iter didn't point to anything, do nothing,
	 * otherwise, delete the element pointed by the iter
	 * and return the iterator point at the same "index"
	 * e.g.
	 * 	if the origin iterator point at the 2nd element
	 * 	the returned iterator also point at the
	 *  2nd element of the list after the operation
	 *  or nothing if the list after the operation
	 *  don't contain 2nd elememt.
	*/
	iterator erase(iterator pos) {
		if (pos == iterator() || pos == end())
			return end();
		if (pos == begin()) {
			delete_head();
			return begin();
		}
		node *now = pos.nd_, *ret = now->next_;
		now->pre_->next_ = now->next_;
		now->next_->pre_ = now->pre_;
		delete now;
		return iterator(ret);
	}

	/**
	 * the following are operations of double list
	*/
	void insert_head(const T &val) {
		node* new_node = new node(new T(val), nullptr, head_);
		head_->pre_ = new_node;
		head_ = new_node;
	}
	void insert_tail(const T &val) {
		node* new_node = new node(new T(val), tail_->pre_, tail_);
		if (tail_->pre_)
			tail_->pre_->next_ = new_node;
		else 
			head_ = new_node;
		tail_->pre_ = new_node;
	}
	void delete_head() {
		if (head_ == tail_)
			throw std::runtime_error("double_list::delete_head : delete empty list");
		node* hold = head_;
		head_ = head_->next_;
		head_->pre_ = nullptr;
		delete hold;
	}
	void delete_tail() {
		if (head_ == tail_)
			throw std::runtime_error("double_list::delete_tail : delete empty list");
		node* pre = tail_->pre_;
		if (pre->pre_)
			pre->pre_->next_ = tail_;
		else 
			head_ = tail_;
		tail_->pre_ = pre->pre_;
	}
	/**
	 * if didn't contain anything, return true, 
	 * otherwise false.
	 */
	bool empty() const {
		return head_ == tail_;
	}

	void clear() {
		node* p = head_;
		while (p != tail_) {
			p = p->next_;
			delete p->pre_;
		}
		tail_node_ = node();
		head_ = tail_ = &tail_node_;
	}
};

// AUXILIARY FUNCTIONS
// static std::mt19937_64 rng((std::random_device())());

size_t quick_pow(size_t a, size_t b, size_t mod) {
	size_t r = 1;
	while (b) {
		if (b & 1)
			r = r * a % mod;
		a = a * a % mod;
		b >>= 1;
	}
	return r;
}

// n in this case will not exceed 2^32, so direct multipication in size_t is fine

bool miller_rabin(size_t n) {
	if (n == 1 || !(n & 1))
		return n == 2;
	static const int test_time = 12;
	static const int test_a[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	size_t u = n - 1, t = 0;
	while (!(u & 1)) u >>= 1, ++t;
	for (int i = 0; i < test_time; i++) {
		// size_t a = rng() % (n - 2) + 2, v = quick_pow(a, u, n);
		size_t a = test_a[i] % n, v = quick_pow(a, u, n);
		if (!a) continue;
		int s = 0;
		for (; s < t; s++) {
			if (v == n - 1 || v == 1) break;
			v = v * v % n;
		}
		if (s == t)
			return false;
	}
	return true;
}

/*
	hashmap class
	改写为 list + hash，之前写二次探测太烂了。
*/
template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class hashmap{
public:
	using value_type = pair<const Key, T>;
	using list = double_list<value_type>;
	using list_it = typename list::iterator;
	
	static constexpr double expand_factor = 2, load_factor = 1;
	static const size_t init_cap = 5;

protected:
	size_t cap_, size_;
	list* data_;

	Hash hash_;
	Equal equal_;
	
public:
	/**
	 * elements
	 * add whatever you want
	*/

// --------------------------

	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	*/
	hashmap(size_t cap = init_cap) 
	: cap_(cap), size_(0), data_(cap_ == -1 ? nullptr : new list[cap_]), hash_(), equal_() {
	}
	hashmap(const hashmap &other)
	: cap_(-1) {
		*this = other;
	}
	~hashmap(){
		destroy();
	}
	hashmap & operator=(const hashmap &other){
		if (this == &other)
			return *this;
		this->destroy();
		cap_ = other.cap_;
		if (cap_ == -1)
			return *this;
		size_ = other.size_;
		data_ = new list[cap_];
		for (size_t i = 0; i < cap_; i++)
			data_[i] = other.data_[i];
		return *this;
	}

	class iterator{
		friend hashmap;
	protected:
		list* lst_;
		list_it it_;
		iterator(list* lst, list_it it)
			: lst_(lst), it_(it) {}
	public:
    	/**
         * elements
         * add whatever you want
        */
// --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
        */
		iterator() 
		: lst_(nullptr), it_() {}
		iterator(const iterator &t)
		: lst_(t.lst_), it_(t.it_) {}
		~iterator() {}

    /**
		 * if point to nothing
		 * throw 
		*/
		value_type &operator*() const {
			if (it_ == list_it() || it_ == lst_->end())
				throw std::runtime_error("hashmap::iterator::opeartor* : dereference of nullptr");
			return *it_;
		}

    /**
		 * other operation
		*/
		value_type *operator->() const noexcept {
			return it_.operator->();
		}
		bool operator==(const iterator &rhs) const {
			return lst_ == rhs.lst_ && it_ == rhs.it_;
    }
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
	};

	void destroy() {
		if (cap_ == size_t(-1))
			return;
		delete[] data_;
		cap_ = -1;
	}

	void clear(){
		for (int i = 0; i < cap_; i++)
			data_[i].clear();
		size_ = 0;
	}
	/**
	 * you need to expand the hashmap dynamically
	*/
	void expand() {
		size_t new_cap = cap_ * expand_factor;
		while (!miller_rabin(new_cap))
			++new_cap;
		hashmap new_hashmap(new_cap);

		for (int i = 0; i < cap_; i++) {
			for (const value_type& item : data_[i])
				new_hashmap.insert(item);
		}

		*this = new_hashmap;
	}

    /**
     * the iterator point at nothing
    */
	iterator end() const{
		return iterator();
	}
	/**
	 * find, return a pointer point to the value
	 * not find, return the end (point to nothing)
	 * type 0 return iterator() if not found
	 * type 1 return iterator(data_[hash % cap_] and its end())
	*/
	iterator find(const Key &key, bool type = 0) const {
		size_t h = hash_(key) % cap_;
		list* lst = data_ + h;
		for (list_it it = lst->begin(); it != lst->end(); ++it) {
			if (equal_(key, it->first))
				return iterator(lst, it);
		}
		if (type)
			return iterator(lst, lst->end()); // not found
		else 
			return iterator();
	}
	/**
	 * already have a value_pair with the same key
	 * -> just update the value, return false
	 * not find a value_pair with the same key
	 * -> insert the value_pair, return true
	*/
	sjtu::pair<iterator,bool> insert(const value_type &value_pair){
		if (cap_ * load_factor <= size_)
			expand();
		
		iterator it = find(value_pair.first, 1);
		if (it.it_ == it.lst_->end()) {
			it.lst_->insert_head(value_pair);
			++size_;
			it = iterator(it.lst_, it.lst_->begin());
			return pair<iterator, bool>(it, true);
		} else {
			std::destroy_at(it.it_.operator->());
			std::construct_at(it.it_.operator->(), value_pair);
			return pair<iterator, bool>(it, false);
		}
	}
	/**
	 * the value_pair exists, remove and return true
	 * otherwise, return false
	*/
	bool remove(const Key &key){
		iterator it = find(key);
		if (it == iterator())
			return false;
		it.lst_->erase(it.it_);
		return true;
	}

	size_t size() const {
		return size_;
	}
	size_t cap() const {
		return cap_;
	}
};

// 不用继承了，太烂了，iterator 非要能支持 key, value 的形式，继承搞起来很麻烦。重写一个。
template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class linked_hashmap {
public:
	using value_type = pair<const Key, T>;
	using LIST = double_list<value_type>;
	using LIST_IT = typename LIST::iterator;
	using list = double_list<LIST_IT>;
	using list_it = typename list::iterator;

	static constexpr double expand_factor = 2, load_factor = 1;
	static const size_t init_cap = 5;

protected:
	size_t cap_, size_;
	list* data_;
	LIST LST_;

	Hash hash_;
	Equal equal_;
	
	class aux_iterator { // the same aux iterator as in hashmap, not used in iterator or const_iterator
		friend linked_hashmap;
	protected:
		list* lst_;
		list_it it_;
		aux_iterator(list* lst, list_it it)
			: lst_(lst), it_(it) {}
	public:
    	/**
         * elements
         * add whatever you want
        */
// --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
        */
		aux_iterator() 
		: lst_(nullptr), it_() {}
		aux_iterator(const aux_iterator &t)
		: lst_(t.lst_), it_(t.it_) {}
		~aux_iterator() {}
	
		LIST_IT &operator*() const {
			if (it_ == list_it() || it_ == lst_->end())
				throw std::runtime_error("linked_hashmap::aux_iterator::opeartor* : dereference of nullptr");
			return *it_;
		}

		LIST_IT *operator->() const noexcept {
			return it_.operator->();
		}
		bool operator==(const aux_iterator &rhs) const {
			return lst_ == rhs.lst_ && it_ == rhs.it_;
    }
		bool operator!=(const aux_iterator &rhs) const {
			return !(*this == rhs);
		}
	};

	aux_iterator find_basic(const Key& key, bool type = 0) const {
		size_t h = hash_(key) % cap_;
		list* lst = data_ + h;
		for (list_it it = lst->begin(); it != lst->end(); ++it) {
			if (equal_(key, (*it)->first)) // *it is a iterator of LST_
				return aux_iterator(lst, it);
		}
		if (type)
			return aux_iterator(lst, lst->end()); // not found
		else 
			return aux_iterator();
	}

public:
	class const_iterator;
	class iterator {
		friend linked_hashmap;
	protected:
		const LIST* lst_;
		LIST_IT it_;
		iterator(const LIST* lst, LIST_IT it)
			: lst_(lst), it_(it) {}
	public:
    	/**
         * elements
         * add whatever you want
        */
    // --------------------------
		iterator() 
		: lst_(nullptr), it_() {}
		iterator(const iterator &t)
		: lst_(t.lst_), it_(t.it_) {}
		~iterator() {}

		/**
		 * iter++
		 */
		iterator operator++(int) {
			iterator hold = *this;
			++*this;
			return *this;
		}
		/**
		 * ++iter
		 */
		iterator &operator++() {
			if (it_ == LIST_IT() || it_ == lst_->end())
				throw std::runtime_error("linked_hashmap::iterator::opeartor++ : invalid");
			++it_;
			return *this;
		}
		/**
		 * iter--
		 */
		iterator operator--(int) {
			iterator hold = *this;
			--*this;
			return *this;
		}
		/**
		 * --iter
		 */
		iterator &operator--() {
			if (it_ == LIST_IT() || it_ == lst_->begin())
				throw std::runtime_error("linked_hashmap::iterator::opeartor-- : invalid");
			--it_;
			return *this;
		}

		/**
		 * if the iter didn't point to a value
		 * throw "star invalid"
		*/
		value_type &operator*() const {
			if (it_ == LIST_IT() || it_ == lst_->end())
				throw std::runtime_error("linked_hashmap::iterator::opeartor* : dereference of nullptr");
			return *it_;
		}
		value_type *operator->() const noexcept {
			return it_.operator->();
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return lst_ == rhs.lst_ && it_ == rhs.it_;
		}
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		// bool operator==(const const_iterator &rhs) const {
		// 	return lst_ == rhs.lst_ && it_ == rhs.it_;
		// }
		// bool operator!=(const const_iterator &rhs) const {
		// 	return !(*this == rhs);
		// }
	};
 
	class const_iterator : public iterator {
		friend linked_hashmap;
		protected:
		const_iterator(const LIST* lst, LIST_IT it)
			: iterator(lst, it) {}
		public:
        	/**
             * elements
             * add whatever you want
            */
    // --------------------------   
		const_iterator() : iterator() {
		}
		const_iterator(const iterator &other) : iterator(other) {
		}

		/**
		 * iter++
		 */
		const_iterator operator++(int) {
			return this->iterator::operator++(0);
		}
		/**
		 * ++iter
		 */
		const_iterator &operator++() {
			this->iterator::operator++();
			return *this;
		}
		/**
		 * iter--
		 */
		const_iterator operator--(int) {
			return this->iterator::operator--(0);
		}
		/**
		 * --iter
		 */
		const_iterator &operator--() {
			this->iterator::operator--();
			return *this;
		}

		/**
		 * if the iter didn't point to a value
		 * throw 
		*/
		const value_type &operator*() const {
			if (this->it_ == LIST_IT() || this->it_ == this->lst_->end())
				throw std::runtime_error("hashmap::iterator::opeartor* : dereference of nullptr");
			return *(this->it_);
		}
		const value_type *operator->() const noexcept {
			return this->it_.operator->();
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		// REUSE BASE OPRATOR= !=
		// bool operator==(const iterator &rhs) const {
		// 	return lst_ == rhs.lst_ && it_ == rhs.it_;
		// }
		// bool operator!=(const iterator &rhs) const {
		// 	return !(*this == rhs);
		// }
		// bool operator==(const const_iterator &rhs) const {
		// 	return lst_ == rhs.lst_ && it_ == rhs.it_;
		// }
		// bool operator!=(const const_iterator &rhs) const {
		// 	return !(*this == rhs);
		// }
	};
 
	linked_hashmap(size_t cap = init_cap) 
	: cap_(cap), size_(0), data_(cap_ == -1 ? nullptr : new list[cap_]), hash_(), equal_() {
	}
	linked_hashmap(const linked_hashmap &other)	: cap_(-1) {
		*this = other;
	}
	~linked_hashmap() {
		destroy();
	}
	linked_hashmap & operator=(const linked_hashmap &other) {
		if (this == &other)
			return *this;
		this->destroy();
		cap_ = other.cap_;
		if (cap_ == -1) return *this;
		size_ = 0;
		data_ = new list[cap_];
		for (const value_type& kv: other)
			this->insert(kv); 
		return *this;
	}

 	/**
	 * return the value connected with the Key(O(1))
	 * if the key not found, throw 
	*/
	T & at(const Key &key) {
		iterator it = find(key);
		if (it == end())
			throw "not found";
		return it->second;
	}
	const T & at(const Key &key) const {
		iterator it = find(key);
		if (it == end())
			throw "not found";
		return it->second;
	}
	T & operator[](const Key &key) {
		return at(key);
	}
	const T & operator[](const Key &key) const {
		return at(key);
	}

	/**
	 * return an iterator point to the first 
	 * inserted and existed element
	 */
	iterator begin() const {
		return iterator(&LST_, LST_.begin());
	}
	const_iterator cbegin() const {
		return const_iterator(&LST_, LST_.begin());
	}
    /**
	 * return an iterator after the last inserted element
	 */
	iterator end() const {
		return iterator(&LST_, LST_.end());
	}
	const_iterator cend() const {
		return const_iterator(&LST_, LST_.end());
	}
  	/**
	 * if didn't contain anything, return true, 
	 * otherwise false.
	 */
	bool empty() const {
		return !size();
	}

	void destroy() {
		if (cap_ == size_t(-1))
			return;
		LST_.clear();
		delete[] data_;
		cap_ = -1;
	}

  void clear() {
		LST_.clear();
		for (int i = 0; i < cap_; i++)
			data_[i].clear();
		size_ = 0;
	}

	size_t size() const {
		return size_;
	}

	void expand() {
		size_t new_cap = cap_ * expand_factor;
		while (!miller_rabin(new_cap))
			++new_cap;
		
		delete[] data_;
		
		cap_ = new_cap;
		data_ = new list[new_cap];
		for (LIST_IT it = LST_.begin(); it != LST_.end(); ++it) {
			value_type& kv = *it;
			data_[hash_(kv.first) % cap_].insert_head(it);
		}
	}
 	/**
	 * insert the value_piar
	 * if the key of the value_pair exists in the map
	 * update the value instead of adding a new element，
     * then the order of the element moved from inner of the 
     * list to the head of the list
	 * and return false
	 * if the key of the value_pair doesn't exist in the map
	 * add a new element and return true
	*/
	pair<iterator, bool> insert(const value_type &value) {
		if (cap_ * load_factor <= size_)
			expand();
		
		aux_iterator it = find_basic(value.first, 1);
		if (it.it_ == it.lst_->end()) {
			LST_.insert_tail(value);
			it.lst_->insert_head(--LST_.end());
			++size_;
			return pair<iterator, bool>(iterator(&LST_, --LST_.end()), true);
		} else{
			LST_.erase(*(it.it_));
			LST_.insert_tail(value);
			*(it.it_) = --LST_.end();
			return pair<iterator, bool>(iterator(&LST_, --LST_.end()), false);
		}
	}
 	/**
	 * erase the value_pair pointed by the iterator
	 * if the iterator points to nothing
	 * throw 
	*/
	void remove(iterator pos) {
		if (pos.lst_ != LST_ || pos.it_ == LST_->end())	
			throw "remove : not corresponding iterator";
		aux_iterator it = find_basic(*pos);
		if (it == aux_iterator())
			throw "remove : shouldn't happen";
		LST_.erase(pos.it_); // it.it_ is a iterator of 
		it.lst_->erase(it.it_);
		return;
	}
	/**
	 * return how many value_pairs consist of key
	 * this should only return 0 or 1
	*/
	size_t count(const Key &key) const {
		return find(key) != end();
	}
	/**
	 * find the iterator points at the value_pair
	 * which consist of key
	 * if not find, return the iterator 
	 * point at nothing
	*/
	iterator find(const Key &key) const {
		aux_iterator it = find_basic(key, 0);
		if (it == aux_iterator())
			return end();
		return iterator(&LST_, *it);
	}

};


class lru{
    using lmap = sjtu::linked_hashmap<Integer,Matrix<int>,Hash,Equal>;
    using value_type = sjtu::pair<const Integer, Matrix<int> >;
public:
    lru(int size){
    }
    ~lru(){
    }
    /**
     * save the value_pair in the memory
     * delete something in the memory if necessary
    */
    void save(const value_type &v) const{
    }
    /**
     * return a pointer contain the value
    */
    Matrix<int>* get(const Integer &v) const{
    }
    /**
     * just print everything in the memory
     * to debug or test.
     * this operation follows the order, but don't
     * change the order.
    */
    void print(){
    }
};
}

#endif