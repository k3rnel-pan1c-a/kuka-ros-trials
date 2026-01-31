// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_robot_interfaces:srv/DoCalibration.idl
// generated code does not contain a copyright notice

#ifndef MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__BUILDER_HPP_
#define MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_robot_interfaces/srv/detail/do_calibration__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_robot_interfaces
{

namespace srv
{

namespace builder
{

class Init_DoCalibration_Request_step_size
{
public:
  Init_DoCalibration_Request_step_size()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::my_robot_interfaces::srv::DoCalibration_Request step_size(::my_robot_interfaces::srv::DoCalibration_Request::_step_size_type arg)
  {
    msg_.step_size = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_robot_interfaces::srv::DoCalibration_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_robot_interfaces::srv::DoCalibration_Request>()
{
  return my_robot_interfaces::srv::builder::Init_DoCalibration_Request_step_size();
}

}  // namespace my_robot_interfaces


namespace my_robot_interfaces
{

namespace srv
{

namespace builder
{

class Init_DoCalibration_Response_message
{
public:
  explicit Init_DoCalibration_Response_message(::my_robot_interfaces::srv::DoCalibration_Response & msg)
  : msg_(msg)
  {}
  ::my_robot_interfaces::srv::DoCalibration_Response message(::my_robot_interfaces::srv::DoCalibration_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_robot_interfaces::srv::DoCalibration_Response msg_;
};

class Init_DoCalibration_Response_success
{
public:
  Init_DoCalibration_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_DoCalibration_Response_message success(::my_robot_interfaces::srv::DoCalibration_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_DoCalibration_Response_message(msg_);
  }

private:
  ::my_robot_interfaces::srv::DoCalibration_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_robot_interfaces::srv::DoCalibration_Response>()
{
  return my_robot_interfaces::srv::builder::Init_DoCalibration_Response_success();
}

}  // namespace my_robot_interfaces

#endif  // MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__BUILDER_HPP_
