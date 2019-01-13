
#include "sorting.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int compCnt = 0, moveCntr = 0;

/// insertion sort reset counters for each time a new sort will be on use
void resetCounters(int ctr1, int ctr2){
    ctr1 = 0;
    ctr2 = 0;
}

/// clock difference calculation
double diffclock( clock_t clock1, clock_t clock2 ) {

    clock_t diffticks = clock2 - clock1*0.99;
    double diffms    =  ((double)diffticks * 1000 / CLOCKS_PER_SEC);///diffticks / ( CLOCKS_PER_SEC / 1000 );

    return diffms;
}

/// insertion sort
void sorting::insertionSort(int *arr, int size, int &compCount, int &moveCount){

    /// while unsorted amount is less than size
    for (int unsorted = 1; unsorted < size; ++unsorted) {

        /// declare next item that will be checked then arrange the current location
        int nextItem = arr[unsorted];
        int loc = unsorted;

        /// compare elements, update location, counter
        for (  ;(loc > 0) && (arr[loc-1] > nextItem); --loc){
            arr[loc] = arr[loc-1];
            compCount++;
        }

        /// update element on current location, counter
        arr[loc] = nextItem;
        moveCount++;
    }

    /// save counts
    compCnt = compCount;
    moveCntr = moveCount;

}

/// merge sort

void sorting::mergeSort(int *arr, int size, int &compCount, int &moveCount){

    /// declare first and last elements
    int&first = arr[0];
    int&last  = arr[size-1];

    /// compare first and last

    if (first < last) {

        int mid = (first + last)/2; 	/// index of midpoint

        ///mergeSort(arr, arr[0], mid);

        last = mid;
        size = sizeof(arr)/4;
        mergeSort(arr, size, compCount, moveCount); ///Divide to left side

        ///mergeSort(arr, mid+1, arr[size-1]);

        first = mid + 1;
        size = sizeof(arr)/4;
        mergeSort(arr, size, compCount, moveCount); ///Divide to right side

    /// merge the two halves -- Conquer ///

        ///merge(arr, first, mid, last);

        const int MAX_SIZE = size;  ///max size for temp array

        int tempArray[MAX_SIZE]; 	/// temporary array

        int first1 = first; 	    /// beginning of first subarray
        int last1 = mid; 		    /// end of first subarray
        int first2 = mid + 1;	    /// beginning of second subarray
        int last2 = last;		    /// end of second subarray
        int index = first1;         /// next available location in tempArray

        ///compare keys and their values, then update keys with counts
        for ( ; (first1 <= last1) && (first2 <= last2); ++index) {
            if (arr[first1] < arr[first2]) {
                tempArray[index] = arr[first1];
                ++first1;

                compCount++;
            }

            else {
                tempArray[index] = arr[first2];
                ++first2;

                compCount++;
            }
        }

        /// finish off the first subarray, if necessary
       for (; first1 <= last1; ++first1, ++index){
            tempArray[index] = arr[first1];
            moveCount++;
       }

       /// finish off the second subarray, if necessary
       for (; first2 <= last2; ++first2, ++index){
            tempArray[index] = arr[first2];
            moveCount++;
       }

       /// copy the result back into the original array
       for (index = first; index <= last; ++index){
            arr[index] = tempArray[index];

       }
    }

    /// save counts
    compCnt = compCount;
    moveCntr = moveCount;
}

/// swapping function
void swap(int &x, int &y) {
   int temp = x;
   x = y;
   y = temp;
}

