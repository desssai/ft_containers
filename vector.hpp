/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 17:55:27 by ncarob            #+#    #+#             */
/*   Updated: 2022/11/02 22:14:18 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <memory>
# include "Additional/enable_if.hpp"
# include "Additional/is_integral.hpp"
# include "Additional/iterator_traits.hpp"
# include "Additional/normal_iterator.hpp"
# include "Additional/reverse_iterator.hpp"
# include "Additional/integral_constant.hpp"
# include "Additional/lexicographical_compare.hpp"

namespace ft
{

template <typename T, typename Allocator = std::allocator<T> >
class vector {

public:
	/* MEMBER TYPES */
	
	typedef Allocator													allocator_type;
	typedef T															value_type;
	typedef typename allocator_type::reference							reference;
	typedef typename allocator_type::const_reference					const_reference;
	typedef typename allocator_type::pointer							pointer;
	typedef typename allocator_type::const_pointer						const_pointer;
	typedef std::ptrdiff_t	 											difference_type;
	typedef std::size_t 												size_type;
	typedef typename ft::normal_iterator<pointer, vector>				iterator;
	typedef typename ft::normal_iterator<const_pointer, vector>			const_iterator;
	typedef typename ft::reverse_iterator<iterator>						reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator>				const_reverse_iterator;

	/* CONSTRUCTORS DESTRUCTORS + ASSIGNMENT OPERATOR */

	explicit vector(const allocator_type& alloc = allocator_type());
	explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type());
	template <typename InputIterator>
	vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());
	vector(const vector& other);
	~vector();

	vector&	operator = (const vector& other);

	/* ITERATORS */

	iterator				begin(void);
	iterator				end(void);
	const_iterator			begin(void) const;
	const_iterator			cbegin(void) const;
	const_iterator			end(void) const;
	const_iterator			cend(void) const;
	reverse_iterator		rbegin(void);
	reverse_iterator		rend(void);
	const_reverse_iterator	rbegin(void) const;
	const_reverse_iterator	crbegin(void) const;
	const_reverse_iterator	rend(void) const;
	const_reverse_iterator	crend(void) const;

	/* CAPACITY */

	size_type	size(void) const;
	size_type	max_size(void) const;
	void		resize(size_type n, const value_type& val = value_type());
	size_type	capacity(void) const;
	bool		empty(void) const;
	void		reserve(size_type n);
	void		shrink_to_fit(void);

	/* ELEMENT ACCESS */

	reference			operator [] (const difference_type& offset);
	const_reference		operator [] (const difference_type& offset) const;
	reference			at(size_type n);
	const_reference		at(size_type n) const;
	reference			front(void);
	const_reference		front(void) const;
	reference			back(void);
	const_reference		back(void) const;
	value_type*			data(void);
	const value_type*	data(void) const;

	/* MODIFIERS */

	template <typename Iterator>
	void		assign(Iterator first, Iterator last);
	void		assign(size_type n, const value_type& val);
	void		push_back(const value_type& val);
	void		pop_back(void);
	template <typename Iterator>
	void		insert(iterator position, Iterator first, Iterator last);
	iterator	insert(iterator position, const value_type& val);
    void		insert(iterator position, size_type n, const value_type& val);
	iterator 	erase(iterator position);
	iterator 	erase(iterator first, iterator last);
	void		swap(vector& other);
	void		clear(void);

	/* ALLOCATOR */

	allocator_type	get_allocator(void) const;

	/* NON-MEMBER FUNCTION OVERLOADS */

	template <typename Y, typename Alloc>
	friend void swap(vector<Y, Alloc>& lhs, vector<Y, Alloc>& rhs);

