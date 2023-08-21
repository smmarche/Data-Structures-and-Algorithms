#pragma once

#include <functional> // std::less
#include <iterator> // std::iterator_traits

namespace sort {

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the 
	// iterator's value type will be float. std::less 
	// will select the < for sorting floats as the 
	// default comparator.
	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - */
	template<typename T>
	void swap(T & a, T & b) noexcept {
		T Temporary = std::move(a);//make a temporary function of type T to swap a and b
		a = std::move(b);
		b = std::move(Temporary);
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {

		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type      = typename _it::value_type;
		using reference       = typename _it::reference;
		using pointer         = typename _it::pointer;

		for(RandomIter i = begin; i != end; i++){ //this will be the right most number
			for(RandomIter j = begin; j < i; j++){ //start at begin and only iterate up to i so left number
				if(comp(*i,*j)){//returns status of i < j so if the right number is less than the left number then swap
					swap(*i,*j);//moves smaller number to the left
				}
			}
		}
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		for(RandomIter i = begin; i != end; i++){ //moving forward-flow control
			for(RandomIter j = i; j != begin; j--){ //moving backward from i's spot
				if(comp(*j,*(j-1))){ //returns if j<(j-1) right most compared to left most
					swap(*j,*(j-1)); //swaps the right element to the left most element
				}
				
				else{
					break;
				}
			}
		}
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		for(RandomIter it = begin; it != end; it++){
			RandomIter minimum = it; //index for the smallest number
			for(RandomIter iter = it + 1; iter != end; iter++){
				if(comp(*iter,*minimum)){ //if rhs<lhs then we've found a lower number
					minimum = iter; //set rhs as the minimum; then keep looping 
					//until you've finished the inner loop and have the most minimum number THEN swap	
				}					
			}
			swap(*minimum,*it); //swamp the minimum number to the 1st spot(1st iteration) and so on
		}	
	}
}
