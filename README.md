# Documenting Errors and Solutions

This document tracks issues encountered during development and the solutions applied to resolve them.

---

## 1. Clock Synchronization Issue When Fetching Current Pose

### **Problem**
When querying the robot’s current pose through the `MoveGroupInterface`, a clock synchronization problem occurred.  
This resulted in failure to fetch the robot's current pose.

### **Cause**
The `MoveGroupInterface` was running on the **same executor** as the main node.  

### **Solution**
Run the `MoveGroupInterface` in a **separate executor** from the main node.

#### **Implemented Fix**
- The main node uses its own executor.  
- A second executor (with its own dedicated thread) is created for the Move Group Interface.  

---
