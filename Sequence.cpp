// FILE: Sequence.cpp
// CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
// INVARIANT for the sequence ADT:
//   1. The number of items in the sequence is in the member variable
//      used;
//   2. The actual items of the sequence are stored in a partially
//      filled array. The array is a dynamic array, pointed to by
//      the member variable data. For an empty sequence, we do not
//      care what is stored in any of data; for a non-empty sequence
//      the items in the sequence are stored in data[0] through
//      data[used-1], and we don't care what's in the rest of data.
//   3. The size of the dynamic array is in the member variable
//      capacity.
//   4. The index of the current item is in the member variable
//      current_index. If there is no valid current item, then
//      current_index will be set to the same number as used.
//      NOTE: Setting current_index to be the same as used to
//            indicate "no current item exists" is a good choice
//            for at least the following reasons:
//            (a) For a non-empty sequence, used is non-zero and
//                a current_index equal to used indexes an element
//                that is (just) outside the valid range. This
//                gives us a simple and useful way to indicate
//                whether the sequence has a current item or not:
//                a current_index in the valid range indicates
//                that there's a current item, and a current_index
//                outside the valid range indicates otherwise.
//            (b) The rule remains applicable for an empty sequence,
//                where used is zero: there can't be any current
//                item in an empty sequence, so we set current_index
//                to zero (= used), which is (sort of just) outside
//                the valid range (no index is valid in this case).
//            (c) It simplifies the logic for implementing the
//                advance function: when the precondition is met
//                (sequence has a current item), simply incrementing
//                the current_index takes care of fulfilling the
//                postcondition for the function for both of the two
//                possible scenarios (current item is and is not the
//                last item in the sequence).

#include <cassert>
#include "Sequence.h"

using namespace std;

namespace CS3358_FA2017
{
   // CONSTRUCTORS and DESTRUCTOR
   sequence::sequence(size_type initial_capacity) : used(0), current_index(0)
           , capacity(initial_capacity)
   {
       // Check initial_capacity validity per pre-condition
       // requirements for function stub listed in Sequence.h
       if(initial_capacity < 1){capacity = 1;}

       // Create new dynamic sequence array.
       data = new value_type[capacity];
   }

   sequence::sequence(const sequence& source) :
           used(source.used), current_index(source.current_index),
           capacity(source.capacity)
   {
       // Create new dynamic array for this data pointer.
       data = new value_type[capacity];

       // Copy data from source to this data.
       for (size_type index = 0; index < used; ++index) {
           data[index] = source.data[index];
       }

   }
   sequence::~sequence()
   {
       // Free up dynamic memory and point to 0.
       delete [] data;
       data = NULL;
   }

   // MODIFICATION MEMBER FUNCTIONS
   void sequence::resize(size_type new_capacity)
   {
       // Check validity of new_capacity to ensure it's inline
       // with class invariant.
       if(new_capacity < 1){new_capacity = 1;}
       if(new_capacity < used) {capacity = used;}
       else {capacity = new_capacity;}

       // Create new dynamic array based on adjusted capacity.
       value_type *temp_data = new value_type[capacity];

       // Copy contents of dynamic array to new location.
       for (size_type index = 0; index < used; ++index) {
           temp_data[index] = data[index];
       }

       // Deallocate the space used by previous data array.
       delete [] data;

       // Move new dynamic array back to private member data.
       data = temp_data;
   }

   void sequence::start()
   {
       // Set current_index according to the invariant #4. If the sequence
       // has items then current_index is the first item in sequence data[0]
       // or current_index == 0 otherwise there's no current item. According
       // to invariant #4 if there's no current item then current_index == used

       current_index = 0;

   }

   void sequence::advance()
   {
       // Protect pre-condition. If false then terminate the program,
       // otherwise continue execution of sequence::advance().
       assert(is_item());

       // According to invariant #4 if there's no current item then
       // current_index == used. Otherwise the current item is the item
       // after current_index.
       current_index = current_index+1;
   }