/// partition function
void partition(int*arr, int first, int last, int &pivotIndex, int &compCount, int &moveCount) {
    /// first element for comparison, second for move

    /// Precondition: theArray[first..last] is an array; first <= last.
    /// Postcondition: Partitions theArray[first..last] such that:
    ///  S1 = theArray[first..pivotIndex-1] < pivot
    ///   theArray[pivotIndex] == pivot
    ///   S2 = theArray[pivotIndex+1..last] >= pivot

    /// place pivot in theArray[first]
    ///choosePivot(theArray, first, last);

    int pivot = arr[first]; /// copy pivot

    /// initially, everything but pivot is in unknown
    int lastS1 = first;           /// index of last item in S1
    int firstUnknown = first + 1; /// index of first item in unknown

    /// move one item at a time until unknown region is empty
    for (  ; firstUnknown <= last; ++firstUnknown) {
        /// Invariant: theArray[first+1..lastS1] < pivot
        ///            theArray[lastS1+1..firstUnknown-1] >= pivot

        /// move item from unknown to proper region, update counts
        if (arr[firstUnknown] < pivot) {  	/// belongs to S1
          ++lastS1;
          swap(arr[firstUnknown], arr[lastS1]);

            compCount++;
            moveCount++;

      }	/// else belongs to S2

    }
    /// place pivot in proper position and mark its location, update count
    swap(arr[first], arr[lastS1]);
    moveCount++;

    pivotIndex = lastS1;

}

/// quick sort
void sorting::quickSort(int *arr, int size, int &compCount, int &moveCount){

    /// Precondition: theArray[first..last] is an array.
    /// Postcondition: theArray[first..last] is sorted.

    /// declare pivot, first and last elements
    int pivotIndex;

    int&firstly = arr[0];
    int&lastly = arr[size-1];

    /// compare elements
    if (firstly < lastly) {

        /// create the partition: S1, pivot, S2
        partition(arr, firstly, lastly, pivotIndex, compCount, moveCount);

        /// sort regions S1 and S2

        ///quicksort(arr, first, pivotIndex-1);
        lastly = arr[pivotIndex--];

        quickSort(arr, size, compCount, moveCount);

        ///quicksort(arr, pivotIndex+1, last);

        firstly = arr[pivotIndex++];
        quickSort(arr, size, compCount, moveCount);

        compCount++; ///update count
    }

    /// save counts
    compCnt = compCount;
    moveCntr = moveCount;

}

/// testing sorting and obtaining results
void performanceOutput(int size, int*arr, int&compCount, int&moveCount, clock_t elapsedTime, string sortName){

    cout<<sortName<<endl;
    cout<<"Elapsed Time: "<<elapsedTime<<" Compcount: "<<compCount<<" Movecount: "<<moveCount<<endl;

    resetCounters(compCount, moveCount);
}

