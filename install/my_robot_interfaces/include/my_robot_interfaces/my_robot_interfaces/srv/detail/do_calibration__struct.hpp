// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_robot_interfaces:srv/DoCalibration.idl
// generated code does not contain a copyright notice

#ifndef MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__STRUCT_HPP_
#define MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__my_robot_interfaces__srv__DoCalibration_Request __attribute__((deprecated))
#else
# define DEPRECATED__my_robot_interfaces__srv__DoCalibration_Request __declspec(deprecated)
#endif

namespace my_robot_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct DoCalibration_Request_
{
  using Type = DoCalibration_Request_<ContainerAllocator>;

  explicit DoCalibration_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->step_size = 0.0;
    }
  }

  explicit DoCalibration_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->step_size = 0.0;
    }
  }

  // field types and members
  using _step_size_type =
    double;
  _step_size_type step_size;

  // setters for named parameter idiom
  Type & set__step_size(
    const double & _arg)
  {
    this->step_size = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_robot_interfaces__srv__DoCalibration_Request
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_robot_interfaces__srv__DoCalibration_Request
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const DoCalibration_Request_ & other) const
  {
    if (this->step_size != other.step_size) {
      return false;
    }
    return true;
  }
  bool operator!=(const DoCalibration_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct DoCalibration_Request_

// alias to use template instance with default allocator
using DoCalibration_Request =
  my_robot_interfaces::srv::DoCalibration_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace my_robot_interfaces


#ifndef _WIN32
# define DEPRECATED__my_robot_interfaces__srv__DoCalibration_Response __attribute__((deprecated))
#else
# define DEPRECATED__my_robot_interfaces__srv__DoCalibration_Response __declspec(deprecated)
#endif

namespace my_robot_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct DoCalibration_Response_
{
  using Type = DoCalibration_Response_<ContainerAllocator>;

  explicit DoCalibration_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit DoCalibration_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_robot_interfaces__srv__DoCalibration_Response
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_robot_interfaces__srv__DoCalibration_Response
    std::shared_ptr<my_robot_interfaces::srv::DoCalibration_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const DoCalibration_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const DoCalibration_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct DoCalibration_Response_

// alias to use template instance with default allocator
using DoCalibration_Response =
  my_robot_interfaces::srv::DoCalibration_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace my_robot_interfaces

namespace my_robot_interfaces
{

namespace srv
{

struct DoCalibration
{
  using Request = my_robot_interfaces::srv::DoCalibration_Request;
  using Response = my_robot_interfaces::srv::DoCalibration_Response;
};

}  // namespace srv

}  // namespace my_robot_interfaces

#endif  // MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__STRUCT_HPP_