   void sequence::insert(const value_type& entry)
   {
       // Check to see if we need to resize the dynamic array. If
       // we do the multiple current capacity by 1.25 and add +1 to
       // satisfy the resize rule.
       if(used == capacity){resize(size_type (1.25 * capacity)+1);}

       if(!is_item()) {

           // There's NO current item. Insert entry at the beginning of the
           // sequence or current_index == 0. Starting from used + 1 shift
           // item's towards used to accommodate inserting entry at beginning
           // of sequence.
           current_index = 0;
           for(size_type index = used + 1; index > current_index; --index){
               data[index] = data[index-1];
           }
           data[current_index] = entry;
           ++used;

       } else {

           // There IS a current item. Insert entry prior to the current item
           // or current_index - 1. Starting from used + 1 shift item's towards
           // used to accommodate inserting entry prior to current item.
           for(size_type index = used + 1; index > current_index; --index){
               data[index] = data[index-1];
           }
           data[current_index] = entry;
           ++used;
       }

   }

   void sequence::attach(const value_type& entry)
   {
       // Check to see if we need to resize the dynamic array. If
       // we do the multiple current capacity by 1.25 and add +1 to
       // satisfy the resize rule.
       if(used == capacity){resize(size_type (1.25 * capacity)+1);}

       if(!is_item()){

           // There's NO current item. Attach entry at the end of the sequence
           // or current_index == used. Make entry the current item.
           data[current_index] = entry;
           used++;

       } else {

           // There IS a current item. Attach entry after the current item,
           // make entry the new current entry. To do this shift all items
           // after current_index to the right by 1.

           // Move current_index up by 1 to accommodate attaching entry
           // after original current_index.
           current_index = current_index+1;

           for (size_type index = used + 1; index > current_index; --index) {
               data[index] = data[index-1];
           }
           data[current_index] = entry; // current_index + 1 = entry
           ++used;
       }
   }

   void sequence::remove_current()
   {
       // Protect pre-condition. If false then terminate the program,
       // otherwise continue execution of sequence::remove_current().
       assert(is_item());


       // According to the pre/post condition's for remove_current() if the
       // current item was the last item then there's no current item. According
       // to invariant #4 if there's no current item then current_index == used.
       //current_index == used-1


       // Valid current item. Remove current and shift items to the left.
       for (size_type index = current_index; index < used-1; ++index) {
           data[index] = data[index + 1];
       }
       // Update used after removing item.
       --used;

   }

   sequence& sequence::operator=(const sequence& source)
   {
       // Self-assignment fail safe. Check for self-assignment.
       // If self-assignment is present then return invoking object.
       if (this == &source)
           return *this;

       // Create temporary dynamic array to safely assign contents
       // of array.
       value_type *temp_data = new value_type[source.capacity];

       // Moved contents of rhs array to temp
       for (size_type index = 0; index < source.used; ++index) {
           temp_data[index] = source.data[index];
       }

       // Deallocate old dynamic array.
       delete [] data;

       // Start assigning member variables from rhs.
       data = temp_data;
       capacity = source.capacity;
       used = source.used;
       current_index = source.current_index;

       return *this;
   }

   // CONSTANT MEMBER FUNCTIONS
   sequence::size_type sequence::size() const
   {
       // Size equates to the number of items in a sequence this number
       // is tracked by the private member variable used.
       return used;

   }

   bool sequence::is_item() const
   {
       // An item is invalid ONLY if a sequence is NOT empty used == 0
       // OR if the current index is NOT at the very last item in the
       // sequence or current_index == used.
       return (current_index != used);
   }

   sequence::value_type sequence::current() const
   {
       // Protect pre-condition. If false then terminate the program,
       // otherwise return the current item of the sequence.
       assert(is_item());

       return data[current_index];
   }
}