private:
	size_type	_size;
	Allocator	_alloc;
	size_type	_capacity;
	pointer		_pointer;

	/* CONSTRUCTOR TEMPLATE OVERLOADING */
	template <typename _Integer>
	void	__construct(_Integer size, _Integer val, ft::true_type);
	template <typename _Iterator>
	void	__construct(_Iterator first, _Iterator last, ft::false_type);
	
	/* ASSIGN TEMPLATE OVERLOADING */
	template <typename _Integer>
	void	__assign(_Integer size, _Integer val, ft::true_type);
	template <typename _Iterator>
	void	__assign(_Iterator first, _Iterator last, ft::false_type);
	template <typename InputIterator>
	void	__assign_range(InputIterator first, InputIterator last, std::input_iterator_tag);
	template <typename ForwardIterator>
	void	__assign_range(ForwardIterator first, ForwardIterator last, std::forward_iterator_tag);

	/* INSERT TEMPLATE OVERLOADING */
	template <typename _Integer>
	void	__insert(iterator position, _Integer n, _Integer val, ft::true_type);
	template <typename _Iterator>
	void	__insert(iterator position, _Iterator first, _Iterator last, ft::false_type);
	template <typename InputIterator>
	void	__insert_range(iterator position, InputIterator first, InputIterator last, std::input_iterator_tag);
	template <typename ForwardIterator>
	void	__insert_range(iterator position, ForwardIterator first, ForwardIterator last, std::forward_iterator_tag);

};

/* CONSTRUCTORS AND DESTRUCTOR START --> */

/* DEFAULT CONSTRUCTOR */
template <typename T, typename Allocator>
vector<T, Allocator>::vector(const allocator_type& alloc) : _size(0), _alloc(alloc), _capacity(0), _pointer(nullptr) {}

/* FILL CONSTRUCTOR */
template <typename T, typename Allocator>
vector<T, Allocator>::vector(size_type n, const value_type& val, const allocator_type& alloc) : _size(0), _alloc(alloc), _capacity(n), _pointer(_alloc.allocate(_capacity)) {
	while (_size < _capacity)
		_alloc.construct(&_pointer[_size++], val);
}

/* RANGE CONSTRUCTOR */
template <typename T, typename Allocator>
template <typename InputIterator>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& alloc) : _size(0), _alloc(alloc), _capacity(0), _pointer(nullptr) {
	typedef typename ft::is_integral<InputIterator>::type _Integer;
	__construct(first, last, _Integer());
}

/* COPY CONSTRUCTOR */
template <typename T, typename Allocator>
vector<T, Allocator>::vector(const vector& other) : _pointer(nullptr) {
	*this = other;
}

/* DESTRUCTOR */
template <typename T, typename Allocator>
vector<T, Allocator>::~vector() {
	if (_pointer) {
		clear();
		_alloc.deallocate(_pointer, _capacity);
	}
}

template <typename T, typename Allocator>
vector<T, Allocator>&	vector<T, Allocator>::operator = (const vector& other) {
	if (*this != other) {
		vector<T, Allocator>::~vector();
		_size = 0;
		_alloc = other._alloc;
		_capacity = other._capacity;
		_pointer = _alloc.allocate(_capacity);
		assign(other.begin(), other.end());
	}
	return (*this);
}

/* <-- CONSTRUCTORS AND DESTRUCTOR END */


