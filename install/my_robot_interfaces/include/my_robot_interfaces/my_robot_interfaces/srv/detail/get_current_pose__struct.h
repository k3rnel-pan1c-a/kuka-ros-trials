// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_robot_interfaces:srv/GetCurrentPose.idl
// generated code does not contain a copyright notice

#ifndef MY_ROBOT_INTERFACES__SRV__DETAIL__GET_CURRENT_POSE__STRUCT_H_
#define MY_ROBOT_INTERFACES__SRV__DETAIL__GET_CURRENT_POSE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/GetCurrentPose in the package my_robot_interfaces.
typedef struct my_robot_interfaces__srv__GetCurrentPose_Request
{
  uint8_t structure_needs_at_least_one_member;
} my_robot_interfaces__srv__GetCurrentPose_Request;

// Struct for a sequence of my_robot_interfaces__srv__GetCurrentPose_Request.
typedef struct my_robot_interfaces__srv__GetCurrentPose_Request__Sequence
{
  my_robot_interfaces__srv__GetCurrentPose_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_robot_interfaces__srv__GetCurrentPose_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/GetCurrentPose in the package my_robot_interfaces.
typedef struct my_robot_interfaces__srv__GetCurrentPose_Response
{
  double x;
  double y;
  double z;
  /// Orientation in Euler angles (radians)
  double roll;
  double pitch;
  double yaw;
  bool success;
  rosidl_runtime_c__String message;
} my_robot_interfaces__srv__GetCurrentPose_Response;

// Struct for a sequence of my_robot_interfaces__srv__GetCurrentPose_Response.
typedef struct my_robot_interfaces__srv__GetCurrentPose_Response__Sequence
{
  my_robot_interfaces__srv__GetCurrentPose_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_robot_interfaces__srv__GetCurrentPose_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_ROBOT_INTERFACES__SRV__DETAIL__GET_CURRENT_POSE__STRUCT_H_
