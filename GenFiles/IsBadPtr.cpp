#include <stdio.h>
#include <stdbool.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/vm_map.h>

//статья документация

bool IsBadPtr(void* ptr)
{
    vm_map_t task = mach_task_self();
    mach_vm_address_t address = (mach_vm_address_t)ptr;
    mach_vm_size_t size = 0;
    vm_region_basic_info_data_64_t info = {};
    mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
    mach_port_t object_name = 0;
    kern_return_t ret = mach_vm_region(task, &address, &size, VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info, &count, &object_name);

    if (ret != KERN_SUCCESS)
        return true;

    if ((mach_vm_address_t)ptr < address || (mach_vm_address_t)ptr >= address + size)
        return true;

    if ((info.protection & VM_PROT_ALL) == 0)
        return true;

    return false;
}