/* ITERATORS START --> */

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin(void) {
	return iterator(_pointer);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin(void) const {
	return const_iterator(_pointer);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end(void) {
	return iterator(_pointer + static_cast<difference_type>(_size));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end(void) const {
	return const_iterator(_pointer + static_cast<difference_type>(_size));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin(void) const {
	return const_iterator(_pointer);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend(void) const {
	return const_iterator(_pointer + static_cast<difference_type>(_size));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin(void) {
	return reverse_iterator(end());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend(void) {
	return reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin(void) const {
	return const_reverse_iterator(end());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend(void) const {
	return const_reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin(void) const {
	return const_reverse_iterator(cend());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend(void) const {
	return const_reverse_iterator(cbegin());
}

/* <-- ITERATORS END */


/* CAPACITY START --> */

template <typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size(void) const {
	return _size;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size(void) const {
	return _alloc.max_size() > __LONG_LONG_MAX__ ? __LONG_LONG_MAX__ : _alloc.max_size();
}

template <typename T, typename Allocator>
void vector<T, Allocator>::resize(size_type n, const value_type& val) {
	if (n <= _size)
		for (size_type i = n; i < _size; ++i)
			_alloc.destroy(&_pointer[i]);
	else if (n <= _capacity)
		for (size_type i = _size; i < n; ++i)
			_alloc.construct(&_pointer[i], val);
	else {
		reserve(n > _capacity * 2 ? n : _capacity * 2);
		for (size_type i = _size; i < n; ++i)
			_alloc.construct(&_pointer[i], val);
	}
	_size = n;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity(void) const {
	return _capacity;
}

template <typename T, typename Allocator>
bool vector<T, Allocator>::empty(void) const {
	return _size == 0 ? true : false;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::reserve(size_type n) {
	if (n > max_size())
		throw std::length_error("vector");
	if (n > _capacity) {
		size_type new_capacity = n * 100 > max_size() ? max_size() : n * 100;
		pointer copy = _alloc.allocate(new_capacity);

		if (_pointer) {
			for (size_type i = 0; i < _size; ++i) {
				_alloc.construct(&copy[i], _pointer[i]);
				_alloc.destroy(&_pointer[i]);
			}
			_alloc.deallocate(_pointer, _capacity);
		}
		_pointer = copy;
		_capacity = new_capacity;
	}
}

template <typename T, typename Allocator>
void vector<T, Allocator>::shrink_to_fit(void) {
	if (_size != _capacity) {
		pointer copy = _alloc.allocate(_size);

		for (size_type i = 0; i < _size; ++i) {
			_alloc.construct(&copy[i], _pointer[i]);
			_alloc.destroy(&_pointer[i]);
		}
		if (_pointer)
			_alloc.deallocate(_pointer, _capacity);
		_pointer = copy;
		_capacity = _size;
	}
}

/* <-- CAPACITY END */


/* ELEMENT ACCESS START --> */

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator [] (const difference_type& offset) {
	return *(_pointer + offset);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator [] (const difference_type& offset) const {
	return *(_pointer + offset);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n) {
	if (n >= _size)
		throw std::out_of_range("vector");
	return _pointer[n];
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const {
	if (n >= _size)
		throw std::out_of_range("vector");
	return _pointer[n];
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::front(void) {
	return *_pointer;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::front(void) const {
	return *_pointer;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::back(void) {
	return *(_pointer + static_cast<difference_type>(_size - 1));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::back(void) const {
	return *(_pointer + static_cast<difference_type>(_size - 1));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::value_type* vector<T, Allocator>::data(void) {
	return static_cast<value_type*>(_pointer);
}

template <typename T, typename Allocator>
const typename vector<T, Allocator>::value_type* vector<T, Allocator>::data(void) const {
	return static_cast<value_type*>(_pointer);
}

/* <-- ELEMENT ACCESS END */

/* MODIFIERS START --> */

template <typename T, typename Allocator>
template <typename Iterator>
void	vector<T, Allocator>::assign(Iterator first, Iterator last) {
	typedef typename ft::is_integral<Iterator>::type	_Integer;
	__assign(first, last, _Integer());
}

template <typename T, typename Allocator>
void	vector<T, Allocator>::assign(size_type n, const value_type& val) {
	clear();
	if (n > _capacity)
		reserve(n);
	_size = 0;
	while (_size < n)
		_alloc.construct(&_pointer[_size++], val);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::push_back(const value_type& val) {
	if (!_capacity)
		reserve(1);
	else if (_size >= _capacity)
		reserve(_capacity * 2);
	_alloc.construct(&_pointer[++_size - 1], val);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::pop_back(void) {
	_alloc.destroy(&_pointer[--_size]);
}

template <typename T, typename Allocator>
template <typename Iterator>
void vector<T, Allocator>::insert (iterator position, Iterator first, Iterator last) {
	typedef typename ft::is_integral<Iterator>::type	_Integral;
	__insert(position, first, last, _Integral());
}

template <typename T, typename Allocator>
void vector<T, Allocator>::insert (iterator position, size_type n, const value_type& val) {
	difference_type	before = position - begin();
	difference_type	after = end() - position;
	
	if (!n)
		return ;
	reserve(_size + n);
	if (_size) {
		for (size_type i = 0; i < n; ++i)
			_alloc.construct(&_pointer[_size + n - 1 - i], _pointer[_size - 1 - i]);
		for (difference_type i = 0; i < after; ++i)
			_pointer[_size - 1 - i] = _pointer[_size - n - i - 1];
		for (size_type i = 0; i < n; ++i)
			_pointer[before + i] = val;
	}
	else
		for (size_type i = 0; i < n; ++i)
			_alloc.construct(&_pointer[i], val);
	_size += n;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert (iterator position, const value_type& val) {
	difference_type	before = position - begin();
	difference_type	after = end() - position;

	reserve(_size + 1);
	if (_size) {
		_alloc.construct(&_pointer[_size], _pointer[_size - 1]);
		for (difference_type i = 1; i < after; ++i)
			_pointer[_size - i] = _pointer[_size - 1 - i];
		_pointer[before] = val;
	}
	else
		_alloc.construct(_pointer, val);
	++_size;
	return vector<T, Allocator>::iterator(&_pointer[before]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(iterator position) {
	difference_type	move_diff = (end() - position) - 1;
	for (difference_type i = 0; i < move_diff; ++i)
		*(position + i) = *(position + i + 1);
	_alloc.destroy(&*(position + move_diff));
	--_size;
	return position;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(iterator first, iterator last) {
	difference_type	move_diff = (end() - last);
	difference_type erase_diff = last - first;
	for (difference_type i = 0; i < move_diff; ++i)
		*(first + i) = *(last + i);
	for (difference_type i = 0; i < erase_diff; ++i) {
		_alloc.destroy(&*(first + move_diff + i));
		_size--;
	}
	return (first + move_diff);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::clear(void) {
	while (_size > 0)
		_alloc.destroy(&_pointer[--_size]);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::swap(vector& other) {
	pointer		p_copy = _pointer;
	size_type	s_copy = _size;
	size_type	c_copy = _capacity;

	_pointer = other._pointer; other._pointer = p_copy;
	_size = other._size; other._size = s_copy;
	_capacity = other._capacity; other._capacity = c_copy;
}

/* <-- MODIFIERS END */

/* ALLOCATOR START --> */

template <typename T, typename Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator(void) const {
	return allocator_type();
}

/* <-- ALLOCATOR END */

/* NON-MEMBER FUNCTION OVERLOADS START --> */

template <class T, class Allocator>
bool operator	==	(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	size_t j = lhs.size();
	for (size_t i = 0; i < j; ++i)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}

template <class T, class Allocator>
bool operator	!=	(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(lhs == rhs);
}

template <class T, class Allocator>
bool operator	<	(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Allocator>
bool operator	<=	(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(rhs < lhs);
}

template <class T, class Allocator>
bool operator	>	(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return (rhs < lhs);
}

template <class T, class Allocator>
bool operator	>=	(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(lhs < rhs);
}

template <typename Y, typename Alloc>
void swap(vector<Y, Alloc>& lhs, vector<Y, Alloc>& rhs) {
	typename vector<Y, Alloc>::pointer		p_copy = lhs._pointer;
	typename vector<Y, Alloc>::size_type	s_copy = lhs._size;
	typename vector<Y, Alloc>::size_type	c_copy = lhs._capacity;

	lhs._pointer = rhs._pointer; rhs._pointer = p_copy;
	lhs._size = rhs._size; rhs._size = s_copy;
	lhs._capacity = rhs._capacity; rhs._capacity = c_copy;
}


/* <-- NON-MEMBER FUNCTION OVERLOADS END */


/* PRIVATE FUNCTIONS START --> */

template <typename T, typename Allocator>
template <typename _Integer>
void vector<T, Allocator>::__construct(_Integer size, _Integer val, ft::true_type) {
	__assign(size, val, ft::true_type());
}

template <typename T, typename Allocator>
template <typename _Iterator>
void vector<T, Allocator>::__construct(_Iterator first, _Iterator last, ft::false_type) {
	typedef typename std::iterator_traits<_Iterator>::iterator_category _Category;
	__assign_range(first, last, _Category());
}

template <typename T, typename Allocator>
template <typename _Integer>
void	vector<T, Allocator>::__assign(_Integer size, _Integer val, ft::true_type) {
	assign(static_cast<size_type>(size), val);
}

template <typename T, typename Allocator>
template <typename _Iterator>
void	vector<T, Allocator>::__assign(_Iterator first, _Iterator last, ft::false_type) {
	typedef typename std::iterator_traits<_Iterator>::iterator_category _Category;
	__assign_range(first, last, _Category());
}

template <typename T, typename Allocator>
template <typename InputIterator>
void	vector<T, Allocator>::__assign_range(InputIterator first, InputIterator last, std::input_iterator_tag) {
	for (; _capacity && first != last && _size <= _capacity; ++_size, ++first)
		_alloc.construct(&_pointer[_size], *first);
	if (first == last && _size)
		_alloc.destroy(&_pointer[_size]);
	else
		__insert_range(end(), first, last, std::input_iterator_tag());
}

template <typename T, typename Allocator>
template <typename ForwardIterator>
void	vector<T, Allocator>::__assign_range(ForwardIterator first, ForwardIterator last, std::forward_iterator_tag) {
	size_type	new_size = std::distance(first, last);

	clear();
	if (new_size > _capacity)
		reserve(new_size);
	_size = 0;
	while (_size < new_size)
		_alloc.construct(&_pointer[_size++], *first++);
}

template <typename T, typename Allocator>
template <typename _Integer>
void	vector<T, Allocator>::__insert(iterator position, _Integer n, _Integer val, ft::true_type) {
	insert(position, static_cast<size_type>(n), val);
}

template <typename T, typename Allocator>
template <typename _Iterator>
void	vector<T, Allocator>::__insert(iterator position, _Iterator first, _Iterator last, ft::false_type) {
	typedef typename std::iterator_traits<_Iterator>::iterator_category	_Category;
	__insert_range(position, first, last, _Category());
}

template <typename T, typename Allocator>
template <typename InputIterator>
void	vector<T, Allocator>::__insert_range(iterator position, InputIterator first, InputIterator last, std::input_iterator_tag) {
	for (; first != last; ++first) {
		position = insert(position, *first);
		++position;
	}
}

template <typename T, typename Allocator>
template <typename ForwardIterator>
void	vector<T, Allocator>::__insert_range(iterator position, ForwardIterator first, ForwardIterator last, std::forward_iterator_tag) {
	difference_type	before = position - begin();
	difference_type	after = end() - position;
	difference_type	n = std::distance(first, last);

	if (!n)
		return ;
	reserve(_size + n);
	if (_size) {
		for (difference_type i = 0; i < n; ++i)
			_alloc.construct(&_pointer[_size + n - 1 - i], _pointer[_size - 1 - i]);
		for (difference_type i = 0; i < after; ++i)
			_pointer[_size - 1 - i] = _pointer[_size - n - 1 - i];
		for (difference_type i = 0; i < n; ++i)
			_pointer[before + i] = *first++;
	}
	else
		for (difference_type i = 0; i < n; ++i)
			_alloc.construct(&_pointer[i], *first++);
	_size += n;
}

/* <-- PRIVATE FUNCTIONS END */

} /* FT NAMESPACE */

#endif /* VECTOR_H */
