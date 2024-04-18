//
// Created by Surya on 13-04-2024
//
#include <stdio.h>
#include "sharedptrfwd.h"
#include <unistd.h>

int main() {

    shared_ptr ptr = sync_with_dynamic_allocator(2);
    allocator_load(&ptr);

    shared_ptr ptr2 = sync_with_dynamic_allocator(2);
    allocator_load(&ptr2);

    shared_ptr ptr3 = sync_with_dynamic_allocator(2);
    allocator_load(&ptr3);

    shared_ptr ptr4 = sync_with_dynamic_allocator(2);
    allocator_load(&ptr4);
    sleep(1);

    sharedAdjList();
    

    // created 4 pointers

    printf("4 Pointers were created\n");

    move_ref(&ptr, &ptr2);  // ptr is pointing to ptr2  so the memory before ptr was pointing is collected as garbage.
    sleep(1); // let the garbage collector do its job.
    move_ref(&ptr3, &ptr4); // ptr3 is pointing to ptr4  so the memory before ptr3 was pointing is collected as garbage.
    sleep(1); // let the garbage collector do its job.
    move_ref(&ptr, &ptr4);  // ptr is now pointing to ptr4 so the earlier ptr was pointing to is not collected as garbage since ptr2 is still pointing to it.
    move_ref(&ptr2, &ptr4); // ptr2 is also now pointing to ptr4 so the earlier ptr2 was pointing is now collected as garbage.
    sleep(1); // let the garbage collector do its job.

    // Since the execution of the program is so fast that before the garbage collector is able to collect the garbage the
    // execution terminates... therefore we need to pause the execution using sleep(1) so that we can see that the garbage is being collected.
     
    sharedAdjList();

    printf("Collected\n");

    struct _mem_tf_info info = get_memory_ref_count();

    for (int i=0; i<info.cnt;i++) {
        printf("%d\n",info._mem_arr[i]);
    }



}