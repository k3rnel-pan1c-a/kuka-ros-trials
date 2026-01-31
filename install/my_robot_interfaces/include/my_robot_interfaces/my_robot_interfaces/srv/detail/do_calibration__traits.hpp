// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from my_robot_interfaces:srv/DoCalibration.idl
// generated code does not contain a copyright notice

#ifndef MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__TRAITS_HPP_
#define MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "my_robot_interfaces/srv/detail/do_calibration__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace my_robot_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const DoCalibration_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: step_size
  {
    out << "step_size: ";
    rosidl_generator_traits::value_to_yaml(msg.step_size, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const DoCalibration_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: step_size
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "step_size: ";
    rosidl_generator_traits::value_to_yaml(msg.step_size, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const DoCalibration_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace my_robot_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use my_robot_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const my_robot_interfaces::srv::DoCalibration_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  my_robot_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use my_robot_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const my_robot_interfaces::srv::DoCalibration_Request & msg)
{
  return my_robot_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<my_robot_interfaces::srv::DoCalibration_Request>()
{
  return "my_robot_interfaces::srv::DoCalibration_Request";
}

template<>
inline const char * name<my_robot_interfaces::srv::DoCalibration_Request>()
{
  return "my_robot_interfaces/srv/DoCalibration_Request";
}

template<>
struct has_fixed_size<my_robot_interfaces::srv::DoCalibration_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<my_robot_interfaces::srv::DoCalibration_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<my_robot_interfaces::srv::DoCalibration_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace my_robot_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const DoCalibration_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const DoCalibration_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const DoCalibration_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace my_robot_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use my_robot_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const my_robot_interfaces::srv::DoCalibration_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  my_robot_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use my_robot_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const my_robot_interfaces::srv::DoCalibration_Response & msg)
{
  return my_robot_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<my_robot_interfaces::srv::DoCalibration_Response>()
{
  return "my_robot_interfaces::srv::DoCalibration_Response";
}

template<>
inline const char * name<my_robot_interfaces::srv::DoCalibration_Response>()
{
  return "my_robot_interfaces/srv/DoCalibration_Response";
}

template<>
struct has_fixed_size<my_robot_interfaces::srv::DoCalibration_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<my_robot_interfaces::srv::DoCalibration_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<my_robot_interfaces::srv::DoCalibration_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<my_robot_interfaces::srv::DoCalibration>()
{
  return "my_robot_interfaces::srv::DoCalibration";
}

template<>
inline const char * name<my_robot_interfaces::srv::DoCalibration>()
{
  return "my_robot_interfaces/srv/DoCalibration";
}

template<>
struct has_fixed_size<my_robot_interfaces::srv::DoCalibration>
  : std::integral_constant<
    bool,
    has_fixed_size<my_robot_interfaces::srv::DoCalibration_Request>::value &&
    has_fixed_size<my_robot_interfaces::srv::DoCalibration_Response>::value
  >
{
};

template<>
struct has_bounded_size<my_robot_interfaces::srv::DoCalibration>
  : std::integral_constant<
    bool,
    has_bounded_size<my_robot_interfaces::srv::DoCalibration_Request>::value &&
    has_bounded_size<my_robot_interfaces::srv::DoCalibration_Response>::value
  >
{
};

template<>
struct is_service<my_robot_interfaces::srv::DoCalibration>
  : std::true_type
{
};

template<>
struct is_service_request<my_robot_interfaces::srv::DoCalibration_Request>
  : std::true_type
{
};

template<>
struct is_service_response<my_robot_interfaces::srv::DoCalibration_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // MY_ROBOT_INTERFACES__SRV__DETAIL__DO_CALIBRATION__TRAITS_HPP_
