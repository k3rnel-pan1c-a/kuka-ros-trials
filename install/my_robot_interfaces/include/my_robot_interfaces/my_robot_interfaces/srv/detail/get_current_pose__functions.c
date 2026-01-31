// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from my_robot_interfaces:srv/GetCurrentPose.idl
// generated code does not contain a copyright notice
#include "my_robot_interfaces/srv/detail/get_current_pose__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
my_robot_interfaces__srv__GetCurrentPose_Request__init(my_robot_interfaces__srv__GetCurrentPose_Request * msg)
{
  if (!msg) {
    return false;
  }
  // structure_needs_at_least_one_member
  return true;
}

void
my_robot_interfaces__srv__GetCurrentPose_Request__fini(my_robot_interfaces__srv__GetCurrentPose_Request * msg)
{
  if (!msg) {
    return;
  }
  // structure_needs_at_least_one_member
}

bool
my_robot_interfaces__srv__GetCurrentPose_Request__are_equal(const my_robot_interfaces__srv__GetCurrentPose_Request * lhs, const my_robot_interfaces__srv__GetCurrentPose_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // structure_needs_at_least_one_member
  if (lhs->structure_needs_at_least_one_member != rhs->structure_needs_at_least_one_member) {
    return false;
  }
  return true;
}

bool
my_robot_interfaces__srv__GetCurrentPose_Request__copy(
  const my_robot_interfaces__srv__GetCurrentPose_Request * input,
  my_robot_interfaces__srv__GetCurrentPose_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // structure_needs_at_least_one_member
  output->structure_needs_at_least_one_member = input->structure_needs_at_least_one_member;
  return true;
}

my_robot_interfaces__srv__GetCurrentPose_Request *
my_robot_interfaces__srv__GetCurrentPose_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_robot_interfaces__srv__GetCurrentPose_Request * msg = (my_robot_interfaces__srv__GetCurrentPose_Request *)allocator.allocate(sizeof(my_robot_interfaces__srv__GetCurrentPose_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_robot_interfaces__srv__GetCurrentPose_Request));
  bool success = my_robot_interfaces__srv__GetCurrentPose_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_robot_interfaces__srv__GetCurrentPose_Request__destroy(my_robot_interfaces__srv__GetCurrentPose_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_robot_interfaces__srv__GetCurrentPose_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__init(my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_robot_interfaces__srv__GetCurrentPose_Request * data = NULL;

  if (size) {
    data = (my_robot_interfaces__srv__GetCurrentPose_Request *)allocator.zero_allocate(size, sizeof(my_robot_interfaces__srv__GetCurrentPose_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_robot_interfaces__srv__GetCurrentPose_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_robot_interfaces__srv__GetCurrentPose_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__fini(my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      my_robot_interfaces__srv__GetCurrentPose_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

my_robot_interfaces__srv__GetCurrentPose_Request__Sequence *
my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * array = (my_robot_interfaces__srv__GetCurrentPose_Request__Sequence *)allocator.allocate(sizeof(my_robot_interfaces__srv__GetCurrentPose_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__destroy(my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__are_equal(const my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * lhs, const my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_robot_interfaces__srv__GetCurrentPose_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_robot_interfaces__srv__GetCurrentPose_Request__Sequence__copy(
  const my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * input,
  my_robot_interfaces__srv__GetCurrentPose_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_robot_interfaces__srv__GetCurrentPose_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_robot_interfaces__srv__GetCurrentPose_Request * data =
      (my_robot_interfaces__srv__GetCurrentPose_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_robot_interfaces__srv__GetCurrentPose_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_robot_interfaces__srv__GetCurrentPose_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_robot_interfaces__srv__GetCurrentPose_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

bool
my_robot_interfaces__srv__GetCurrentPose_Response__init(my_robot_interfaces__srv__GetCurrentPose_Response * msg)
{
  if (!msg) {
    return false;
  }
  // x
  // y
  // z
  // roll
  // pitch
  // yaw
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    my_robot_interfaces__srv__GetCurrentPose_Response__fini(msg);
    return false;
  }
  return true;
}

void
my_robot_interfaces__srv__GetCurrentPose_Response__fini(my_robot_interfaces__srv__GetCurrentPose_Response * msg)
{
  if (!msg) {
    return;
  }
  // x
  // y
  // z
  // roll
  // pitch
  // yaw
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
my_robot_interfaces__srv__GetCurrentPose_Response__are_equal(const my_robot_interfaces__srv__GetCurrentPose_Response * lhs, const my_robot_interfaces__srv__GetCurrentPose_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // z
  if (lhs->z != rhs->z) {
    return false;
  }
  // roll
  if (lhs->roll != rhs->roll) {
    return false;
  }
  // pitch
  if (lhs->pitch != rhs->pitch) {
    return false;
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
my_robot_interfaces__srv__GetCurrentPose_Response__copy(
  const my_robot_interfaces__srv__GetCurrentPose_Response * input,
  my_robot_interfaces__srv__GetCurrentPose_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // z
  output->z = input->z;
  // roll
  output->roll = input->roll;
  // pitch
  output->pitch = input->pitch;
  // yaw
  output->yaw = input->yaw;
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

my_robot_interfaces__srv__GetCurrentPose_Response *
my_robot_interfaces__srv__GetCurrentPose_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_robot_interfaces__srv__GetCurrentPose_Response * msg = (my_robot_interfaces__srv__GetCurrentPose_Response *)allocator.allocate(sizeof(my_robot_interfaces__srv__GetCurrentPose_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_robot_interfaces__srv__GetCurrentPose_Response));
  bool success = my_robot_interfaces__srv__GetCurrentPose_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_robot_interfaces__srv__GetCurrentPose_Response__destroy(my_robot_interfaces__srv__GetCurrentPose_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_robot_interfaces__srv__GetCurrentPose_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__init(my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_robot_interfaces__srv__GetCurrentPose_Response * data = NULL;

  if (size) {
    data = (my_robot_interfaces__srv__GetCurrentPose_Response *)allocator.zero_allocate(size, sizeof(my_robot_interfaces__srv__GetCurrentPose_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_robot_interfaces__srv__GetCurrentPose_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_robot_interfaces__srv__GetCurrentPose_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__fini(my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      my_robot_interfaces__srv__GetCurrentPose_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

my_robot_interfaces__srv__GetCurrentPose_Response__Sequence *
my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * array = (my_robot_interfaces__srv__GetCurrentPose_Response__Sequence *)allocator.allocate(sizeof(my_robot_interfaces__srv__GetCurrentPose_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__destroy(my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__are_equal(const my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * lhs, const my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_robot_interfaces__srv__GetCurrentPose_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_robot_interfaces__srv__GetCurrentPose_Response__Sequence__copy(
  const my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * input,
  my_robot_interfaces__srv__GetCurrentPose_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_robot_interfaces__srv__GetCurrentPose_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_robot_interfaces__srv__GetCurrentPose_Response * data =
      (my_robot_interfaces__srv__GetCurrentPose_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_robot_interfaces__srv__GetCurrentPose_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_robot_interfaces__srv__GetCurrentPose_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_robot_interfaces__srv__GetCurrentPose_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