/// calculation part for performance testing
void sorting::performanceAnalysis(){

    ///3 identical arrays with random 1000 integers, use arrays for sorts///

    ///  variables for random generator and ctime
    int randNum = 0;
    clock_t begin, end;

    ///size (5000; 10000; 15000; 20000)
    int SIZE1 = 5000, SIZE2 = 10000, SIZE3 = 15000, SIZE4 = 20000;

    ///(b1) random order

    ///size 5000///

    ///initialize array and values
    int*array1 = new int[SIZE1];
    int*array2 = new int[SIZE1];
    int*array3 = new int[SIZE1];

    for(int i=0;i<SIZE1;i++){
        randNum = rand()%1000;
        array1[i] = randNum;
        array2[i] = randNum;
        array3[i] = randNum;

    }

    /// in order, testing insertion, merge and quick sorts for size 5000

    cout<<"\nArray Size: "<<SIZE1<<endl;

    begin = clock();
    insertionSort(array1, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array1,compCnt,moveCntr,diffclock(begin,end),"Merge sort");

    begin = clock();
    mergeSort(array2, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 10000

    ///initialize array and values

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE2];
    array2 = new int[SIZE2];
    array3 = new int[SIZE2];

    for(int i=0;i<SIZE2;i++){
        randNum = rand()%1000;
        array1[i] = randNum;
        array2[i] = randNum;
        array3[i] = randNum;
    }

    /// in order, testing insertion, merge and quick sorts for size 10000

    cout<<"\nArray Size: "<<SIZE2<<endl;

    begin = clock();
    insertionSort(array1, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 15000

    ///initialize array and values

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE3];
    array2 = new int[SIZE3];
    array3 = new int[SIZE3];

    for(int i=0;i<SIZE3;i++){
        randNum = rand()%1000;
        array1[i] = randNum;
        array2[i] = randNum;
        array3[i] = randNum;
    }

    /// in order, testing insertion, merge and quick sorts for size 15000

    cout<<"\nArray Size: "<<SIZE3<<endl;

    begin = clock();
    insertionSort(array1, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 20000

    ///initialize array and values

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE4];
    array2 = new int[SIZE4];
    array3 = new int[SIZE4];

    for(int i=0;i<SIZE4;i++){
        randNum = rand()%1000;
        array1[i] = randNum;
        array2[i] = randNum;
        array3[i] = randNum;
    }

    /// in order, testing insertion, merge and quick sorts for size 20000

    cout<<"\nArray Size: "<<SIZE4<<endl;

    begin = clock();
    insertionSort(array1, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    /// (b2) ascending order

    ///size 5000
    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE1];
    array2 = new int[SIZE1];
    array3 = new int[SIZE1];

    for(int i=0;i<SIZE1;i++){

        array1[i] = i;
        array2[i] = i;
        array3[i] = i;
    }
    ///result

    cout<<"\nArray Size: "<<SIZE1<<endl;

    begin = clock();
    insertionSort(array1, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 10000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE2];
    array2 = new int[SIZE2];
    array3 = new int[SIZE2];

    for(int i=0;i<SIZE2;i++){
         array1[i] = i;
        array2[i] = i;
        array3[i] = i;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE2<<endl;

    begin = clock();
    insertionSort(array1, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 15000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE3];
    array2 = new int[SIZE3];
    array3 = new int[SIZE3];

    for(int i=0;i<SIZE3;i++){
         array1[i] = i;
        array2[i] = i;
        array3[i] = i;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE3<<endl;

    begin = clock();
    insertionSort(array1, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 20000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE4];
    array2 = new int[SIZE4];
    array3 = new int[SIZE4];

    for(int i=0;i<SIZE4;i++){
         array1[i] = i;
        array2[i] = i;
        array3[i] = i;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE4<<endl;

    begin = clock();
    insertionSort(array1, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///(b3) descending order

    ///size 5000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE1];
    array2 = new int[SIZE1];
    array3 = new int[SIZE1];

    for(int i=0;i<SIZE1;i++){

        array1[i] = SIZE1-i-1;
        array2[i] = SIZE1-i-1;
        array3[i] = SIZE1-i-1;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE1<<endl;

    begin = clock();
    insertionSort(array1, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE1, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE1, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 10000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE2];
    array2 = new int[SIZE2];
    array3 = new int[SIZE2];

    for(int i=0;i<SIZE2;i++){
        array1[i] = SIZE2-i-1;
        array2[i] = SIZE2-i-1;
        array3[i] = SIZE2-i-1;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE2<<endl;

    begin = clock();
    insertionSort(array1, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE2, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE2, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 15000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE3];
    array2 = new int[SIZE3];
    array3 = new int[SIZE3];

    for(int i=0;i<SIZE3;i++){
        array1[i] = SIZE3-i-1;
        array2[i] = SIZE3-i-1;
        array3[i] = SIZE3-i-1;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE3<<endl;

    begin = clock();
    insertionSort(array1, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE3, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE3, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    ///size 20000

    delete[] array1;
    delete[] array2;
    delete[] array3;
    array1 = new int[SIZE4];
    array2 = new int[SIZE4];
    array3 = new int[SIZE4];

    for(int i=0;i<SIZE4;i++){
        array1[i] = SIZE4-i-1;
        array2[i] = SIZE4-i-1;
        array3[i] = SIZE4-i-1;
    }

    ///result

    cout<<"\nArray Size: "<<SIZE4<<endl;

    begin = clock();
    insertionSort(array1, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array1, compCnt, moveCntr, diffclock(begin, end), "Merge sort");

    begin= clock();
    mergeSort(array2, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array2, compCnt, moveCntr, diffclock(begin, end), "Quick sort");

    begin = clock();
    quickSort(array3, SIZE4, compCnt, moveCntr);
    end = clock();

    performanceOutput(SIZE4, array3, compCnt, moveCntr, diffclock(begin, end), "Insertion sort");

    delete[] array1;
    delete[] array2;
    delete[] array3;

    ///delete arrays that wont be used anymore

    array1 = NULL;
    array2 = NULL;
    array3 = NULL;

}
///end of sorting.cpp
