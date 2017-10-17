#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <math.h>
#include <assert.h>
#include <chrono>

// STLSort
template<typename T>
std::vector<T> stl_sort(const std::vector<T>& v)
{
	auto a = v;
	sort(a.begin(), a.end());
	return a;
}

// BubbleSort
template<typename T>
std::vector<T> bubble_sort(const std::vector<T>& v)
{
	auto a = v;
	if (v.size() == 0)
	{
		return a;
	}
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i = 0; i < a.size() - 1; i++)
		{
			if (a[i] > a[i+1])
			{
				std::swap(a[i], a[i+1]);
				changed = true;
			}
		}
	}
	return a;
}

// SelectionSort
template<typename T>
std::vector<T> selection_sort(const std::vector<T>& v)
{
	auto a = v;
	for (int i = 0; i < v.size(); i++)
	{
		auto minIdx = i;
		for (int j = i; j < v.size(); j++)
		{
			if (a[j] < a[minIdx])
			{
				minIdx = j;
			}
		}
		std::swap(a[i], a[minIdx]);
	}
	return a;
}

// InsertionSort
template<typename T>
std::vector<T> insertion_sort(const std::vector<T> &v)
{
	auto a = v;
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = i; j > 0; j--)
		{
			if (a[j] < a[j-1])
			{
				std::swap(a[j], a[j-1]);
			}
			else
			{
				break;
			}
		}
	}
	return a;
}

// MergeSort
template<typename T>
void merge(std::vector<T>& v, std::vector<T>& helper, int low, int middle, int high)
{
	// Copy sub-arrays to be merged into helper array
	for (int i = low; i <= high; i++)
	{
		helper[i] = v[i];
	}

	int leftIndex = low;
	int rightIndex = middle+1;
	int crtIndex = low;
	while (leftIndex <= middle && rightIndex <= high)
	{
		if (helper[leftIndex] < helper[rightIndex])
		{
			v[crtIndex++] = helper[leftIndex++];
		}
		else
		{
			v[crtIndex++] = helper[rightIndex++];
		}
	}

	// Copy left sub-array. Right is already there from first the copying.
	while (leftIndex <= middle)
	{
		v[crtIndex++] = helper[leftIndex++];
	}
}

template<typename T>
void merge_sort_helper(std::vector<T>& v, std::vector<T>& helper, int low, int high)
{
	if (low < high)
	{
		int middle = (low + high)/2;
		merge_sort_helper(v, helper, low, middle);
		merge_sort_helper(v, helper, middle+1, high);
		merge(v, helper, low, middle, high);
	}
}

template<typename T>
std::vector<T> merge_sort(const std::vector<T>& v)
{
	std::vector<T> a = v;
	std::vector<T> helper = std::vector<T>(v.size());
	merge_sort_helper(a, helper, 0, a.size()-1);
	return a;
}


// QuickSort
template<typename T>
int quick_sort_partition(std::vector<T>& v, int left, int right)
{
	T pivot = v[(left+right)/2];
	while (left <= right)
	{
		while(v[left] < pivot)
		{
			left++;
		}
		while (v[right] > pivot)
		{
			right--;
		}
		if (left <= right)
		{
			std::swap(v[left], v[right]);
			left++;
			right--;
		}
	}
	return left;
}

template<typename T>
void quick_sort_helper(std::vector<T>& v, int left, int right)
{
	int pivotIndex = quick_sort_partition(v, left, right);
	if (left < pivotIndex - 1)
	{
		quick_sort_helper(v, left, pivotIndex - 1);
	}
	if (pivotIndex < right)
	{
		quick_sort_helper(v, pivotIndex, right);
	}
}

template<typename T>
std::vector<T> quick_sort(const std::vector<T>& v)
{
	auto a = v;
	if (a.size() == 0)
	{
		return a;
	}
	quick_sort_helper(a, 0, a.size()-1);
	return a;
}


// RadixSort
// Returns digit at index (0-based index)
int radix_get_digit(int n, int index)
{
	assert(index >= 0);
	if (n < 0)
	{
		n *= -1;
	}
	n = n / pow(10, index);
	int digit = n % 10;
	assert(digit >= 0 && digit < 10);
	return digit;
}

