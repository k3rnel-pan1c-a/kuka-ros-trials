// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_robot_interfaces:srv/GetCurrentPose.idl
// generated code does not contain a copyright notice

#ifndef MY_ROBOT_INTERFACES__SRV__DETAIL__GET_CURRENT_POSE__BUILDER_HPP_
#define MY_ROBOT_INTERFACES__SRV__DETAIL__GET_CURRENT_POSE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_robot_interfaces/srv/detail/get_current_pose__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_robot_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_robot_interfaces::srv::GetCurrentPose_Request>()
{
  return ::my_robot_interfaces::srv::GetCurrentPose_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace my_robot_interfaces


namespace my_robot_interfaces
{

namespace srv
{

namespace builder
{

class Init_GetCurrentPose_Response_message
{
public:
  explicit Init_GetCurrentPose_Response_message(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  ::my_robot_interfaces::srv::GetCurrentPose_Response message(::my_robot_interfaces::srv::GetCurrentPose_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_success
{
public:
  explicit Init_GetCurrentPose_Response_success(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  Init_GetCurrentPose_Response_message success(::my_robot_interfaces::srv::GetCurrentPose_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_GetCurrentPose_Response_message(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_yaw
{
public:
  explicit Init_GetCurrentPose_Response_yaw(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  Init_GetCurrentPose_Response_success yaw(::my_robot_interfaces::srv::GetCurrentPose_Response::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_GetCurrentPose_Response_success(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_pitch
{
public:
  explicit Init_GetCurrentPose_Response_pitch(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  Init_GetCurrentPose_Response_yaw pitch(::my_robot_interfaces::srv::GetCurrentPose_Response::_pitch_type arg)
  {
    msg_.pitch = std::move(arg);
    return Init_GetCurrentPose_Response_yaw(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_roll
{
public:
  explicit Init_GetCurrentPose_Response_roll(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  Init_GetCurrentPose_Response_pitch roll(::my_robot_interfaces::srv::GetCurrentPose_Response::_roll_type arg)
  {
    msg_.roll = std::move(arg);
    return Init_GetCurrentPose_Response_pitch(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_z
{
public:
  explicit Init_GetCurrentPose_Response_z(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  Init_GetCurrentPose_Response_roll z(::my_robot_interfaces::srv::GetCurrentPose_Response::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_GetCurrentPose_Response_roll(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_y
{
public:
  explicit Init_GetCurrentPose_Response_y(::my_robot_interfaces::srv::GetCurrentPose_Response & msg)
  : msg_(msg)
  {}
  Init_GetCurrentPose_Response_z y(::my_robot_interfaces::srv::GetCurrentPose_Response::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_GetCurrentPose_Response_z(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

class Init_GetCurrentPose_Response_x
{
public:
  Init_GetCurrentPose_Response_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetCurrentPose_Response_y x(::my_robot_interfaces::srv::GetCurrentPose_Response::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_GetCurrentPose_Response_y(msg_);
  }

private:
  ::my_robot_interfaces::srv::GetCurrentPose_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_robot_interfaces::srv::GetCurrentPose_Response>()
{
  return my_robot_interfaces::srv::builder::Init_GetCurrentPose_Response_x();
}

}  // namespace my_robot_interfaces

#endif  // MY_ROBOT_INTERFACES__SRV__DETAIL__GET_CURRENT_POSE__BUILDER_HPP_