template<typename T>
std::vector<T> radix_sort(const std::vector<T>& v)
{
	auto a = v;
	if (a.size() == 0)
	{
		return a;
	}

	std::vector<std::queue<T>> buckets = std::vector<std::queue<T>>(10);

	// 1. Find biggest number
	bool hasNegatives = false;
	T max = a[0];
	for (int i = 0; i < a.size(); i++)
	{
		auto e = abs(a[i]);
		if (e > max)
		{
			max = e;
		}
		if (a[i] < 0)
		{
			hasNegatives = true;
		}
	}
	// 2. Get number of passes	
	int passes = 0;
	while (max > 0)
	{
		max = max / 10;
		passes++;
	}
	// 3. Do passes
	for (int k = 0; k < passes; k++)
	{
		// 3.1 Push into buckets
		for (int i = 0; i < a.size(); i++)
		{
			int bucketIdx = radix_get_digit(a[i], k);
			buckets[bucketIdx].push(a[i]);
		}
		// 3.2 Pop from buckets
		int arrayIdx = 0;
		for (int i = 0; i < buckets.size(); i++)
		{
			std::queue<T>& bucket = buckets[i];
			while(!bucket.empty())
			{
				a[arrayIdx++] = bucket.front();
				bucket.pop();
			}
		}
	}
	if (hasNegatives)
	{
		// 4. Do another pass to separate negatives and positives
		std::vector<T> positives, negatives;
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i] < 0)
			{
				negatives.push_back(a[i]);
			}
			else
			{
				positives.push_back(a[i]);
			}
		}

		reverse(negatives.begin(), negatives.end());
		negatives.insert(negatives.end(), positives.begin(), positives.end());
		a = negatives;
	}
	
	return a;
}

template<typename T>
void print_array(const std::string& msg, const std::vector<T>& v)
{
	std::cout << msg << ": [ ";
	for (auto& e : v)
	{
		std::cout << e << " ";
	}
	std::cout << "]" << std::endl;
}

template<typename T>
bool test_equality(const std::string& msg, const std::vector<T>& ref, const std::vector<T>& v)
{
	bool areEqual = equal(ref.begin(), ref.end(), v.begin());
	if (!areEqual)
	{
		print_array(std::string("[FAIL] ") + msg, v);
	}
	return areEqual;
}

template<typename T>
void test(const std::vector<T>& v)
{
	bool ok = true;
	print_array("Testing", v);
	auto stlSort = stl_sort(v);
	print_array("Sorted", stlSort);
	
	auto bubbleSort = bubble_sort(v);
	ok = test_equality("BubbleSort", stlSort, bubbleSort) && ok;

	auto selectionSort = selection_sort(v);
	ok = test_equality("SelectionSort", stlSort, selectionSort) && ok;

	auto insertionSort = insertion_sort(v);
	ok = test_equality("InsertionSort", stlSort, insertionSort) && ok;

	auto mergeSort = merge_sort(v);
	ok = test_equality("MergeSort", stlSort, mergeSort) && ok;

	auto quickSort = quick_sort(v);
	ok = test_equality("QuickSort", stlSort, quickSort) && ok;

	if (std::is_same<T, int>::value)
	{
		auto radixSort = radix_sort(v);
		ok = test_equality("RadixSort", stlSort, radixSort) && ok;
	}
	
	std::cout << (ok ? "[OK]\n" : "") << std::endl;

}

void do_common_tests()
{
	test(std::vector<int>{1, 2});
	test(std::vector<int>{2, 1});
	test(std::vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
	test(std::vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1});
	test(std::vector<int>{5, -3, 2, 4, -1, -5, 0, 1, -2, -4, 3});
	test(std::vector<int>{ -3, 2, 4, -1, -5, 0, 1, -2, -4, 3});
	test(std::vector<int>{10000, 1000, 100, 10, 1, 0, -1, -10, -100, -1000, -10000});
	test(std::vector<int>{0});
	test(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
	test(std::vector<int>{1, 1, 1, 1});
	test(std::vector<int>{1, 1, 1});
	test(std::vector<int>{});
}

typedef std::vector<int> (*sortFunction)(const std::vector<int>& v);

template<typename T>
void timed_test(const std::string& name, sortFunction f, const std::vector<T>& v, const std::vector<T>& sorted)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto a = f(v);
	auto end = std::chrono::high_resolution_clock::now();
	test_equality(name, sorted, a);
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << name << ": " << time << " ms" << std::endl;
}

void do_long_test(int n = 1000000)
{
	std::vector<int> v(n);
	for (int i = 0; i < n; i++)
	{
		v[i] = rand()%50000-25000;
	}

	auto stlSort = stl_sort(v);
	//print_array("STL", stlSort);

	timed_test("STLSort", stl_sort, v, stlSort);
	//timed_test("BubbleSort", bubble_sort, v);
	//timed_test("InsertionSort", insertion_sort, v);
	//timed_test("SelectionSort", selection_sort, v);
	timed_test("MergeSort", merge_sort, v, stlSort);
	timed_test("QuickSort", quick_sort, v, stlSort);
	timed_test("RadixSort", radix_sort, v, stlSort);
}

int main()
{
	do_common_tests();
	//do_long_test();
	return 0;
}